namespace xstdtsl_internal
{
	template <class T> class safe_vector
	{
	private:
		read_write_mutex m_cMutex;
		T * 				m_pData;
		T * 				m_pPointer_To_End;
		size_t 				m_nSize;
		size_t				m_nCapacity;
		size_t				m_nBlock_Allocation_Size;
		read_write_mutex	m_mMutex;
	private:
		void nl_realloc(size_t i_nNew_Size) noexcept
		{
			size_t nAlloc_Size = i_nNew_Size / m_nBlock_Allocation_Size;
			if ((i_nNew_Size % m_nBlock_Allocation_Size) == 0)
				nAlloc_Size++;
			m_nCapacity = nAlloc_Size;
			nAlloc_Size *= m_nBlock_Allocation_Size;
			T * pNew = new T[nAlloc_Size];
			if (m_pData != nullptr)
			{
				memcpy(pNew,m_pData,m_nSize * sizeof(T));
				delete [] m_pData;
			}
			m_pData = pNew;
			m_pPointer_To_End = &m_pData[m_nSize - 1];
			m_pPointer_To_End++;
			/// m_nSize is unchanged
		}
		void nl_clear(void) noexcept
		{
			m_pPointer_To_End = m_pData;
			m_nSize = 0;
		}
		void nl_sizing(void) noexcept
		{
			m_nType_Size = sizeof(T);
			switch (m_nType_Size)
			{
			case 2:
				m_nBlock_Allocation_Size = 2;
				break;
			case 4:
				m_nBlock_Allocation_Size = 1;
				break;
			default:
				m_nBlock_Allocation_Size = 4 * m_nType_Size;
				break;
			}
		}
		void copy(const safe_vector<T> &i_cRHO) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			read_lock_guard cLock(i_cRHO.m_mMutex);
			nl_clear();
			nl_realloc(i_cRHO.m_nSize);
			memcpy(m_pData,i_cRHO.m_pData,sizeof(T)
		}
		void nl_nullify(void) noexcept
		{
			m_pData = nullptr;
			m_nCapacity = 0;
		}
		void nl_constructor_common(void) noexcept
		{
			nl_nullify();
			nl_clear();
			nl_sizing();
		}

	public:
		void push_back(const T &i_tT) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			if (m_nCapacity < (m_nSize + 1))
				nl_realloc(m_nSize + 1);

			m_pPointer_To_End[0] = i_tT;
			m_pPointer_To_End++;
			m_nSize++;
		}
		void clear(void) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_clear();
		}
		T load(size_t i_nIndex) const noexcept
		{
			T tRet = T();
			read_lock_guard cLock(m_mMutex);
			if (i_nIndex < m_nSize)
				tRet = m_pData[i_nIndex];
			return tRet;
		}
		void store(size_t i_nIndex, const T& i_tT) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			if (i_nIndex < m_nSize)
				m_pData[i_nIndex] = i_tT;
		}
		size_t size(void) const noexcept
		{
			return m_nSize;
		}

		safe_vector<T> & operator =(const safe_vector<T> & i_cRHO) noexcept
		{
			copy(i_cRHO);
			return *this;
		}

		safe_vector(void) noexcept
		{
			constructor_common();
		}
		safe_vector(const safe_vector<T> i_cRHO) noexcept
		{
			constructor_common();
			copy(i_cRHO);
		}
		safe_vector(std::initializer_list<T> i_lT) noexcept
		{
			constructor_common();

			write_lock_guard cLock(m_mMutex);
			nl_realloc(i_lT.size());
			
			for (auto iterI = i_lT.begin(); iterI != i_lT.end(); iterI++)
			{
				m_pData[m_nSize] = *iterI;
				m_nSize++;
			}
		}
		~safe_vector(void) noexcept
		{
			if (m_pData != nullptr)
				delete [] m_pData;
			nl_nullify();
			nl_clear();
		}
		void shrink_to_fit(void) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_realloc(m_nSize);
		}
		bool empty(void)  noexcept
		{
			read_lock_guard cLock(m_mMutex);
			return m_nSize == 0;
		}
		size_t capacity(void) noexcept
		{
			read_lock_guard cLock(m_mMutex);
			return m_nCapacity;
		}
		void reserve(size_t i_nSize) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_realloc(i_nSize);
		}
		size_t max_size(void) noexcept
		{
			return (get_max_memory() / (m_nBlock_Allocation_Size * sizeof(T))) * m_nBlock_Allocation_Size;
		}
			


		class read_iterator
		{
		public:
			enum class start_point = {beginning,end};
		private:
			safe_vector<T> & m_cVector;
			const T * m_pCursor;
		public:
			read_iterator(void) = delete;
			read_iterator(const safe_vector<T> & i_cVector, start_point i_eStart_Point)  noexcept: m_cVector(i_cVector)
			{
				m_cVector.m_cMutex.read_lock();
				if (i_eStart_Point == start_point::end)
				{
					m_pCursor = i_cVector.m_pPointer_To_End;
					m_pCursor--;
				}
				else
					m_pCursor = i_cVector.m_pData;
			}
			read_iterator(const read_iterator & i_cIterator)  noexcept: m_cVector(i_cIterator.m_cVector)
			{
				m_cVector.m_cMutex.read_lock();
				m_pCursor = i_cIterator.m_pCursor;
			}
			~read_iterator(void) noexcept
			{
				m_cVector.m_cMutex.read_unlock();
			}
			read_iterator & operator = (const read_iterator & i_cIterator) noexcept
			{
				m_cVector.m_cMutex.read_unlock();
				m_cVector = i_cIterator.m_cVector;
				m_cVector.m_cMutex.read_lock();
				m_pCursor = i_cIterator.m_pCursor;
				return *this;
			}
			read_iterator & operator ++ (void) noexcept
			{
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return *this;
			}
			read_iterator operator ++ (int i_iValue) noexcept
			{
				read_iterator cRet(*this);
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return cRet;
			}
			read_iterator & operator -- (void) noexcept
			{
				if (m_pCursor > m_cVector.m_pData)
					m_pCursor--;
				return *this;
			}
			read_iterator operator -- (int i_nValue) noexcept
			{
				read_iterator cRet(*this);
				if (m_pCursor > m_cVector.m_pData)
					m_pCursor--;
				return cRet;
			}
			read_iterator & operator += (int i_nValue) noexcept
			{
				m_pCursor += i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData;
				return *this;
			}
			read_iterator & operator -= (int i_nValue) noexcept
			{
				m_pCursor -= i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData;
				return *this;
			}
			bool operator ==(const read_iterator & i_cRHO) noexcept
			{
				return (m_cVector == i_cRHO.m_cVector && m_pCursor == i_cRHO.m_pCursor);
			}
			bool operator !=(const read_iterator & i_cRHO) noexcept
			{
				return (m_cVector != i_cRHO.m_cVector || m_pCursor != i_cRHO.m_pCursor);
			}


			T read(void) noexcept
			{
				T tRet = T();
				if (m_pCursor >= m_cVector.m_pData && m_pCursor <= m_cVector.m_pPointer_To_End)
					tRet = m_pCursor[0];
				return tRet;
			}
			bool is_at_end(void) noexcept
			{
				return m_pCursor >= m_cVector.m_pPointer_To_End;
			}
			bool is_at_beginning(void) noexcept
			{
				return m_pCursor <= m_cVector.m_pData;
			}
		};

		class write_iterator
		{
		public:
			enum class start_point = {beginning,end};
		private:
			safe_vector<T> & m_cVector;
			const T * m_pCursor;
		public:
			write_iterator(void) = delete;
			write_iterator(const safe_vector<T> & i_cVector, start_point i_eStart_Point)  noexcept: m_cVector(i_cVector)
			{
				m_cVector.m_cMutex.write_lock();
				if (i_eStart_Point == start_point::end)
				{
					m_pCursor = i_cVector.m_pPointer_To_End;
					m_pCursor--;
				}
				else
					m_pCursor = i_cVector.m_pData;
			}
			write_iterator(const read_iterator & i_cIterator)   = delete;
			~write_iterator(void) noexcept
			{
				m_cVector.m_cMutex.write_unlock();
			}
			write_iterator & operator = (const read_iterator & i_cIterator) = delete;
			write_iterator & operator ++ (void) noexcept
			{
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return *this;
			}
			write_iterator operator ++ (int i_iValue) noexcept
			{
				read_iterator cRet(*this);
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return cRet;
			}
			write_iterator & operator -- (void) noexcept
			{
				if (m_pCursor > m_cVector.m_pData)
					m_pCursor--;
				return *this;
			}
			write_iterator operator -- (int i_nValue) noexcept
			{
				read_iterator cRet(*this);
				if (m_pCursor > m_cVector.m_pData)
					m_pCursor--;
				return cRet;
			}
			write_iterator & operator += (int i_nValue) noexcept
			{
				m_pCursor += i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData;
				return *this;
			}
			write_iterator & operator -= (int i_nValue) noexcept
			{
				m_pCursor -= i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData;
				return *this;
			}


			T read(void) noexcept
			{
				T tRet = T();
				if (m_pCursor >= m_cVector.m_pData && m_pCursor <= m_cVector.m_pPointer_To_End)
					tRet = m_pCursor[0];
				return tRet;
			}
			void write(T i_tT) noexcept
			{
				if (m_pCursor >= m_cVector.m_pData && m_pCursor <= m_cVector.m_pPointer_To_End)
					m_pCursor[0] = i_tT;
			}
			bool is_at_end(void) noexcept
			{
				return m_pCursor >= m_cVector.m_pPointer_To_End;
			}
			bool is_at_beginning(void) noexcept
			{
				return m_pCursor <= m_cVector.m_pData;
			}
		};
	};
}

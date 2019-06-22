namespace xstdtsl_internal
{
	///
	/// vector type that is safe for crossing library boundaries and is thread safe; similar to a cross between std::atomic and std::vector; more restrictive on access to data than is std::vector. read and write iterators lock access to the data and capacity to change the vector. multiple read operations may occur simultaneously, but write operations or operations that modify the contents or size of the vector are atomic.
	///
	template <class T> class safe_vector
	{
	private:
		read_write_mutex 	m_mMutex; ///< mutex for control of data contents
		T * 				m_pData; ///< pointer to data block
		T * 				m_pPointer_To_End; ///< pointer to end of data block (for convenience)
		size_t 				m_nSize; ///< current size of data (number of objects of type T)
		size_t				m_nCapacity; ///< current allocated space in the data block in terms of the number of objects of type T
		size_t				m_nBlock_Allocation_Size; ///< minimum block size to be allocated to ensure 8-byte alignment
	private:
		///
		size_t nl_copy_other(T * i_pSource, size_t i_nSize, T * i_pDest, size_t i_nCapacity, bool i_bDestroy_Source)
		{
			size_t nCopy_Size = 0;
			if (i_pSource != nullptr && i_pDest != nullptr)
			{
				nCopy_Size = i_nSize;
				if (i_nCapacity < i_nSize)
					nCopy_Size = i_nCapacity;
				for (size_t nI = 0; nI < i_nSize; nI++)
				{
					if (nI < nCopy_Size)
						new (&i_pDest[nI]) T (i_pSource[nI]); // copy constructor for existing data
					if (i_bDestroy_Source)
						i_pSource[nI].~T(); // call destructor
				}
				if (i_bDestroy_Source)
					std::free(i_pSource);
			}
			return nCopy_Size;
		}
		void nl_destroy(T * i_pData, size_t i_nSize, bool i_bFree)
		{
			if (i_pData != nullptr)
			{
				for (size_t nI = 0; nI < i_nSize; nI++)
				{
					i_pData[nI].~T(); // call destructor
				}
				if (i_bFree)
					std::free(i_pData);
			}
		}
		///
		/// allocate or reallocate data block, copying old contents to the new data block if needed
		///
		void nl_realloc(
				size_t i_nNew_Size, ///< the desired size of the new data block; if smaller than the current size will truncate the existing data
				bool i_bCopy_Old  ///< flag to indicate whether existing data should be copied
				) noexcept
		{
			size_t nAlloc_Size = i_nNew_Size / m_nBlock_Allocation_Size;
			if ((i_nNew_Size % m_nBlock_Allocation_Size) == 0)
				nAlloc_Size++;
			nAlloc_Size *= m_nBlock_Allocation_Size;
			m_nCapacity = nAlloc_Size;
			T * pNew = reinterpret_cast<T*>(std::malloc(sizeof(T) * nAlloc_Size));
			if (m_pData != nullptr)
			{
				if (i_bCopy_Old)
					m_nSize = nl_copy_other(m_pData,m_nSize,pNew,m_nCapacity,true);
				else
				{
					nl_destroy(m_pData,m_nSize,true);
					m_nSize = 0;
				}
			}
			m_pData = pNew;
			m_pPointer_To_End = m_pData + m_nSize;
		}
		///
		/// clear the existing data by setting the current size to 0 and the end pointer to the starting point
		///
		void nl_clear(void) noexcept
		{
			nl_destroy(m_pData,m_nSize,false);
			m_nSize = 0;
			m_pPointer_To_End = m_pData + m_nSize;
		}
		///
		/// determine block allocation size asssuing 8-byte alignment @@TODO: allow 4-byte alignment; generally should only be called in the constructor
		///
		void nl_sizing(void) noexcept
		{
			m_nType_Size = sizeof(T);
			switch (m_nType_Size)
			{
			case 2:
				m_nBlock_Allocation_Size = 4;
				break;
			case 4:
				m_nBlock_Allocation_Size = 2;
				break;
			default:
				m_nBlock_Allocation_Size = 8 * m_nType_Size;
				break;
			}
		}
		///
		/// copy a safe_vector of the same type; blocking write lock on this, blocking read lock on the vector to be copied; 
		///
		void copy(
			const safe_vector<T> &i_cRHO ///< the vector to be copied
			) noexcept
		{
			dual_read_write_lock cLock(i_cRHO.m_mMutex,m_mMutex);
			nl_clear();
			nl_realloc(i_cRHO.m_nSize);
			m_nSize = nl_copy_other(i_cRHO.m_pData,i_cRHO.m_nSize,m_pData,m_nCapacity,false);
		}
		///
		/// set data pointer and pointer to end to null; set capactiy to 0; will NOT delete data block
		///
		void nl_nullify(void) noexcept
		{
			m_pData = m_pPointer_To_End = nullptr;
			m_nCapacity = 0;
		}
		///
		/// common components of constructors: nullify all pointers, perform a clear; and determine block allocation size
		///
		void nl_constructor_common(void) noexcept
		{
			nl_nullify();
			nl_clear();
			nl_sizing();
		}

	public:
		///
		/// place a new member at the back of the vector. if the new size exceeds capacity a reallocate will be performed. a blocking write lock will be attempted
		///
		void push_back(
			const T &i_tT ///< the new data to emplace at the back of the vector
			) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			if (m_nCapacity < (m_nSize + 1))
				nl_realloc(m_nSize + 1);

			new (m_pPointer_To_End) T (i_tT);
			m_pPointer_To_End++;
			m_nSize++;
		}
		///
		/// reset vector to size 0; will
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
			{
				m_pData[i_nIndex].~T();
				new (&m_pData[i_nIndex]) T (i_tT);
			}
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
				new (&m_pData[m_nSize]) T (*iterI);
				m_nSize++;
			}
		}
		~safe_vector(void) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_destroy(m_pData,m_nSize,true);
			m_nSize = 0;
			nl_nullify();
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

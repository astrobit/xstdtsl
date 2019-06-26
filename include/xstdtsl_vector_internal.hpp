#include <xstdtsl_mutex>
#include <xstdtsl_system_C.h>

namespace xstdtsl_internal
{
	///
	/// vector type that is safe for crossing library boundaries and is thread safe; similar to a cross between std::atomic and std::vector; more restrictive on access to data than is std::vector. read and write iterators lock access to the data and capacity to change the vector. multiple read operations may occur simultaneously, but write operations or operations that modify the contents or size of the vector are atomic.
	///
	template <class T> class safe_vector
	{
	protected:
		mutable read_write_mutex 	m_mMutex; ///< mutex for control of data contents
		T * 				m_pData; ///< pointer to data block
		T * 				m_pPointer_To_End; ///< pointer to end of data block (for convenience)
		size_t 				m_nSize; ///< current size of data (number of objects of type T)
		size_t				m_nCapacity; ///< current allocated space in the data block in terms of the number of objects of type T
		size_t				m_nBlock_Allocation_Size; ///< minimum block size to be allocated to ensure 8-byte alignment
	protected:
		///
		size_t nl_copy_nondestruct(T * i_pSource, size_t i_nSize, T * i_pDest, size_t i_nCapacity) noexcept(noexcept(T(const T&)))
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
				}
			}
			return nCopy_Size;
		}
		size_t nl_copy_destruct(T * i_pSource, size_t i_nSize, T * i_pDest, size_t i_nCapacity) noexcept(noexcept(T(const T&)) && noexcept(~T()))
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
					i_pSource[nI].~T(); // call destructor
				}
			}
			return nCopy_Size;
		}
		void nl_destruct_contents(T * i_pData, size_t i_nSize) noexcept(noexcept(~T()) // don't know if ~T throws exceptions
		{
			if (i_pData != nullptr)
			{
				for (size_t nI = 0; nI < i_nSize; nI++)
				{
					i_pData[nI].~T(); // call destructor
				}
			}
		}
		///
		/// allocate or reallocate data block, copying old contents to the new data block if needed
		///
		void nl_realloc_copy(
				size_t i_nNew_Size, ///< the desired size of the new data block; if smaller than the current size will truncate the existing data
				) noexcept(false) // don't know if T constructor or destructor throws exceptions
		{
			size_t nAlloc_Size = i_nNew_Size / m_nBlock_Allocation_Size;
			if ((i_nNew_Size % m_nBlock_Allocation_Size) == 0)
				nAlloc_Size++;
			nAlloc_Size *= m_nBlock_Allocation_Size;
			m_nCapacity = nAlloc_Size;
			T * pNew = reinterpret_cast<T*>(std::malloc(sizeof(T) * nAlloc_Size));
			if (m_pData != nullptr)
			{
				m_nSize = nl_copy_destruct(m_pData,m_nSize,pNew,m_nCapacity);
				std::free(m_pData);
			}
			m_pData = pNew;
			m_pPointer_To_End = m_pData + m_nSize;
		}
		///
		/// allocate or reallocate data block, copying old contents to the new data block if needed
		///
		void nl_realloc_no_copy(
				size_t i_nNew_Size, ///< the desired size of the new data block; if smaller than the current size will truncate the existing data
				) noexcept(false) // don't know if T constructor or destructor throws exceptions
		{
			size_t nAlloc_Size = i_nNew_Size / m_nBlock_Allocation_Size;
			if ((i_nNew_Size % m_nBlock_Allocation_Size) == 0)
				nAlloc_Size++;
			nAlloc_Size *= m_nBlock_Allocation_Size;
			m_nCapacity = nAlloc_Size;
			T * pNew = reinterpret_cast<T*>(std::malloc(sizeof(T) * nAlloc_Size));
			if (m_pData != nullptr)
			{
				nl_destroy(m_pData,m_nSize);
				m_nSize = 0;
				std::free(m_pData);
			}
			m_pData = pNew;
			m_pPointer_To_End = m_pData + m_nSize;
		}
		///
		/// clear the existing data by setting the current size to 0 and the end pointer to the starting point
		///
		void nl_clear(void) noexcept(false) // don't know if T destructor throws exceptions
		{
			nl_destruct_contents(m_pData,m_nSize);
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
			) noexcept(false) // don't know if T constructor or destructor throw exceptions 
		{
			dual_read_write_lock cLock(i_cRHO.m_mMutex,m_mMutex);
			nl_clear();
			nl_realloc(i_cRHO.m_nSize);
			m_nSize = nl_copy_nondestruct(i_cRHO.m_pData,i_cRHO.m_nSize,m_pData,m_nCapacity);
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
		void nl_constructor_common(void) noexcept(false) // don't know if T constructor or destructor throw exceptions
		{
			nl_nullify();
			nl_clear();
			nl_sizing();
		}

		///
		/// place a new member at the back of the vector. if the new size exceeds capacity a reallocate will be performed. a blocking write lock will be attempted
		///
		void nl_push_back(
			const T &i_tT ///< the new data to emplace at the back of the vector
			) noexcept(noexcept(T)) // don't know if t is 
		{
			if (m_nCapacity < (m_nSize + 1))
				nl_realloc(m_nSize + 1);

			new (m_pPointer_To_End) T (i_tT);
			m_pPointer_To_End++;
			m_nSize++;
		}
		///
		/// retrieve data from within the vector
		/// \returns the data at the selected location; if the location is invalid a type T constructed with the default constructor will be returned
		///
		T nl_load(size_t i_nIndex) const noexcept(false)
		{
			T tRet = T();
			if (i_nIndex < m_nSize)
				tRet = m_pData[i_nIndex];
			return tRet;
		}
		///
		/// store data within the vector at a given location if the location is within the existing vector. destructor will be called on existing data at the location
		///
		void nl_store(
				size_t i_nIndex, ///< the location at which to store the data
				const T& i_tT ///< the data to be stored
				) noexcept
		{
			if (i_nIndex < m_nSize)
			{
				m_pData[i_nIndex].~T();
				new (&m_pData[i_nIndex]) T (i_tT);
			}
		}
		///
		/// get the current size of the vector
		/// \returns the current size of the vector
		///
		size_t nl_size(void) const noexcept
		{
			return m_nSize;
		}

		///
		/// shrinks the capacity to minimize memory use; after shrink may still have larger capacity than size
		///
		void nl_shrink_to_fit(void) noexcept
		{
			nl_realloc(m_nSize);
		}
		///
		/// test if the vector is empty
		/// \returns true if the vector is empty; false otherwise
		///
		bool nl_empty(void) const  noexcept
		{
			return m_nSize == 0;
		}
		///
		/// returns the current capacity of the vector
		/// \returns the current capacity of the vector
		///
		size_t nl_capacity(void) const noexcept
		{
			return m_nCapacity;
		}
		///
		/// expands the vector capacity if the requested capacity is larger than the existing capacity
		///
		void nl_reserve(
			size_t i_nCapacity ///< the desired new capacity
			) noexcept
		{
			if (i_nSize > m_nCapacity)
				nl_realloc(i_nSize);
		}

	public:
		///
		/// place a new member at the back of the vector. if the new size exceeds capacity a reallocate will be performed. a blocking write lock will be attempted
		///
		void push_back(
			const T &i_tT ///< the new data to emplace at the back of the vector
			) noexcept(noexcept(T)) // don't know if t is 
		{
			write_lock_guard cLock(m_mMutex);
			nl_push_back(i_tT);
		}
		///
		/// reset vector to size 0; will call destructor on any existing contents; blocking (write)
		/// 
		void clear(void) noexcept(false) // don't know if T destructor will throw exceptions
		{
			write_lock_guard cLock(m_mMutex);
			nl_clear();
		}
		///
		/// retrieve data from within the vector; blocking (read)
		/// \returns the data at the selected location; if the location is invalid a type T constructed with the default constructor will be returned
		///
		T load(size_t i_nIndex) const noexcept(false)
		{
			read_lock_guard cLock(m_mMutex);
			return nl_load();
		}
		///
		/// store data within the vector at a given location if the location is within the existing vector. destructor will be called on existing data at the location; blocking (write)
		///
		void store(
				size_t i_nIndex, ///< the location at which to store the data
				const T& i_tT ///< the data to be stored
				) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_store(i_nIndex,i_tT);
		}
		///
		/// get the current size of the vector; blocking (read)
		/// \returns the current size of the vector
		///
		size_t size(void) const noexcept
		{
			read_lock_guard cLock(m_mMutex);
			return nl_size();
		}

		///
		/// shrinks the capacity to minimize memory use; after shrink may still have larger capacity than size; blocking (write)
		///
		void shrink_to_fit(void) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_shrink_to_fit();
		}
		///
		/// test if the vector is empty; blocking (read)
		/// \returns true if the vector is empty; false otherwise
		///
		bool empty(void) const  noexcept
		{
			read_lock_guard cLock(m_mMutex);
			return nl_empty();
		}
		///
		/// returns the current capacity of the vector; blocking (read)
		/// \returns the current capacity of the vector
		///
		size_t capacity(void) const noexcept
		{
			read_lock_guard cLock(m_mMutex);
			return nl_capacity();
		}
		///
		/// expands the vector capacity if the requested capacity is larger than the existing capacity
		///
		void reserve(
			size_t i_nCapacity ///< the desired new capacity
			) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			return nl_reserve(i_nCapacity);
		}
		///
		/// returned the maximum possible capacity of the vector given memory limitations of the system
		/// \returns the maximum possible capacity for the given type
		///
		size_t max_size(void) const noexcept
		{
			return (get_available_memory() / (m_nBlock_Allocation_Size * sizeof(T))) * m_nBlock_Allocation_Size;
		}

		///
		/// assignment operator: copys data from one vector to another; blocking (write)
		///
		safe_vector<T> & operator =(const safe_vector<T> & i_cRHO) noexcept(false)
		{
			copy(i_cRHO);
			return *this;
		}

		///
		/// default constructor; creates an empty vector with no space allocated
		///
		safe_vector(void) noexcept
		{
			constructor_common();
		}
		///
		/// copy constructor; copys data from one vector to another; blocking (read/write)
		///
		safe_vector(const safe_vector<T> i_cRHO) noexcept(false) // don't know if T(const T&) will cause exception
		{
			constructor_common();
			copy(i_cRHO);
		}
		///
		/// construct from an initializer list; blocking (write)
		///
		safe_vector(
			std::initializer_list<T> i_lT ///< the data to use to construct the vector
			) noexcept
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
		///
		/// default destructor; blocking (write)
		///
		~safe_vector(void) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			nl_destroy(m_pData,m_nSize,true);
			m_nSize = 0;
			nl_nullify();
		}
			

		class iterator_base
		{
		private:
			bool m_bLock_Type_Write; // type of lock to hold on the vector; true indicates a write lock, false indicates a read lock
		public:
			/// 
			/// enumeration to describe whether the iteration begins at the beginning or end of the vector
			enum class start_point = {
					beginning, ///< iteration will begin at the start of the vector data
					end ///< iteration will begin at the end of the vector data
					};
		protected:
			const safe_vector<T> & m_cVector; ///< the vector that is being iterated over
			const T * m_pCursor; ///< a cursor pointing to the current data location within the vector
		public:
			iterator_base(void) = delete;
			///
			/// constructor that initializes the iterator and aquires a read lock on the vector
			///
			iterator_base(
				const safe_vector<T> & i_cVector, ///< the vector to iterate over
				start_point i_eStart_Point, ///< the starting point to use within the vector (beginning or end)
				bool i_bLock_Type_Write ///< flag to indicate lock type; true indicates write lock, false indicates read lock
				)  noexcept: m_cVector(i_cVector)
			{
				m_bLock_Type_Write = i_bLock_Type_Write;
				if (m_bLock_Type_Write)
					m_cVector.write_lock();
				else
					m_cVector.read_lock();
				if (i_eStart_Point == start_point::end)
				{
					m_pCursor = i_cVector.m_pPointer_To_End;
					m_pCursor--;
				}
				else
					m_pCursor = i_cVector.m_pData;
			}
			///
			/// copy constructor (deleted)
			///
			iterator_base(const iterator_base & i_cIterator)  = delete;
			///
			/// destructor: releases lock
			///
			virtual ~base_constructor(void)
			{
				if (m_bLock_Type_Write)
					m_cVector.write_unlock();
				else
					m_cVector.read_unlock();
			}
			///
			/// pre-increment operator; advances the iterator within the vector; will not advance past the end of the vector
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator ++ (void) noexcept
			{
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return *this;
			}
			///
			/// post-increment operator; advances the iterator within the vector; will not advance past the end of the vector
			/// \returns an iterator reflecting the state prior to increment
			///
			virtual iterator_base operator ++ (
				int i_iValue ///< ignored
				) noexcept
			{
				iterator_base cRet(*this);
				if (m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor++;
				return cRet;
			}
			///
			/// pre-decrement operator; rewinds the iterator within the vector; will not rewind past the start of the vector
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator -- (void) noexcept
			{
				if (m_pCursor >= m_cVector.m_pData)
					m_pCursor--;
				return *this;
			}
			///
			/// post-decrement operator; rewinds the iterator within the vector; will not rewind past the start of the vector
			/// \returns an iterator reflecting the state prior to decrement
			///
			virtual iterator_base operator -- (
				int i_nValue ///< ignored
				) noexcept
			{
				iterator_base cRet(*this);
				if (m_pCursor >= m_cVector.m_pData)
					m_pCursor--;
				return cRet;
			}
			///
			/// add-assign operator; advances the iterator within the vector; will not advance past the end of the vector or prior to the start of the data
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator += (int i_nValue) noexcept
			{
				m_pCursor += i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData - 1;
				return *this;
			}
			///
			/// subtract-assign operator; advances the iterator within the vector; will not advance past the end of the vector or prior to the start of the data
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator -= (int i_nValue) noexcept
			{
				m_pCursor -= i_nValue;
				if (m_pCursor > m_cVector.m_pPointer_To_End)
					m_pCursor = m_cVector.m_pPointer_To_End;
				else if (m_pCursor < m_cVector.m_pData)
					m_pCursor = m_cVector.m_pData - 1;
				return *this;
			}
			///
			/// equality operator
			/// \returns true if both iterators are pointing to the same vector and are pointing to the same location within the vector
			///
			virtual bool operator ==(const read_iterator & i_cRHO) noexcept
			{
				return (m_cVector == i_cRHO.m_cVector && m_pCursor == i_cRHO.m_pCursor);
			}
			///
			/// inequality operator
			/// \returns true if the iterators are not pointing to the same vector or are not pointing to the same location within one vector
			///
			virtual bool operator !=(const read_iterator & i_cRHO) noexcept
			{
				return (m_cVector != i_cRHO.m_cVector || m_pCursor != i_cRHO.m_pCursor);
			}
			///
			/// retrive the value at the current location of the iterator; if the iterator is not pointing to valid data will return type T with default contructor
			/// \returns the data at the current location of the iterator if the iterator is valid
			///
			virtual T load(void) noexcept(false)
			{
				T tRet = T();
				if (m_pCursor >= m_cVector.m_pData && m_pCursor < m_cVector.m_pPointer_To_End)
					tRet = m_pCursor[0];
				return tRet;
			}
			///
			/// determine if the iterator has reached the end of the vector
			/// \returns true if the iterator is at or past the end of the vector; false otherwise
			///
			virtual bool is_at_end(void) noexcept
			{
				return m_pCursor >= m_cVector.m_pPointer_To_End;
			}
			///
			/// determine if the iterator is prior to the beginning of the vector data
			/// \returns true if the iterator is prior to the beginning of the vector data; false otherwise
			///
			virtual bool is_at_beginning(void) noexcept
			{
				return m_pCursor < m_cVector.m_pData;
			}
		};

		///
		/// class for iterating through a vector; unlike typical iterators this class is scoped and holds a read lock on the vector to ensure that the vector does not change data or size during iteration
		///
		class read_iterator : public iterator_base
		{
		public:
			///
			/// default constructor (deleted)
			///
			read_iterator(void) = delete;
			///
			/// constructor that initializes the iterator and aquires a read lock on the vector; blocking (read)
			///
			read_iterator(
				const safe_vector<T> & i_cVector, ///< the vector to iterate over
				start_point i_eStart_Point ///< the starting point to use within the vector (beginning or end)
				)  noexcept : iterator_base(i_cVector,i_eStart_Point,false)
			{
			}
			///
			/// copy constructor; aquires an additional read lock on the vector; blocking (read)
			///
			read_iterator(
				const read_iterator & i_cIterator ///< the iterator to copy
				)  noexcept : iterator_base(i_cVector,iterator_base::start_point::beginning,false)
			{
				m_pCursor = i_cIterator.m_pCursor;
			}
			///
			/// assignment operator; releases lock on existing vector and aquires a read lock on the vector that the right hand iterator refers to; blocking (read)
			///
			read_iterator & operator = (
					const read_iterator & i_cIterator ///< the iterator to copy
				) noexcept
			{
				m_cVector.m_cMutex.read_unlock();
				m_cVector = i_cIterator.m_cVector;
				m_cVector.m_cMutex.read_lock();
				m_pCursor = i_cIterator.m_pCursor;
				return *this;
			}
		};

		class write_iterator : public iterator_base
		{

		public:
			///
			/// default constructor: deleted (need reference to vector)
			///
			write_iterator(void) = delete;
			///
			/// constructor that initializes the iterator and aquires a read lock on the vector; blocking (write)
			///
			write_iterator(const safe_vector<T> & i_cVector, start_point i_eStart_Point)  noexcept : iterator_base(i_cVector,i_eStart_Point,true)
			{
			}

			///
			/// copy constructor; deleted (due to requirement for multiple write locks); use write_control instead
			///
			write_iterator(
				const write_iterator & i_cIterator ///< the iterator to copy
				)  noexcept = delete
			{
				m_pCursor = i_cIterator.m_pCursor;
			}
			///
			/// assignment operator; deleted (due to requirement for multiple write locks); use write_control instead
			///
			write_iterator & operator = (
					const write_iterator & i_cIterator ///< the iterator to copy
				) noexcept = delete;

			///
			/// store the value at the current location of the iterator; if the iterator is not pointing to valid data the request will be ignored
			///
			void store(T i_tT) noexcept
			{
				if (m_pCursor >= m_cVector.m_pData && m_pCursor < m_cVector.m_pPointer_To_End)
					m_pCursor[0] = i_tT;
			}
		};

		class control_base
		{
		private:
			bool m_bLock_Type_Write; // type of lock to hold on the vector; true indicates a write lock, false indicates a read lock
		protected:
			safe_vector<T> & m_cVector; ///< reference to the vector to control
		public:
			///
			/// default contructor (deleted)
			///
			control_base(void) = delete;
			///
			/// contructor: tie the read control to a particular vector and lock the vector for read; blocking
			///
			control_base(
				const safe_vector<T> & i_cVector, ///< the vector to be accessed
				bool i_bLock_Type_Write ///< flag to indicate the type of lock control to assume
				)  noexcept: m_cVector(i_cVector)
			{
				m_bLock_Type_Write = i_bLock_Type_Write;
				if (m_bLock_Type_Write)
					m_cVector.m_cMutex.write_lock();
				else
					m_cVector.m_cMutex.read_lock();

			}
			///
			/// copy contructor (deleted)
			///
			control_base(const control_base & i_cIterator)   = delete;
			///
			/// destructor: release read lock 
			///
			~control_base(void) noexcept
			{
				if (m_bLock_Type_Write)
					m_cVector.m_cMutex.write_unlock();
				else
					m_cVector.m_cMutex.read_unlock();
			}
			///
			/// assignment / copy operator (deleted)
			///
			control_base & operator = (const control_base & i_cIterator) = delete;
			///
			/// test if the vector is empty
			/// \returns true if the vector is empty; false otherwise
			///
			bool empty(void)  noexcept
			{
				return m_cVector.nl_empty();
			}
			///
			/// returns the current size of the vector
			/// \returns the size of the vector; 0 if empty
			///
			size_t size(void) const noexcept
			{
				return m_cVector.nl_size();
			}
			///
			/// returns the current capacity of the vector
			/// \returns the current capacity of the vector
			///
			virtual size_t capacity(void) noexcept
			{
				return m_cVector.nl_capacity();
			}

			///
			/// returns the maximum capacity of the vector
			///
			virtual size_t max_size(void) const noexcept
			{
				return m_cVector.max_size();
			}
			///
			/// retrieve data from within the vector
			/// \returns the data at the selected location; if the location is invalid a type T constructed with the default constructor will be returned
			///
			virtual T load(
					size_t i_nIndex ///< the location within the vector at which to retrieve the data
					) const noexcept(false) // don't know if T() throws an exception
			{
				return m_cVector.nl_load(i_nIndex);
			}
		};	

		///
		/// the read control class is designed to allow scoped read access to the vector that maintains a read lock throughout the scope. This is usefl when many random acess reads occur
		///
		class read_control : public control_base
		{
		public:
			///
			/// default contructor (deleted)
			///
			read_control(void) = delete;
			///
			/// contructor: tie the read control to a particular vector and lock the vector for read; blocking
			///
			read_control(
				const safe_vector<T> & i_cVector ///< the vector to be accessed
				)  noexcept: control_base(i_cVector,false)
			{
			}
			///
			/// copy contructor (deleted)
			///
			read_control(const read_iterator & i_cIterator)   = delete;
			///
			/// assignment / copy operator (deleted)
			///
			read_control & operator = (const read_iterator & i_cIterator) = delete;
		};		

		///
		/// the read control class is designed to allow scoped read access to the vector that maintains a read lock throughout the scope. This is usefl when many random acess reads occur
		///
		class write_control : public control_base
		{
		public:
			///
			/// default contructor (deleted)
			///
			write_control(void) = delete;
			///
			/// contructor: tie the read control to a particular vector and lock the vector for read; blocking
			///
			write_control(
				const safe_vector<T> & i_cVector ///< the vector to be accessed
				)  noexcept: control_base(i_cVector,true)
			{
			}
			///
			/// copy contructor (deleted)
			///
			write_control(const write_control & i_cIterator)   = delete;
			///
			/// assignment / copy operator (deleted)
			///
			write_control & operator = (const write_control & i_cIterator) = delete;
			///
			/// store the value at the current location of the iterator; if the iterator is not pointing to valid data the request will be ignored
			///
			void store(
				size_t i_nIndex, ///< the location at which to store the data
				const T& i_tT ///< the data to be stored
				) noexcept
			{
				m_cVector.nl_store(i_nIndex,i_tT);
			}
			///
			/// store the value at the current location of the iterator; if the iterator is not pointing to valid data the request will be ignored
			///
			void push_back(const T &i_tT) noexcept
			{
				m_cVector.nl_push_back(i_tT);
			}
			///
			/// expands the vector capacity if the requested capacity is larger than the existing capacity
			///
			void reserve(
				size_t i_nCapacity ///< the desired new capacity
				) noexcept
			{
				m_cVector.nl_reserve(i_nCapacity);
			}

			///
			/// shrinks the capacity to minimize memory use; after shrink may still have larger capacity than size
			///
			void shrink_to_fit(void)
			{
				m_cVector.nl_shrink_to_fit();
			}

		};		
	};
}

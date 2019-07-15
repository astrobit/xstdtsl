#pragma once
#ifndef XSTDTSL_BINARY_TREE_HELPER_H
#define XSTDTSL_BINARY_TREE_HELPER_H
#include <xstdtsl_enums.hpp>

namespace xstdtsl
{
	namespace binary_tree_helper
	{

		///
		/// a node within the tree
		///
		template <class T> class node
		{ 
		public:
			typedef node<T>			nodetype;
			typedef T				datatype;
//			using datatype  = T;
			typedef node<T> node_t;
			typedef node<T> * node_p;
		protected:
			node_p 				m_pLeft; ///< the left child of the node
			node_p 				m_pRight; ///< the right child of the node
			node_p 				m_pParent; ///< the parent of the node

			T 						m_tKey; ///< the value of the node
		public:
			///
			/// get the value of the current node
			///
			inline T get_key(void) const noexcept
			{
				return m_tKey;
			}
			///
			/// void constructor; create an empty, valueless node (i.e. no children or parent; key  constructed with default constructor)
			///
			node(void) noexcept(noexcept(T(std::declval<T>()))) : m_tKey(), m_pParent(nullptr), m_pLeft(nullptr), m_pRight(nullptr)
			{
				;
			}
			///
			/// void constructor; create an empty, valueless node (i.e. no children or parent; key  constructed with default constructor)
			///
			node(node_p i_pParent, T i_tKey) noexcept(noexcept(T(std::declval<T>()))) : m_tKey(i_tKey), m_pParent(i_pParent), m_pLeft(nullptr), m_pRight(nullptr)
			{
				;
			}
			///
			/// destructor: destroy the left children then the right children of the current node
			///
			~node(void) noexcept(noexcept(std::declval<T>().~T()))
			{
				if (m_pLeft != nullptr)
					delete m_pLeft;
				m_pLeft = nullptr;
				if (m_pRight != nullptr)
					delete m_pRight;
				m_pRight = nullptr;
			}

			///
			/// find the node within this sub-tree that contains the given value
			/// \returns a pointer to the node containing the value
			///
			node_p search(T i_tKey) const noexcept
			{
				node_p pRet = nullptr;
				
				if (m_tKey == i_tKey)
					pRet = const_cast<node_p>(this);
				else if (i_tKey < m_tKey && m_pLeft != nullptr)
					pRet = m_pLeft->search(i_tKey);
				else if (i_tKey > m_tKey && m_pRight != nullptr)
					pRet = m_pRight->search(i_tKey);
				return pRet;
			}
			///
			/// get the left child of the current node
			/// \returns a pointer to the left child, or nullptr if the node does not have a right child
			///
			inline node_p get_left(void) const noexcept
			{
				return m_pLeft;
			}
			///
			/// set the left child of the current node
			///
			void set_left(node_p i_pNew_Left) noexcept
			{
				m_pLeft = i_pNew_Left;
			}

			///
			/// get the right child of the current node
			/// \returns a pointer to the right child, or nullptr if the node does not have a right child
			///
			node_p get_right(void) const noexcept
			{
				return m_pRight;
			}
			///
			/// set the right child of the current node
			///
			void set_right(node_p i_pNew_Right) noexcept
			{
				m_pRight = i_pNew_Right;
			}
			///
			/// get the parent of the current node
			/// \returns a pointer to the parent, or nullptr if the node is root of the tree
			///
			node_p get_parent(void) const noexcept
			{
				return m_pParent;
			}

			///
			/// set the parent of the current node
			///
			void set_parent(node_p i_pNew_Parent) noexcept
			{
				m_pParent = i_pNew_Parent;
			}



			///
			/// rotate the tree based at the current node to the right
			///
			void rotate_right(node_p * &io_pTree_Root) noexcept
			{ 
				if (m_pLeft != nullptr)
				{
					node_p pNew_Root = m_pLeft; 
					node_p pRightOfLeft = pNew_Root->m_pRight; 

					// Perform rotation 
					pNew_Root->m_pRight = const_cast<node_p>(this); 
					m_pLeft = pRightOfLeft; 
					// update link from parent if this node is not the root
					if (m_pParent != nullptr)
					{
						if (m_pParent->m_pLeft == this)
							m_pParent->m_pLeft = pNew_Root;
						else
							m_pParent->m_pRight = pNew_Root;
					}
					else
						io_pTree_Root = pNew_Root;
					// update parents
					pNew_Root->m_pParent = m_pParent;
					m_pParent = pNew_Root;
				} //if (m_pLeft != nullptr)
			} //function nodetype *rotate_right(void)

			///
			/// rotate the tree based at the current node to the left
			///
			void rotate_left(node_p * &io_pTree_Root) noexcept
			{ 
				if (m_pRight != nullptr)
				{
					node_p pNew_Root = m_pRight; 
					node_p pLeftOfRight = pNew_Root->m_pLeft; 

					// Perform rotation 
					pNew_Root->m_pLeft = const_cast<node_p>(this); 
					m_pRight = pLeftOfRight; 
					// update link from parent if this node is not the root
					if (m_pParent != nullptr)
					{
						if (m_pParent->m_pLeft == this)
							m_pParent->m_pLeft = pNew_Root;
						else
							m_pParent->m_pRight = pNew_Root;
					}
					else
						io_pTree_Root = pNew_Root;
					// update parents
					pNew_Root->m_pParent = m_pParent;
					m_pParent = pNew_Root;
				} // if (m_pRight != nullptr)
			} //function node<T> *rotate_left(void)
		};
		///
		/// a node within the tree
		///
		template <class T> class avl_node : node<T>
		{ 
		public:
			typedef avl_node<T>			avl_node_p;
		protected: 
			int							m_nHeight; ///< height of the tree from this node
		protected:

		public:
			///
			/// void constructor; create an empty, valueless node (i.e. no children or parent; key  constructed with default constructor)
			///
			avl_node(void) noexcept(noexcept(T(std::declval<T>()))) : node<T>::m_tKey(), m_nHeight(1)
			{
				;
			}
			avl_node(T i_tKey) noexcept(noexcept(T(std::declval<T>()))) : node<T>::m_tKey(i_tKey), m_nHeight(1)
			{
				;
			}

			int get_height(void)
			{
				return m_nHeight;
			}
			void update_height(void)
			{
				int iLeft_Height = 0, iRight_Height = 0;
				if (node<T>::m_pLeft != nullptr)
				{
					iLeft_Height = node<T>::m_pLeft;
				}
				if (node<T>::m_pRight != nullptr)
				{
					iRight_Height = node<T>::m_pRight;
				}
				m_nHeight = std::max(iLeft_Height,iRight_Height) + 1;
			}
			void update_height_recursive(void)
			{
				int iLeft_Height = 0, iRight_Height = 0;
				avl_node_p pLeft = reinterpret_cast<avl_node_p>(node<T>::m_pLeft);
				avl_node_p pRight = reinterpret_cast<avl_node_p>(node<T>::m_pRight);
				if (pLeft != nullptr)
				{
					pLeft->update_height_recursive();
					iLeft_Height = pLeft->m_nHeight;
				}
				if (pRight != nullptr)
				{
					pRight->update_height_recursive();
					iRight_Height = pRight->m_nHeight;
				}
				m_nHeight = std::max(iLeft_Height,iRight_Height) + 1;
			}
		};



		///
		/// insert a new node as a child of the current node
		///
		template <template<typename> typename nodetype, class T> nodetype<T> * insert(nodetype<T> * &io_pRoot,T i_tNew_Key) noexcept(noexcept(T(std::declval<T>())))
		{
			nodetype<T> * pRet = nullptr;
			if (io_pRoot != nullptr)
			{
				nodetype<T> * pParent = io_pRoot;
				while (pRet == nullptr && pParent->get_key() != i_tNew_Key)
				{
					if (i_tNew_Key < pParent->get_key())
					{
						if (pParent->get_left() == nullptr)
							pParent->set_left(pRet = new nodetype<T>(pParent,i_tNew_Key));
						else
							pParent = pParent->get_left();
					}
					else if (i_tNew_Key > pParent->get_key())
					{
						if (pParent->get_right() == nullptr)
							pParent->set_right(pRet = new nodetype<T>(pParent,i_tNew_Key));
						else
							pParent = pParent->get_right();
					}
				}
			}
			else
				pRet = io_pRoot = new nodetype<T>(nullptr,i_tNew_Key);
			return pRet;
		}
		///
		/// find the node within a tree that contains the given value
		/// \returns a pointer to the node containing the value
		///
		template<template<typename> typename nodetype, class T> nodetype<T> * search(const nodetype<T> * i_pRoot,T i_tKey) noexcept
		{
			const nodetype<T> * pRet = i_pRoot;
			while (pRet != nullptr && pRet->get_key() != i_tKey)
			{
				if (pRet->get_key() < i_tKey)
					pRet = pRet->get_left();
				else
					pRet = pRet->get_right();
			}
			return const_cast<nodetype<T>*>(pRet);
		}
		///
		/// insert a new node as a child of the current node
		///
		template <template<typename> typename nodetype, class T> nodetype<T> * insert_avl(nodetype<T> * &io_pRoot,T i_tNew_Key) noexcept(noexcept(T(std::declval<T>())))
		{
			// perform standard BST insert
			nodetype<T> * pRet = insert<nodetype,T>(io_pRoot,i_tNew_Key);
			// update heights of node and parents
			nodetype<T> * pParent = pRet->get_parent();
			while (pParent != nullptr)
			{
				pParent->update_height();
				pParent->balance(io_pRoot);
				pParent = pParent->get_parent();
			}

			return pRet;
		}
		///
		/// create an identical copy of the tree stating at the current node
		/// \returns a pointer to the new tree
		///

		template <template<typename> typename nodetype, class T> nodetype<T> * create_node_copy(nodetype<T> * i_pNode_To_Copy, nodetype<T> * i_pParent) noexcept(noexcept(nodetype<T>(std::declval<nodetype<T> *>(),std::declval<T>())))
		{
			nodetype<T> * pRet = new nodetype<T>(i_pParent,i_pNode_To_Copy->get_key());
			if (i_pNode_To_Copy->get_left() != nullptr)
				pRet->set_left(create_node_copy<nodetype,T>(i_pNode_To_Copy->get_left(),pRet));
			if (i_pNode_To_Copy->get_right() != nullptr)
				pRet->set_right(create_node_copy<nodetype,T>(i_pNode_To_Copy->get_right(),pRet));
			return pRet;
		}

		///
		/// base class for tree iterators
		///
		template<template<typename> typename treetype, template<typename> typename nodetype, class datatype> class iterator_base
		{
		private:
			bool m_bLock_Type_Write; ///< type of lock to hold on the tree; true indicates a write lock, false indicates a read lock
		public:
		protected:
			const treetype<datatype> * m_pTree; ///< the tree that is being iterated over
			nodetype<datatype> * m_pCursor; ///< a cursor pointing to the current data location within the tree
		public:
			///
			/// void constructor (deleted)
			///
			iterator_base(void) = delete;
			///
			/// constructor that initializes the iterator and aquires a read lock on the tree
			///
			iterator_base(
				const treetype<datatype> & i_cTree, ///< the tree to iterate over
				binary_trees::start_point i_eStart_Point, ///< the starting point to use within the tree (beginning or end)
				bool i_bLock_Type_Write ///< flag to indicate lock type; true indicates write lock, false indicates read lock
				)  noexcept: m_pTree(&i_cTree)
			{
				m_bLock_Type_Write = i_bLock_Type_Write;
				if (m_bLock_Type_Write)
					m_pTree->m_mMutex.write_lock();
				else
					m_pTree->m_mMutex.read_lock();
				set_position(i_eStart_Point);
			}
			///
			/// constructor that initializes the iterator and aquires a read lock on the tree
			///
			iterator_base(
				const treetype<datatype> &i_cTree, ///< the tree to iterate over
				nodetype<datatype> * i_pCursor, ///< the starting point to use within the tree
				bool i_bLock_Type_Write ///< flag to indicate lock type; true indicates write lock, false indicates read lock
				)  noexcept: m_pTree(&i_cTree)
			{
				m_bLock_Type_Write = i_bLock_Type_Write;
				m_pTree = &i_cTree;
				if (m_bLock_Type_Write)
					m_pTree->m_mMutex.write_lock();
				else
					m_pTree->m_mMutex.read_lock();
				m_pCursor = i_pCursor;
			}
			///
			/// copy constructor (deleted)
			///
			iterator_base( const iterator_base & i_cRHO) = delete;

			///
			/// destructor: releases lock
			///
			virtual ~iterator_base(void)
			{
				if (m_bLock_Type_Write)
					m_pTree->m_mMutex.write_unlock();
				else
					m_pTree->m_mMutex.read_unlock();
			}
			///
			/// assign operator (deleted)
			///
			iterator_base & operator =( const iterator_base & i_cRHO) = delete;

			///
			/// set the position of the iterator to a selected point (beginning, end, or root of the tree)
			///
			virtual void set_position(binary_trees::start_point i_eStart_Point) noexcept
			{
				// set cursor
				m_pCursor = m_pTree->m_pRoot;
				switch (i_eStart_Point)
				{
				case binary_trees::start_point::end:
					{
						nodetype<datatype> * pCurr = m_pTree->m_pRoot;
						while (pCurr != nullptr)
						{
							m_pCursor = pCurr;
							pCurr = pCurr->get_right();
						}
					}
					break;
				case binary_trees::start_point::beginning:
					{
						nodetype<datatype> * pCurr = m_pTree->m_pRoot;
						while (pCurr != nullptr)
						{
							m_pCursor = pCurr;
							pCurr = pCurr->get_left();
						}
					}
					break;
				case binary_trees::start_point::root:
				default:
					break;
				}
			}
			///
			/// up ; moves the iterator to the parent of the current node
			/// \returns a reference to this iterator
			///
			virtual iterator_base & up(void) noexcept
			{
				if (m_pCursor != nullptr)
					m_pCursor = m_pCursor->get_parent();
				return *this;
			}

			///
			/// moves the iterator to the left child of the current node
			/// \returns a reference to this iterator
			///
			virtual iterator_base & left(void) noexcept
			{
				if (m_pCursor != nullptr)
					m_pCursor = m_pCursor->get_left();
				return *this;
			}
			
			///
			/// moves the iterator to the right child of the current node
			/// \returns a reference to this iterator
			///
			virtual iterator_base & right(void) noexcept
			{
				if (m_pCursor != nullptr)
					m_pCursor = m_pCursor->get_right();
				return *this;
			}

			///
			/// pre-increment operator; advances the iterator within the tree; will not advance past the end of the tree
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator ++ (void) noexcept
			{
				if (m_pCursor != nullptr)
				{
					if (m_pCursor->get_right() != nullptr)
					{
						m_pCursor = m_pCursor->get_right();
						nodetype<datatype> * pLeft = m_pCursor->get_left();
						while (pLeft != nullptr)
						{
							m_pCursor = pLeft;
							pLeft = m_pCursor->get_left();
						}
					}
					else
					{
						nodetype<datatype> * pChild = m_pCursor;
						nodetype<datatype> * pParent = m_pCursor->get_parent();
						while (pParent != nullptr && pParent->get_right() == pChild)
						{
							pChild = pParent;
							pParent = pParent->get_parent();
						}
						m_pCursor = pParent;
					}
				}
				return *this;
			}
			///
			/// pre-decrement operator; rewinds the iterator within the tree; will not rewind past the start of the tree
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator -- (void) noexcept
			{
				if (m_pCursor != nullptr)
				{
					if (m_pCursor->get_left() != nullptr)
					{
						m_pCursor = m_pCursor->get_left();
						nodetype<datatype> * pRight = m_pCursor->get_right();
						while (pRight != nullptr)
						{
							m_pCursor = pRight;
							pRight = m_pCursor->get_right();
						}
					}
					else
					{
						nodetype<datatype> * pChild = m_pCursor;
						nodetype<datatype> * pParent = m_pCursor->get_parent();
						while (pParent != nullptr && pParent->get_left() == pChild)
						{
							pChild = pParent;
							pParent = pParent->get_parent();
						}
						m_pCursor = pParent;
					}
				}
				return *this;
			}
			///
			/// add-assign operator; advances the iterator within the tree; will not advance past the end of the tree or prior to the start of the data
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator += (int i_nValue) noexcept
			{
				size_t nCount = std::abs(i_nValue);
				for (size_t nI = 0; m_pCursor != nullptr && nI < nCount; nI++)
				{
					if (i_nValue < 0)
						--(*this);
					else
						++(*this);
				}
				return *this;
			}
			///
			/// subtract-assign operator; advances the iterator within the tree; will not advance past the end of the tree or prior to the start of the data
			/// \returns a reference to this iterator
			///
			virtual iterator_base & operator -= (int i_nValue) noexcept
			{
				size_t nCount = std::abs(i_nValue);
				for (size_t nI = 0; m_pCursor != nullptr && nI < nCount; nI++)
				{
					if (i_nValue < 0)
						++(*this);
					else
						--(*this);
				}
				return *this;
			}
			///
			/// equality operator
			/// \returns true if both iterators are pointing to the same tree and are pointing to the same location within the tree
			///
			virtual bool operator ==(const iterator_base & i_cRHO) noexcept
			{
				return (m_pTree == i_cRHO.m_pTree && m_pCursor == i_cRHO.m_pCursor);
			}
			///
			/// inequality operator
			/// \returns true if the iterators are not pointing to the same tree or are not pointing to the same location within one tree
			///
			virtual bool operator !=(const iterator_base & i_cRHO) noexcept
			{
				return (m_pTree != i_cRHO.m_pTree || m_pCursor != i_cRHO.m_pCursor);
			}
			///
			/// retrive the value at the current location of the iterator; if the iterator is not pointing to valid data will return type T with default contructor
			/// \returns the data at the current location of the iterator if the iterator is valid
			///
			virtual datatype load(void) noexcept(noexcept(datatype()))
			{
				datatype tRet = datatype();
				if (m_pCursor != nullptr)
					tRet = m_pCursor->get_key();
				return tRet;
			}
			///
			/// determine if the iterator is pointing to a valid place within the tree
			/// \returns true if the iterator is valid; false otherwise
			///
			virtual bool is_valid(void) noexcept
			{
				return m_pCursor != nullptr;
			}
		};

		///
		/// class for iterating through a tree; unlike typical iterators this class is scoped and holds a read lock on the tree to ensure that the tree does not change data or size during iteration
		///
		template <template<typename> typename treetype, template<typename> typename nodetype, class datatype> class read_iterator : public iterator_base<treetype,nodetype,datatype>
		{
		public:
			///
			/// default constructor (deleted)
			///
			read_iterator(void) = delete;
			///
			/// constructor that initializes the iterator and aquires a read lock on the tree; blocking (read)
			///
			read_iterator(
				const treetype<datatype> & i_cTree, ///< the tree to iterate over
				enum binary_trees::start_point i_eStart_Point ///< the starting point to use within the tree (beginning or end)
				)  noexcept : iterator_base<treetype,nodetype,datatype>(i_cTree,i_eStart_Point,false)
			{
			}
			///
			/// copy constructor; aquires an additional read lock on the tree; blocking (read)
			///
			read_iterator(
				const read_iterator<treetype,nodetype,datatype> & i_cIterator ///< the iterator to copy
				)  noexcept : iterator_base<treetype,nodetype,datatype>(*i_cIterator.m_pTree,binary_trees::start_point::beginning,false)
			{
				iterator_base<treetype,nodetype,datatype>::m_pCursor = i_cIterator.m_pCursor;
			}
			///
			/// assignment operator; releases lock on existing tree and aquires a read lock on the tree that the right hand iterator refers to; blocking (read)
			///
			read_iterator & operator = (
					const read_iterator<treetype,nodetype,datatype> & i_cIterator ///< the iterator to copy
				) noexcept
			{
				iterator_base<treetype,nodetype,datatype>::m_pTree->m_mMutex.read_unlock();
				iterator_base<treetype,nodetype,datatype>::m_pTree = i_cIterator.m_pTree;
				iterator_base<treetype,nodetype,datatype>::m_pTree->m_mMutex.read_lock();
				iterator_base<treetype,nodetype,datatype>::m_pCursor = i_cIterator.m_pCursor;
				return *this;
			}

			///
			/// post-increment operator; advances the iterator within the tree; will not advance past the end of the tree
			/// \returns a reference to this iterator
			///
			read_iterator<treetype,nodetype,datatype> operator ++ (int i_nValue) noexcept
			{
				read_iterator<treetype,nodetype,datatype> cRet(*this);
				iterator_base<treetype,nodetype,datatype>::operator++();
				return cRet;
			}
			///
			/// post-decrement operator; rewinds the iterator within the tree; will not rewind past the start of the tree
			/// \returns a reference to this iterator
			///
			read_iterator<treetype,nodetype,datatype> operator -- (int i_nValue) noexcept
			{
				read_iterator<treetype,nodetype,datatype> cRet(*this);
				iterator_base<treetype,nodetype,datatype>::operator--();
				return cRet;
			}
		};


		template <template<typename> typename treetype, class datatype> class control_base
		{
		private:
			bool m_bLock_Type_Write; // type of lock to hold on the tree; true indicates a write lock, false indicates a read lock
		protected:
			treetype<datatype> * m_pTree; ///< reference to the tree to control
		public:
			///
			/// default contructor (deleted)
			///
			control_base(void) = delete;
			///
			/// contructor: tie the read control to a particular tree and lock the tree for read; blocking
			///
			control_base(
				treetype<datatype> & i_cTree, ///< the tree to be accessed
				bool i_bLock_Type_Write ///< flag to indicate the type of lock control to assume
				)  noexcept: m_pTree(&i_cTree)
			{
				m_bLock_Type_Write = i_bLock_Type_Write;
				if (m_bLock_Type_Write)
					m_pTree->m_mMutex.write_lock();
				else
					m_pTree->m_mMutex.read_lock();

			}
			///
			/// copy contructor (deleted)
			///
			control_base(const control_base<treetype,datatype> & i_cIterator)   = delete;
			///
			/// destructor: release read lock 
			///
			~control_base(void) noexcept
			{
				if (m_bLock_Type_Write)
					m_pTree->m_mMutex.write_unlock();
				else
					m_pTree->m_mMutex.read_unlock();
			}
			///
			/// assignment / copy operator (deleted)
			///
			control_base & operator = (const control_base<treetype,datatype> & i_cIterator) = delete;
			///
			/// assignement operator; releases read access to existing tree and aquires read access to a different tree; blocking(Read)
			///
			control_base & operator =(const treetype<datatype> & i_cTree) noexcept
			{
				if (&i_cTree != m_pTree) // don't do anything if being set to the tree that is already under control)
				{
					if (m_bLock_Type_Write)
					{
						i_cTree.m_mMutex.write_lock();
						m_pTree->m_mMutex.write_unlock();
					}
					else
					{
						i_cTree.m_mMutex.read_lock();
						m_pTree->m_mMutex.read_unlock();
					}
					control_base::m_pTree = &i_cTree;
				}
				return *this;
			}
			///
			/// test if the tree is empty
			/// \returns true if the tree is empty; false otherwise
			///
			bool empty(void) const noexcept
			{
				return m_pTree->nl_empty();
			}
			///
			/// test if a value exists within the tree
			/// \returns true if the key is within the tree; false otherwise
			///
			bool has_key(datatype i_tT) const  noexcept
			{
				return m_pTree->nl_has_key(i_tT);
			}
		};

		///
		/// the write control class is designed to allow scoped write access to the tree that maintains a write lock throughout the scope. This is useful when many writes occur
		///
		template <template<typename> typename treetype, class datatype> class read_control : public control_base<treetype,datatype>
		{
		public:
			///
			/// void constructor; deleted
			///
			read_control(void) = delete;
			///
			/// contructor: tie the read control to a particular tree and lock the tree for read; blocking
			///
			read_control(
				treetype<datatype> & i_cTree ///< the tree to be accessed
				)  noexcept: control_base<treetype,datatype>(i_cTree,false)
			{
				;
			}

			///
			/// copy contructor; creates a new read_control on a tree that an existing read control is accessing; blocking(read)
			///
			read_control(const read_control<treetype,datatype> & i_cController) noexcept : control_base<treetype,datatype>(*i_cController.m_pTree,false)
			{
				;
			}
			///
			/// assignement operator; releases read access to existing tree and aquires read access to tree referred to by other controller; blocking(Read)
			///
			read_control & operator =(const read_control<treetype,datatype> & i_cController) noexcept
			{
				i_cController.m_pTree->m_mMutex.read_lock();
				control_base<treetype,datatype>::m_pTree->m_mMutex.read_unlock();
				control_base<treetype,datatype>::m_pTree = i_cController.m_pTree;
				return *this;
			}
		};
			


		///
		/// the write control class is designed to allow scoped write access to the tree that maintains a write lock throughout the scope. This is useful when many writes occur
		///
		template <template<typename> typename treetype, class datatype> class write_control : public control_base<treetype,datatype>
		{
		public:
			///
			/// default contructor (deleted)
			///
			write_control(void) = delete;
			///
			/// contructor: tie the read control to a particular tree and lock the tree for read; blocking
			///
			write_control(
				treetype<datatype> & i_cTree ///< the tree to be accessed
				)  noexcept: control_base<treetype,datatype>(i_cTree,true)
			{
			}
			///
			/// copy contructor (deleted)
			///
			write_control(const write_control<treetype,datatype> & i_cIterator)   = delete;
			///
			/// assignment / copy operator (deleted)
			///
			write_control & operator = (const write_control<treetype,datatype> & i_cIterator) = delete;
			///
			/// store the value at the current location of the iterator; if the iterator is not pointing to valid data the request will be ignored
			///
			void insert(
				const datatype& i_tT ///< the data to be stored
				) noexcept
			{
				control_base<treetype,datatype>::m_pTree->nl_insert(i_tT);
			}
			///
			/// erase a value from the tree
			///
			void erase(
				const datatype& i_tT ///< the data to be stored
				) noexcept
			{
				control_base<treetype,datatype>::m_pTree->nl_erase(i_tT);
			}
			///
			/// clear the tree; 
			///
			void clear(void)
			{
				control_base<treetype,datatype>::m_pTree->nl_clear();
			}

		};
		template <template<typename>typename Tnode, class T> class basic_binary_tree_insert_erase
		{
		public:
			typedef Tnode<T>																nodetype;
			typedef T																		datatype;
		protected:
			nodetype * 					m_pRoot; ///< the root of the tree
			mutable read_write_mutex	m_mMutex; ///< a read-write mutex for control of insertion, erasing, clearing, and searching the tree
			/// insert a potentially new key into the tree
			///
			virtual void nl_insert(T i_tKey) noexcept(noexcept(T(std::declval<T>())))
			{
				binary_tree_helper::insert<binary_tree_helper::node,datatype>(m_pRoot,i_tKey);
			}
			///
			/// erase a node in the tree based on its key
			///
			virtual void nl_erase(T i_tKey) noexcept(noexcept(std::declval<T>().~T()))
			{
				nodetype * pSearch_Result = nullptr;
				if (m_pRoot != nullptr)
					pSearch_Result = m_pRoot->search(i_tKey);
				if (pSearch_Result != nullptr) // make sure the key is in the tree
				{
					nodetype * pParent = pSearch_Result->get_parent();

					// perform a right pivot with delete
					nodetype * pNew_Root = pSearch_Result->get_right();
					if (pNew_Root != nullptr)
					{
						// traverse the left children of the new root to find where to place the left children
						nodetype * pCurr = pNew_Root->get_left();
						nodetype * pLast = pNew_Root;
						while (pCurr != nullptr)
						{
							pLast = pCurr;
							pCurr = pCurr->get_left();
						}
						// add the left children of the deleted node to the left children of the identified edge
						pLast->set_left(pSearch_Result->get_left());
					}
					else // no right children; use the left child as the new root
					{
						pNew_Root = pSearch_Result->get_left();
					}
					// delete the erased node
					pSearch_Result->set_left(nullptr);
					pSearch_Result->set_right(nullptr);
					delete pSearch_Result;

					// place the new root and set its parent
					if (pNew_Root != nullptr)
						pNew_Root->set_parent(pParent);
					if (pParent != nullptr)
					{
						if (i_tKey < pParent->get_key()) // replace the left child
							pParent->set_left(pNew_Root);
						else
							pParent->set_right(pNew_Root);
					}
					else /// no parent and the key exists -- we are erasing the root of the tree
						m_pRoot = pNew_Root;
				}
			}
		};

		///
		/// a basic binary tree that doesn't self-balance
		///
		template <template<typename>typename Tnode, class T> class safe_binary_tree
		{
		public:
			typedef safe_binary_tree<Tnode,T>												treetype;
			typedef Tnode<T>																nodetype;
			typedef T																		datatype;
		protected:
			nodetype * 					m_pRoot; ///< the root of the tree
			mutable read_write_mutex	m_mMutex; ///< a read-write mutex for control of insertion, erasing, clearing, and searching the tree


		public:

			///
			/// void constructor; creates an empty tree; blocking(write)
			///
			safe_binary_tree(void) noexcept
			{
				m_pRoot = nullptr;
			}
			///
			/// copy constructor; creates a tree that is identical to an existing tree; blocking(read/write)
			///
			safe_binary_tree(const treetype & i_cRHO) noexcept(noexcept(T(std::declval<datatype>())))
			{
				write_lock_guard cLock(m_mMutex);
				m_pRoot = nullptr;
				nl_copy(i_cRHO);
			}
			///
			/// destructor; destroys the tree; blocking(write)
			///
			virtual ~safe_binary_tree(void) noexcept(noexcept(std::declval<datatype>().~datatype()))
			{
				write_lock_guard cLock(m_mMutex);
				if (m_pRoot != nullptr)
					delete m_pRoot;
				m_pRoot = nullptr;
			}
			///
			/// copy constructor; creates a tree that is identical to an existing tree; blocking(read/write)
			///
			safe_binary_tree & operator=(const treetype & i_cRHO) noexcept(noexcept(datatype(std::declval<datatype>())))
			{
				write_lock_guard cLock(m_mMutex);
				nl_copy(i_cRHO);
				return *this;
			}

		protected:
			///
			/// create a copy of an existing tree; blocking(read on tree to copy)
			///
			void nl_copy(const treetype & i_cRHO) noexcept(noexcept(datatype(std::declval<datatype>())))
			{
				read_lock_guard cLock(i_cRHO.m_mMutex);
				if (m_pRoot != nullptr)
					delete m_pRoot;
				m_pRoot = nullptr;
				if (i_cRHO.m_pRoot != nullptr)
					m_pRoot = binary_tree_helper::create_node_copy<Tnode,datatype>(i_cRHO.m_pRoot,nullptr);
			}
			///
			/// determine if a key exists within the tree; non-blocking
			/// \returns true if the key exists within the tree; false otherwise
			///
			bool nl_has_key(datatype i_tKey) const noexcept
			{
				nodetype * pSearch_Result = nullptr;
				if (m_pRoot != nullptr)
					pSearch_Result = binary_tree_helper::search<Tnode,T>(m_pRoot,i_tKey);
				return (pSearch_Result != nullptr);
			}
			

			///
			/// clear the tree; non-blocking
			///
			void nl_clear(void) noexcept(noexcept(std::declval<datatype>().~datatype()))
			{
				if (m_pRoot != nullptr)
					delete m_pRoot;
				m_pRoot = nullptr;
			}

			///
			/// determine if the tree is empty
			///
			bool nl_empty(void) const noexcept
			{
				return m_pRoot == nullptr;
			}

		public:
			///
			/// insert a new value into the key
			///
			void insert(T i_tKey) noexcept(noexcept(T(std::declval<T>())))
			{
				write_lock_guard cLock(m_mMutex);
				treetype::nl_insert(i_tKey);
			}
			///
			/// erases a given key from the tree
			///
			void erase(datatype i_tKey) noexcept(noexcept(std::declval<datatype>().~datatype()))
			{
				write_lock_guard cLock(m_mMutex);
				treetype::nl_erase(i_tKey);
			}
			///
			/// deletes all nodes in the tree
			///
			void clear(void) noexcept(noexcept(std::declval<datatype>().~datatype()))
			{
				write_lock_guard cLock(m_mMutex);
				nl_clear();
			}
			///
			/// determine if a given key is in the tree
			/// \returns true if the key is in the tree; false otherwise
			///
			bool has_key(datatype i_tKey) const noexcept
			{
				read_lock_guard cLock(m_mMutex);
				return nl_has_key(i_tKey);
			}
			
			///
			/// determine if the tree is empty
			/// \returns true if the tree is empty; false otherwise
			///
			bool empty(void) const noexcept
			{
				read_lock_guard cLock(m_mMutex);
				return nl_empty();
			}


		};

	}
}
#endif // #ifndef XSTDTSL_BINARY_TREE_HELPER_H


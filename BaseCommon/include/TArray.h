#include "OEMBase.h"
#include "TypesBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace oes
{
namespace common_base
{

#define appFree				free
#define appRealloc			realloc

void* appMemmove(void* Dest, const void* Src, size_t Count);
void  appMemset(void* Dest, int C, size_t Count);
void  appMemzero(void* Dest, size_t Count);
void  appMemcpy(void* Dest, const void* Src, size_t Count);
void  appMemswap(void* Ptr1, void* Ptr2, size_t Size);

template <class T> struct TTypeInfoBase
{
public:
	typedef const T& ConstInitType;
	static bool NeedsDestructor() { return 1; }
	static bool DefinitelyNeedsDestructor() { return 0; }
	static const T& ToInit( const T& In ) { return In; }
};

template <class T> struct TTypeInfo : public TTypeInfoBase<T>
{
};

template <> struct TTypeInfo<TUInt8> : public TTypeInfoBase<TUInt8>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<signed char> : public TTypeInfoBase<signed char>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<char> : public TTypeInfoBase<char>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<int> : public TTypeInfoBase<int>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<TUInt32> : public TTypeInfoBase<TUInt32>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<unsigned short> : public TTypeInfoBase<unsigned short>
{
public:
	static bool NeedsDestructor() { return 0; }
};

template <> struct TTypeInfo<short int> : public TTypeInfoBase<short int>
{
public:
	static bool NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<unsigned __int64> : public TTypeInfoBase<unsigned __int64>
{
public:
	static bool NeedsDestructor() { return 0; }
};
template <> struct TTypeInfo<signed __int64> : public TTypeInfoBase<signed __int64>
{
public:
	static bool NeedsDestructor() { return 0; }
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class Array
{

public:
	Array()
		: ArrayNum(0)
		, ArrayMax(0)
		, Data(0)
	{
	}

	Array(int InNum, int ElementSize)
		: ArrayNum(InNum)
		, ArrayMax(InNum)
		, Data(nullptr)
	{
		Realloc(ElementSize);
	}

	~Array()
	{
		if (Data){
            free(Data);
		}

		Data = nullptr;
		ArrayNum = ArrayMax = 0;
	}

	void* GetData()
	{
		return Data;
	}

	const void* GetData() const
	{
		return Data;
	}

	bool CheckIndex(int Index) const
	{
		return Index >= 0 && Index < ArrayNum;
	}

	inline_ int Num() const
	{
		assert(ArrayNum >= 0);
		assert(ArrayMax >= ArrayNum);
		return ArrayNum;
	}

	void InsertZeroed(int Index, int Count, int ElementSize)
	{
		Insert(Index, Count, ElementSize);
		appMemzero((TUInt8*)Data + Index * ElementSize, Count * ElementSize);
	}

	void Insert(int Index, int Count, int ElementSize)
	{
		assert(Count > 0);
		assert(ArrayNum >= 0);
		assert(ArrayMax >= ArrayNum);
		assert(Index >= 0);
		assert(Index <= ArrayNum);

		int OldNum = ArrayNum;
		if ((ArrayNum += Count) > ArrayMax)
		{
			ArrayMax = ArrayNum + 3 * ArrayNum / 8 + 32;
			Realloc(ElementSize);
		}

		memmove(
			(TUInt8*)Data + (Index + Count) * ElementSize,
			(TUInt8*)Data + (Index) * ElementSize,
			(OldNum - Index) * ElementSize
			);
	}

	int Add(int Count, int ElementSize)
	{
		assert(Count >= 0);
		assert(ArrayNum >= 0);
		assert(ArrayMax >= ArrayNum);

		int Index = ArrayNum;
		if ((ArrayNum += Count) > ArrayMax)
		{
			ArrayMax = ArrayNum + 3 * ArrayNum / 8 + 32;
			Realloc(ElementSize);
		}
		return Index;
	}

	int AddZeroed(int ElementSize, int Num = 1)
	{
		int Index  = Add(Num, ElementSize);
		appMemzero((TUInt8*)Data + Index * ElementSize, Num * ElementSize);
		return Index;
	}

	void Shrink(int ElementSize)
	{
		assert(ArrayNum >= 0);
		assert(ArrayMax >= ArrayNum);
		if (ArrayMax != ArrayNum)
		{
			ArrayMax = ArrayNum;
			Realloc(ElementSize);
		}
	}

	void Empty(int ElementSize, int Stack = 0)
	{
		ArrayNum = 0;
		ArrayMax = Stack;
		Realloc(ElementSize);
	}

	void Swap(int A, int B, int ElementSize)
	{
		appMemswap((TUInt8*)Data + (ElementSize * A), (TUInt8*)Data + (ElementSize * B), ElementSize);
	}

	void Remove(int Index, int Count, int ElementSize )
	{
		if (Count)
		{
			memmove
				(
				(TUInt8*)Data + (Index      ) * ElementSize,
				(TUInt8*)Data + (Index+Count) * ElementSize,
				(ArrayNum - Index - Count ) * ElementSize
				);
			ArrayNum -= Count;
			
			if ((3 * ArrayNum < 2 * ArrayMax || (ArrayMax - ArrayNum) * ElementSize >= 16384)
				&&	(ArrayMax - ArrayNum > 64 || ArrayNum == 0))
			{
				ArrayMax = ArrayNum;
				Realloc( ElementSize );
			}
		}
		assert(ArrayNum>=0);
		assert(ArrayMax>=ArrayNum);
	}


protected:
	void Realloc( int ElementSize )
	{
		// Avoid calling appRealloc( NULL, 0 ) as ANSI C mandates returning a valid pointer which is not what we want.
		if (Data || ArrayMax){
			Data = realloc(Data, ArrayMax * ElementSize);
		}
	}

	void	*Data;
	int		ArrayNum;
	int		ArrayMax;
};


//------------------------------------------------------------
// Templated dynamic array.
//------------------------------------------------------------
template<class T> class TArray : public Array
{
public:
	typedef T ElementType;

	TArray()
	:	Array()
	{
	}

	TArray(int InNum)
	:	Array(InNum, sizeof(T))
	{
	}

	TArray(const TArray& Other)
	:	Array(Other.ArrayNum, sizeof(T))
	{
		if (TTypeInfo<T>::NeedsDestructor())
		{
			ArrayNum=0;
			for(int i = 0; i < Other.ArrayNum; ++i){
				new(*this)T(Other(i));
			}
		}
		else if(sizeof(T) != 1)
		{
			for(int i=0; i<ArrayNum; ++i){
				(*this)(i) = Other(i);
			}
		}
		else
		{
			appMemcpy(&(*this)(0), &Other(0), ArrayNum * sizeof(T));
		}
	}
	//TArray( ENoInit )
	//: FArray( E_NoInit )
	//{}

	~TArray()
	{
		assert(ArrayNum >= 0);
		assert(ArrayMax >= ArrayNum);
		Remove(0, ArrayNum);

// #if (defined _MSC_VER) && (defined _DEBUG)
// 		//@todo it were nice if we had a cleaner solution for DebugGet
// 		volatile const T* Dummy = DebugGet(-1);
// 	}
// 	/**
// 	 * Helper function that can be used inside the debuggers watch window to debug TArrays. E.g. "*Class->Defaults.DebugGet(5)". 
// 	 *
// 	 * @param	i	Index
// 	 * @return		pointer to type T at Index i
// 	 */
// 	const T* DebugGet( int i ) const
// 	{
// 		if( i >= 0 )
// 			return &((T*)Data)[i];
// 		else
// 			return NULL;
// 	}
// #else
// 	}
// #endif
	}

	T& operator()(int i)
	{
		assert(i >= 0);
		assert(ArrayMax>=ArrayNum);
		return ((T*)Data)[i];
	}

	const T& operator()(int i ) const
	{
		assert(i>=0);
		assert(i<=ArrayNum);
		assert(ArrayMax>=ArrayNum);
		return ((T*)Data)[i];
	}

	T Pop()
	{
		assert(ArrayNum>0);
		assert(ArrayMax>=ArrayNum);
		
		T Result = ((T*)Data)[ArrayNum - 1];
		Remove(ArrayNum - 1);
		return Result;
	}

	T& Last(int c = 0)
	{
		assert(c < ArrayNum);
		assert(ArrayMax >= ArrayNum);
		return ((T*)Data)[ArrayNum - c - 1];
	}

	const T& Last( int c = 0 ) const
	{
		assert(c < ArrayNum);
		assert(ArrayMax >= ArrayNum);
		return ((T*)Data)[ArrayNum - c - 1];
	}

	void Shrink()
	{
		Array::Shrink(sizeof(T));
	}

	bool FindItem(const T& Item, int& Index) const
	{
		for (Index = 0; Index < ArrayNum; Index++)
		{
			if( (*this)(Index)==Item ){
				return 1;
			}
		}
		return 0;
	}
	
	int FindItemIndex(const T& Item) const
	{
		for (int Index = 0; Index < ArrayNum; ++Index)
		{
			if ((*this)(Index) == Item){
				return Index;
			}
		}
		return INDEX_NONE;
	}

	bool ContainsItem( const T& Item ) const
	{
		return ( FindItemIndex(Item) != INDEX_NONE );
	}

	// Add, Insert, Remove, Empty interface.
	int Add(int n = 1)
	{
		assert(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		return Array::Add( n, sizeof(T) );
	}

	void Insert(int Index, int Count = 1)
	{
		assert(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		Array::Insert(Index, Count, sizeof(T));
	}

	void InsertZeroed(int Index, int Count = 1)
	{
		assert(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		Array::InsertZeroed(Index, Count, sizeof(T));
	}

	void Remove(int Index, int Count = 1)
	{
		assert(Index>=0);
		assert(Index<=ArrayNum);
		assert(Index+Count<=ArrayNum);

		if (TTypeInfo<T>::NeedsDestructor())
		{
			for (int i = Index; i < Index + Count; i++ )
			{
				(&(*this)(i))->~T();
			}
		}

		Array::Remove( Index, Count, sizeof(T) );
	}

	void Empty(int Slack = 0)
	{
		if (TTypeInfo<T>::NeedsDestructor())
		{
			for (int i=0; i<ArrayNum; i++)
			{
				(&(*this)(i))->~T();
			}
		}
		Array::Empty(sizeof(T), Slack);
	}

	// Functions dependent on Add, Remove.
	TArray& operator+(const TArray& Other)
	{
		if (this != &Other)
		{
			for (int i=0; i < Other.ArrayNum; ++i)
			{
				new( *this )T( Other(i) );
			}
		}
		return *this;
	}

	TArray& operator+=(const TArray& Other)
	{
		if (this != &Other)
		{
			*this = *this + Other;
		}
		return *this;
	}

	TArray& operator=( const TArray& Other )
	{
		if (this != &Other)
		{
			Empty(Other.ArrayNum);

			for (int i=0; i<Other.ArrayNum; ++i){
				new( *this )T( Other(i) );
			}
		}
		return *this;
	}

	int AddItem(const T& Item)
	{
		new(*this) T(Item);
		return Num() - 1;
	}

	int AddZeroed( int n = 1 )
	{
		return Array::AddZeroed(sizeof(T), n);
	}

	int AddUniqueItem( const T& Item )
	{
		for (int Index = 0; Index < ArrayNum; ++Index)
		{
			if( (*this)(Index)==Item )
				return Index;
		}
		return AddItem( Item );
	}

	int RemoveItem(const T& Item)
	{
		int OriginalNum = ArrayNum;
		for (int Index = 0; Index < ArrayNum; ++Index)
		{
			if ((*this)(Index) == Item)
				Remove(Index--);
		}
		return OriginalNum - ArrayNum;
	}

	void SwapItems(int A, int B)
	{
		Array::Swap(A, B, sizeof(T));
	}

	// Iterator.
	class TIterator
	{
	public:
		TIterator( TArray<T>& InArray ) : Array(InArray), Index(-1) { ++*this;      }
		void operator++()      { ++Index;                                           }
		void RemoveCurrent()   { Array.Remove(Index--); }
		int GetIndex()   const { return Index;                                      }
		operator int() const { return Index < Array.Num();                        }
		T& operator*()   const { return Array(Index);                               }
		T* operator->()  const { return &Array(Index);                              }
		T& GetCurrent()  const { return Array( Index );                             }
		T& GetPrev()     const { return Array( Index ? Index-1 : Array.Num()-1 );   }
		T& GetNext()     const { return Array( Index<Array.Num()-1 ? Index+1 : 0 ); }
	private:
		TArray<T>& Array;
		int Index;
	};
};

template<class T, unsigned int Max> struct TStaticArray
{
	T		        Elements[Max];
    unsigned int	Num;

	// Constructor.

	TStaticArray(): Num(0) {}

	// AddItem

    size_t AddItem(const T& Item)
	{
		assert(Num < Max);
		Elements[Num] = Item;
		return Num++;
	}

	// operator()

	T& operator()(size_t Index)
	{
		assert(Index < Num);
		return Elements[Index];
	}
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
// template<class T>
// class TArray
// {
// 	typedef typename std::vector<T>	TVec;
// 	typedef typename std::vector<T>::iterator TVecIterator;
// 
// public:
// 	TArray()
// 	{
// 	}
// 
// 	TArray(const TArray& Source)
// 	{
// 	}
// 
// 	~TArray()
// 	{
// 		VecArray.empty();
// 	}
// 
// 	const T& operator[](size_t Index) const
// 	{
// 		return VecArray[Index];
// 	}
// 
// 	size_t Size() const 
// 	{ 
// 		return VecArray.size(); 
// 	}
// 
// 	void AddItem(T &Value) 
// 	{ 
// 		VecArray.push_back(Value); 
// 	}
// 
// 	bool Erase(size_t Index)
// 	{
// 		bool bResult = false;
// 
// 		TVec::iterator Iter = VecArray.begin();
// 		std::advance(Iter, Index);
// 
// 		if (Iter != VecArray.end())
// 		{
// 			VecArray.erase(Iter);
// 			bResult = true;
// 		}
// 		return bResult;
// 	}
// 
// 	size_t AutoGenerate()
// 	{
// 		T Value;
// 		VecArray.push_back(Value);
// 		return Size() - 1;
// 	}
// 
// protected:
// private:
// 	TVec	VecArray;
// };


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T> class TNode
{
public:
	TNode()
		: m_pValue(0)
		, m_pNodeNext(0)
		, m_pNodeNextPlain(0)
		, m_pNodeParent(0)
		, m_NumChilds(0)
	{
	}
	TNode(const T* Value)
		: m_pValue(Value)
		, m_pNodeNext(0)
		, m_pNodeNextPlain(0)
		, m_pNodeParent(0)
		, m_NumChilds(0)
	{
	}

	T*		GetValue() const { return m_pValue; }
	void	SetValue(T* Value) { m_pValue = Value; }

	TNode<T>* GetNext() const { return m_pNodeNext; }
	TNode<T>* GetPlainNext() const { return m_pNodeNextPlain; }

	TNode<T>* GetRootNode() const { return m_pNodeParent; }
	size_t	GetNumChilds() const { return m_NumChilds; }

//private:
	TNode<T> *m_pNodeNext;
	TNode<T> *m_pNodeNextPlain;
	TNode<T> *m_pNodeParent;

	size_t	  m_NumChilds;
	
	T *m_pValue;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T> class TTreeArray
{
public:
	TTreeArray()
		: m_pFirstElement(0)
		, m_pLastElement(0)
	{
	}

	void PushBack(const T* Value, TNode<T>* NodeParent = 0)
	{
		if (Empty())
		{
			m_pFirstElement = m_pLastElement = new TNode<T>(Value);
		}
		else
		{
			TNode<T> *lNode = m_pLastElement; // push after last by default
			
			if (NodeParent)
			{
				lNode = NodeParent;
				size_t num = NodeParent->m_NumChilds;

				for (int Index = 0; Index < num; Index++){
					lNode = lNode->m_pNodeNextPlain;
				}
			}

			Insert(Value, lNode);
		}
	}

	void PushBack(const T* Value, const T* NodeParent = 0)
	{
		TNode<T> * lFindNode = 0;

		if (NodeParent)
		{
			TNode<T> * lFindNode = m_pFirstElement;
			while (lFindNode)
			{
				if (lFindNode->GetValue() == NodeParent){
					break;
				}
				lFindNode = lFindNode->m_pNodeNext;
			}
		}
		
		PushBack(Value, lFindNode);
	}

	void Insert(const T* Value, TNode<T> *lNode)
	{
		assert(lNode);

		// skip all plain childs
		TNode<T> *lInsNode = lNode;
		size_t num = lInsNode->m_NumChilds;

		for (int Index = 0; Index < num; ++Index){
			lInsNode = lInsNode->m_pNodeNextPlain;
		}

		TNode<T> *rNode = lNode->GetNext();

		// allocate new element
		TNode<T> * NewNode = new TNode<T>(Value);

		NewNode->m_pNodeNext = rNode;
		NewNode->m_pNodeNextPlain = lNode->m_pNodeNextPlain;
		NewNode->m_pNodeParent = lNode->m_pNodeParent;
		
		lNode->m_pNodeNextPlain = NewNode;

		if (NewNode->m_pNodeParent){
			NewNode->m_pNodeParent->m_NumChilds++;
		}

		if (!rNode){
			m_pLastElement = NewNode; // set last element
		}
	}

	void Erase(TNode<T> *Node)
	{
		if (Empty()){
			return;
		}

		TNode<T> * lNode = (Node->m_pNodeParent) ? Node->m_pNodeParent : m_pFirstElement;
		TNode<T> * rNode = Node->m_pNodeNext;

		if (lNode != Node)
		{
			while (lNode)
			{
				if (lNode->m_pNodeNext == Node){
					break;
				}
				lNode = lNode->m_pNodeNext;
			}
		}
		else{
			lNode = 0; // first element
		}
		
		size_t SubChildsCount = GetSubChildsCount(Node);
		if (SubChildsCount > 0)
		{
			while (SubChildsCount)
			{
				assert(rNode);

				TNode<T> * ptrDel = rNode;
				rNode = rNode->m_pNodeNext;
				delete ptrDel;

				--SubChildsCount;
			}
		}

		m_pLastElement = rNode;

		// decrement child count
		if (Node->m_pNodeParent){
			Node->m_pNodeParent->m_NumChilds--;
		}

		if (lNode)
		{
			lNode->m_pNodeNext = rNode;
			if (lNode != Node->m_pNodeParent) { // plain 
				lNode->m_pNodeNextPlain = rNode;
			}
		}
		else{
			m_pFirstElement = rNode;
		}
		delete Node;
	}

	void Erase(const T* Value)
	{
		TNode<T> * lFindNode = m_pFirstElement;
		while (lFindNode)
		{
			if (lFindNode->GetValue() == Value){
				break;
			}
			lFindNode = lFindNode->m_pNodeNext;
		}

		if (lFindNode) {
			Erase(lFindNode);
		}
	}

	void Clear()
	{
		TNode<T> *lNode = m_pFirstElement; // push after last by default

		while (lNode)
		{
			TNode<T> * rNext = lNode->m_pNodeNext;
			delete lNode;
			lNode = rNext;
		}
		m_pFirstElement = m_pLastElement = 0;
	}

	bool Empty() const { return m_pFirstElement != 0; }

private:
	size_t GetSubChildsCount(TNode<T> *Node)
	{
		size_t OutCount = Node->m_NumChilds;

		for (int Index = 0; Index < Node->m_NumChilds; ++Index)
		{
			TNode<T> *NodeChild = Node->m_pNodeNext;
			while(NodeChild)
			{
				OutCount += GetSubChildsCount(NodeChild);
				NodeChild = NodeChild->m_pNodeNextPlain;
			}
		}
		return OutCount;
	}


private:
	TNode<T> *m_pFirstElement;
	TNode<T> *m_pLastElement;
};


//----------------------------------------------------------------------------------------------
// Desc: Plain tree array
// KEY <-> VALUE node presentation
//----------------------------------------------------------------------------------------------
template<class T_KEY, class T_CLASS> class TNodeMap
{
public:
	TNodeMap()
		: m_pKey(0)
		, m_pValue(0)
		, m_pNodeNext(0)
		, m_pNodeNextPlain(0)
		, m_pNodeParent(0)
		, m_NumChilds(0)
	{
	}
	TNodeMap(const T_KEY *Key, const T_CLASS *Value)
		: m_pKey(Key)
		, m_pValue(Value)
		, m_pNodeNext(0)
		, m_pNodeNextPlain(0)
		, m_pNodeParent(0)
		, m_pNodePrev(0)
		, m_NumChilds(0)
	{
	}

	T_CLASS*	GetValue() const { return m_pValue; }
	void		SetValue(T_CLASS *pValue) { m_pValue = pValue; }

	TNodeMap<T_KEY,T_CLASS>* GetNext() const { return m_pNodeNext; }
	TNodeMap<T_KEY,T_CLASS>* GetPlainNext() const { return m_pNodeNextPlain; }

	TNodeMap<T_KEY,T_CLASS>* GetRootNode() const { return m_pNodeParent; }
	size_t	GetNumChilds() const { return m_NumChilds; }

//private:
	TNodeMap<T_KEY,T_CLASS> *m_pNodeNext;		// pointer to next element
	TNodeMap<T_KEY,T_CLASS> *m_pNodePrev;		// pointer to prev element
	TNodeMap<T_KEY,T_CLASS> *m_pNodeNextPlain;	// pointer to next same depth element
	TNodeMap<T_KEY,T_CLASS> *m_pNodeParent;
	
	size_t				m_NumChilds;	// number of plain childs

	const T_KEY*		m_pKey;
	const T_CLASS*		m_pValue;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_KEY, class T_CLASS> class TTreeMap
{
  typedef TNodeMap<T_KEY, T_CLASS> TTMapNode;
  
public:
	TTreeMap()
		: m_pFirstElement(0)
		, m_pLastElement(0)
	{
	}

    TTMapNode* begin() const { return m_pFirstElement; }

    TTMapNode* end() const { return m_pLastElement; }

	TTMapNode* PushBack(const T_KEY &Key, const T_CLASS *Value, const T_KEY *KeyParent)
	{
		return PushBack(new T_KEY(Key), Value, KeyParent);
	}

	TTMapNode* PushBack(const T_KEY *Key, const T_CLASS *Value, const T_KEY *KeyParent)
	{
		TTMapNode* pOutNode = 0;

		if (Empty()) // create base element
		{
			pOutNode = m_pFirstElement = m_pLastElement = new TTMapNode(Key, Value);
		}
		else
		{
			TTMapNode *lNode		= m_pLastElement;  // push after last by default
			TTMapNode *lNodePlain	= m_pFirstElement; // left plain is the end by default
			
			while (lNodePlain->m_pNodeNextPlain){
				lNodePlain = lNodePlain->m_pNodeNextPlain;
			}

			TTMapNode *lParentNode = 0;			  // closest left plain node
			if (KeyParent && FindNodeByKey(KeyParent)) // check key exists
			{
				lParentNode = m_pFirstElement;
				lNode = lNodePlain = 0;

				TTMapNode *IterNode = m_pFirstElement;
	
				while (IterNode)
				{
					if (IterNode->m_pKey == KeyParent) // find corresponding parent
					{
						lNode = lParentNode = IterNode; // set by default

						// find target lPlain (child plain nodes)
						size_t num = lParentNode->m_NumChilds;
						if (num > 0)
						{
							lNodePlain = lParentNode->m_pNodeNext;

							for (size_t Index = 0; Index < (num - 1); ++Index)
							{
								lNodePlain = lNodePlain->m_pNodeNextPlain;
							}
						}

						// find target lNode (delta subchilds)
						size_t SubChildsCount = GetSubChildsCount(lParentNode);
						if (SubChildsCount > 0)
						{
							while (SubChildsCount)
							{
								lNode = lNode->m_pNodeNext;
								--SubChildsCount;
							}
						}
						break;
					}

					lNode = IterNode; // store prev
					IterNode = IterNode->m_pNodeNext; // update iter 
				}
			}
			pOutNode = Insert(Key, Value, lNode, lNodePlain, lParentNode);
		}

		return pOutNode;
	}

	TTMapNode* Insert(const T_KEY *Key, const T_CLASS *Value, TTMapNode *lNode, TTMapNode *lNodePlain, TTMapNode *lNodeParent)
	{
		TTMapNode *rNode = lNode ? lNode->GetNext() : lNodeParent->GetNext();
		TTMapNode *rNodePlain = lNodePlain ? lNodePlain->m_pNodeNextPlain : 0;

		// allocate new element
		TTMapNode *pNewNode = new TTMapNode(Key, Value);

		pNewNode->m_pNodeNext = rNode;
		pNewNode->m_pNodeNextPlain = rNodePlain;
		pNewNode->m_pNodeParent = lNodeParent;
		pNewNode->m_pNodePrev = lNode;


		if (lNodeParent){ 
			lNodeParent->m_NumChilds++; // rise childs number
		}
		
		if (lNodePlain){
			lNodePlain->m_pNodeNextPlain = pNewNode; // setup same depth node reference
		}
		
		if (lNode){
			lNode->m_pNodeNext = pNewNode; // just left bound pointer to newly created
		}

		if (!rNode)
		{
			m_pLastElement = pNewNode; // set last element
		}
		else
		{
			rNode->m_pNodePrev = pNewNode; // right bound pointer to newly created
		}

		return pNewNode;
	}

	void Erase(TTMapNode *pNode)
	{
		if (Empty()) { 
			return; //nothing to delete
		}

		TTMapNode *lNode = (pNode->m_pNodeParent) ? pNode->m_pNodeParent : m_pFirstElement; // select left node by default
		TTMapNode *lNodePlain = lNode;
		TTMapNode *rNode = pNode->m_pNodeNext; // right plane node

		if (lNode != pNode)
		{
			while (lNode) 
			{
				if (lNode->m_pNodeNext == pNode) { // find corresponding lNode
					break;
				}
				lNode = lNode->m_pNodeNext;
			}
		}
		else{
			lNode = lNodePlain = 0; // first element
		}

		// remove all subchilds
		size_t SubChildsCount = GetSubChildsCount(pNode);
		if (SubChildsCount > 0)
		{
			while (SubChildsCount)
			{
				assert(rNode);

				TTMapNode * ptrDel = rNode;
				rNode = rNode->m_pNodeNext; // shift rNode
				delete ptrDel;

				--SubChildsCount;
			}
		}

		// decrement child count
		if (pNode->m_pNodeParent){
			pNode->m_pNodeParent->m_NumChilds--;
		}

		if (lNode)
		{
			lNode->m_pNodeNext = rNode; // relink
			if (rNode){
				rNode->m_pNodePrev = lNode;
			}

			// relink plain nodes
			if (pNode->m_pNodeParent)
			{
				if (pNode->m_pNodeParent->m_NumChilds > 0)
				{
					TTMapNode * lChild = pNode->m_pNodeParent->m_pNodeNext;

					if (lChild != pNode) // not first child
					{
						while (lChild) // iterate throw all plain childs
						{
							if (lChild->m_pNodeNextPlain == pNode) // hit erase node
							{
								lChild->m_pNodeNextPlain = pNode->m_pNodeNextPlain; // relink to next plain
								break;
							}
							lChild = lChild->m_pNodeNextPlain;
						}
					}
				}
			}
			else
			{
				lNode->m_pNodeNextPlain = rNode; // top root nodes
			}
		}
		else
		{
			m_pFirstElement = rNode;

			if (rNode){
				rNode->m_pNodePrev = 0;
			}
		}	
		
		if (!rNode){ //empty tail
			m_pLastElement = lNode;
		}

		delete pNode;
	}

	void Clear()
	{
		TTMapNode *lNode = m_pFirstElement; // push after last by default
		while (lNode)
		{
			TTMapNode * rNext = lNode->m_pNodeNext;
			delete lNode;
			lNode = rNext;
		}
		m_pFirstElement = m_pLastElement = 0;
	}

	size_t GetSubChildsCount(TTMapNode *Node)
	{
		size_t OutCount = Node->m_NumChilds;

		if (Node->m_NumChilds > 0)
		{
			TTMapNode *NodeChild = Node->m_pNodeNext;
			while(NodeChild)
			{
				OutCount += GetSubChildsCount(NodeChild);
				NodeChild = NodeChild->m_pNodeNextPlain;
			}
		}
		return OutCount;
	}

	TTMapNode* FindNodeByKey(const T_KEY* pKey) const
	{
		TTMapNode *OutNode = 0;

		TTMapNode *lNode = m_pFirstElement; // push after last by default
		while (lNode)
		{
			if (lNode->m_pKey == pKey)
			{
				OutNode = lNode;
				break;
			}
			lNode = lNode->m_pNodeNext;
		}
		return OutNode;
	}

	bool Empty() const { return m_pFirstElement == 0; }

	/*
		simulate standard tree iterating
	*/
	TTMapNode* GetNext(TTMapNode* pNode) const
	{
		assert(pNode);

		if (pNode == m_pLastElement){
			return 0;
		}
		
		return (pNode->m_NumChilds || pNode->m_pNodeNext) ? pNode->m_pNodeNext : // first priority childs nodes or regular ones
				(pNode->m_pNodeParent->GetPlainNext() ? pNode->m_pNodeParent->GetPlainNext() : 0);
	}

	TTMapNode* GetPrev(TTMapNode *pNode) const
	{
		assert(pNode);
		return pNode->m_pNodePrev;
	}

//private:
	TTMapNode *m_pFirstElement;
	TTMapNode *m_pLastElement;
};
}
}
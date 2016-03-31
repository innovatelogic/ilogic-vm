///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Contains a simple container class.
*  \file       NVBContainer.cpp
*  \author     Pierre Terdiman
*  \date       February, 5, 2000
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Contains a list of 32-bits values.
*  Use this class when you need to store an unknown number of values. The list is automatically
*  resized and can contains 32-bits entities (dwords or floats)
*
*  \class      Container
*  \author     Pierre Terdiman
*  \version    1.0
*  \date       08.15.98
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "UtContainer.h"
#include "UtMemory.h"

// Static members
#ifdef CONTAINER_STATS
TUInt32 Container::mNbContainers = 0;
TUInt32 Container::mUsedRam = 0;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Constructor. No entries allocated there.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container::Container() 
: mMaxNbEntries(0)
, mCurNbEntries(0)
, mEntries(nullptr)
, mGrowthFactor(2.0f)
{

#ifdef CONTAINER_STATS
	mNbContainers++;
	mUsedRam += sizeof(Container);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Constructor. Also allocates a given number of entries.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container::Container(TUInt32 size, float growthfactor)
: mMaxNbEntries(0)
, mCurNbEntries(0)
, mEntries(nullptr)
, mGrowthFactor(growthfactor)
{

#ifdef CONTAINER_STATS
	mNbContainers++;
	mUsedRam+=sizeof(Container);
#endif
	SetSize(size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Destructor. Frees everything and leaves.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container::~Container()
{
	Empty();
#ifdef CONTAINER_STATS
	mNbContainers--;
	mUsedRam-=GetUsedRam();
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Resizes the container.
*  \param      needed  [in] assume the container can be added at least "needed" values
*  \return     true if success.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Resize(TUInt32 needed)
{
#ifdef CONTAINER_STATS
	// Subtract previous amount of bytes
	mUsedRam -= mMaxNbEntries*sizeof(TUInt32);
#endif

	// Get more entries
	mMaxNbEntries = mMaxNbEntries ? TUInt32(float(mMaxNbEntries)*mGrowthFactor) : 2; // Default nb Entries = 2
	
	if(mMaxNbEntries < mCurNbEntries + needed)
	{
		mMaxNbEntries = mCurNbEntries + needed;
	}

	// Get some bytes for new entries
	TUInt32* NewEntries = new TUInt32[mMaxNbEntries];
	CHECKALLOC(NewEntries);

#ifdef CONTAINER_STATS
	// Add current amount of bytes
	mUsedRam+=mMaxNbEntries*sizeof(TUInt32);
#endif

	// Copy old data if needed
	if(mCurNbEntries)  
	{
		CopyMemory(NewEntries, mEntries, mCurNbEntries*sizeof(TUInt32));
	}

	// Delete old data
	DELETEARRAY(mEntries);

	// Assign new pointer
	mEntries = NewEntries;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Sets the initial size of the container. If it already contains something, it's discarded.
*  \param      nb      [in] Number of entries
*  \return     true if success
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::SetSize(TUInt32 nb)
{
	// Make sure it's empty
	Empty();

	// Checkings
	if(!nb)
	{
		return false;
	}

	// Initialize for nb entries
	mMaxNbEntries = nb;

	// Get some bytes for new entries
	mEntries = new TUInt32[mMaxNbEntries];
	CHECKALLOC(mEntries);

#ifdef CONTAINER_STATS
	// Add current amount of bytes
	mUsedRam+=mMaxNbEntries*sizeof(TUInt32);
#endif
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Refits the container and get rid of unused bytes.
*  \return     true if success
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Refit()
{
#ifdef CONTAINER_STATS
	// Subtract previous amount of bytes
	mUsedRam-=mMaxNbEntries*sizeof(TUInt32);
#endif

	// Get just enough entries
	mMaxNbEntries = mCurNbEntries;
	if(!mMaxNbEntries) 
	{
		return false;
	}

	// Get just enough bytes
	TUInt32* NewEntries = new TUInt32[mMaxNbEntries];
	CHECKALLOC(NewEntries);

#ifdef CONTAINER_STATS
	// Add current amount of bytes
	mUsedRam+=mMaxNbEntries*sizeof(TUInt32);
#endif

	// Copy old data
	CopyMemory(NewEntries, mEntries, mCurNbEntries*sizeof(TUInt32));

	// Delete old data
	DELETEARRAY(mEntries);

	// Assign new pointer
	mEntries = NewEntries;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Checks whether the container already contains a given value.
*  \param      entry           [in] the value to look for in the container
*  \param      location        [out] a possible pointer to store the entry location
*  \see        Add(TUInt32 entry)
*  \see        Add(float entry)
*  \see        Empty()
*  \return     true if the value has been found in the container, else false.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Contains(TUInt32 entry, TUInt32* location) const
{
	// Look for the entry
	for(TUInt32 i=0;i<mCurNbEntries;i++)
	{
		if(mEntries[i]==entry)
		{
			if(location) 
			{
				*location = i;
			}
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Deletes an entry. If the container contains such an entry, it's removed.
*  \param      entry       [in] the value to delete.
*  \return     true if the value has been found in the container, else false.
*  \warning    This method is arbitrary slow (O(n)) and should be used carefully. Insertion order is not preserved.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::Delete(TUInt32 entry)
{
	// Look for the entry
	for(TUInt32 i=0;i<mCurNbEntries;i++)
	{
		if(mEntries[i]==entry)
		{
			// Entry has been found at index i. The strategy is to copy the last current entry at index i, and decrement the current number of entries.
			DeleteIndex(i);
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Deletes an entry, preserving the insertion order. If the container contains such an entry, it's removed.
*  \param      entry       [in] the value to delete.
*  \return     true if the value has been found in the container, else false.
*  \warning    This method is arbitrary slow (O(n)) and should be used carefully.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::DeleteKeepingOrder(TUInt32 entry)
{
	// Look for the entry
	for(TUInt32 i=0;i<mCurNbEntries;i++)
	{
		if(mEntries[i]==entry)
		{
			// Entry has been found at index i.
			// Shift entries to preserve order. You really should use a linked list instead.
			mCurNbEntries--;
			for(TUInt32 j=i;j<mCurNbEntries;j++)
			{
				mEntries[j] = mEntries[j+1];
			}
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Gets the next entry, starting from input one.
*  \param      entry       [in/out] On input, the entry to look for. On output, the next entry
*  \param      wrap        [in] true to wrap at the end of the array
*  \return     Self-Reference
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container& Container::FindNext(TUInt32& entry, bool wrap)
{
	TUInt32 Location;
	if(Contains(entry, &Location))
	{
		Location++;
		if(Location==mCurNbEntries) Location = wrap ? 0 : mCurNbEntries-1;
		entry = mEntries[Location];
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Gets the previous entry, starting from input one.
*  \param      entry       [in/out] On input, the entry to look for. On output, the previous entry
*  \param      wrap        [in] true to wrap at the end of the array
*  \return     Self-Reference
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Container& Container::FindPrev(TUInt32& entry, bool wrap)
{
	TUInt32 Location;
	if(Contains(entry, &Location))
	{
		Location--;
		if(Location==0xffffffff)    Location = wrap ? mCurNbEntries-1 : 0;
		entry = mEntries[Location];
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Gets the ram used by the container.
*  \return     the ram used in bytes.
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TUInt32 Container::GetUsedRam() const
{
	return sizeof(Container) + mMaxNbEntries * sizeof(TUInt32);
}

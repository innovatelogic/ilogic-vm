///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __container_h__
#define __container_h__

#pragma once

#include "TypesBase.h"
#include "math_float.h"
#include "UtMemory.h"

//#define CONTAINER_STATS

class EXPORT Container
{
   
public:
	// Constructor/Destructor
	Container();
	Container(TUInt32 size, float growthfactor);
	~Container();

	// Management
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline_ Container& Add(TUInt32 entry)
	{
		// Resize if needed
		if(mCurNbEntries==mMaxNbEntries) 
		{
			Resize();
		}

		// Add new entry
		mEntries[mCurNbEntries++]   = entry;
		return *this;
	}

	inline_ Container& Add(TUInt32* entries, TUInt32 nb)
	{
		// Resize if needed
		if(mCurNbEntries+nb>mMaxNbEntries)
		{
			Resize(nb);
		}

		// Add new entry
		CopyMemory(&mEntries[mCurNbEntries], entries, nb*sizeof(TUInt32));
		mCurNbEntries+=nb;
		return *this;
	}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline_ Container& Add(float entry)
	{
		// Resize if needed
		if(mCurNbEntries==mMaxNbEntries)
		{
			Resize();
		}

		// Add new entry
		mEntries[mCurNbEntries++]   = IR(entry);
		return *this;
	}

	inline_ Container& Add(float* entries, TUInt32 nb)
	{
		// Resize if needed
		if(mCurNbEntries+nb>mMaxNbEntries)
		{
			Resize(nb);
		}

		// Add new entry
		CopyMemory(&mEntries[mCurNbEntries], entries, nb*sizeof(float));
		mCurNbEntries+=nb;
		return *this;
	}

	//! Add unique [slow]
	Container& AddUnique(TUInt32 entry)
	{
		if(!Contains(entry)) 
		{
			Add(entry);
		}

		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline_ Container&  Empty()
	{
#ifdef CONTAINER_STATS
		mUsedRam-=mMaxNbEntries*sizeof(TUInt32);
#endif
		DELETEARRAY(mEntries);
		mCurNbEntries = mMaxNbEntries = 0;
		return *this;
	}

	inline_ void Reset()
	{
		// Avoid the write if possible
		// ### CMOV
		if(mCurNbEntries)   mCurNbEntries = 0;
	}

	bool            SetSize(TUInt32 nb);
	bool            Refit();

	// Checks whether the container already contains a given value.
	bool            Contains(TUInt32 entry, TUInt32* location = nullptr) const;
	// Deletes an entry - doesn't preserve insertion order.
	bool            Delete(TUInt32 entry);
	// Deletes an entry - does preserve insertion order.
	bool            DeleteKeepingOrder(TUInt32 entry);
	//! Deletes the very last entry.
	inline_ void    DeleteLastEntry()                       { if(mCurNbEntries) mCurNbEntries--;            }
	//! Deletes the entry whose index is given
	inline_ void    DeleteIndex(TUInt32 index)               { mEntries[index] = mEntries[--mCurNbEntries];  }

	// Helpers
	Container&      FindNext(TUInt32& entry, bool wrap=false);
	Container&      FindPrev(TUInt32& entry, bool wrap=false);

	// Data access.
	inline_ TUInt32          GetNbEntries()                  const   { return mCurNbEntries;     }   //!< Returns the current number of entries.
	inline_ TUInt32          GetEntry(TUInt32 i)              const   { return mEntries[i];       }   //!< Returns ith entry
	inline_ TUInt32*         GetEntries()                    const   { return mEntries;          }   //!< Returns the list of entries.

	// Growth control
	inline_ float           GetGrowthFactor()               const   { return mGrowthFactor;     }   //!< Returns the growth factor
	inline_ void            SetGrowthFactor(float growth)           { mGrowthFactor = growth;   }   //!< Sets the growth factor

	//! Access as an array
	inline_ TUInt32&         operator[](TUInt32 i)            const   { assert(i>=0 && i<mCurNbEntries); return mEntries[i];  }

	// Stats
	TUInt32          GetUsedRam()                    const;

	//! Operator for Container A = Container B
	void      operator = (const Container& object)
	{
		SetSize(object.GetNbEntries());
		CopyMemory(mEntries, object.GetEntries(), mMaxNbEntries*sizeof(TUInt32));
		mCurNbEntries = mMaxNbEntries;
	}

#ifdef CONTAINER_STATS
	inline_ TUInt32          GetNbContainers()               const   { return mNbContainers;     }
	inline_ TUInt32          GetTotalBytes()                 const   { return mUsedRam;          }
private:

	static  TUInt32          mNbContainers;      //!< Number of containers around
	static  TUInt32          mUsedRam;           //!< Amount of bytes used by containers in the system
#endif
private:
	// Resizing
	bool            Resize(TUInt32 needed=1);
	// Data
	TUInt32          mMaxNbEntries;      //!< Maximum possible number of entries
	TUInt32          mCurNbEntries;      //!< Current number of entries
	TUInt32*         mEntries;           //!< List of entries
	float           mGrowthFactor;      //!< Resize: new number of entries = old number * mGrowthFactor
};

//////////////////////////////////////////////////////////////////////////
/*class Pairs : public Container
{
public:
	// Constructor / Destructor
	inline_             Pairs()                     {}
	inline_             ~Pairs()                    {}

	_inline TUInt32      GetNbPairs()    const       { return GetNbEntries()>>1;                 }
	_inline Pair*       GetPairs()      const       { return (Pair*)GetEntries();               }

	Pairs&      AddPair(const Pair& p)      { Add(p.id0).Add(p.id1);    return *this;   }

};*/

#endif // __container_h__
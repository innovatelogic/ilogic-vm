///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __container_h__
#define __container_h__

#pragma once

//#include "platform_specific.h"
#include "math_float.h"
#include "UtMemory.h"

//#define CONTAINER_STATS

class EXPORT Container
{
   
public:
	// Constructor/Destructor
	Container();
	Container(udword size, float growthfactor);
	~Container();

	// Management
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline_ Container& Add(udword entry)
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

	inline_ Container& Add(udword* entries, udword nb)
	{
		// Resize if needed
		if(mCurNbEntries+nb>mMaxNbEntries)
		{
			Resize(nb);
		}

		// Add new entry
		CopyMemory(&mEntries[mCurNbEntries], entries, nb*sizeof(udword));
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

	inline_ Container& Add(float* entries, udword nb)
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
	Container& AddUnique(udword entry)
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
		mUsedRam-=mMaxNbEntries*sizeof(udword);
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

	bool            SetSize(udword nb);
	bool            Refit();

	// Checks whether the container already contains a given value.
	bool            Contains(udword entry, udword* location = nullptr) const;
	// Deletes an entry - doesn't preserve insertion order.
	bool            Delete(udword entry);
	// Deletes an entry - does preserve insertion order.
	bool            DeleteKeepingOrder(udword entry);
	//! Deletes the very last entry.
	inline_ void    DeleteLastEntry()                       { if(mCurNbEntries) mCurNbEntries--;            }
	//! Deletes the entry whose index is given
	inline_ void    DeleteIndex(udword index)               { mEntries[index] = mEntries[--mCurNbEntries];  }

	// Helpers
	Container&      FindNext(udword& entry, bool wrap=false);
	Container&      FindPrev(udword& entry, bool wrap=false);

	// Data access.
	inline_ udword          GetNbEntries()                  const   { return mCurNbEntries;     }   //!< Returns the current number of entries.
	inline_ udword          GetEntry(udword i)              const   { return mEntries[i];       }   //!< Returns ith entry
	inline_ udword*         GetEntries()                    const   { return mEntries;          }   //!< Returns the list of entries.

	// Growth control
	inline_ float           GetGrowthFactor()               const   { return mGrowthFactor;     }   //!< Returns the growth factor
	inline_ void            SetGrowthFactor(float growth)           { mGrowthFactor = growth;   }   //!< Sets the growth factor

	//! Access as an array
	inline_ udword&         operator[](udword i)            const   { assert(i>=0 && i<mCurNbEntries); return mEntries[i];  }

	// Stats
	udword          GetUsedRam()                    const;

	//! Operator for Container A = Container B
	void      operator = (const Container& object)
	{
		SetSize(object.GetNbEntries());
		CopyMemory(mEntries, object.GetEntries(), mMaxNbEntries*sizeof(udword));
		mCurNbEntries = mMaxNbEntries;
	}

#ifdef CONTAINER_STATS
	inline_ udword          GetNbContainers()               const   { return mNbContainers;     }
	inline_ udword          GetTotalBytes()                 const   { return mUsedRam;          }
private:

	static  udword          mNbContainers;      //!< Number of containers around
	static  udword          mUsedRam;           //!< Amount of bytes used by containers in the system
#endif
private:
	// Resizing
	bool            Resize(udword needed=1);
	// Data
	udword          mMaxNbEntries;      //!< Maximum possible number of entries
	udword          mCurNbEntries;      //!< Current number of entries
	udword*         mEntries;           //!< List of entries
	float           mGrowthFactor;      //!< Resize: new number of entries = old number * mGrowthFactor
};

//////////////////////////////////////////////////////////////////////////
/*class Pairs : public Container
{
public:
	// Constructor / Destructor
	inline_             Pairs()                     {}
	inline_             ~Pairs()                    {}

	_inline udword      GetNbPairs()    const       { return GetNbEntries()>>1;                 }
	_inline Pair*       GetPairs()      const       { return (Pair*)GetEntries();               }

	Pairs&      AddPair(const Pair& p)      { Add(p.id0).Add(p.id1);    return *this;   }

};*/

#endif // __container_h__
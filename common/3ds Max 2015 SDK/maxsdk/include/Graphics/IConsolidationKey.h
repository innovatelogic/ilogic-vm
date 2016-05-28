//
// Copyright 2013 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/IRefObject.h"
#include "../baseinterface.h"
#include "ICustomRenderItem.h"
#include "DrawContext.h"

namespace MaxSDK { namespace Graphics {

/** This class is used as a key for hash table. Render items with same keys will be sent to a consolidation item.
Plugins can implement this interface to provide their own consolidation properties. Since this class stores key
data to judge which consolidation item a render item belongs, we don't recommended put any voliate variables in the key.
Furthermore, since GetInterfaceID is the only way to determine the type of a key, if plugins want to implement
their own consolidation method for existing type of keys, they must inherit the key and provide a new interface ID.
Otherwise, the render items may not send to plugins' consolidation item.
*/
class IConsolidationKey : public ARefObject
{
public:
	/** Returns hash key of this consolidation key.
		Usually the hash key consists at least the corresponding consolidation item's interface ID for its initial value.
	\return hash key
	*/
	virtual size_t GetHashKey() = 0;

	/** Returns comparing result of two keys
	\return true if two keys are same, false if they're different.
	*/
	virtual bool Equals(const IConsolidationKey& rhs) const = 0;

	/** Returns the class ID that can identify this type of key
		Keys with different class ID always return false.
	\return the keys' class ID
	*/
	virtual const Class_ID GetClassID() const = 0;

	/** Returns an copy of current key. Please note, all members of the new keys should also been copied. It's not
	recommended to share pointers between keys because the life time of a key is managed by Max. The key's dispose time
	is unknown from plug-ins' perspective.
	\return new copy of current key
	*/
	virtual IConsolidationKey* Clone() = 0;

	/** Same as Match(), returns comparing result of two keys
	\return true if two keys are same, false if they're different.
	*/
	GraphicsDriverAPI bool operator==(const IConsolidationKey& rhs) const;
};


/** An utility function for generate hash code.
\code
Interface_ID id = PLUGIN_CONSOLIDATION_INTERFACE_ID;
size_t key = CombineHashCode(0, id.PartA()); //Initialize a new key
mHashKey = CombineHashCode(key, id.PartB()); //Combine new data with exist key
\endcode
\return new code from seed and hash code.
*/
GraphicsDriverAPI size_t CombineHashCode(size_t seed, size_t hashCode);

typedef RefPtr<IConsolidationKey> IConsolidationKeyPtr;

} } // end namespace

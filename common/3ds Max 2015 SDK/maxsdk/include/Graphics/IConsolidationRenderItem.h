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
#include "IConsolidationKey.h"
#include "ICustomRenderItem.h"
#include "IConsolidationStrategy.h"
#include "DrawContext.h"

namespace MaxSDK { namespace Graphics {

/** This class defines a consolidation render item. Which is used to improve
rendering performance by batching similar render items together.
*/
class IConsolidationRenderItem : public ICustomRenderItem
{
public:
	/** This function adds a render item to the consolidation item. The consolidation
	item should build geometries of these items in Realize() function.
	After been consolidated, the render item's Realize() and Display() functions will
	no longer be called by Max.
	\return true if the item added successfully. Otherwise false.
	*/
	virtual bool AddSourceItem(ICustomRenderItemPtr& pSourceItem, DrawContext& dc) = 0;

	/** This function should clear all render items holds by the consolidation item.
	There're lots of situations that cause Max call this function. For example,
	Add/Remove render items of a node, moving a object, change a object's data etc...
	*/
	virtual void ClearSourceItems() = 0;
};

typedef RefPtr<IConsolidationRenderItem> IConsolidationRenderItemPtr;

} } // end namespace

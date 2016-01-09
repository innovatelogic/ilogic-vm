#include "StyleSet.h"
#include "ValueParser.h"

//----------------------------------------------------------------------------------
StyleSet::StyleSet()
{

}

//----------------------------------------------------------------------------------
StyleSet::~StyleSet()
{

}

//------------------------------------------------------------------------
BOOL StyleSet::Load(tinyxml2::XMLElement *Tree)
{
// 	XML_FOR_EACH_TREE(Tree)
// 	{
// 		XML_CHECK_NODE("Style") // load style
// 		{ 	
//             XML_DEF_ATTRIBUTES_NODE(Value);
// 
// 			ValueParser ValueStore(Value);
// 
// 			if (ValueStore.IsValue("Name")) 
// 			{
//                 VisualStyle * Style = new VisualStyle();
// 
// 				if (Style)
// 				{
// 					Style->SetName(ValueStore.GetValueString("Name"));
// 					Style->SetSize(ValueStore.GetValueVector2f("Size"));
// 					Style->SetStateStyle(0, ValueStore.GetValueString("StateNormal"),  ValueStore.GetValueVector4f("StateNormalUV"));
// 					Style->SetStateStyle(1, ValueStore.GetValueString("StateFocused"), ValueStore.GetValueVector4f("StateFocusedUV"));
// 					Style->SetStateStyle(2, ValueStore.GetValueString("StateActive"),  ValueStore.GetValueVector4f("StateActiveUV"));
// 					Style->SetStateStyle(3, ValueStore.GetValueString("StatePressed"), ValueStore.GetValueVector4f("StatePressedUV"));
// 					Style->SetStateStyle(4, ValueStore.GetValueString("StateDisable"), ValueStore.GetValueVector4f("StateDisableUV"));
// 			
//                     MapStyleset.insert(std::make_pair<std::string,VisualStyle*>(ValueStore.GetValueString("Name"), Style));
// 				}
// 			}
// 		}
// 	}

	return TRUE;
}

//------------------------------------------------------------------------
bool StyleSet::GetStyle(const std::string& Name, VisualStyle *& OutStyle)
{
	std::map<std::string, VisualStyle*>::iterator Iter = MapStyleset.find(Name);

	if (Iter != MapStyleset.end())
	{
		OutStyle = Iter->second;
		return true;
	}

	return false;
}
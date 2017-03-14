#include "coresdkafx.h"

REGISTER_CLASS(AnimationSequerence, ActorAllocator);

//----------------------------------------------------------------------------------
AnimationSequerence::AnimationSequerence(const CObjectAbstract* Parent)
: Super(Parent)
{

}

//----------------------------------------------------------------------------------
AnimationSequerence::~AnimationSequerence()
{

}

//------------------------------------------------------------------------
// void AnimationSequerence::Deserialize(TiXmlElement *xml_current_tree)
// {
// 	// deserialization of array
//  	XML_FOR_EACH_TREE(xml_current_tree)
//  	{
//  		XML_CHECK_NODE("array")// find current tag
//  		{
//  			XML_DEF_ATTRIBUTES_NODE(Value);
//  			ValueParser ValueStore(Value);
//  
//  			if (ValueStore.GetValueString("Name") == "AnimFrames")
//  			{
//  				XML_FOR_EACH_TREE(xml_current_tree)
//  				{
//  					XML_CHECK_NODE("AnimFrame")
//  					{
//  						XML_DEF_ATTRIBUTES_NODE(Value);
//  						ValueParser ValueStore(Value);
//  
//  						AnimFrame tmp;
//  						tmp.UV = ValueStore.GetValueVector4f("UV");
//  						AnimFrames.AddItem(tmp);
//  					}
//  				}
//  			}
//  		}
//  	}
// 
// 	//Super::Deserialize(xml_current_tree);
// }
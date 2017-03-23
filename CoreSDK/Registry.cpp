#include "Registry.h"
#include "ValueParser.h"

//----------------------------------------------------------------------------------------------
Registry::Registry()
: Resolution(800,600)
, ControlAxisX(100.f,0.f)
, ControlAxisY(0.f,-100.f)
, ControlBox(10.f,10.f)
, ControlBoxOrigin(10.f,10.f)
{
	RegisterPath("data/$registry");
	RegisterPath("data/$cashe");
	RegisterPath("data/$Games");
	RegisterPath("data/$System");
	RegisterPath("data/$System/interface");
	RegisterPath("data/$System/fonts");
	RegisterPath("data/$cashe/Textures");
	RegisterPath("data/$cashe/Models");
	RegisterPath("data/$cashe/Materials");
}

//----------------------------------------------------------------------------------------------
Registry::~Registry()
{
	MapTypeIcons.empty();
	MapOprationsIcons.empty();

	UnRegisterPath("data/$registry");
	UnRegisterPath("data/$cashe");
	UnRegisterPath("data/$Games");
	UnRegisterPath("data/$System");
	UnRegisterPath("data/$System/interface");
}

//----------------------------------------------------------------------------------------------
bool Registry::Load(const std::string & filename)
{
    bool bResult = false;

	std::string sFilename;  

	if (!FindFile(filename.c_str(), &sFilename))
	{
		//MessageBoxW(NULL, ConvertStringToWideString(sFilename).c_str(), L"Super message", MB_OK);
		return false;
	}

	XML_PARSE_FILE( sFilename.c_str())
	{
		XML_FOR_EACH_TREE(XML_CURRENT_NODE)
		{
			XML_CHECK_NODE("resolution")
			{
				XML_VALUE_ATTRIBUTE("Value", Resolution);
			}

			XML_CHECK_NODE("Types")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("Type")
					{
						XML_DEF_ATTRIBUTES_NODE(Name);
						XML_DEF_ATTRIBUTES_NODE(Icon);
						XML_DEF_ATTRIBUTES_NODE(Editor);

						ValueParser ValueString(Editor);

						TypeInfo Info = {ValueString.IsValue("Visible") ? ValueString.GetValueBOOL("Visible") : true, Icon};
						//Info.bEdVisible = ValueString.IsValue("Visible") ? ValueString.GetValueBOOL("Visible") : true;
						//Info.IconRef = Icon;

						MapTypeIcons.insert(std::make_pair(Name, Icon));
						MapTypeInfo.insert(std::make_pair(Name, Info));
					}
				}
			}

			XML_CHECK_NODE("Operations")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("Operation")
					{
						XML_DEF_ATTRIBUTES_NODE(Name);
						XML_DEF_ATTRIBUTES_NODE(Icon);

						MapOprationsIcons.insert(std::make_pair(Name, Icon));
					}
				}
			}
		}

		bResult = true;
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool Registry::IsEditorVisible(const char * TypeName) const
{
	TMapTypeInfo::const_iterator IterFind = MapTypeInfo.find(TypeName);

	if (IterFind != MapTypeInfo.end()){
		return IterFind->second.bEdVisible;
	}
	return true;
}
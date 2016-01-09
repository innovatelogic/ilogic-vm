#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
GameStaticInfo::GameStaticInfo()
{

}

//----------------------------------------------------------------------------------------------
GameStaticInfo::~GameStaticInfo()
{

}

//----------------------------------------------------------------------------------------------
bool GameStaticInfo::Load(const std::string& filename)
{
	bool bResult = false;

	std::string sFilename;  

	if (!FindFile(filename.c_str(),&sFilename)){
		return false;
	}

	XML_PARSE_FILE( sFilename.c_str() )
	{
		XML_FOR_EACH_TREE(XML_CURRENT_NODE)
		{
			XML_CHECK_NODE("Levels")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("Level")
					{
						XML_DEF_ATTRIBUTES_NODE(Id);
						XML_DEF_ATTRIBUTES_NODE(Value);

						int value_id = atoi(Id.c_str());  
						IdLevelMap.insert(std::make_pair(value_id, Value));
					}
				}
			}
		}

		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
std::string	GameStaticInfo::GetLevelNameById(int Id)
{
	TMapIntString::iterator Iter = IdLevelMap.find(Id);

	if (Iter != IdLevelMap.end())
	{
		return Iter->second;
	}
	return "none";
}
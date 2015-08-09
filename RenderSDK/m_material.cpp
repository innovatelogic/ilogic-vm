#include "m_material.h"
#include "xmlparser.h"
#include "ValueParser.h"

//----------------------------------------------------------------------------------------------
m_material::m_material()
: id(BAD_IDX)
, textures(0)
, diffuse(1.f,1.f,1.f,1.f)
, specular(0.f,0.f,0.f,0.f)
, shininess(0.f)
, ambient(0.f,0.f,0.f,0.f)
, emission(0.f,0.f,0.f,0.f)
, transparent(1.f)
, fog(false)
, fog_color(1.f,1.f,1.f,1.f)
, fx_file("ogl_diffuse.vs")
, tehnique("DiffuseOnly")
{

}

//----------------------------------------------------------------------------------------------
m_material::~m_material()
{

}

//----------------------------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_material & rMaterial)
{
	// Write the material's name, length first.
	rOutputStream << (unsigned int)strlen(rMaterial.name.c_str());
	rOutputStream.write(rMaterial.name.c_str(), (unsigned int)strlen(rMaterial.name.c_str()));

	rOutputStream << rMaterial.id;

	rOutputStream << (unsigned int)rMaterial.textures.size();

	for (size_t iTexture = 0; iTexture < rMaterial.textures.size(); ++iTexture)
	{
		rOutputStream << rMaterial.textures[iTexture];
		rOutputStream << rMaterial.tex_channel[iTexture];
	}

	rOutputStream << rMaterial.diffuse;
	rOutputStream << rMaterial.specular;
	rOutputStream << rMaterial.shininess;
	rOutputStream << rMaterial.ambient;
	rOutputStream << rMaterial.emission;

	rOutputStream << rMaterial.transparent;

	rOutputStream << rMaterial.fog;
	rOutputStream << rMaterial.fog_color;
	rOutputStream << rMaterial.attr;

	return rOutputStream;
}

//----------------------------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_material & rMaterial)
{
	int nNameLength;
	rInputStream >> nNameLength;

	char * _name = new char[nNameLength + 1];
	assert(_name);
	rInputStream.read(_name, nNameLength);
	_name[nNameLength] = '\0';
	rMaterial.name = _name;
	delete _name;

	rInputStream >> rMaterial.id;

	unsigned int num_textures;
	rInputStream >> num_textures;
	
	rMaterial.textures.resize(num_textures);
	rMaterial.tex_channel.resize(num_textures);

	for (unsigned int iTexture = 0; iTexture < num_textures; ++iTexture)
	{
		rInputStream >> rMaterial.textures[iTexture];
		rInputStream >> rMaterial.tex_channel[iTexture];
	}

	rInputStream >> rMaterial.diffuse;
	rInputStream >> rMaterial.specular;
	rInputStream >> rMaterial.shininess;
	rInputStream >> rMaterial.ambient;
	rInputStream >> rMaterial.emission;

	rInputStream >> rMaterial.transparent;

	rInputStream >> rMaterial.fog;
	rInputStream >> rMaterial.fog_color;
	rInputStream >> rMaterial.attr;

   return rInputStream;
}

//----------------------------------------------------------------------------------------------
void WriteToStreamMaterial(std::stringstream &rOutputStream, const m_material * material)
{
	// Write the material's name, length first.
	std::string AdaptedName(material->name);
	std::replace(AdaptedName.begin(), AdaptedName.end(), ' ', '_');

	rOutputStream << AdaptedName << std::endl;

	rOutputStream << material->fx_file << std::endl;
	rOutputStream << material->tehnique << std::endl;

	rOutputStream << material->id << std::endl;

	rOutputStream << material->textures.size() << std::endl;

	for (size_t iTexture = 0; iTexture < material->textures.size(); ++iTexture)
	{
		rOutputStream << material->textures[iTexture] << std::endl;
		rOutputStream << material->tex_channel[iTexture] << std::endl;
	}

	rOutputStream << material->diffuse.x << " " << material->diffuse.y << " " << material->diffuse.z << " " << material->diffuse.w << std::endl;
	rOutputStream << material->specular.x << " " << material->specular.y << " " << material->specular.z << " " << material->specular.w << std::endl;
	rOutputStream << material->shininess << std::endl;
	rOutputStream << material->ambient.x << " " << material->ambient.y << " " << material->ambient.z << " " << material->ambient.w << std::endl;
	rOutputStream << material->emission.x << " " << material->emission.y << " " << material->emission.z << " " << material->emission.w << std::endl;

	rOutputStream << material->transparent << std::endl;

	rOutputStream << material->fog << std::endl;
	rOutputStream << material->fog_color.x << " " << material->fog_color.y << " " << material->fog_color.z << " " << material->fog_color.w << std::endl;
}

//----------------------------------------------------------------------------------------------
void LoadMaterial(const char *Path, m_material *pOutMaterial)
{
 	XML_PARSE_FILE( Path )
 	{
 		XML_FOR_EACH_TREE(XML_CURRENT_NODE)
 		{
 			XML_CHECK_NODE("fx"){
				pOutMaterial->fx_file = XML_CURRENT_NODE->FirstChild()->ValueStr();
 			}

			XML_CHECK_NODE("tehnique"){
				pOutMaterial->tehnique = XML_CURRENT_NODE->FirstChild()->ValueStr();
			}
			
			XML_CHECK_NODE("diffuse")
			{
				CStringUtility<float> ParseString(XML_CURRENT_NODE->FirstChild()->ValueStr(), " ");
				pOutMaterial->diffuse = Vector4f(ParseString.m_vector[0], ParseString.m_vector[1], ParseString.m_vector[2], ParseString.m_vector[3]);
			}
 		}
 	}
}

#include "m_texture.h"
#include "m_streams.h"

//------------------------------------------------------------------------
m_texture::m_texture()
: type(CUSTOM)
{
	//tex_mat = mat4_id;
}

//------------------------------------------------------------------------
m_texture::m_texture(const m_texture & tex)
{
    copy_from(tex);
}

//------------------------------------------------------------------------
m_texture::~m_texture()
{

}

//------------------------------------------------------------------------
const m_texture & m_texture::operator= (const m_texture & tex)
{
	copy_from(tex);
	return *this;
}

//------------------------------------------------------------------------
void m_texture::copy_from(const m_texture & tex)
{
	type = tex.type;
	name = tex.name;
	tex_mat = tex.tex_mat;
}

//------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_texture & rTexture)
{  
	rOutputStream << static_cast<unsigned int>(rTexture.type);

    // Write the namestring, length first.
	rOutputStream << (unsigned int)strlen(rTexture.name.c_str());
	rOutputStream.write(rTexture.name.c_str(), (unsigned int)strlen(rTexture.name.c_str()));

	rOutputStream << rTexture.tex_mat;

	return rOutputStream; 
}

//------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_texture & rTexture)
{
	unsigned int nType;
	rInputStream >> nType;
	rTexture.type = static_cast<m_texture::_tex_type>(nType);

	unsigned int nStringLength;
	rInputStream >> nStringLength;

	char * _name = new char[nStringLength + 1] ;
	_name[nStringLength] = '\0';
	rInputStream.read(_name, nStringLength);
	rTexture.name = _name;
	delete _name;

	rInputStream >> rTexture.tex_mat;

	return rInputStream;
}

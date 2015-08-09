#include "m_attribute.h"

#include <algorithm>
#include <string>

//////////////////////////////////////////////////////////////////////////
m_attribute::m_attribute()
{
	_pdata = 0;
	_size = 0;
	_flag = M_UNASSIGNED;
}

//////////////////////////////////////////////////////////////////////////
m_attribute::m_attribute(const m_attribute& attr)
{
	_pdata = 0;
	_size  = 0;
	copy_from(&attr);
}

//////////////////////////////////////////////////////////////////////////
m_attribute::~m_attribute()
{
	erase();

	_dico_it it = _attr.begin();
	while (it != _attr.end())
	{
		delete (*it).second;
		++it;
	}
	_attr.clear();
	_flag = M_UNASSIGNED;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const char * str)
{
	array(str, (unsigned int)strlen(str) + 1);
	_flag = M_STRING;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const float & val)
{
	_float = val;
	_flag = M_FLOAT;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const unsigned int & val)
{
	_udword = val;
	_flag = M_UNSIGNED_INT;
	return *this;
}
//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const int & val)
{
	_sdword = val;
	_flag = M_INT;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const unsigned char & val)
{
	_ubyte = val;
	_flag = M_UNSIGNED_CHAR;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const char & val)
{
	_byte = val;
	_flag = M_CHAR;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const short & val)
{
	_word = val;
	_flag = M_SHORT;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator= (const m_attribute & attr)
{
	erase();
	// delete the dictionnary
	_dico_it it = _attr.begin();
	while (it != _attr.end())
	{
		delete (*it).second;
		++it;
	}
	_attr.clear();
	copy_from(&attr);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
m_attribute & m_attribute::operator[] (const char * str)
{
	std::string name(str);
	_dico_it it = _attr.find(name);
	if (it != _attr.end())
		return *(*it).second;

	m_attribute * attrib = new m_attribute;
	_attr.insert(_dico_pair(name,attrib));
	return *attrib;
}

//////////////////////////////////////////////////////////////////////////
// array of structs replacement?
void m_attribute::array(const m_attribute * attribs, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new m_attribute[size]);
		// do the copy...
		for (unsigned int i = 0; i < size; ++i)
			(reinterpret_cast<m_attribute*>(_pdata))[i] = attribs[i];
	}
	_size = size;
	_flag = M_ATTRIBUTE_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const float * fvector, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new float[size]);
		memcpy(_pdata,fvector,size * sizeof(float));
	}
	_size = size;
	_flag = M_FLOAT_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const char * str, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new char[size]);
		memcpy(_pdata,str,size * sizeof(char));
	}
	_size = size;
	_flag = M_CHAR_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const unsigned char * str, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new unsigned char[size]);
		memcpy(_pdata,str,size * sizeof(unsigned char));
	}
	_size = size;
	_flag = M_UNSIGNED_CHAR_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const unsigned int * array, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new unsigned int[size]);
		memcpy(_pdata,array,size * sizeof(unsigned int));
	}
	_size = size;
	_flag = M_UNSIGNED_INT_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const int * array, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new int[size]);
		memcpy(_pdata,array,size * sizeof(int));
	}
	_size = size;
	_flag = M_INT_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::array(const short * array, unsigned int size)
{
	erase();
	if (size)
	{
		_pdata = reinterpret_cast<void*>(new short[size]);
		memcpy(_pdata,array,size * sizeof(short));
	}
	_size = size;
	_flag = M_SHORT_ARRAY;
}

//////////////////////////////////////////////////////////////////////////
bool m_attribute::is_null () const
{
	return _flag == M_UNASSIGNED;
}

//////////////////////////////////////////////////////////////////////////
bool m_attribute::is_valid () const
{
	return _flag != M_UNASSIGNED;
}

//////////////////////////////////////////////////////////////////////////
// value accessors...
float m_attribute::as_float() const
{
	return _float;
}

//////////////////////////////////////////////////////////////////////////
unsigned int m_attribute::as_unsigned_int() const
{
	return _udword;
}

//////////////////////////////////////////////////////////////////////////
unsigned int m_attribute::as_int() const
{
	return _sdword;
}

//////////////////////////////////////////////////////////////////////////
short m_attribute::as_word() const
{
	return _word;
}

//////////////////////////////////////////////////////////////////////////
unsigned char m_attribute::as_uchar() const
{
	return _ubyte;
}

//////////////////////////////////////////////////////////////////////////
char m_attribute::as_char() const
{
	return _byte;
}
//////////////////////////////////////////////////////////////////////////
const char * m_attribute::as_char_array() const
{
	return reinterpret_cast<const char*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const unsigned char * m_attribute::as_unsigned_char_array() const
{
	return reinterpret_cast<unsigned char*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const int * m_attribute::as_int_array() const
{
	return reinterpret_cast<int*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const unsigned int * m_attribute::as_unsigned_int_array() const
{
	return reinterpret_cast<unsigned int*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const float * m_attribute::as_float_array() const
{
	return reinterpret_cast<const float*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const short * m_attribute::as_short_array() const
{
	return reinterpret_cast<const short*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const char * m_attribute::as_string() const
{
	return reinterpret_cast<const char*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const m_attribute * m_attribute::as_attribute_array() const
{
	return reinterpret_cast<const m_attribute*>(_pdata);
}

//////////////////////////////////////////////////////////////////////////
const unsigned int m_attribute::get_size() const
{
	return _size;
}

//////////////////////////////////////////////////////////////////////////
const unsigned int m_attribute::get_type() const
{
	return _flag;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::erase()
{
	if (_size)
	{
		switch(_flag)
		{
		case M_FLOAT_ARRAY:
			delete [] (float*)_pdata;
			break;
		case M_CHAR_ARRAY:
			delete [] (char*)_pdata;
			break;
		case M_UNSIGNED_CHAR_ARRAY:
			delete [] (unsigned char*)_pdata;
			break;
		case M_UNSIGNED_INT_ARRAY:
			delete [] (unsigned int*)_pdata;
			break;
		case M_INT_ARRAY:
			delete [] (int*)_pdata;
			break;
		case M_ATTRIBUTE_ARRAY:
			delete [] (m_attribute*)_pdata;
			break;
		case M_SHORT_ARRAY:
			delete [] (short*)_pdata;
			break;
		default:
			break;
		}
	}
	_pdata = 0;
	_size = 0;
}

//////////////////////////////////////////////////////////////////////////
void m_attribute::copy_from(const m_attribute * attr)
{
	_flag = attr->_flag;

	if (attr->_size)
	{
		_size = attr->_size;
		if (_flag == M_ATTRIBUTE_ARRAY)
		{
			_pdata = reinterpret_cast<void*>(new m_attribute[_size]);
			for (unsigned int i = 0; i < _size; ++i)
				(reinterpret_cast<m_attribute*>(_pdata))[i] = attr->as_attribute_array()[i];
		}
		else
		{
			unsigned int byte_size;
			unsigned int flag = _flag;
			flag &= ~m_attribute::M_ARRAY;
			if (flag & M_FLOAT)
			{
				_pdata = reinterpret_cast<void*>(new float[attr->_size]);
				byte_size = _size * sizeof(float);
			}

			if (flag & M_CHAR)
			{
				_pdata = reinterpret_cast<void*>(new char[attr->_size]);
				byte_size = _size * sizeof(char);
			}

			if (flag & M_SHORT)
			{
				_pdata = reinterpret_cast<void*>(new short[attr->_size]);
				byte_size = _size * sizeof(short);
			}

			if (flag & M_INT)
			{
				_pdata = reinterpret_cast<void*>(new int[attr->_size]);
				byte_size = _size * sizeof(int);
			}

			if (flag & M_UNSIGNED_INT)
			{
				_pdata = reinterpret_cast<void*>(new unsigned int[attr->_size]);
				byte_size = _size * sizeof(unsigned int);
			}

			if (flag & M_UNSIGNED_CHAR)
			{
				_pdata = reinterpret_cast<void*>(new unsigned char[attr->_size]);
				byte_size = _size * sizeof(unsigned char);
			}

			memcpy(_pdata, attr->_pdata, byte_size);
		}
	}
	else
		_udword = attr->_udword;

	_dico_cit it = attr->_attr.begin();
	while (it != attr->_attr.end())
	{
		m_attribute* attrib = new m_attribute(*(*it).second);
		_attr.insert(_dico_pair((*it).first,attrib));
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////
unsigned int m_attribute::get_num_attributes() const
{
	return (unsigned int)_attr.size();
}

//////////////////////////////////////////////////////////////////////////
const char * m_attribute::get_attribute_name(unsigned int i) const
{
	unsigned int idx = 0;
	_dico_cit it = _attr.begin();
	while (it != _attr.end())
	{
		if (idx == i)
			return (*it).first.c_str();
		++idx;
		++it;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
const m_attribute * m_attribute::get_attribute(unsigned int i) const
{
	unsigned int idx = 0;
	_dico_cit it = _attr.begin();
	while (it != _attr.end())
	{
		if (idx == i)
			return (*it).second;
		++idx;
		++it;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream& os ,const m_attribute & attr)
{
	static int tab = 0;
	unsigned int type = attr.get_type();

	if (type & m_attribute::M_ARRAY && type != m_attribute::M_UNASSIGNED)
	{
		os << "(array of ";
		type &= ~m_attribute::M_ARRAY;
	}
	else
	{
		os << "("; 
	}

	switch (type)
	{
	case m_attribute::M_FLOAT:{
		os << "float)";
							  }break;

	case m_attribute::M_CHAR:{
		os << "char)";
							 }break;

	case m_attribute::M_UNSIGNED_INT:{
		os << "unsigned int)";
									 }break;

	case m_attribute::M_UNSIGNED_CHAR:{
		os << "unsigned char)";
									  }break;

	case m_attribute::M_SHORT:{
		os << "short)";
							  }break;

	case m_attribute::M_INT:{
		os << "int)";
							}break;

	default:
		break;
	}

	if (type == m_attribute::M_UNASSIGNED)
	{
		os << "unassigned)";
	}

	os << std::endl;

	m_attribute::_dico_cit it = attr._attr.begin();

	while ( it != attr._attr.end())
	{
		for ( int i=0; i<tab-1; ++i )
		{
			os << " |   ";
		}

		if (tab > 0)
		{
			os << "o-->";
		}

		os << (*it).first.c_str() << ".";

		++tab;

		os << *(*it).second;

		--tab;

		++it;
	}

	return os;
}
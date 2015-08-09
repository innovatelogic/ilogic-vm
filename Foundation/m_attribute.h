#ifndef _M_ATTRIBUTE_
#define _M_ATTRIBUTE_

#ifdef _WIN32
#pragma once
#pragma warning (disable:4251) 
#endif

#include "platform_specific.h"
#include "m_streams.h"


class EXPORT m_attribute
{
	typedef std::map<std::string,m_attribute*>  _dico;
	typedef _dico::value_type                   _dico_pair;
	typedef _dico::iterator                     _dico_it;
	typedef _dico::const_iterator               _dico_cit;

public:
	enum
	{
		M_FLOAT                = 0x00000001,
		M_CHAR                 = 0x00000002,
		M_UNSIGNED_CHAR        = 0x00000004,
		M_UNSIGNED_INT         = 0x00000008,
		M_INT                  = 0x00000010,
		M_SHORT                = 0x00000040,

		M_ARRAY                = 0x00010000,
		M_FLOAT_ARRAY          = 0x00010001,
		M_CHAR_ARRAY           = 0x00010002,
		M_UNSIGNED_CHAR_ARRAY  = 0x00010004,
		M_UNSIGNED_INT_ARRAY   = 0x00010008,
		M_INT_ARRAY            = 0x00010010,
		M_ATTRIBUTE_ARRAY      = 0x00010020,
		M_SHORT_ARRAY          = 0x00010040,    
		M_STRING               = 0x00010002,

		M_UNASSIGNED           = 0xFFFFFFFF
	};

	m_attribute();
	m_attribute(const m_attribute& attr);
	~m_attribute();

	// l-value assignation
	m_attribute &          operator=           (const char * str);
	m_attribute &          operator=           (const float & val);
	m_attribute &          operator=           (const unsigned int & val);
	m_attribute &          operator=           (const int & val);
	m_attribute &          operator=           (const unsigned char & val);
	m_attribute &          operator=           (const char & val);
	m_attribute &          operator=           (const short & val);
	m_attribute &          operator=           (const m_attribute & attr);

	// validation tests
	bool                    is_null             () const;
	bool                    is_valid            () const;

	// accessor interface
	m_attribute &          operator[]          (const char * name);

	// array assignation
	void                    array               (const m_attribute * attribs, unsigned int size);
	void                    array               (const float * fvector, unsigned int size);
	void                    array               (const char * str, unsigned int size);
	void                    array               (const unsigned char * str, unsigned int size);
	void                    array               (const unsigned int * array, unsigned int size);
	void                    array               (const int * array, unsigned int size);
	void                    array               (const short * array, unsigned int size);


	// r-value accessors...
	float                   as_float            () const;
	unsigned int            as_unsigned_int     () const;
	unsigned int            as_int              () const;
	short                   as_word             () const;
	unsigned char           as_uchar            () const;
	char                    as_char             () const;
	const char *            as_char_array       () const;
	const unsigned char *   as_unsigned_char_array() const;
	const int *             as_int_array        () const;
	const unsigned int *    as_unsigned_int_array() const;
	const float *           as_float_array      () const;
	const short *           as_short_array      () const;
	const char *            as_string           () const;
	const m_attribute *     as_attribute_array  () const;

	// retrieve the size array of the array
	const unsigned int      get_size            () const;

	// retrieve the type of the attribute
	const unsigned int      get_type            () const;

	unsigned int            get_num_attributes  () const;
	const char *            get_attribute_name  (unsigned int i) const;
	const m_attribute *     get_attribute       (unsigned int i) const;


protected:
	// delete the container and reset the value to zero
	void                    erase               ();
	void                    copy_from           (const m_attribute * attr);

private: 

	// dictionnary
	_dico                   _attr;

	friend EXPORT std::ostream & operator << ( std::ostream & os,  const m_attribute & att);
	friend m_input_stream & operator >> ( m_input_stream & rInputStream, m_attribute & rAttribute);
	friend m_output_stream & operator << ( m_output_stream & rOutputStream, const m_attribute & rAttribute); 	 


public:
	// 32 bit union for data storage
	union
	{
		void *              _pdata;
		unsigned char       _ubyte;
		unsigned char       _byte;
		short               _word;
		unsigned int        _udword;
		unsigned int        _sdword;
		float               _float;
	};


	// to be only used for arrays only. The size is always in reference to the type.
	// example: if the type is float and size is 4, there is 3 floats in the array
	unsigned int    _size; 

	// used to tell whether an attribute contains a valid value as well as its type
	unsigned int    _flag;
};

#endif
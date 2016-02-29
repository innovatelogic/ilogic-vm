#include "m_streams.h"

#include <assert.h>

#include "vector2f.h"
#include "vector4f.h"
#include "vector.h"

#include "matrix3.h"
#include "matrix.h"
#include "quat.h"

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, bool bBool)
{
	assert(sizeof(bBool)==1);
	rOutputStream.write(&bBool,1);

	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write a char.
m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned char nChar)
{
	assert(1 == sizeof(nChar));
	rOutputStream.write(&nChar,1);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, char nChar)
{
	assert(1 == sizeof(nChar));
	rOutputStream.write(&nChar,1);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned short nShort)
{
	assert(2 == sizeof(nShort));
	rOutputStream.write(&nShort,2);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, short nShort)
{
	assert(2 == sizeof(nShort));
	rOutputStream.write(&nShort,2);

	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write an unsigned int.
m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned int nInt)
{
	assert(4 == sizeof(nInt));

	rOutputStream.write(&nInt, 4);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, int nInt)
{
	assert(4 == sizeof(nInt));

	rOutputStream.write(&nInt, 4);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream &
operator << (m_output_stream & rOutputStream, float nFloat)
{
	assert(4 == sizeof(nFloat));

	rOutputStream.write(&nFloat, 4);

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream& operator << (m_output_stream & rOutputStream, double nDouble)
{
	assert(8 == sizeof(nDouble));

	rOutputStream.write(&nDouble, 8);

	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write a Vector2f
m_output_stream& operator << (m_output_stream & rOutputStream, const Vector2f & rVector)
{
	rOutputStream << rVector.x << rVector.y;

	return rOutputStream;
}

//////////////////////////////////////////////////////////////////////////
m_output_stream& operator << (m_output_stream & rOutputStream, const Vector & rVector)
{
	rOutputStream << rVector.x << rVector.y << rVector.z;

	return rOutputStream;
}

//------------------------------------------------------------------------
m_output_stream& operator << (m_output_stream & rOutputStream, const Vector4f & rVector)
{
	rOutputStream << rVector.x << rVector.y << rVector.z << rVector.w;

	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write a Matrix3f
EXPORT m_output_stream& operator << (m_output_stream & rOutputStream, const Matrix3f & rMatrix)
{
	for (int i = 0; i < 9; i++)
	{
		rOutputStream << rMatrix.m[i];
	}
	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write a Matrix
m_output_stream& operator << (m_output_stream & rOutputStream, const Matrix & rMatrix)
{
	for (int i=0; i<16; i++)
	{
		rOutputStream << rMatrix.m[i];
	}
	return rOutputStream;
}

//------------------------------------------------------------------------
/// Write a quaternion.
m_output_stream& operator << (m_output_stream & rOutputStream, const Quaternion & rQuaternion)
{
	rOutputStream << rQuaternion.x << rQuaternion.y << rQuaternion.z << rQuaternion.w;

	return rOutputStream;
}

//------------------------------------------------------------------------
m_input_stream& operator >> (m_input_stream & rInputStream, bool & bBool)
{
	assert(1 == sizeof(bBool));

	char * pTemp = reinterpret_cast<char *>(&bBool);

	rInputStream.read(pTemp, 1);

	bBool = ((*pTemp) ? true : false);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read an unsigned char.
m_input_stream& operator >> (m_input_stream & rInputStream, unsigned char & nChar)
{
	assert(1 == sizeof(nChar));

	rInputStream.read(&nChar, 1);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a char.
m_input_stream& operator >> (m_input_stream & rInputStream, char & nChar)
{
	assert(1 == sizeof(nChar));

	rInputStream.read(&nChar, 1);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read an unsigned short.
m_input_stream& operator >> (m_input_stream & rInputStream, unsigned short & nShort)
{
	assert(2 == sizeof(nShort));

	rInputStream.read(&nShort, 2);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a short.
m_input_stream& operator >> (m_input_stream & rInputStream, short & nShort)
{
	assert(2 == sizeof(nShort));

	rInputStream.read(&nShort, 2);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read an unsigned int.
m_input_stream& operator >> (m_input_stream & rInputStream, unsigned int & nInt)
{
	assert(4 == sizeof(nInt));

	rInputStream.read(&nInt, 4);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read an int.
m_input_stream &
operator >> (m_input_stream & rInputStream, int & nInt)
{
	assert(4 == sizeof(nInt));

	rInputStream.read(&nInt, 4);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a float.
m_input_stream& operator >> (m_input_stream & rInputStream, float & nFloat)
{
	assert(4 == sizeof(nFloat));

	rInputStream.read(&nFloat, 4);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a double.
m_input_stream& operator >> (m_input_stream & rInputStream, double & nDouble)
{
	assert(8 == sizeof(nDouble));

	rInputStream.read(&nDouble, 8);

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a Vector2f
m_input_stream& operator >> (m_input_stream & rInputStream, Vector2f & rVector)
{
	rInputStream.read(rVector.vec_array, 2*sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a Vector
m_input_stream& operator >> (m_input_stream & rInputStream, Vector & rVector)
{
	rInputStream.read(rVector.vec_array, 3*sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a Vector4f
m_input_stream& operator >> (m_input_stream & rInputStream, Vector4f & rVector)
{
	rInputStream.read(rVector.vec_array, 4*sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a Matrix3f
m_input_stream& operator >> (m_input_stream & rInputStream, Matrix3f & rMatrix)
{
	rInputStream.read(rMatrix.m, 9 * sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a Matrix
m_input_stream& operator >> (m_input_stream & rInputStream, Matrix & rMatrix)
{
	rInputStream.read(rMatrix.m, 16*sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
/// Read a quaternion
m_input_stream& operator >> (m_input_stream & rInputStream, Quaternion & rQuaternion)
{
	rInputStream.read(rQuaternion.q, 4*sizeof(float));

	return rInputStream;
}

//------------------------------------------------------------------------
m_output_stream& operator << (m_output_stream & rOutputStream, const m_attribute & rAttribute)
{

    rOutputStream << rAttribute.get_num_attributes();

    for (unsigned int i = 0; i < rAttribute.get_num_attributes(); ++i)
    {
        rOutputStream << (unsigned int)strlen(rAttribute.get_attribute_name(i));

        rOutputStream.write((void*)rAttribute.get_attribute_name(i),
            (int)strlen(rAttribute.get_attribute_name(i)));

        rOutputStream << *(rAttribute.get_attribute(i));
    }

    rOutputStream << rAttribute._size;
    rOutputStream << rAttribute._flag;

    switch (rAttribute._flag)
    {
    case m_attribute::M_ATTRIBUTE_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_attribute_array()[iAttribute];
    }break;

    case m_attribute::M_FLOAT_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_float_array()[iAttribute];
    }break;

    case m_attribute::M_CHAR_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_char_array()[iAttribute];
    }break;

    case m_attribute::M_SHORT_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_short_array()[iAttribute];
    }break;

    case m_attribute::M_INT_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_int_array()[iAttribute];
    }break;

    case m_attribute::M_UNSIGNED_INT_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_unsigned_int_array()[iAttribute];
    }break;

    case m_attribute::M_UNSIGNED_CHAR_ARRAY:
    {
        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rOutputStream << rAttribute.as_unsigned_char_array()[iAttribute];
    }break;
    }

    // and save the 32-bit data word 
    // if it's not used as data pointer.
    if (rAttribute._size == 0)
        rOutputStream << rAttribute._udword;

    return rOutputStream;
}

//------------------------------------------------------------------------
m_input_stream& operator >> (m_input_stream & rInputStream, m_attribute & rAttribute)
{
    // Read the dictionary. First its size
    unsigned int nEntries;
    rInputStream >> nEntries;

    // ... then its elements.
    for (unsigned int iAttribute = 0; iAttribute < nEntries; ++iAttribute)
    {
        int nNameLength;
        rInputStream >> nNameLength;

        char * zName = new char[nNameLength + 1];
        zName[nNameLength] = '\0';

        rInputStream.read(zName, nNameLength);

        m_attribute oAttribute;
        rInputStream >> oAttribute;

        rAttribute[zName] = oAttribute;

        delete zName;
    }

    rInputStream >> rAttribute._size;
    rInputStream >> rAttribute._flag;

    switch (rAttribute._flag)
    {
    case m_attribute::M_ATTRIBUTE_ARRAY:
    {
        m_attribute * aAttributes = new m_attribute[rAttribute._size];

        for (unsigned int iAttribute = 0; iAttribute < rAttribute._size; ++iAttribute)
            rInputStream >> aAttributes[iAttribute];

        rAttribute.array(aAttributes, rAttribute._size);

        delete aAttributes;
    }break;

    case m_attribute::M_FLOAT_ARRAY:
    {
        float * aFloats = new float[rAttribute._size];

        for (unsigned int iFloat = 0; iFloat < rAttribute._size; ++iFloat)
            rInputStream >> aFloats[iFloat];

        rAttribute.array(aFloats, rAttribute._size);

        delete aFloats;

    }break;

    case m_attribute::M_CHAR_ARRAY:
    {
        char * aChars = new char[rAttribute._size];

        for (unsigned int iChar = 0; iChar < rAttribute._size; ++iChar)
            rInputStream >> aChars[iChar];

        rAttribute.array(aChars, rAttribute._size);

        delete aChars;
    }break;

    case m_attribute::M_SHORT_ARRAY:
    {
        short * aShorts = new short[rAttribute._size];

        for (unsigned int iShort = 0; iShort < rAttribute._size; ++iShort)
            rInputStream >> aShorts[iShort];

        rAttribute.array(aShorts, rAttribute._size);

        delete aShorts;
    }break;

    case m_attribute::M_INT_ARRAY:
    {
        int * aInts = new int[rAttribute._size];

        for (unsigned int iInt = 0; iInt < rAttribute._size; ++iInt)
            rInputStream >> aInts[iInt];

        rAttribute.array(aInts, rAttribute._size);

        delete aInts;
    }break;

    case m_attribute::M_UNSIGNED_INT_ARRAY:
    {
        unsigned int * aInts = new unsigned int[rAttribute._size];

        for (unsigned int iInt = 0; iInt < rAttribute._size; ++iInt)
            rInputStream >> aInts[iInt];
        rAttribute.array(aInts, rAttribute._size);

        delete aInts;
    }break;

    case m_attribute::M_UNSIGNED_CHAR_ARRAY:
    {
        unsigned char * aChars = new unsigned char[rAttribute._size];

        for (unsigned int iChar = 0; iChar < rAttribute._size; ++iChar)
            rInputStream >> aChars[iChar];

        rAttribute.array(aChars, rAttribute._size);

        delete aChars;
    }break;
    }

    // and save the 32-bit data word 
    // if it's not used as data pointer.
    if (rAttribute._size == 0)
        rInputStream >> rAttribute._udword;

    return rInputStream;
}
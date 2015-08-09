#ifndef _m_input_output_stream_h_
#define _m_input_output_stream_h_

#pragma once

#include "platform_specific.h"

class Vector2f;
class Vector4f;
class Vector;
class Matrix3f;
class Matrix;
class Quaternion;
class m_attribute;

//----------------------------------------------------------------------------------------------
class EXPORT m_input_stream
{
public:
	virtual ~m_input_stream()
	{
	}

	virtual int read(void *pData, int nBytes) = 0;
};

class EXPORT m_output_stream
{
public:
	virtual ~m_output_stream()
	{
	}

	virtual int write(const void *pData, int nBytes) = 0;
};

class EXPORT m_input_output_stream
	: public virtual m_input_stream,
	  public virtual m_output_stream
{
public:
	virtual ~m_input_output_stream()
	{
	}

	virtual int read(void *pData,int nBytes) = 0;
	virtual int write(const void *pData,int nBytes) = 0;
};

/// Write a boolean value.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, bool bBool);

/// Write an unsigned char.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned char nChar);

EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, char nChar);

/// Write an unsigned short.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned short nShort);

/// Write a short.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, short nShort);

///Write an unsigned int
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, unsigned int nInt);

///Write an int
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, int nInt);

/// Write a float.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, float nFloat);

/// Write a double.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, double nDouble);

/// Write a Vector2f
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Vector2f & rVector);

/// Write a Vector
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Vector & rVector);

/// Write a Vector4f
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Vector4f & rVector);

/// Write a Matrix3f
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Matrix3f & rMatrix);

/// Write a Matrix
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Matrix & rMatrix);

/// Write a quaternion.
EXPORT m_output_stream &
operator << (m_output_stream & rOutputStream, const Quaternion & rQuaternion);

/// Write an m_attribute
EXPORT m_output_stream & 
operator << (m_output_stream & rOutputStream, const m_attribute & rAttribute);

//
// Extracting functions
//

/// Read a boolean.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, bool & bBool);

/// Read an unsigned char.
EXPORT m_input_stream & 
operator >> (m_input_stream & rInputStream, unsigned char & nChar);

/// Read a char.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, char & nChar);

/// Read an unsigned short.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, unsigned short & nShort);

/// Read a short.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, short & nShort);

/// Read an unsigned int.
EXPORT m_input_stream & 
operator >> (m_input_stream & rInputStream, unsigned int & nInt);

/// Read an int.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, int & nInt);

/// Read a float.
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, float & nFloat);

///Read a double
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, double & nDouble);

/// Read a Vector2f
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Vector2f & rVector);

/// Read a Vector
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Vector & rVector);

/// Read a Vector4f
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Vector4f & rVector);

/// Read a Matrix3f
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Matrix3f & rMatrix);

/// Read a Matrix
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Matrix & rMatrix);

/// Read a quaternion
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, Quaternion & rQuaternion);

/// Read an m_attribute
EXPORT m_input_stream &
operator >> (m_input_stream & rInputStream, m_attribute & rAttribute);

#endif//_m_input_output_stream_h_
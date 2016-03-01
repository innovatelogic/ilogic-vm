#ifndef _streams_file_
#define _streams_file_

#pragma once

#include "StdafxFoundation.h"

//////////////////////////////////////////////////////////////////////////
class EXPORT input_stream
{
public:
	virtual ~input_stream()
	{
	}

	virtual int read(void *pData,int nBytes) = 0;
};

//////////////////////////////////////////////////////////////////////////
class EXPORT output_stream
{
public:
	virtual ~output_stream()
	{
	}

	virtual int write(const void *pData,int nBytes) = 0;
};

//////////////////////////////////////////////////////////////////////////
class EXPORT input_output_stream
	 :public virtual input_stream,
	         virtual output_stream
{
public:
	virtual ~input_output_stream()
	{
	}

	virtual int read(void *pData,int nBytes) = 0;
	virtual int write(const void *pData,int nBytes) = 0;
};

/// Write a boolean value.
EXPORT output_stream &
operator << (output_stream & rOutputStream, bool bBool);


/// Write an unsigned char.
EXPORT output_stream &
operator << (output_stream & rOutputStream, unsigned char nChar);

EXPORT output_stream &
operator << (output_stream & rOutputStream, char nChar);

/// Write an unsigned short.
EXPORT output_stream &
operator << (output_stream & rOutputStream, unsigned short nShort);

/// Write a short.
EXPORT output_stream &
operator << (output_stream & rOutputStream, short nShort);

///Write an unsigned int
EXPORT output_stream &
operator << (output_stream & rOutputStream, unsigned int nInt);

///Write an int
EXPORT output_stream &
operator << (output_stream & rOutputStream, int nInt);

/// Write a float.
EXPORT output_stream &
operator << (output_stream & rOutputStream, float nFloat);

/// Write a double.
EXPORT output_stream &
operator << (output_stream & rOutputStream, double nDouble);

/// Write a Vector2f
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Vector2f & rVector);

/// Write a Vector
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Vector & rVector);

/// Write a Vector4f
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Vector4f & rVector);

/// Write a Matrix3f
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Matrix3f & rMatrix);

/// Write a Matrix
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Matrix & rMatrix);

/// Write a quaternion.
EXPORT output_stream &
operator << (output_stream & rOutputStream, const Quaternion & rQuaternion);

/// Write a std::string
EXPORT output_stream &
operator << (output_stream & rOutputStream, const std::string & _string);

//
// Extracting functions
//

/// Read a boolean.
EXPORT input_stream &
operator >> (input_stream & rInputStream, bool & bBool);

/// Read an unsigned char.
EXPORT input_stream & 
operator >> (input_stream & rInputStream, unsigned char & nChar);

/// Read a char.
EXPORT input_stream &
operator >> (input_stream & rInputStream, char & nChar);

/// Read an unsigned short.
EXPORT input_stream &
operator >> (input_stream & rInputStream, unsigned short & nShort);

/// Read a short.
EXPORT input_stream &
operator >> (input_stream & rInputStream, short & nShort);

/// Read an unsigned int.
EXPORT input_stream & 
operator >> (input_stream & rInputStream, unsigned int & nInt);

/// Read an int.
EXPORT input_stream &
operator >> (input_stream & rInputStream, int & nInt);

/// Read a float.
EXPORT input_stream &
operator >> (input_stream & rInputStream, float & nFloat);

///Read a double
EXPORT input_stream &
operator >> (input_stream & rInputStream, double & nDouble);

/// Read a Vector2f
EXPORT input_stream &
operator >> (input_stream & rInputStream, Vector2f & rVector);

/// Read a Vector
EXPORT input_stream &
operator >> (input_stream & rInputStream, Vector & rVector);

/// Read a Vector4f
EXPORT input_stream &
operator >> (input_stream & rInputStream, Vector4f & rVector);

/// Read a Matrix3f
EXPORT input_stream &
operator >> (input_stream & rInputStream, Matrix3f & rMatrix);

/// Read a Matrix
EXPORT input_stream &
operator >> (input_stream & rInputStream, Matrix & rMatrix);

/// Read a quaternion
EXPORT input_stream &
operator >> (input_stream & rInputStream, Quaternion & rQuaternion);

// Read std::string
EXPORT input_stream &
operator << (input_stream& rOutputStream, std::string & _string);

#endif
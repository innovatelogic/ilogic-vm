#include "Streams.h"
#include "vector2f.h"
#include "vector4f.h"
#include "vector.h"
#include "matrix3.h"
#include "matrix.h"
#include "quat.h"
#include <assert.h>

//-----------------------------------------------------------------------------
output_stream & operator << (output_stream & rOutputStream, bool bBool)
{
	assert(sizeof(bBool)==1);
	rOutputStream.write(&bBool,1);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
/// Write a char.
output_stream& operator << (output_stream & rOutputStream, unsigned char nChar)
{
	assert(1 == sizeof(nChar));
	rOutputStream.write(&nChar,1);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, char nChar)
{
	assert(1 == sizeof(nChar));
	rOutputStream.write(&nChar,1);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, unsigned short nShort)
{
	assert(2 == sizeof(nShort));
	rOutputStream.write(&nShort,2);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, short nShort)
{
	assert(2 == sizeof(nShort));
	rOutputStream.write(&nShort,2);

	return rOutputStream;
}
//-----------------------------------------------------------------------------
/// Write an unsigned int.
output_stream& operator << (output_stream & rOutputStream, unsigned int nInt)
{
	assert(4 == sizeof(nInt));

	rOutputStream.write(&nInt, 4);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, int nInt)
{
	assert(4 == sizeof(nInt));

	rOutputStream.write(&nInt, 4);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, float nFloat)
{
	assert(4 == sizeof(nFloat));

	rOutputStream.write(&nFloat, 4);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, double nDouble)
{
	assert(8 == sizeof(nDouble));

	rOutputStream.write(&nDouble, 8);

	return rOutputStream;
}

//-----------------------------------------------------------------------------
/// Write a Vector2f
output_stream& operator << (output_stream & rOutputStream, const Vector2f & rVector)
{
	rOutputStream << rVector.x << rVector.y;

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, const Vector & rVector)
{
	rOutputStream << rVector.x << rVector.y << rVector.z;

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, const Vector4f & rVector)
{
	rOutputStream << rVector.x << rVector.y << rVector.z << rVector.w;

	return rOutputStream;
}

//-----------------------------------------------------------------------------
/// Write a Matrix3f
EXPORT output_stream& operator << (output_stream & rOutputStream, const Matrix3f & rMatrix)
{
	for(int i=0;i<9;i++)
	{
		rOutputStream << rMatrix.m[i];
	}
	return rOutputStream;
}

//-----------------------------------------------------------------------------
// Write a Matrix
output_stream& operator << (output_stream & rOutputStream, const Matrix & rMatrix)
{
	for(int i=0;i<16;i++)
	{
		rOutputStream << rMatrix.m[i];
	}
	return rOutputStream;
}

//-----------------------------------------------------------------------------
// Write a quaternion.
output_stream& operator << (output_stream & rOutputStream, const Quaternion & rQuaternion)
{
	rOutputStream << rQuaternion.x << rQuaternion.y << rQuaternion.z << rQuaternion.w;

	return rOutputStream;
}

//-----------------------------------------------------------------------------
output_stream& operator << (output_stream & rOutputStream, const std::string & _string)
{
    unsigned int size =  (unsigned int)_string.size();
	
	rOutputStream << size;

	for(unsigned int i=0; i<size; ++i)
	{
       rOutputStream << *(_string.c_str()+i);
	}

	return rOutputStream;
}

//-----------------------------------------------------------------------------
input_stream& operator >> (input_stream & rInputStream, bool & bBool)
{
	assert(1 == sizeof(bBool));

	char * pTemp = reinterpret_cast<char *>(&bBool);

	rInputStream.read(pTemp, 1);

	bBool = ((*pTemp) ? true : false);

	return rInputStream;
}

//-----------------------------------------------------------------------------
// Read an unsigned char.
input_stream& operator >> (input_stream & rInputStream, unsigned char & nChar)
{
	assert(1 == sizeof(nChar));

	rInputStream.read(&nChar, 1);

	return rInputStream;
}

//-----------------------------------------------------------------------------
// Read a char.
input_stream& operator >> (input_stream & rInputStream, char & nChar)
{
	assert(1 == sizeof(nChar));

	rInputStream.read(&nChar, 1);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read an unsigned short.
input_stream& operator >> (input_stream & rInputStream, unsigned short & nShort)
{
	assert(2 == sizeof(nShort));

	rInputStream.read(&nShort, 2);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a short.
input_stream& operator >> (input_stream & rInputStream, short & nShort)
{
	assert(2 == sizeof(nShort));

	rInputStream.read(&nShort, 2);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read an unsigned int.
input_stream& operator >> (input_stream & rInputStream, unsigned int & nInt)
{
	assert(4 == sizeof(nInt));

	rInputStream.read(&nInt, 4);

	return rInputStream;
}

//-----------------------------------------------------------------------------
// Read an int.
input_stream& operator >> (input_stream & rInputStream, int & nInt)
{
	assert(4 == sizeof(nInt));

	rInputStream.read(&nInt, 4);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a float.
input_stream& operator >> (input_stream & rInputStream, float & nFloat)
{
	assert(4 == sizeof(nFloat));

	rInputStream.read(&nFloat, 4);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a double.
input_stream& operator >> (input_stream & rInputStream, double & nDouble)
{
	assert(8 == sizeof(nDouble));

	rInputStream.read(&nDouble, 8);

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a Vector2f
input_stream& operator >> (input_stream & rInputStream, Vector2f & rVector)
{
	rInputStream.read(rVector.vec_array, 2*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a Vector
input_stream& operator >> (input_stream & rInputStream, Vector & rVector)
{
	rInputStream.read(rVector.vec_array, 3*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a Vector4f
input_stream& operator >> (input_stream & rInputStream, Vector4f & rVector)
{
	rInputStream.read(rVector.vec_array, 4*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a Matrix3f
input_stream& operator >> (input_stream & rInputStream, Matrix3f & rMatrix)
{
	rInputStream.read(rMatrix.m, 9*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a Matrix
input_stream& operator >> (input_stream & rInputStream, Matrix & rMatrix)
{
	rInputStream.read(rMatrix.m, 16*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
/// Read a quaternion
input_stream& operator >> (input_stream & rInputStream, Quaternion & rQuaternion)
{
	rInputStream.read(rQuaternion.q, 4*sizeof(float));

	return rInputStream;
}

//-----------------------------------------------------------------------------
input_stream& operator >> (input_stream& rInputStream, std::string & _string)
{
   int size;

   rInputStream >> size;

   _string.reserve(size+1);
   
   // read string 

   for(int i=0;i<size;++i)
   {
	   char symbol;

	   rInputStream >> symbol;

	   _string += symbol; 
   }

   return rInputStream;
}
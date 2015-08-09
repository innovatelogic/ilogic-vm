#include "m_texcoord_set.h"
#include "m_streams.h"

//------------------------------------------------------------------------
m_texcoord_set::m_texcoord_set()
: dim(0)
, texcoords(0)
, binormals(0)
, tangents(0)
{
	
}

//------------------------------------------------------------------------
m_texcoord_set::~m_texcoord_set()
{
	if (texcoords)
	{
		delete [] texcoords;
		texcoords = 0;
	}

	if (binormals)
	{
		delete [] binormals;
		binormals = 0;
	}

	if (tangents)
	{
		delete [] tangents;
		tangents = 0;
	}
}

//------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_texcoord_set & rTexCoordSet)
{
	rOutputStream << rTexCoordSet.num_coords;

	rOutputStream << rTexCoordSet.dim;

	unsigned int iCoord, iDim;

	for (iCoord = 0; iCoord < rTexCoordSet.num_coords; ++iCoord)
	{
		for (iDim = 0; iDim < rTexCoordSet.dim; ++iDim)
		{
		   rOutputStream << rTexCoordSet.texcoords[rTexCoordSet.dim * iCoord + iDim];
		}

		rOutputStream << rTexCoordSet.binormals[iCoord];
		rOutputStream << rTexCoordSet.tangents[iCoord];
	}

    return rOutputStream;
}

//------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_texcoord_set & rTexCoordSet)
{
	rInputStream >> rTexCoordSet.num_coords;
	rInputStream >> rTexCoordSet.dim;

	if (0 == rTexCoordSet.num_coords)
	{
		rTexCoordSet.texcoords = 0;
		rTexCoordSet.binormals = 0;
		rTexCoordSet.tangents  = 0;
	}
	else
	{
		rTexCoordSet.texcoords = new float[rTexCoordSet.num_coords * rTexCoordSet.dim];
		rTexCoordSet.binormals = new Vector[rTexCoordSet.num_coords];
		rTexCoordSet.tangents  = new Vector[rTexCoordSet.num_coords];
	}

	unsigned int iCoord, iDim;

	for (iCoord = 0; iCoord < rTexCoordSet.num_coords; ++iCoord)
	{
		for (iDim = 0; iDim < rTexCoordSet.dim; ++iDim)
		{
			rInputStream >> rTexCoordSet.texcoords[rTexCoordSet.dim * iCoord + iDim];
		}
			
		rInputStream >> rTexCoordSet.binormals[iCoord];
		rInputStream >> rTexCoordSet.tangents[iCoord];
	}

    return rInputStream;
}

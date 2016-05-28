/**********************************************************************
 *<
	FILE: SkinEngine.cpp

	DESCRIPTION:  Implementation of the SkinEngine

	CREATED BY: Nikolai Sander
				
	HISTORY: created 7/10/99

 *>	Copyright (c) 1999, All Rights Reserved.
 **********************************************************************/

#include "stdafx.h"
#include "MAXComponents.h"
#include "SkinEngine.h"
#include "defines.h"


/////////////////////////////////////////////////////////////////////////////
// CSkinEngine

CSkinEngine::CSkinEngine()
{
	BoneData.removeAll();
	PointData.removeAll();
}

CSkinEngine::~CSkinEngine()
{
	BoneData.removeAll();
	PointData.removeAll();
}


STDMETHODIMP CSkinEngine::SetNumPoints(int numPoints)
{

	PointData.setLengthUsed(numPoints);

	return S_OK;
}

STDMETHODIMP CSkinEngine::SetNumBones(int numBones)
{

	if(BoneData.lengthUsed() != numBones)
	{
		BoneData.setLengthUsed(numBones);		
	}
	
	return S_OK;
}

STDMETHODIMP CSkinEngine::SetPointData(int pointIdx, int numData, 
										DWORD b_stride, int *BoneIndexArray, 
										DWORD w_stride, float *WeightArray, 
										DWORD sci_stride, int *SubCurveIdxArray, 
										DWORD ssi_stride, int *SubSegIdxArray, 
										DWORD ssd_stride, float *SubSegDistArray, 
										DWORD t_stride, float *TangentsArray, 
										DWORD op_stride, float *OPointsArray)
{
	PointDataClass& data = PointData[pointIdx];
	// Bone Data
	data.m_BoneIndices = StrideArray<int>(BoneIndexArray, b_stride);
	data.m_BoneWeights = StrideArray<float>(WeightArray, w_stride);
	data.m_numBones = numData;
	
	// Spline Data
	data.m_SubCurveIndices = StrideArray<int>(SubCurveIdxArray, sci_stride);
	data.m_SubSegIndices = StrideArray<int>(SubSegIdxArray, ssi_stride);
	data.m_SubSegDistance = StrideArray<float>( SubSegDistArray, ssd_stride);
	data.m_Tangents = StrideArray<Point3>( (Point3 *) TangentsArray, t_stride);
	data.m_OPoints = StrideArray<Point3>( (Point3 *) OPointsArray, op_stride);


	return S_OK;
}

STDMETHODIMP CSkinEngine::SetInitBoneTM(int boneIdx, float *InitTM)
{
	BoneData[boneIdx].SetInitTM(InitTM);
	return S_OK;
}

STDMETHODIMP CSkinEngine::SetBoneTM(int boneIdx, float *currentTM)
{
	BoneData[boneIdx].SetCurrentTM(Matrix3(&currentTM[0],&currentTM[3],&currentTM[6],&currentTM[9]));
	
	return S_OK;
}

STDMETHODIMP CSkinEngine::MapPoint(int idx, float *pin, float *pout)
{
	Point3 p(pin);
//	Point3 initP(10.f,10.f,-100.f);
//	initP.x = p[0];
//	initP.y = p[1];
//	initP.z = p[2];
	PointDataClass& data = PointData[idx];
	if(data.m_numBones > 0)
	{

		Point3 tp(0.0f,0.0f,0.0f);
		float influence = 0.0f;
		if (data.m_numBones ==1)
		{
			Point3 vec;
			
			float influ = data.m_BoneWeights[0];
			
			int bid;
			bid = data.m_BoneIndices[0];
			BoneDataClass& boneDataRef = BoneData[bid];
			vec = (p*boneDataRef.GetXFormTM(m_MeshTM));
			vec = vec - p;
			vec = vec * influ;
			p += vec;
			
			// Spline animation :
			
			if ((boneDataRef.GetFlags() & BONE_SPLINE_FLAG) && (influ != 0.0f))
			{
				p = SplineAnimation(idx,0,p);
			}
			
			pout[0] = p[0] ; pout[1] = p[1] ; pout[2] = p[2];
			return S_OK;
		}
		for (int j=0;j<data.m_numBones;j++)
		{
			float influ = data.m_BoneWeights[j];
			int bid = data.m_BoneIndices[j];
			if (influ != 0.0f)
			{
				tp  += (p*BoneData[bid].GetXFormTM(m_MeshTM))*influ;
				influence += influ;
			}
		}
		
		// Spline animation :
		
		for (int j=0;j<data.m_numBones;j++)
		{
			int bid = data.m_BoneIndices[j];
			if (BoneData[bid].GetFlags() & BONE_SPLINE_FLAG) 
			{
				float influ = data.m_BoneWeights[j];
				
				if (influ != 0.0f)
				{
					tp = SplineAnimation(idx,j,tp);
				}
			}
		}
		
		if (influence > 0.00001)
		{
			pout[0] = tp[0] ; pout[1] = tp[1] ; pout[2] = tp[2];
			return S_OK;
		}
	}
	pout[0] = p[0];
	pout[1] = p[1];
	pout[2] = p[2];
	return S_OK;
}

STDMETHODIMP CSkinEngine::SetInitTM(float *InitTM)
{
	m_MeshTM = Matrix3(&InitTM[0],&InitTM[3],&InitTM[6],&InitTM[9]);

	return S_OK;
}

STDMETHODIMP CSkinEngine::SetBoneFlags(int boneIdx, DWORD flags)
{
	BoneData[boneIdx].SetFlags(flags);
	return S_OK;
}

Point3 CSkinEngine::SplineAnimation(int vertex, int bone, const Point3& p)
{
	Point3 ps(0.0f,0.0f,0.0f),pr(0.0f,0.0f,0.0f),pdef(0.0f,0.0f,0.0f),pt(0.0f,0.0f,0.0f);
	Point3 MovedU,MovedTan;
	PointDataClass& data = PointData[vertex];
	int bid = data.m_BoneIndices[bone];
	int cid = data.m_SubCurveIndices[bone];
	int sid = data.m_SubSegIndices[bone];
	float u = data.m_SubSegDistance[bone];
	
	Matrix3 tm    = Inverse(m_MeshTM * Inverse(BoneData[bid].GetCurrentTM()));
	
	Fire_GetInterpCurvePiece3D(bid, cid, sid ,u, MovedU);
	MovedU = MovedU * tm;

	Fire_GetTangentPiece3D(bid, cid, sid, u, MovedTan);
	MovedTan = VectorTransform(tm,MovedTan);
	
	Point3 OPoint;
	OPoint = data.m_OPoints[bone] * tm;
	Point3 OTan = VectorTransform(tm,data.m_Tangents[bone]);
	
	float s = 1.0f;  //scale 
	float angle = 0.0f;
	float influ = data.m_BoneWeights[bone];
	
	OTan = Normalize(OTan);
	MovedTan = Normalize(MovedTan);
	
	float dot = 0.0f;
	
	if ( OTan != MovedTan)
	{
		dot = DotProd(OTan,MovedTan);
		if (fabs(1.0-fabs(dot)) >= 0.000001f)
			angle = (float) acos(DotProd(OTan,MovedTan)) * influ;
	}
	
	Point3 perp = CrossProd(OTan,MovedTan);
	Matrix3 RotateMe(1);
	
	if (dot != 1.0f)
		RotateMe = RotAngleAxisMatrix(Normalize(perp), angle);
	
	ps = p-OPoint;
	pr = ps * RotateMe + OPoint;
	pt = (MovedU - OPoint) * influ;
	pdef = pr + pt;
	return pdef;
	
}

void BoneDataClass::SetInitTM(float *ptm)
{
	bCacheValid = false;
	m_InitTM = Matrix3( &ptm[0], &ptm[3], &ptm[6], &ptm[9]);
}

void BoneDataClass::SetCurrentTM(Matrix3 &tm)
{
	bCacheValid = false;
	m_CurrentTM = tm;
}
void BoneDataClass::SetFlags(DWORD flags)
{
	m_flags = flags;
}

Matrix3 &BoneDataClass::GetXFormTM(Matrix3 &MeshTM)
{
	if(!bCacheValid)		
	{
		m_XFormTM = MeshTM * Inverse(m_InitTM) * m_CurrentTM * Inverse(MeshTM);		
		bCacheValid = true;
	}
	
	return m_XFormTM;
}

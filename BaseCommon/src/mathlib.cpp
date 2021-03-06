
#include "mathlib.h"

namespace oes
{
namespace common_base
{

#define USE_EPSILON_TEST true
#define EPSILON 0.000001


//! sort so that a<=b
#define SORT(a,b)      \
    if(a>b)            \
    {                  \
    const float c=a;   \
    a=b;               \
    b=c;               \
    }

//! Edge to edge test based on Franlin Antonio's gem: "Faster Line Segment Intersection",
//in Graphics Gems III, pp. 199-202
#define EDGE_EDGE_TEST(V0, U0, U1)						\
    Bx = U0[i0] - U1[i0];								\
    By = U0[i1] - U1[i1];								\
    Cx = V0[i0] - U0[i0];								\
    Cy = V0[i1] - U0[i1];								\
    f  = Ay*Bx - Ax*By;									\
    d  = By*Cx - Bx*Cy;									\
    if((f>0.0f && d>=0.0f && d<=f) || (f<0.0f && d<=0.0f && d>=f))	\
    {													\
        const float e=Ax*Cy - Ay*Cx;					\
        if(f>0.0f)										\
        {												\
        if(e>=0.0f && e<=f) return true;			    \
        }												\
        else											\
        {												\
        if(e<=0.0f && e>=f) return true;			    \
        }												\
    }

//-------------------------------------------------------------------------------------
#define EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2)		\
{														\
    float Bx,By,Cx,Cy,d,f;								\
    const float Ax = V1[i0] - V0[i0];					\
    const float Ay = V1[i1] - V0[i1];					\
    /* test edge U0,U1 against V0,V1 */					\
    EDGE_EDGE_TEST(V0, U0, U1);							\
    /* test edge U1,U2 against V0,V1 */					\
    EDGE_EDGE_TEST(V0, U1, U2);							\
    /* test edge U2,U1 against V0,V1 */					\
    EDGE_EDGE_TEST(V0, U2, U0);							\
}

//--------------------------------------------------------------------------------
#define NEWCOMPUTE_INTERVALS(VV0, VV1, VV2, D0, D1, D2, D0D1, D0D2, A, B, C, X0, X1)	\
{																						\
    if(D0D1>0.0f)																		\
    {																					\
    /* here we know that D0D2<=0.0 */												\
    /* that is D0, D1 are on the same side, D2 on the other or on the plane */		\
    A=VV2; B=(VV0 - VV2)*D2; C=(VV1 - VV2)*D2; X0=D2 - D0; X1=D2 - D1;				\
    }																					\
    else if(D0D2>0.0f)																	\
    {																					\
    /* here we know that d0d1<=0.0 */												\
    A=VV1; B=(VV0 - VV1)*D1; C=(VV2 - VV1)*D1; X0=D1 - D0; X1=D1 - D2;				\
    }																					\
    else if(D1*D2>0.0f || D0!=0.0f)														\
    {																					\
    /* here we know that d0d1<=0.0 or that D0!=0.0 */								\
    A=VV0; B=(VV1 - VV0)*D0; C=(VV2 - VV0)*D0; X0=D0 - D1; X1=D0 - D2;				\
    }																					\
    else if(D1!=0.0f)																	\
    {																					\
    A=VV1; B=(VV0 - VV1)*D1; C=(VV2 - VV1)*D1; X0=D1 - D0; X1=D1 - D2;				\
    }																					\
    else if(D2!=0.0f)																	\
    {																					\
    A=VV2; B=(VV0 - VV2)*D2; C=(VV1 - VV2)*D2; X0=D2 - D0; X1=D2 - D1;				\
    }																					\
    else																				\
    {																					\
    /* triangles are coplanar */													\
    return CoplanarTriTri(N1, V0, V1, V2, U0, U1, U2);								\
    }																					\
}

//-------------------------------------------------------------------------------------
//! TO BE DOCUMENTED
#define POINT_IN_TRI(V0, U0, U1, U2)					\
{														\
    /* is T1 completly inside T2? */					\
    /* check if V0 is inside tri(U0,U1,U2) */			\
    float a  = U1[i1] - U0[i1];							\
    float b  = -(U1[i0] - U0[i0]);						\
    float c  = -a*U0[i0] - b*U0[i1];					\
    float d0 = a*V0[i0] + b*V0[i1] + c;					\
    \
    a  = U2[i1] - U1[i1];								\
    b  = -(U2[i0] - U1[i0]);							\
    c  = -a*U1[i0] - b*U1[i1];							\
    const float d1 = a*V0[i0] + b*V0[i1] + c;			\
    \
    a  = U0[i1] - U2[i1];								\
    b  = -(U0[i0] - U2[i0]);							\
    c  = -a*U2[i0] - b*U2[i1];							\
    const float d2 = a*V0[i0] + b*V0[i1] + c;			\
    if(d0*d1>0.0f)										\
    {													\
    if(d0*d2>0.0f) return true;						\
    }													\
}

//-------------------------------------------------------------------------------------
int rand(int r1, int r2)
{
    return int(r1 + (float)(::rand()%(r2-r1)));
}

//-------------------------------------------------------------------------------------
float rand(TFlt32 r1, TFlt32 r2)
{
    const TFlt32 r = (TFlt32)::rand()/((TFlt32)RAND_MAX+1);
    return r*(r2-r1) + r1;
}

//-------------------------------------------------------------------------------------
float GetHalfSpace(const Vector2f& Point, const Vector2f& LineStart, const Vector2f& LineEnd)
{
	return ((Point.x - LineStart.x) * (LineEnd.y - LineStart.y)) - ((LineEnd.x - LineStart.x) * (Point.y - LineStart.y));
}

//-------------------------------------------------------------------------------------
float det2x2(float a1, float a2, float b1, float b2)
{
	return a1 * b2 - b1 * a2;
}

//-------------------------------------------------------------------------------------
float det3x3(float a1, float a2, float a3, 
			 float b1, float b2, float b3, 
			 float c1, float c2, float c3)
{
	return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}


//-------------------------------------------------------------------------------------
void VectorMultiply( const Vector& a, float b, Vector& c )
{
    c.x = a.x * b;
    c.y = a.y * b;
    c.z = a.z * b;
}

//-------------------------------------------------------------------------------------
void VectorAdd( const Vector& a, const Vector& b, Vector& c )
{
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
}

//-------------------------------------------------------------------------------------
void VectorScale ( const Vector& in, float scale, Vector& result )
{
    VectorMultiply( in, scale, result );
}

//-------------------------------------------------------------------------------------
bool EdgeIntersectPlane(const Vector& v0,const Vector & v1,
                        const Vector& p0,const Vector& p1,const Vector& p2)
{
    bool bResult = false;

    //compute plane equation
    Vector E1 = p1 - p0;
    Vector E2 = p2 - p0;
    const Vector N1 = E1.Cross(E2);
    
    float hs1 = N1.x * (v0.x - p0.x) + N1.y * (v0.y - p0.y) + N1.z * (v0.z - p0.z);
    float hs2 = N1.x * (v1.x - p0.x) + N1.y * (v1.y - p0.y) + N1.z * (v1.z - p0.z);

    if (( hs1 < 0 && hs2 > 0) || ( hs2 < 0 && hs1 > 0))
    {
        bResult = true;
    }

    return bResult;
}

//-------------------------------------------------------------------------------------
bool EdgeIntersectPlane(const Vector& v0,const Vector & v1,
                        const Vector& pN,const Vector& pPt)
{
    bool bResult = false;

    float hs1 = pN.x * (v0.x - pPt.x) + pN.y * (v0.y - pPt.y) + pN.z * (v0.z - pPt.z);
    float hs2 = pN.x * (v1.x - pPt.x) + pN.y * (v1.y - pPt.y) + pN.z * (v1.z - pPt.z);

    if (( hs1 < 0 && hs2 > 0) || ( hs2 < 0 && hs1 > 0))
    {
        bResult = true;
    }

    return bResult;
}

//-------------------------------------------------------------------------------------
bool CoplanarTriTri(const Vector& n, const Vector& v0, const Vector& v1, const Vector& v2, 
                    const Vector& u0, const Vector& u1, const Vector& u2)
{
    float A[3];
    short i0,i1;
    /* first project onto an axis-aligned plane, that maximizes the area */
    /* of the triangles, compute indices: i0,i1. */
    A[0] = fabsf(n[0]);
    A[1] = fabsf(n[1]);
    A[2] = fabsf(n[2]);
    if(A[0]>A[1])
    {
        if(A[0]>A[2])
        {
            i0=1;      /* A[0] is greatest */
            i1=2;
        }
        else
        {
            i0=0;      /* A[2] is greatest */
            i1=1;
        }
    }
    else   /* A[0]<=A[1] */
    {
        if(A[2]>A[1])
        {
            i0=0;      /* A[2] is greatest */
            i1=1;
        }
        else
        {
            i0=0;      /* A[1] is greatest */
            i1=2;
        }
    }

    /* test all edges of triangle 1 against the edges of triangle 2 */
    EDGE_AGAINST_TRI_EDGES(v0, v1, u0, u1, u2);
    EDGE_AGAINST_TRI_EDGES(v1, v2, u0, u1, u2);
    EDGE_AGAINST_TRI_EDGES(v2, v0, u0, u1, u2);

    /* finally, test if tri1 is totally contained in tri2 or vice versa */
    POINT_IN_TRI(v0, u0, u1, u2);
    POINT_IN_TRI(u0, v0, v1, v2);

    return false;
}

//-------------------------------------------------------------------------------------
Vector& mult_pos(Vector &vOut, const Matrix &MIn, const Vector &vIn)
{
	float oow;
	float divider = vIn.x * MIn.a30 + vIn.y * MIn.a31 + vIn.z * MIn.a32 + MIn.a33;

	if (divider < m_eps && divider > -m_eps)
		oow = 1.f ;
	else
		oow = 1.f / divider;

	vOut.x = (MIn[0] * vIn.x + MIn[4] * vIn.y + MIn[8] * vIn.z + MIn[12]) * oow;
	vOut.y = (MIn[1] * vIn.x + MIn[5] * vIn.y + MIn[9] * vIn.z + MIn[13]) * oow;
	vOut.z = (MIn[2] * vIn.x + MIn[6] * vIn.y + MIn[10]* vIn.z + MIn[14]) * oow; 

	return vOut;
}

//-------------------------------------------------------------------------------------
Vector& mult_pos(Vector& u, const Vector& v, const Matrix& M)
{
	float oow;
	float divider = v.x * M.a03 + v.y * M.a13 + v.z * M.a23 + M.a33;

	if (divider < m_eps && divider > -m_eps)
		oow = 1.f ;
	else
		oow = 1.f / divider;

	u.x = (M.a00 * v.x + M.a10 * v.y + M.a20 * v.z + M.a30) * oow;
	u.y = (M.a01 * v.x + M.a11 * v.y + M.a21 * v.z + M.a31) * oow;
	u.z = (M.a02 * v.x + M.a12 * v.y + M.a22 * v.z + M.a32) * oow;
	return u;
}

//-------------------------------------------------------------------------------------
Vector& transform_coord(Vector& vOut, const Vector& vIn, const Matrix& MIn)
{
	float oow;
	float divider = vIn.x * MIn.a30 + vIn.y * MIn.a31 + vIn.z * MIn.a32 + MIn.a33;

	if (divider < m_eps && divider > -m_eps){
		oow = 1.f;
	}else{
		oow = 1.f / divider;
	}

	if (&vOut != &vIn)
	{
		vOut.x = (MIn[0] * vIn.x + MIn[4] * vIn.y + MIn[8] * vIn.z + MIn[12]) * oow;
		vOut.y = (MIn[1] * vIn.x + MIn[5] * vIn.y + MIn[9] * vIn.z + MIn[13]) * oow;
		vOut.z = (MIn[2] * vIn.x + MIn[6] * vIn.y + MIn[10]* vIn.z + MIn[14]) * oow; 
	}
	else
	{
		Vector vTemp;
		vTemp.x = (MIn[0] * vIn.x + MIn[4] * vIn.y + MIn[8] * vIn.z + MIn[12]) * oow;
		vTemp.y = (MIn[1] * vIn.x + MIn[5] * vIn.y + MIn[9] * vIn.z + MIn[13]) * oow;
		vTemp.z = (MIn[2] * vIn.x + MIn[6] * vIn.y + MIn[10]* vIn.z + MIn[14]) * oow;

		vOut = vTemp;
	}
	
	return vOut;
}

//-------------------------------------------------------------------------------------
Vector4f& transform_coord(Vector4f& vOut, const Vector4f& vIn, const Matrix& MIn)
{
	float oow;
	float divider = vIn.x * MIn.a30 + vIn.y * MIn.a31 + vIn.z * MIn.a32 + vIn.w * MIn.a33;

	if (divider < m_eps && divider > -m_eps){
		oow = 1.f;
	}else{
		oow = 1.f / divider;
	}

	vOut.x = (MIn[0] * vIn.x + MIn[4] * vIn.y + MIn[8] * vIn.z + MIn[12] * vIn.w) * oow;
	vOut.y = (MIn[1] * vIn.x + MIn[5] * vIn.y + MIn[9] * vIn.z + MIn[13] * vIn.w) * oow;
	vOut.z = (MIn[2] * vIn.x + MIn[6] * vIn.y + MIn[10]* vIn.z + MIn[14] * vIn.w) * oow; 

	return vOut;
}

//-------------------------------------------------------------------------------------
Vector transform_coord(const Vector& v, const Matrix& M)
{
	Vector outVal;
	transform_coord(outVal, v, M);
	return outVal;
}
//-------------------------------------------------------------------------------------
Vector2f transform_coord(const Vector2f& v, const Matrix& M)
{
	Vector2f outVal;
	transform_coord(outVal, v, M);
	return outVal;
}
//-------------------------------------------------------------------------------------
Vector4f transform_coord(const Vector4f& v, const Matrix& M)
{
	Vector4f outVal;
	transform_coord(outVal, v, M);
	return outVal;
}

//-------------------------------------------------------------------------------------
Vector2f& transform_coord(Vector2f& u, const Vector2f& v, const Matrix& M)
{
	u.x = M.a00 * v.x + M.a10 * v.y;
	u.y = M.a01 * v.x + M.a11 * v.y;
	return u;
}

//-------------------------------------------------------------------------------------
Bounds3f& transform_bound(Bounds3f& OutBound, const Bounds3f& InBound, const Matrix& M)
{
	transform_coord(OutBound.bound_min, InBound.bound_min, M);
	transform_coord(OutBound.bound_max, InBound.bound_max, M);

	// rearrange
 	if (OutBound.bound_max.x < OutBound.bound_min.x){
 		std::swap(OutBound.bound_max.x, OutBound.bound_min.x);
 	}
 	if (OutBound.bound_max.y < OutBound.bound_min.y){
 		std::swap(OutBound.bound_max.y, OutBound.bound_min.y);
 	}
 	if (OutBound.bound_max.z < OutBound.bound_min.z){
 		std::swap(OutBound.bound_max.z, OutBound.bound_min.z);
 	}
	return OutBound;
}

//-------------------------------------------------------------------------------------
Bounds3f transform_bound(const Bounds3f InBound, const Matrix& M)
{
	Bounds3f OutValue;
	transform_bound(OutValue, InBound, M);
	return OutValue;
}

//-------------------------------------------------------------------------
Matrix & frustum( Matrix& M, const float l, const float r, const float b, 
                  const float t, const float n, const float f)
{
    M.a00 = (m_two*n) / (r-l);
    M.a10 = 0.0;
    M.a20 = 0.0;
    M.a30 = 0.0;

    M.a01 = 0.0;
    M.a11 = (m_two*n) / (t-b);
    M.a21 = 0.0;
    M.a31 = 0.0;

    M.a02 = (r+l) / (r-l);
    M.a12 = (t+b) / (t-b);
    M.a22 =  f / (f + n); //-(f+n) / (f-n);
    M.a32 = -m_one;

    M.a03 = 0.0;
    M.a13 = 0.0;
    M.a23 = -M.a22 * n; //-(m_two*f*n) / (f-n);
    M.a33 = 0.0;
    return M;
}

//-------------------------------------------------------------------------
Matrix& perspective(Matrix &vOut, float fov, float aspect, float n, float f)
{
	float h, w, Q;

	vOut.Identitly();

    const float fov_h = fov * m_to_rad;
    const float fov_v = (fov / aspect) * m_to_rad;

    w = (float)1.f / tanf(fov_h * m_zero_5);   // 1/tanf(x) == cot(x)
	h = (float)1.f / tanf(fov_v * m_zero_5);   // 1/tanf(x) == cot(x)
	Q = f /(f - n);
	    
	vOut[0]  = w;
    vOut[5]  = h;
    vOut[10] = Q;
    vOut[11] = 1.f;
    vOut[14] = -Q*n;
    vOut[15] = 0.f;

    return vOut;
}
//-------------------------------------------------------------------------
// pOut [in, out] Pointer to the resulting 
// l  [in] Minimum x-value of view volume.
// r  [in] Maximum x-value of view volume.
// b  [in] Minimum y-value of view volume.
// t  [in] Maximum y-value of view volume.
// zn [in] Minimum z-value of the view volume. 
// zf [in] Maximum z-value of the view volume. 
Matrix& ortho(Matrix &vOut, float l, float r, float b, float t, float zn, float zf)
{
	// 2/(r-l)      0            0           0
	// 0            2/(t-b)      0           0
	// 0            0            1/(zf-zn)   0
	// (l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l

	vOut.Identitly();

	vOut[0]  = 2.f/(r-l);
	vOut[5]  = 2.f/(t-b);
	vOut[10] = 1.f/(zf-zn);	
	vOut[12] = (l+r)/(l-r);	
	vOut[13] = (t+b)/(b-t);	
	vOut[14] = zn/(zn-zf);

	return vOut;
}

//-------------------------------------------------------------------------
Matrix& look_at(Matrix& vOut, const Vector& eye, const Vector& center, const Vector& up)
{
    Vector x,y,z;

    z = center - eye;
    z.normalize();

    y = up;

    cross(x,y,z);
    cross(y,z,x);

    x.normalize();
    y.normalize();

    vOut.a00 = x.x; vOut.a01 = x.y; vOut.a02 = x.z; vOut.a03 = -x.Dot(eye);
    vOut.a10 = y.x; vOut.a11 = y.y; vOut.a12 = y.z; vOut.a13 = -y.Dot(eye);
    vOut.a20 = z.x; vOut.a21 = z.y; vOut.a22 = z.z; vOut.a23 = -z.Dot(eye);
    vOut.a30 = 0.f; vOut.a31 = 0.f; vOut.a32 = 0.f; vOut.a33 = 1.f;

	return vOut;
}

//-------------------------------------------------------------------------
Vector UnprojectViewport(Vector &v_out, const Matrix &proj, const Matrix& view,
                         const Vector2f& pos, const Vector2f& dimension, float d/*= 1.f*/)
{
   Vector v;
   Matrix m;

   v.x =  ((( 2.f * pos.x ) / dimension.x ) - 1.f) / proj._11;
   v.y = -((( 2.f * pos.y ) / dimension.y ) - 1.f) / proj._22;
   v.z = d;

   m = view;
   
   invert(m, view);

   v_out.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
   v_out.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
   v_out.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
 
   return v_out;
}

//-------------------------------------------------------------------------
Vector ProjectViewport(Vector &v_out, const Vector& InPos, const Matrix& view, const Matrix &proj, const Vector2f& dimension)
{
	float HalfWidth = (float)dimension.x * 0.5f;
	float HalfHeight = (float)dimension.y * 0.5f;

	v_out = InPos;
	v_out = transform_coord(v_out, view);
	v_out = transform_coord(v_out, proj);

	v_out.x = (1.f + v_out.x) * HalfWidth;
	v_out.y = (1.f - v_out.y) * HalfHeight;
	v_out.z = (1.f + v_out.z) / 2;

	return v_out;
}

//-------------------------------------------------------------------------
bool IsPointOnPlane(const Vector& V0,
                    const Vector& V1,
                    const Vector& V2,
                    const Vector& p)
{
     bool bResult = false;

     //compute plane equation
     Vector E1 = V1 - V0;
     Vector E2 = V2 - V0;
     Vector N = E1.Cross(E2);

     N.normalize();

     float val = N.x*(p.x-V0.x) + N.y*(p.y-V0.y) + N.z*(p.z-V0.z);

     if (val == 0 )
     {
         bResult = true;
     }

     return bResult;
}

//-------------------------------------------------------------------------
bool IsPointOnPlane(const Vector& pNorm,
                    const Vector& pPt,
                    const Vector& p)
{
    bool bResult = false;

    float val = pNorm.x*(p.x-pPt.x) + pNorm.y*(p.y-pPt.y) + pNorm.z*(p.z-pPt.z);

    if (val == 0 )
    {
        bResult = true;
    }

    return bResult;
}

//-------------------------------------------------------------------------
int GetHalfSpace(const Vector& V0,
                 const Vector& V1,
                 const Vector& V2,
                 const Vector& p)
{
    //compute plane equation
    Vector E1 = V1 - V0;
    Vector E2 = V2 - V0;
    const Vector N = E1.Cross(E2);

    float val = N.x * (p.x - V0.x) + N.y * (p.y - V0.y)  + N.z * (p.z - V0.z);

    if ( val == 0 )
    {
        return 0;
    }

    if (val < 0)
    {
        return -1;
    }

    return 1;
}

//-------------------------------------------------------------------------
int GetHalfSpace(const Vector& PlaneN,
                 const Vector& PlanePt,
                 const Vector& p)
{
    float val = PlaneN.x * (p.x - PlanePt.x) + PlaneN.y * (p.y - PlanePt.y)  + PlaneN.z * (p.z - PlanePt.z);

    if ( val == 0 )
    {
        return 0;
    }

    if (val < 0)
    {
        return -1;
    }

    return 1;
}

//-------------------------------------------------------------------------
COMMON_BASE_EXPORT inline_ Vector RayPlaneIntersect(const Vector &planePos,
    const Vector &planeNorm,
    const Vector &linePos,
    const Vector &lineDir,
    float &t)
{
    // plane determination n*(p-p0)=0; 
    // n_x(x-x0) + n_y(y-y0) + n_z(z-z0) = 0
    // (n_x*x - n_x*x0) + (n_y*y - n_y*y0) + (n_z*z - n_z*z0) = 0;
    // (n_x*x + n_y*y + n_z*z) +(- n_x*x0  - n_y*y0  - n_z*z0) = 0;
    // d = (- n_x*x0  - n_y*y0  - n_z*z0)

    // line determination 
    // x = x0 + x*t;
    // y = y0 + y*t;
    // z = z0 + z*t;

    Vector vOut;
    Vector plane_normal = planeNorm;
    Vector plane_point = planePos;

    float plane_dot_line = lineDir.Dot(plane_normal);

    if (fabs(plane_dot_line) <= m_eps)
    {
        return vOut;
    }
        
    float d = 0;

    t = -(plane_normal.x * linePos.x +
        plane_normal.y * linePos.y +
        plane_normal.z * linePos.z -
        plane_normal.x * plane_point.x -
        plane_normal.y * plane_point.y -
        plane_normal.z * plane_point.z) / (plane_dot_line);

    vOut.x = linePos.x + lineDir.x * t;
    vOut.y = linePos.y + lineDir.y * t;
    vOut.z = linePos.z + lineDir.z * t;

    return vOut;
}

//-------------------------------------------------------------------------
Vector RayPlaneIntersect( const Vector& planePos, const Vector& planeNorm,
                          const Vector& linePos, const Vector& lineDir)
{
    float t = 0;
    return RayPlaneIntersect(planePos, planeNorm, linePos, lineDir, t);
}

//-------------------------------------------------------------------------
int IntersectRayWithTriangle(const Vector& start, 
                             const Vector& end,
                             const Vector& v0,
                             const Vector& v1,
                             const Vector& v2, 
                             float *t )
{
    // Vo(1-u-v) + V1u + V2v and intersect that with a line Po + Dt
    // E1x u + E2x v - Dx t = Pox - Vox

    Vector edge1, edge2, org, pvec,tvec,qvec;
    Vector dir = end - start;

    edge1 = v1 - v0;
    edge2 = v2 - v0;

    // Compute some intermediary terms
    cross(pvec,dir,edge2);

    float det = edge1.Dot(pvec);

    if((det > -0.001f) && (det < 0.001f))
        return 0;

    float inv_det = 1.0f / det;

    // calculate distance from vert0 to ray origin
    tvec = start - v0; 

    float u = tvec.Dot(pvec) * inv_det;

    if ((u < 0.0f) || (u > 1.0f))
        return 0;

    // Compute t and v the same way...
    // In barycentric coords, u + v < 1
    cross(qvec, tvec, edge1);

    float v = dir.Dot(qvec) * inv_det;

    if ((v < 0.0f) || (v + u > 1.0f))
        return 0;

    *t = edge2.Dot(qvec) * inv_det;

    return 1;
}

//----------------------------------------------------------------------
bool IntersectRayWithTriangle(const Vector2f& Point,
 							  const Vector2f& v0,
							  const Vector2f& v1,
							  const Vector2f& v2)
{
	float hs_v0_v1 = ((Point.x - v0.x) * (v1.y - v0.y)) - ((v1.x - v0.x) * (Point.y - v0.y));
	float hs_v1_v2 = ((Point.x - v1.x) * (v2.y - v1.y)) - ((v2.x - v1.x) * (Point.y - v1.y));
    float hs_v2_v0 = ((Point.x - v2.x) * (v0.y - v2.y)) - ((v0.x - v2.x) * (Point.y - v2.y));

    return ((hs_v0_v1 < 0.f) && (hs_v1_v2 < 0.f) && (hs_v2_v0 < 0.f) || 
			(hs_v0_v1 > 0.f) && (hs_v1_v2 > 0.f) && (hs_v2_v0 > 0.f));
}

//----------------------------------------------------------------------
bool TriangleTriangleIntersection(const Vector& V0,
                                  const Vector& V1,
                                  const Vector& V2, 
                                  const Vector& U0,
                                  const Vector& U1,
                                  const Vector& U2 )
{
   //compute plane equation
   Vector E1 = V1 - V0;
   Vector E2 = V2 - V0;
   const Vector N1 = E1.Cross(E2);
   const float  d1 = -(N1.Dot(V0));
   //Plane equation 1: N1.X+d1=0

   /**//* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
   float du0= N1.Dot(U0)+d1;
   float du1= N1.Dot(U1)+d1;
   float du2= N1.Dot(U2)+d1;

   /**//* coplanarity robustness check */
#if USE_EPSILON_TEST==true
   if(FABS(du0)<EPSILON) du0=0.0;
   if(FABS(du1)<EPSILON) du1=0.0;
   if(FABS(du2)<EPSILON) du2=0.0;
#endif
   float du0du1=du0*du1;
   float du0du2=du0*du2;

   if(du0du1>0.0f && du0du2>0.0f) /**//* same sign on all of them + not equal 0 ? */
       return false;                    /**//* no intersection occurs */

   //compute plane equation
   E1 = U1 - U0;
   E2 = U2 - U0;
   const Vector N2 = E1.Cross(E2);
   const float  d2 = -(N2.Dot(U0));
   //Plane equation 2: N2.X+d2=0

   /**//* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
   float dv0= N2.Dot(V0)+d2;
   float dv1= N2.Dot(V1)+d2;
   float dv2= N2.Dot(V2)+d2;

#if USE_EPSILON_TEST==true
   if(FABS(dv0)<EPSILON) dv0=0.0;
   if(FABS(dv1)<EPSILON) dv1=0.0;
   if(FABS(dv2)<EPSILON) dv2=0.0;
#endif

   float dv0dv1=dv0*dv1;
   float dv0dv2=dv0*dv2;

   if(dv0dv1>0.0f && dv0dv2>0.0f) /**//* same sign on all of them + not equal 0 ? */
       return false;                    /**//* no intersection occurs */


   // Compute direction of intersection line
   const Vector D = N1.Cross(N2);
 
   // Compute and index to the largest component of D
   float max=fabsf(D[0]);
   short index=0;
   float bb=fabsf(D[1]);
   float cc=fabsf(D[2]);
   if(bb>max) max=bb,index=1;
   if(cc>max) max=cc,index=2;

   // This is the simplified projection onto L
   const float vp0 = V0[index];
   const float vp1 = V1[index];
   const float vp2 = V2[index];

   const float up0 = U0[index];
   const float up1 = U1[index];
   const float up2 = U2[index];

   // Compute interval for triangle 1
   float a,b,c,x0,x1;
   NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

   // Compute interval for triangle 2
   float d,e,f,y0,y1;
   NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

   const float xx=x0*x1;
   const float yy=y0*y1;
   const float xxyy=xx*yy;

   float isect1[2], isect2[2];

   float tmp=a*xxyy;
   isect1[0]=tmp+b*x1*yy;
   isect1[1]=tmp+c*x0*yy;

   tmp=d*xxyy;
   isect2[0]=tmp+e*xx*y1;
   isect2[1]=tmp+f*xx*y0;

   SORT(isect1[0],isect1[1]);
   SORT(isect2[0],isect2[1]);

   if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;

   return true;
}
//---------------------------------------------------------------------------
bool TriangleTriangleSplit(const Vector& V0,
                           const Vector& V1, 
                           const Vector& V2,
                           const Vector& S0,
                           const Vector& S1,
                           const Vector& S2,
                           int& out_triangle_count,
                           Vector* triangle_strip)
{
    bool bResult = false;

    bool point_flag[3] = {false,false,false};

    Vector SE1 = S1 - S0;
    Vector SE2 = S2 - S0;
    const Vector SN = SE1.Cross(SE2);

    out_triangle_count = 0;
    
    if (TriangleTriangleIntersection(V0,V1,V2,S0,S1,S2) == true)
    {

        int VertesSpaceSign[3];

        VertesSpaceSign[0] = GetHalfSpace(S0,S1,S2,V0);
        VertesSpaceSign[1] = GetHalfSpace(S0,S1,S2,V1);
        VertesSpaceSign[2] = GetHalfSpace(S0,S1,S2,V2);


        if (VertesSpaceSign[0] == VertesSpaceSign[1] &&
            VertesSpaceSign[1] == VertesSpaceSign[2] &&
            VertesSpaceSign[2] == VertesSpaceSign[0])
        {
            return bResult;
        }


        point_flag[0] = IsPointOnPlane(S0,S1,S2,V0);
        point_flag[1] = IsPointOnPlane(S0,S1,S2,V1);
        point_flag[2] = IsPointOnPlane(S0,S1,S2,V2);

        int counter = 0;

        for (int index = 0; index < 3; index++)
        {
            if (point_flag[index])
                counter++;
        }
       
        if (counter > 2)
           return bResult; // edge-edge intersection
       
        if (counter == 1)
        {
            Vector divider;

            out_triangle_count = 2;

            if (point_flag[0] == true)
            {
                if (VertesSpaceSign[1] == VertesSpaceSign[2])
                {
                    return false; // no intersection
                }

                // calculate intersection V1-V2 
                divider = RayPlaneIntersect(S0,SN,V0,(V2-V1));

                triangle_strip[0] = V0;
                triangle_strip[1] = V1;
                triangle_strip[2] = divider;

                triangle_strip[3] = V0;
                triangle_strip[4] = divider;
                triangle_strip[5] = V2;
            }
            else if (point_flag[1] == true)
            {
                if (VertesSpaceSign[0] == VertesSpaceSign[2])
                {
                    return false; // no intersection
                }

                // calculate intersection V0-V2 
                divider = RayPlaneIntersect(S0,SN,V2,(V0-V2));

                triangle_strip[0] = V1;
                triangle_strip[1] = V2;
                triangle_strip[2] = divider;

                triangle_strip[3] = V1;
                triangle_strip[4] = divider;
                triangle_strip[5] = V0;
            }
            else if (point_flag[2] == true)
            {
                if (VertesSpaceSign[0] == VertesSpaceSign[1])
                {
                    return false; // no intersection
                }

                // calculate intersection V1-V0 
                divider = RayPlaneIntersect(S0,SN,V2,(V1-V0));

                triangle_strip[0] = V2;
                triangle_strip[1] = V1;
                triangle_strip[2] = divider;

                triangle_strip[3] = V2;
                triangle_strip[4] = divider;
                triangle_strip[5] = V0;
            }
        }
        else 
        {
            bool EdjeIntersected[3];

            EdjeIntersected[0] = false;
            EdjeIntersected[1] = false;
            EdjeIntersected[2] = false;

            //test edge V0 - V1
            EdjeIntersected[0] = EdgeIntersectPlane(V0,V1,S0,S1,S2);

            // test edge V1 - V2
            EdjeIntersected[1] = EdgeIntersectPlane(V1,V2,S0,S1,S2);

            // test edge V2- V0
            EdjeIntersected[2] = EdgeIntersectPlane(V2,V0,S0,S1,S2);

            Vector d1;
            Vector d2;

            out_triangle_count = 3;

            // 
            if (EdjeIntersected[0] == true && EdjeIntersected[1] == true)
            {
                // calculate intersection V1-V0 
                d1 = RayPlaneIntersect(S0,SN,V0,(V1-V0));
           
                // calculate intersection V2-V1 
                d2 = RayPlaneIntersect(S0,SN,V1,(V2-V1));

                triangle_strip[0] = V0;
                triangle_strip[1] = V2;
                triangle_strip[2] = d1;

                triangle_strip[3] = d1;
                triangle_strip[4] = V1;
                triangle_strip[5] = d2;

                triangle_strip[6] = d2;
                triangle_strip[7] = d1;
                triangle_strip[8] = V2;

            }
            else if (EdjeIntersected[0] == true && EdjeIntersected[2] == true)
            {
                // calculate intersection V1-V0 
                d1 = RayPlaneIntersect(S0,SN,V0,(V1-V0));

                // calculate intersection V0-V2 
                d2 = RayPlaneIntersect(S0,SN,V2,(V0-V2));

                triangle_strip[0] = V0;
                triangle_strip[1] = d2;
                triangle_strip[2] = d1;

                triangle_strip[3] = d1;
                triangle_strip[4] = V1;
                triangle_strip[5] = d2;

                triangle_strip[6] = d2;
                triangle_strip[7] = V1;
                triangle_strip[8] = V2;

            }
            else if (EdjeIntersected[2] == true && EdjeIntersected[1] == true)
            {
                // calculate intersection V2-V1 
                d1 = RayPlaneIntersect(S0,SN,V1,(V2-V1));

                // calculate intersection V0-V2 
                d2 = RayPlaneIntersect(S0,SN,V2,(V0-V2));

                triangle_strip[0] = V0;
                triangle_strip[1] = V1;
                triangle_strip[2] = d1;

                triangle_strip[3] = d1;
                triangle_strip[4] = V0;
                triangle_strip[5] = d2;

                triangle_strip[6] = d2;
                triangle_strip[7] = d1;
                triangle_strip[8] = V2;
            }
        }

        bResult = true;
    }

    return bResult;
}

//----------------------------------------------------------------------------------
bool PlaneTriangleSplit(const Vector& V0,
                        const Vector& V1,
                        const Vector& V2, 
                        const Vector& pN,
                        const Vector& pPt,
                        int& out_triangle_count,
                        Vector * triangle_strip)
{
    bool bResult = false;

    bool point_flag[3] = {false,false,false};

    out_triangle_count = 0;

    int VertesSpaceSign[3];

    VertesSpaceSign[0] = GetHalfSpace(pN,pPt,V0);
    VertesSpaceSign[1] = GetHalfSpace(pN,pPt,V1);
    VertesSpaceSign[2] = GetHalfSpace(pN,pPt,V2);

    if (VertesSpaceSign[0] == VertesSpaceSign[1] &&
        VertesSpaceSign[1] == VertesSpaceSign[2] &&
        VertesSpaceSign[2] == VertesSpaceSign[0])
    {
        return bResult;
    }

    point_flag[0] = IsPointOnPlane(pN,pPt,V0);
    point_flag[1] = IsPointOnPlane(pN,pPt,V1);
    point_flag[2] = IsPointOnPlane(pN,pPt,V2);

    int counter = 0;

    for (int index = 0; index < 3; index++)
    {
        if (point_flag[index])
            counter++;
    }

    if (counter >= 2)
    {
        return bResult; // edge-edge intersection
    }

    if (counter == 1)
    {
        Vector divider;

        out_triangle_count = 2;

        if (point_flag[0] == true)
        {
            if (VertesSpaceSign[1] == VertesSpaceSign[2])
            {
                return false; // no intersection
            }

            // calculate intersection V1-V2 
            divider = RayPlaneIntersect(pPt,pN,V0,(V2-V1));

            triangle_strip[0] = V0;
            triangle_strip[1] = V1;
            triangle_strip[2] = divider;

            triangle_strip[3] = V0;
            triangle_strip[4] = divider;
            triangle_strip[5] = V2;
        }
        else if (point_flag[1] == true)
        {
            if (VertesSpaceSign[0] == VertesSpaceSign[2])
            {
                return false; // no intersection
            }

            // calculate intersection V0-V2 
            divider = RayPlaneIntersect(pPt,pN,V2,(V0-V2));

            triangle_strip[0] = V1;
            triangle_strip[1] = V2;
            triangle_strip[2] = divider;

            triangle_strip[3] = V1;
            triangle_strip[4] = divider;
            triangle_strip[5] = V0;
        }
        else if (point_flag[2] == true)
        {
            if (VertesSpaceSign[0] == VertesSpaceSign[1])
            {
                return false; // no intersection
            }

            // calculate intersection V1-V0 
            divider = RayPlaneIntersect(pPt,pN,V2,(V1-V0));

            triangle_strip[0] = V2;
            triangle_strip[1] = V1;
            triangle_strip[2] = divider;

            triangle_strip[3] = V2;
            triangle_strip[4] = divider;
            triangle_strip[5] = V0;
        }
    }
    else if (counter == 0)
    {
        bool EdjeIntersected[3];

        EdjeIntersected[0] = false;
        EdjeIntersected[1] = false;
        EdjeIntersected[2] = false;

        //test edge V0 - V1
        EdjeIntersected[0] = EdgeIntersectPlane(V0,V1,pN,pPt);

        // test edge V1 - V2
        EdjeIntersected[1] = EdgeIntersectPlane(V1,V2,pN,pPt);

        // test edge V2- V0
        EdjeIntersected[2] = EdgeIntersectPlane(V2,V0,pN,pPt);

        Vector d1;
        Vector d2;

        out_triangle_count = 3;

        // 
        if (EdjeIntersected[0] == true && EdjeIntersected[1] == true)
        {
            // calculate intersection V1-V0 
            d1 = RayPlaneIntersect(pPt,pN,V0,(V1-V0));

            // calculate intersection V2-V1 
            d2 = RayPlaneIntersect(pPt,pN,V1,(V2-V1));

            triangle_strip[0] = V0;
            triangle_strip[1] = V2;
            triangle_strip[2] = d1;

            triangle_strip[3] = d1;
            triangle_strip[4] = V1;
            triangle_strip[5] = d2;

            triangle_strip[6] = d2;
            triangle_strip[7] = d1;
            triangle_strip[8] = V2;
        }
        else if (EdjeIntersected[0] == true && EdjeIntersected[2] == true)
        {
            // calculate intersection V1-V0 
            d1 = RayPlaneIntersect(pPt,pN,V0,(V1-V0));

            // calculate intersection V0-V2 
            d2 = RayPlaneIntersect(pPt,pN,V2,(V0-V2));

            triangle_strip[0] = V0;
            triangle_strip[1] = d2;
            triangle_strip[2] = d1;

            triangle_strip[3] = d1;
            triangle_strip[4] = V1;
            triangle_strip[5] = d2;

            triangle_strip[6] = d2;
            triangle_strip[7] = V1;
            triangle_strip[8] = V2;

        }
        else if (EdjeIntersected[2] == true && EdjeIntersected[1] == true)
        {
            // calculate intersection V2-V1 
            d1 = RayPlaneIntersect(pPt,pN,V1,(V2-V1));

            // calculate intersection V0-V2 
            d2 = RayPlaneIntersect(pPt,pN,V2,(V0-V2));

            triangle_strip[0] = V0;
            triangle_strip[1] = V1;
            triangle_strip[2] = d1;

            triangle_strip[3] = d1;
            triangle_strip[4] = V0;
            triangle_strip[5] = d2;

            triangle_strip[6] = d2;
            triangle_strip[7] = d1;
            triangle_strip[8] = V2;
        }

        bResult = true;
    }

    return true;
}

//------------------------------------------------------------------------
void Catmull_Rom_Spline(const Vector &p1,
                        const Vector &p2,
                        const Vector &p3,
                        const Vector &p4,
                        float t, 
                        Vector& output )
{
    float tSqr = t*t*0.5f;
    float tSqrSqr = t*tSqr;
    t *= 0.5f;

    Vector a, b, c, d;

    output.x = output.y = output.z = 0;

    // matrix row 1
    VectorScale( p1, -tSqrSqr, a );		// 0.5 t^3 * [ (-1*p1) + ( 3*p2) + (-3*p3) + p4 ]
    VectorScale( p2, tSqrSqr*3, b );
    VectorScale( p3, tSqrSqr*-3, c );
    VectorScale( p4, tSqrSqr, d );

    VectorAdd( a, output, output );
    VectorAdd( b, output, output );
    VectorAdd( c, output, output );
    VectorAdd( d, output, output );

    // matrix row 2
    VectorScale( p1, tSqr*2,  a );		// 0.5 t^2 * [ ( 2*p1) + (-5*p2) + ( 4*p3) - p4 ]
    VectorScale( p2, tSqr*-5, b );
    VectorScale( p3, tSqr*4,  c );
    VectorScale( p4, -tSqr,    d );

    VectorAdd( a, output, output );
    VectorAdd( b, output, output );
    VectorAdd( c, output, output );
    VectorAdd( d, output, output );

    // matrix row 3
    VectorScale( p1, -t, a );			// 0.5 t * [ (-1*p1) + p3 ]
    VectorScale( p3, t,  b );

    VectorAdd( a, output, output );
    VectorAdd( b, output, output );

    // matrix row 4
    VectorAdd( p2, output, output );	// p2
}

//------------------------------------------------------------------------
void Cubic_Bezier_Spline(const Vector &A,
 						 const Vector &B,
						 const Vector &C,
						 const Vector &D,
						 float t, 
						 Vector &output)
{
	//(a+b)³ = a^3 + 3·a^2·b + 3·a·b^2 + b^3

	//X(a) = Ax·a^3 + Bx·3·a^2·b + Cx·3·a·b^2 + Dx·b^3
	//Y(a) = Ay·a^3 + By·3·a^2·b + Cy·3·a·b^2 + Dy·b^3
	//Z(a) = Az·a^3 + Bz·3·a^2·b + Cz·3·a·b^2 + Dz·b^3

	float b = 1.f - t;

	float a3 = t * t * t;
	float b3 = b * b * b;

	float _3_a2_b = 3 * (t * t) * b; 
	float _3_a_b2 = 3 * t * (b * b); 

	output.x = A.x * a3 + B.x * _3_a2_b + C.x * _3_a_b2 + D.x * b3;
	output.y = A.y * a3 + B.y * _3_a2_b + C.y * _3_a_b2 + D.y * b3;
	output.z = A.z * a3 + B.z * _3_a2_b + C.z * _3_a_b2 + D.z * b3;
}

//------------------------------------------------------------------------
Matrix & invert(Matrix& B, const Matrix& A)
{
	float det, oodet;

	B.a00 =  det3x3(A.a11, A.a21, A.a31, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
	B.a10 = -det3x3(A.a10, A.a20, A.a30, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
	B.a20 =  det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a13, A.a23, A.a33);
	B.a30 = -det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a12, A.a22, A.a32);

	B.a01 = -det3x3(A.a01, A.a21, A.a31, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
	B.a11 =  det3x3(A.a00, A.a20, A.a30, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
	B.a21 = -det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a03, A.a23, A.a33);
	B.a31 =  det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a02, A.a22, A.a32);

	B.a02 =  det3x3(A.a01, A.a11, A.a31, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
	B.a12 = -det3x3(A.a00, A.a10, A.a30, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
	B.a22 =  det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a03, A.a13, A.a33);
	B.a32 = -det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a02, A.a12, A.a32);

	B.a03 = -det3x3(A.a01, A.a11, A.a21, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
	B.a13 =  det3x3(A.a00, A.a10, A.a20, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
	B.a23 = -det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a03, A.a13, A.a23);
	B.a33 =  det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a02, A.a12, A.a22);

	det = (A.a00 * B.a00) + (A.a01 * B.a10) + (A.a02 * B.a20) + (A.a03 * B.a30);

	// The following divions goes unchecked for division
	// by zero. We should consider throwing an exception
	// if det < eps.
	oodet = 1.f / det;

	B.a00 *= oodet;
	B.a10 *= oodet;
	B.a20 *= oodet;
	B.a30 *= oodet;

	B.a01 *= oodet;
	B.a11 *= oodet;
	B.a21 *= oodet;
	B.a31 *= oodet;

	B.a02 *= oodet;
	B.a12 *= oodet;
	B.a22 *= oodet;
	B.a32 *= oodet;

	B.a03 *= oodet;
	B.a13 *= oodet;
	B.a23 *= oodet;
	B.a33 *= oodet;

	return B;
}
//------------------------------------------------------------------------
Matrix & transpose(Matrix& A)
{
	float tmp;
	tmp = A.a01;
	A.a01 = A.a10;
	A.a10 = tmp;

	tmp = A.a02;
	A.a02 = A.a20;
	A.a20 = tmp;

	tmp = A.a03;
	A.a03 = A.a30;
	A.a30 = tmp;

	tmp = A.a12;
	A.a12 = A.a21;
	A.a21 = tmp;

	tmp = A.a13;
	A.a13 = A.a31;
	A.a31 = tmp;

	tmp = A.a23;
	A.a23 = A.a32;
	A.a32 = tmp;
	return A;
}

//----------------------------------------------------------------------------------------------
Matrix& scale(Matrix &pValue, float sx, float sy, float sz)
{
	Matrix mTemp;
	Matrix S(sx, 0.f, 0.f, 0.f,
			 0.f, sy, 0.f, 0.f,
			 0.f, 0.f, sz, 0.f,
			 0.f, 0.f, 0.f, 1.f);

	mTemp.a00 = S.a00 * pValue.a00 + S.a10 * pValue.a01 + S.a20 * pValue.a02 + S.a30 * pValue.a03;
	mTemp.a10 = S.a00 * pValue.a10 + S.a10 * pValue.a11 + S.a20 * pValue.a12 + S.a30 * pValue.a13;
	mTemp.a20 = S.a00 * pValue.a20 + S.a10 * pValue.a21 + S.a20 * pValue.a22 + S.a30 * pValue.a23;
	mTemp.a30 = S.a00 * pValue.a30 + S.a10 * pValue.a31 + S.a20 * pValue.a32 + S.a30 * pValue.a33;

	mTemp.a01 = S.a01 * pValue.a00 + S.a11 * pValue.a01 + S.a21 * pValue.a02 + S.a31 * pValue.a03;
	mTemp.a11 = S.a01 * pValue.a10 + S.a11 * pValue.a11 + S.a21 * pValue.a12 + S.a31 * pValue.a13;
	mTemp.a21 = S.a01 * pValue.a20 + S.a11 * pValue.a21 + S.a21 * pValue.a22 + S.a31 * pValue.a23;
	mTemp.a31 = S.a01 * pValue.a30 + S.a11 * pValue.a31 + S.a21 * pValue.a32 + S.a31 * pValue.a33;

	mTemp.a02 = S.a02 * pValue.a00 + S.a12 * pValue.a01 + S.a22 * pValue.a02 + S.a32 * pValue.a03;
	mTemp.a12 = S.a02 * pValue.a10 + S.a12 * pValue.a11 + S.a22 * pValue.a12 + S.a32 * pValue.a13;
	mTemp.a22 = S.a02 * pValue.a20 + S.a12 * pValue.a21 + S.a22 * pValue.a22 + S.a32 * pValue.a23;
	mTemp.a32 = S.a02 * pValue.a30 + S.a12 * pValue.a31 + S.a22 * pValue.a32 + S.a32 * pValue.a33;

	pValue.row0 = mTemp.row0;
	pValue.row1 = mTemp.row1;
	pValue.row2 = mTemp.row2;

	return pValue;
}

//----------------------------------------------------------------------------------------------
void RotateMatrixAxisSelf(Matrix &out,const Vector &u, float rad)
{
	float c = ::cos(rad);
	float s = ::sin(rad);
	float ic = 1.0f - c;

	float icux = ic*u.x;
	float icuxy = icux*u.y;
	float icuxz = icux*u.z;
	float icuyz = ic*u.y*u.z;

	float suy = s * u.y;
	float suz = s * u.z;
	float sux = s * u.x;

	Matrix m;

	m.a00 = c+ic*u.x*u.x; m.a01 = icuxy - suz;  m.a02 = icuxz + suy; 
	m.a10 = icuxy + suz;  m.a11 = c+ic*u.y*u.y; m.a12 = icuyz - sux; 
	m.a20 = icuxz - suy;  m.a21 = icuyz + sux;  m.a22 = c+ic*u.z*u.z; 

	out = out * m;
}
//------------------------------------------------------------------------
void inline SinCos( float radians, float *sine, float *cosine )
{
	_asm
	{
		fld		DWORD PTR [radians]
		fsincos

		mov edx, DWORD PTR [cosine]
		mov eax, DWORD PTR [sine]

		fstp DWORD PTR [edx]
		fstp DWORD PTR [eax]
	}
}

//------------------------------------------------------------------------
void AngleMatrix( const float Yaw, const float Pitch, const float Roll, Matrix& matrix )
{
	float		sr, sp, sy, cr, cp, cy;

	SinCos( Yaw, &sy, &cy );
	SinCos( Pitch, &sp, &cp );
	SinCos( Roll, &sr, &cr );

	// matrix = (YAW * PITCH) * ROLL
	matrix.a00 = cp*cy;
	matrix.a10 = cp*sy;
	matrix.a20 = -sp;
	matrix.a01 = sr*sp*cy+cr*-sy;
	matrix.a11 = sr*sp*sy+cr*cy;
	matrix.a21 = sr*cp;
	matrix.a02 = (cr*sp*cy+-sr*-sy);
	matrix.a12 = (cr*sp*sy+-sr*cy);
	matrix.a22 = cr*cp;
	matrix.a03 = 0.f;
	matrix.a13 = 0.f;
	matrix.a23 = 0.f;
}

//------------------------------------------------------------------------
inline_ bool IsPointInRect(float x, float y, const Vector2f& Start, const Vector2f& End)
{
   return (Start.x <= x && x <= End.x) && (Start.y <= y && y <= End.y);
}

//------------------------------------------------------------------------
inline Vector2f GetSegmentNormal(const Vector2f& Start, const Vector2f& End)
{
	// Ax + By + C = 0:
	// A = y2 − y1;B = x1 − x2;C = − (Ax1 + By1)
	Vector2f OutVector(End.y - Start.y,  Start.x - End.x);
	OutVector.Normalize();
	return OutVector;
}

//------------------------------------------------------------------------
bool SegmentIntersection2D(Vector2f & out_pos,
						   const Vector2f &s1_start,
						   const Vector2f &s1_end,
						   const Vector2f &s2_start,
						   const Vector2f &s2_end )
{
	Vector2f p1_v = s1_end - s1_start;
	Vector2f p2_v = s2_end - s2_start;

	Vector2f p1_p0 = s1_start;
	Vector2f p2_p0 = s2_start;

	float det_p1p2 = ( p1_v.x * p2_v.y - p1_v.y * p2_v.x );

	if ( fabs(det_p1p2) <= 0.001f)
	{
		return false;
	}

	float t1 = ( p2_v.x * (p1_p0.y - p2_p0.y) - 
		p2_v.y * (p1_p0.x - p2_p0.x)) / det_p1p2;

	float t2 = ( p1_v.x * (p1_p0.y - p2_p0.y) - 
		p1_v.y * (p1_p0.x - p2_p0.x)) / det_p1p2;

	if( (t1>=0.f) && (t1<=1.f) && (t2>=0.f) && (t2<=1.f))
	{
		out_pos.x =  p1_p0.x + p1_v.x * t1;
		out_pos.y =  p1_p0.y + p1_v.y * t1;  

		return true;
	}

	return false;
}
//------------------------------------------------------------------------
bool SegmentSphereIntersection2D(const Vector2f &s_start,
								 const Vector2f &s_end,
								 const Vector2f &Center,
								 float Radius,
								 float *mu1,
								 float *mu2)
{
	float a, b, c, square; 

	Vector2f localSpherePos = s_start - Center;
	Vector2f lineDir		= s_end - s_start;

	a = lineDir.Dot(lineDir);
	b = 2.0f * localSpherePos.Dot(lineDir);
	c = localSpherePos.Dot(localSpherePos) - (Radius * Radius);

	square = b * b - (4 * a * c);

	if (square <= 0.000001f){
		return false;
	}

	float sqr = (float)sqrt(square);

	*mu1 = (-b + sqr) / (2 * a);
	*mu2 = (-b - sqr) / (2 * a);

	if (*mu1 < 0.f && *mu2 < 0.f || *mu1 > 1.f && *mu2 > 1.f){
		return false;
	}

	if (*mu1 < 0.f && *mu2 > 1.f || *mu1 > 1.f && *mu2 < 0.f){
		return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool SegmentSphereIntersection(const Vector &s_start,
							   const Vector &s_end,
							   const Vector &Center,
							   float Radius,
							   float *mu1,
							   float *mu2)
{
	float a, b, c, square; 

	Vector localSpherePos = s_start - Center;
	Vector lineDir = s_end - s_start;

	a = lineDir.Dot(lineDir);
	b = 2.0f * localSpherePos.Dot(lineDir);
	c = localSpherePos.Dot(localSpherePos) - (Radius * Radius);

	square = b * b - (4 * a * c);

	if (square <= 0.000001f){
		return false;
	}

	float sqr = (float)sqrt(square);

	*mu1 = (-b + sqr) / (2 * a);
	*mu2 = (-b - sqr) / (2 * a);

	if (*mu1 < 0.f && *mu2 < 0.f || *mu1 > 1.f && *mu2 > 1.f){
		return false;
	}

	if (*mu1 < 0.f && *mu2 > 1.f || *mu1 > 1.f && *mu2 < 0.f){
		return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool TestBBoxIntercection(const Bounds2f& A, const Bounds2f& B)
{
	if (A.bound_max.x < B.bound_min.x || A.bound_min.x > B.bound_max.x) { return false; }
	if (A.bound_max.y < B.bound_min.y || A.bound_min.y > B.bound_max.y) { return false; }
	return true;
}

//------------------------------------------------------------------------
bool TestBBoxIntercection(const Bounds3f& A, const Bounds3f& B)
{
	if (A.bound_max.x < B.bound_min.x || A.bound_min.x > B.bound_max.x) { return false; }
	if (A.bound_max.y < B.bound_min.y || A.bound_min.y > B.bound_max.y) { return false; }
	if (A.bound_max.z < B.bound_min.z || A.bound_min.z > B.bound_max.z) { return false; }
	return true;
}

//------------------------------------------------------------------------
bool FrustrumIntersectBBox(const FrustrumCullinfo& cullinfo, const Bounds3f& B)
{
	Vector BoxVertices[8];

	BoxVertices[0] = B.bound_min; // xyz
	BoxVertices[1] = Vector( B.bound_max.x, B.bound_min.y, B.bound_min.z); // Xyz
	BoxVertices[2] = Vector( B.bound_min.x, B.bound_max.y, B.bound_min.z); // xYz
	BoxVertices[3] = Vector( B.bound_max.x, B.bound_max.y, B.bound_min.z); // XYz
	BoxVertices[4] = Vector( B.bound_min.x, B.bound_min.y, B.bound_max.z); // xyZ
	BoxVertices[5] = Vector( B.bound_max.x, B.bound_min.y, B.bound_max.z); // XyZ
	BoxVertices[6] = Vector( B.bound_min.x, B.bound_max.y, B.bound_max.z); // xYZ
	BoxVertices[7] = B.bound_max; // XYZ

	for (int Index = 0; Index < FRUSTRUM_MAX; ++Index)
	{
		const Vector & Normal = cullinfo.PlaneNormals[Index];
		const Vector & Point  = cullinfo.PlanePoints[Index];

		if (GetHalfSpace(Normal, Point, BoxVertices[0]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[1]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[2]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[3]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[4]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[5]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[6]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[7]) < 0)
			continue;

		return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool FrustrumIntersectBBox(const UniCullinfo& cullinfo, const Bounds3f& B)
{
	Vector BoxVertices[8];

	BoxVertices[0] = B.bound_min; // xyz
	BoxVertices[1] = Vector( B.bound_max.x, B.bound_min.y, B.bound_min.z); // Xyz
	BoxVertices[2] = Vector( B.bound_min.x, B.bound_max.y, B.bound_min.z); // xYz
	BoxVertices[3] = Vector( B.bound_max.x, B.bound_max.y, B.bound_min.z); // XYz
	BoxVertices[4] = Vector( B.bound_min.x, B.bound_min.y, B.bound_max.z); // xyZ
	BoxVertices[5] = Vector( B.bound_max.x, B.bound_min.y, B.bound_max.z); // XyZ
	BoxVertices[6] = Vector( B.bound_min.x, B.bound_max.y, B.bound_max.z); // xYZ
	BoxVertices[7] = B.bound_max; // XYZ

	for (size_t Index = 0; Index < cullinfo.Planes.size(); ++Index)
	{
		const Vector & Normal = cullinfo.Planes[Index].Normal;
		const Vector & Point  = cullinfo.Planes[Index].Point;

		if (GetHalfSpace(Normal, Point, BoxVertices[0]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[1]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[2]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[3]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[4]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[5]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[6]) < 0)
			continue;
		if (GetHalfSpace(Normal, Point, BoxVertices[7]) < 0)
			continue;

		return false;
	}
	return true;
}

//------------------------------------------------------------------------
float Lerp(float t, float a, float b)
{
	return a * (1.f - t) + b * t;
}

//------------------------------------------------------------------------
Vector& Lerp(Vector &OutVec, float t, const Vector &a, const Vector &b)
{
	OutVec.x = Lerp(t, a.x, b.x);
	OutVec.y = Lerp(t, a.y, b.y);
	OutVec.z = Lerp(t, a.z, b.z);

	return OutVec;
}

//------------------------------------------------------------------------
Vector4f& Lerp(Vector4f &OutVec, float t, const Vector4f &a, const Vector4f &b)
{
	OutVec.x = Lerp(t, a.x, b.x);
	OutVec.y = Lerp(t, a.y, b.y);
	OutVec.z = Lerp(t, a.z, b.z);
	OutVec.w = Lerp(t, a.w, b.w);

	return OutVec;
}

//------------------------------------------------------------------------
Pose& Lerp(Pose& OutPose, float t, const Pose & a, const Pose& b)
{
	 Lerp(OutPose.Position, t, a.Position, b.Position);
	 Slerp(OutPose.Rotation, t, a.Rotation, b.Rotation);

	 return OutPose;
}

//------------------------------------------------------------------------
bool BuildEdgeList(std::vector<UniEdge> &OutEdges, const std::vector<UniTriangle> &TriInfo)
{
	for (size_t Index = 0; Index < TriInfo.size(); ++Index)
	{
		std::vector<unsigned int> tempIndexList;

		tempIndexList.push_back(TriInfo[Index].Index0);
		tempIndexList.push_back(TriInfo[Index].Index1);
		tempIndexList.push_back(TriInfo[Index].Index2);

		std::sort(tempIndexList.begin(), tempIndexList.end());

		UniEdge edje0(tempIndexList[0], tempIndexList[1]);
		UniEdge edje1(tempIndexList[1], tempIndexList[2]);
		UniEdge edje2(tempIndexList[0], tempIndexList[2]);

		edje0.triangle0.Index0 = edje1.triangle0.Index0 = edje2.triangle0.Index0 = tempIndexList[0];
		edje0.triangle0.Index1 = edje1.triangle0.Index1 = edje2.triangle0.Index1 = tempIndexList[1]; 
		edje0.triangle0.Index2 = edje1.triangle0.Index2 = edje2.triangle0.Index2 = tempIndexList[2];
		edje0.triangle0.IdxNormal = edje1.triangle0.IdxNormal = edje2.triangle0.IdxNormal = TriInfo[Index].IdxNormal;

		OutEdges.push_back(edje0);
		OutEdges.push_back(edje1);
		OutEdges.push_back(edje2);
	}

	std::vector<UniEdge>::iterator Iter = OutEdges.begin();

	while (Iter != OutEdges.end())
	{
		std::vector<UniEdge>::iterator IterNext = Iter + 1;

		if (IterNext != OutEdges.end())
		{
			std::vector<UniEdge>::iterator IterFind = std::find(IterNext, OutEdges.end(), *Iter);

			if (IterFind != OutEdges.end())
			{
				Iter->triangle1.Index0 = IterFind->triangle0.Index0;
				Iter->triangle1.Index1 = IterFind->triangle0.Index1; 
				Iter->triangle1.Index2 = IterFind->triangle0.Index2;
				Iter->triangle1.IdxNormal = IterFind->triangle0.IdxNormal;

				IterFind->bValid = false;
			}
		}

		++Iter;
	}

	OutEdges.erase(std::remove_if(OutEdges.begin(), OutEdges.end(), std::not1(std::mem_fun_ref(&UniEdge::IsValid))), OutEdges.end());

	return true;
}

//----------------------------------------------------------------------------------------------
float GetDistToLine(const Vector &point, const Vector &start, const Vector &end)
{
	Vector ab = end - start;
	Vector c = point - start;

	// project c onto ab, computing parametrized position d(t) = a + t*(b - a)
	float t = c.Dot(ab) / ab.Dot(ab);

	return ((start + (ab * t)) - point).LengthSqr();
}

//----------------------------------------------------------------------------------------------
bool fequal(float a, float with)
{
    return (with - m_eps) <= a && a <= (with + m_eps);
}

}
}
#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CPhysDebugDrawerGL::CPhysDebugDrawerGL(CCoreSDK *pSDK)
	: m_debugMode(0)
	, m_pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
CPhysDebugDrawerGL::~CPhysDebugDrawerGL()
{

}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor)
{
	m_pCoreSDK->GetRenderSDK()->DrawLine(Vector(from[0], from[1], from[2]), Vector(to[0], to[1], to[2]), 0xffffffff);	
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::drawLine(const btVector3 &from,const btVector3 &to, const btVector3 &color)
{
	m_pCoreSDK->GetRenderSDK()->DrawLine(Vector(from[0], from[1], from[2]), Vector(to[0], to[1], to[2]), 0xffffffff);
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color)
{
	m_pCoreSDK->GetRenderSDK()->DrawSphere(Vector(p[0], p[1], p[2]), radius, 0xffcccc00); // test
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::drawTriangle(const btVector3 &a, const btVector3 &b, const btVector3 &c, const btVector3 &color, btScalar alpha)
{
	m_pCoreSDK->GetRenderSDK()->DrawTriangle(Vector(a[0], a[1], a[2]), Vector(b[0], b[1], b[2]), Vector(c[0], c[1], c[2]), COLOR_BLUE);
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::reportErrorWarning(const char *pWarningString)
{
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::draw3dText(const btVector3 &location, const char *pTextString)
{
}

//----------------------------------------------------------------------------------------------
void CPhysDebugDrawerGL::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}
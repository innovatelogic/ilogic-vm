#ifndef __physdebugdrawergl_h__
#define __physdebugdrawergl_h__

#include <LinearMath/btIDebugDraw.h>

class CPhysDebugDrawerGL : public btIDebugDraw
{
public:
	CPhysDebugDrawerGL(class CCoreSDK *pSDK);
	virtual ~CPhysDebugDrawerGL(); 

	virtual void	drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor);

	virtual void	drawLine(const btVector3 &from,const btVector3 &to, const btVector3 &color);

	virtual void	drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color);

	virtual void	drawTriangle(const btVector3 &a,const btVector3 &b,const btVector3 &c,const btVector3 &color, btScalar alpha);
	
	virtual void	drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void	reportErrorWarning(const char *pWarningString);

	virtual void	draw3dText(const btVector3 &location, const char *pTextString);

	virtual void	setDebugMode(int debugMode);

	virtual int		getDebugMode() const { return m_debugMode;}

private:
	int				m_debugMode;
	class CCoreSDK	*m_pCoreSDK;
};

#endif//__physdebugdrawergl_h__
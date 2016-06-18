#include "transform_traits.h"

namespace core_sdk_api
{
    float transform_traits::GetControllerScaleMultiplicator(const Matrix &view, const Vector &viewPos, const Vector &pos) const
    {
        Vector camStrafe = Vector(view._11, view._21, view._31);
        Vector transDelta(viewPos - pos);

        camStrafe.normalize();

        float fCathetusOppositLen = transDelta.Length() * ::tan(0.1f);
        Vector vCathetusOpposit = transDelta.Cross(Vector(0.f, 1.f, 0.f));
        vCathetusOpposit.normalize();
        vCathetusOpposit *= fCathetusOppositLen;

        return camStrafe.Dot(vCathetusOpposit); // projection length 
    }

    //----------------------------------------------------------------------------------------------
    void transform_traits::TransformPosition(IDrawInterface *object, const Vector &pivot, const Vector &ypr)
    {

    }

    //----------------------------------------------------------------------------------------------
    float transform_traits::GetDeltaRotationAngle(const Matrix &view,
        const Matrix &proj,
        const Vector &viewPos,
        const Vector2f viewportSize,
        const Vector2f &mousePos,
        const Vector2f &mousePrev,
        const Vector   &pivotPos,
        EAxis axis)
    {
        Matrix invView, I;
        invert(invView, view);

        Vector viewDirection = UnprojectViewport(viewDirection,
            proj,
            view,
            mousePos,
            viewportSize);

        Vector prevViewDirection = UnprojectViewport(prevViewDirection,
            proj,
            view,
            mousePrev,
            viewportSize);
        
        const Vector secantPlaneN(axis == EAxisY ? 1.f : 0.f,
                                  axis == EAxisZ ? 1.f : 0.f,
                                  axis == EAxisX ? 1.f : 0.f);

        Vector intersect = RayPlaneIntersect(pivotPos, secantPlaneN, viewPos, viewDirection);
        Vector intersectPrev = RayPlaneIntersect(pivotPos, secantPlaneN, viewPos, prevViewDirection);

        Vector d1 = intersect - pivotPos;
        Vector d2 = intersectPrev - pivotPos;

        d1.normalize();
        d2.normalize();

        float dot = d2.Dot(d1);
       // float lenSq1 = d1.LengthSqr();
       // float lenSq2 = d2.LengthSqr();

        return (GetHalfSpace(d2.Cross(secantPlaneN), pivotPos, intersect) >= 0 ? 1.f : -1.f) * ::acos(dot);

       /* float dot = d2.Dot(d1);

        float sign1 = d1.Dot(Vector(1.f, 0.f, 0.f));
        float sign2 = d2.Dot(Vector(1.f, 0.f, 0.f));

        if (d1.y > 0.f)
        {
            sign1 *= -1.f;
            sign2 *= -1.f;
        }*/

       // return /*((sign1 > sign2) ? 1.f : -1.f) */ ::acosf(dot);
    }
}
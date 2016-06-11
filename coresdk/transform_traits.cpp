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
}
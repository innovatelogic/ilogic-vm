#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "game_types.h"

//class IDrawInterface;

namespace core_sdk_api
{
    class CORESDK_API transform_traits
    {
    public:

        void Foo(const Matrix &view);
     //   float GetControllerScaleMultiplicator(const Matrix &view, 
       //                                       const Vector &viewPos, 
         //                                     const Vector &pos) const;

        float transform_traits::GetControllerScaleMultiplicator(const Matrix &view,
            const Vector &viewPos,
            const Vector &pos) const
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
        //Vector GetIntersectPosition(const Vector &controllerPos, const  EScrObjectEvent mode, float &out_mult) const;

  //      void TransformPosition(IDrawInterface *object, const Vector &pivot, const Vector &ypr);
    };
}
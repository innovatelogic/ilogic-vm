#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "game_types.h"
#include "IDrawInterface.h"

namespace core_sdk_api
{
    class CORESDK_API transform_traits
    {
    public:

        float GetControllerScaleMultiplicator(const Matrix &view, const Vector &viewPos, const Vector &pos) const;

        void TransformPosition(IDrawInterface *object, const Vector &pivot, const Vector &ypr);

        float GetDeltaRotationAngle(const Matrix &view,
                                    const Matrix &proj,
                                    const Vector &view_pos,
                                    const Vector2f ViewportSize,
                                    const Vector2f &mousePos,
                                    const Vector2f &mousePrev,
                                    EAxis axis);
    };
}
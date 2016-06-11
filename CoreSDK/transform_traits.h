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
    };
}
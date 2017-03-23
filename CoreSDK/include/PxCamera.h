#pragma once

#include "BaseActor.h"

namespace core_sdk_api
{
    class CORESDK_API PxCamera : public BaseActor
    {
    public:

        virtual void    SetOrtho(bool Value) = 0;
        virtual bool    GetOrtho() const = 0;

        virtual void	SetFov(float fov) = 0;
        virtual float	GetFov() const = 0;

        virtual void	SetAspect(float aspect) = 0;
        virtual float	GetAspect() const = 0;

        virtual void	SetNearDist(float _near) = 0;
        virtual float	GetNearDist() const = 0;

        virtual void	SetFarDist(float _far) = 0;
        virtual float	GetFarDist() const = 0;

        virtual void	SetYaw(float yaw) = 0;
        virtual float	GetYaw() const = 0;

        virtual void	SetPitch(float pitch) = 0;
        virtual float	GetPitch() const = 0;

        virtual void	SetRoll(float roll) = 0;
        virtual float	GetRoll() const = 0;
    };
}

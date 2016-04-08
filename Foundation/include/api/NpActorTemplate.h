#pragma once

#include "../../Actor.h"

namespace core_sdk_api
{
    template<class APIClass>
    class NpActorTemplate : public APIClass, public CActor
    {
        NpActorTemplate(const NpActorTemplate &source) = delete;
        NpActorTemplate& operator= (const NpActorTemplate &other) = delete;

    public:
        void    release()   { CActor::Release(); }

    protected:
    private:
    };
}
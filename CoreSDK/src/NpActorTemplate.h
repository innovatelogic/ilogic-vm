#pragma once

#include "../ActorAllocator.h"

namespace core_sdk_api
{
    template<typename APIClass>
    class CORESDK_API NpActorTemplate : public APIClass, public ActorAllocator
    {
        DECLARE_CLASS_SIMPLE(NpActorTemplate, ActorAllocator);

        //NpActorTemplate(const NpActorTemplate &source) = delete;
        //NpActorTemplate& operator= (const NpActorTemplate &other) = delete;

        NpActorTemplate(const CObjectAbstract *parent)
            : ActorAllocator(parent)
        {

        }

        virtual ~NpActorTemplate()
        {

        }

    public:
        void    release() { ActorAllocator::Release(); }

    protected:
    private:
    };
}
#pragma once

#include "OEMBase.h"

namespace oes {
    namespace common_base {

        class COMMON_BASE_EXPORT IObjectAbstract
        {
        public:
            virtual ~IObjectAbstract() {}

            virtual const char*		GetType() const = 0;

            /**
            * @return unique class name value.
            */
            virtual void SetTypeId(size_t type_id) = 0;

            virtual const char*		GetName() const = 0;

            virtual void			SetName(const char *name) = 0;

            virtual IObjectAbstract* GetParent() const = 0;

            virtual void    SetExternal(bool Flag) = 0;
            virtual bool    GetExternal() const = 0;

            virtual long    GetUID() const = 0;

            virtual void	SetType(const char * type) = 0;

            /**
            * Calls every time to finalize object generation.
            */
            virtual void	FinishGeneration() = 0;
        };
    }
}
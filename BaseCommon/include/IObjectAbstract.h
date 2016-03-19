#pragma once

#include "OEMBase.h"

class COMMON_BASE_EXPORT IObjectAbstract
{
public:
    virtual ~IObjectAbstract() {}

    virtual const char*		GetType() const = 0;
};

#pragma once

#include "OEMBase.h"

class COMMON_BASE_EXPORT IObjectAbstract
{
public:
    virtual ~IObjectAbstract() {}

    virtual const char*		GetType() const = 0;

    /*!
    * redefine this function to support messaging route
    */
    //virtual void	OnEventNotify(unsigned int id) = 0;
    //virtual void	OnEventNotify(unsigned int Id, IObjectAbstract *param) = 0;
};

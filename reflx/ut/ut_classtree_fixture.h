#pragma once

#include "reflx.h"
#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class TestClassTree : public ::testing::Test
{
private:
    void SetUp() override;

    void TearDown() override;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class Base
{
    DECLARE_CLASS_SIMPLE_NOBASE(Base);

public:
    bool bval0;
    int  nval1;
};

//----------------------------------------------------------------------------------------------
class A : public Base
{
    DECLARE_CLASS_SIMPLE(A, Base);

public:
    bool bvalA;
    int  nvalA;
};

//----------------------------------------------------------------------------------------------
struct STestStruct
{
    DECLARE_CLASS_SIMPLE_NOBASE(STestStruct);

    bool bvalS;
    int  nvalS;

    bool	IsInitialized() const {}
    bool	OnPrePropertyChange(const char *name) {}
    void	OnPropertyChanged(const char *name) {}

    STestStruct()
        : bvalS(true)
        , nvalS(0)

    {
    }
};

//----------------------------------------------------------------------------------------------
class D : public Base
{
    DECLARE_CLASS_SIMPLE(D, Base);

public:
    const size_t arrSize = 3;

public:
    bool bvalD;
    STestStruct arrTestStruct[3];
};

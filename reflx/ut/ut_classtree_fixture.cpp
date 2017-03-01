#include "ut_classtree_fixture.h"

//----------------------------------------------------------------------------------------------
static const bool V_FALSE = false;
REGISTER_CLASS_PURE_NOBASE(Base)
    new oes::rflex::PropertyBOOL("bval0", DATAFIELD_OFFSET(Base, bval0), "Base", "Value", READ_ONLY, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE)
END_REGISTER_CLASS_PURE_NOBASE(Base)

REGISTER_CLASS_PURE(A, Base)
    new oes::rflex::PropertyBOOL("bvalA", DATAFIELD_OFFSET(A, bvalA), "A", "Value", CTRL_COMBO, READ_WRITE, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE),
END_REGISTER_CLASS_PURE(A, Base);

REGISTER_CLASS_PURE_NOBASE(STestStruct)
    new oes::rflex::PropertyBOOL("bvalS", DATAFIELD_OFFSET(STestStruct, bvalS), "STestStruct", "Value", READ_ONLY, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE)
END_REGISTER_CLASS_PURE_NOBASE(STestStruct)

REGISTER_CLASS_PURE(D, Base)
    new oes::rflex::PropertyBOOL("bvalA", DATAFIELD_OFFSET(A, bvalA), "A", "Value", CTRL_COMBO, READ_WRITE, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE),
//S_ARRAY(D::arrSize, STestStruct),
END_REGISTER_CLASS_PURE(D, Base);

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
void TestClassTree::SetUp()
{

}

void TestClassTree::TearDown()
{

}
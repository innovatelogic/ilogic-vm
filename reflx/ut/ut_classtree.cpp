#include "reflx.h"
#include <gtest/gtest.h>

class Base
{
    DECLARE_CLASS_SIMPLE_NOBASE(Base);

public:
    bool bval0;
    int  nval1;
};

static const bool V_FALSE = false;
REGISTER_CLASS_PURE_NOBASE(Base)
new PropertyBOOL("bval0", DATAFIELD_OFFSET(Base, bval0), "Base", "bval0", READ_ONLY, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE)
END_REGISTER_CLASS_PURE_NOBASE(Base)
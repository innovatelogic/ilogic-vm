#pragma once

#include "OEMBase.h"
#include "matrix.h"

namespace oes {
    namespace common_base {

        class COMMON_BASE_EXPORT ICollisionInterface
        {
        public:
            virtual void Enable(bool bFlag) = 0;

            virtual void SetWorldPos(const Matrix &matrix) = 0;

        protected:
        private:
        };

    }
}
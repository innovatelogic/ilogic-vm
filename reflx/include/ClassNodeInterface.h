
#pragma once

#include "defexport.h"
#include "IBaseInterface.h"
#include "ClassNode.h"

namespace oes
{
    namespace rflex
    {

        template<typename T>
        class REFLX_EXPORT CClassNodeInterface : public ClassNode
        {
            typedef T OwnerClass;

        public:
            CClassNodeInterface(const char *name)
                : ClassNode(name)
            {
            }

            CClassNodeInterface(const char *name, oes::rflex::ClassNode::TVecProperties &PropInfo)
                : ClassNode(name, PropInfo)
            {
            }

            virtual ~CClassNodeInterface()
            {
            }

            virtual bool PrePropertyChange(const void *pPrt, oes::rflex::Property_Base *pProp)
            {
                return true;
            }

            virtual void PostPropertyChange(const void *pPtr, oes::rflex::Property_Base *pProp)
            {
                CActor *pClass = const_cast<CActor*>(reinterpret_cast<const CActor*>(pPtr));

                pClass->OnPropertyChanged(pProp->GetName());
            }

        protected:
        private:
        public:
        };
    }
}
#pragma once

#include "defexport.h"
#include "OEMBase.h"
#include "ObjectDeclaration.h"
#include "Property.h"
#include "ClassNode.h"
#include "ObjectFactory.h"
#include "classinfo.h"
#include "singleton.h"
#include <string>
#include <memory>
#include <map>

namespace oes
{
    namespace rflex
    {
        // define specified global class factory
        //typedef TClassFactory::CAuto::TGlobalFactory TGlobalClassFactory;

        struct ProperyInfo
        {
            std::string			name;
            int					memOffset;
        };

        struct Parameter
        {};

        template<typename T>
        struct TypedParameter : public Parameter
        {
            TypedParameter(const T &_data) 
                : data(_data)
            {}
            T data;
        };

        //----------------------------------------------------------------------------------------------
        struct ParameterList
        {
            std::map<std::string, std::shared_ptr<Parameter>> list;

            template<class... NamedParams>
            ParameterList(NamedParams... namedParams)
            {
                buildList(namedParams...);
            }

            template<class T> void addParameter(const T &param)
            {
               // list[param.name] = std::shared_ptr<Parameter>(new TypedParameter<T>(param));
            }

            template<class T> T& getParameter(const std::string &name) const
            {
                return static_cast<TypedParameter<T>*>(list.at(name).get())->data;
            }

        private:
            template <class... NamedParams>
            void buildList() {}

            template <class Value, class... NamedParams>
            void buildList(Value &&value, NamedParams... restParams)
            {
                addParameter(value);
                buildList(restParams...);
            }
            template<class Single, class... Rest>
            void buildList(const Single&, Rest... rest);
        };

        //----------------------------------------------------------------------------------------------
        template<typename T>
        struct rflex_type_allocator
        {
            rflex_type_allocator(ParameterList p)
                : _param(p)
            {}

            rflex_type_allocator(const T &param0, const T &param1)
                : _param(param0, param1)
            {}

        private:
            const ParameterList _param;
        };

        //----------------------------------------------------------------------------------------------
        template <typename T> 
        struct rflex_allocator
        {
            typedef T* (*TGenFunction) (const char *name, const T *parent);
            typedef T* (*TCloneFunction) (const T *source, const T *parent);

            TGenFunction func;
            TCloneFunction cfunc;
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        class REFLX_EXPORT Rflex
        {
            using TGenerator = Utility::CSingleton<Rflex>;
            friend TGenerator;

        public:
            template <typename rflex_alloc>
            struct STypeWrapper
            {
                STypeWrapper(const char *key, const std::vector<ProperyInfo> props/*, rflex_alloc &alloc, const char *className, const char *baseClassName*/)
                {
                    rflex_type_allocator<std::vector<ProperyInfo>> test(props);
                }
            };

            Rflex* GetInstance()
            {
                return TGenerator::GetInstance();
            }

        protected:
        private:
            Rflex()
            {}

            Rflex(const Rflex&) = delete;

            ~Rflex()
            {}
        };

        //----------------------------------------------------------------------------------------------
       // template <typename CLASS_T, typename CLASS_BASE /*, typename rflex_alloc*/>
        void _RegisterType(const char *key/*, rflex_alloc &alloc, const char *className, const char *baseClassName*/);
        
    }
}
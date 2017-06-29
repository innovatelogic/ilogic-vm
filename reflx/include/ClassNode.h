#pragma once

#include "defexport.h"

#include "itype.h"
#include <vector>
#include <memory>

namespace oes
{
    namespace rflex
    {
        class REFLX_EXPORT ClassNode
        {
            enum class inheritance
            {
                plain = 0,
                virt,
            };

        public:
            typedef std::vector<ClassNode*>		        TVecClassNode;
            typedef TVecClassNode::iterator				TVecClassNodeIterator;
            typedef TVecClassNode::const_iterator		TVecClassNodeConstIterator;
            typedef TVecClassNode::reverse_iterator		TVecClassNodeRevIterator;

            typedef std::vector<struct SInterfaceDecl*>	TVecInterfaces;
            typedef TVecInterfaces::iterator			TVecInterfaceIter;
            typedef TVecInterfaces::const_iterator		TVecInterfaceConstIter;

            typedef std::vector<class Property_Base*>	TVecProperties;
            typedef TVecProperties::iterator			TVecPropertyIterator;
            typedef TVecProperties::const_iterator		TVecPropertyConstIterator;

        public:
            ClassNode(const char *name);
            virtual ~ClassNode();

            ClassNode(const ClassNode &other) = delete;
            ClassNode& operator=(const ClassNode &other) = delete;

            void		Release();

            ClassNode*	Find(const char *Type);
            ClassNode*	AddChild(ClassNode *pNode);
            ClassNode*	RemoveNode(ClassNode *pNode);

            class Property_Base*	GetProperty(const char* Name) const;
            bool					AddProperty(const char* Name, Property_Base *pProp);

            void	SetProprties(const class IPropertiesAllocator *pPropAlloc);
            void	SetProprties(const class Property_Base** Arr, int Count);

            ClassNode*  GetRootNode() const { return m_pRootNode; }
            void        SetRootNode(ClassNode *pNode) { m_pRootNode = pNode; }

            ClassNode*  Parent() const { return m_pRootNode; }

            ClassNode* GetChild(char *type) const;

            virtual bool	PrePropertyChange(const void *pPrt, class Property_Base *pProp);
            virtual void	PostPropertyChange(const void *pPrt, class Property_Base *pProp);

            virtual const char* GetName() const { return m_name.c_str(); }

            Property_Base** GetPropsRaw() const { return m_pPropsRaw; }
            unsigned int	GetPropsSize() const { return m_PropsSize; }

            const ClassNode::TVecInterfaces& GetInterfaceProps() const { return interfaces; }
            const ClassNode::TVecProperties& GetProperties() const { return propertyMap; }

            SInterfaceDecl* GetInterfaceDecl(const char *Name) const;

            bool IsInheritBaseClass(const char *Name) const;

            const TVecClassNode&	GetChilds() const { return childs; }

            const TVecInterfaces&	GetInterfaces() const { return interfaces; }

        public:
            std::string				m_name;

            TVecClassNode			childs;
            TVecInterfaces			interfaces;

            /** property map */
            TVecProperties			propertyMap;

        private:
            std::vector<std::pair<ClassNode*, inheritance>> parents;

            ClassNode				*m_pRootNode;
            
            IPropertiesAllocator	*m_pPropAlloc;

            Property_Base			**m_pPropsRaw;
            unsigned int			m_PropsSize;

            struct SInterfaceDecl	**m_pInterfaces;
            unsigned int			m_nIntfSize;

            //std::unique_ptr<oes::rflex::itype> m_type;
        };
    }
}



#pragma once

#ifdef _WIN32
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class SubMeshNode;
        class BaseMaterial;
        class CSceneMeshNode;

        //----------------------------------------------------------------------------------------------
        // mesh 
        struct D3DDRIVER_API SMeshMaterial
        {
            std::string Name;
            int id;

            std::string FX;
            std::string tehnique;

            // material colors...
            float				diffuse[4];			// diffuse color
            float				specular[4];		// specular color
            float               shininess;			// specular exponent
            float				ambient[4];			// ambient color
            float				emission[4];		// emitted light intensity of the material
            float               transparent;		// level of transparency
            // fog properties...
            bool                fog;				// fog
            float				fog_color[4];		// fog color

            std::vector<unsigned int>	textures;
            std::vector<unsigned int>	tex_channel;
        };

        //----------------------------------------------------------------------------------------------
        struct D3DDRIVER_API SMeshMaterialSet
        {
            std::string	ObjectFile;
            std::string	SklFile;
            std::string	TrkFile;
            std::string MrkFile;
            std::string ColFile;

            std::vector<std::string> Textures;
            std::vector<SMeshMaterial> Materials;
        };

        //----------------------------------------------------------------------------------------------
        struct D3DDRIVER_API SMeshEffectPair
        {
            SubMeshNode	*SubMesh;
            BaseMaterial	*Effect;

            SMeshEffectPair()
                : SubMesh(NULL)
                , Effect(NULL)
            {
            }

            SMeshEffectPair(SubMeshNode *pSubMesh, BaseMaterial *pEffect)
                : SubMesh(pSubMesh)
                , Effect(pEffect)
            {
            }
        };
       
        typedef std::vector<SMeshEffectPair>	TVecSubMeshes;
        typedef TVecSubMeshes::iterator			TVecSubMeshesIterator;
        typedef TVecSubMeshes::const_iterator	TVecSubMeshesConstIterator;

        struct SVecSubMeshes
        {
            TVecSubMeshes VSubMeshes;
        };

        struct SMeshEffectBank
        {
            std::vector<SVecSubMeshes> VBankNodes;
        };

        //----------------------------------------------------------------------------------------------
        class D3DDRIVER_API D3DMesh : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(D3DMesh, D3DRenderInterface);

        protected:




        public:
            D3DMesh(const D3DDriver *pDrv);
            D3DMesh(const D3DMesh &Sender);
            virtual ~D3DMesh();

            virtual void	Release();
            virtual void	Clear(bool SkipEffects = false);

            virtual bool	LoadFromDesc(const SMeshMaterialSet &Desc);

            virtual BaseMaterial*	ApplyMeshMaterial(int materialID, const SMeshMaterialSet &MaterialSet);

            void	AddMesh(SMeshEffectPair &Pair, size_t LOD = 0, size_t Dstr = 0);

            void	Render();

            bool	GetShowDebug() const { return m_bShowDebug; }
            void	SetShowDebug(bool Value) { m_bShowDebug = Value; }

            bool	GetShowNormals() const { return bShowNormals; }
            void	SetShowNormals(bool Value) { bShowNormals = Value; }

            bool	GetShowTangent() const { return bShowTangent; }
            void	SetShowTangent(bool Value) { bShowTangent = Value; }

            bool	GetShowBinormal() const { return bShowBinormal; }
            void	SetShowBinormal(bool Value) { bShowBinormal = Value; }

            bool	GetCastShadows() const { return bCastShadow; }
            void	SetCastShadows(bool Value) { bCastShadow = Value; }

            bool	GetReciveShadows() const { return bReciveShadow; }
            void	SetReciveShadows(bool Value) { bReciveShadow = Value; }

            unsigned int GetForceLODLvl() const { return m_nForceLODLvl; }
            void		 SetForceLODLvl(unsigned int Value) { m_nForceLODLvl = Value; }

            unsigned int GetDestructLvl() const { return m_nDestructLvl; }
            void		 SetDestructLvl(unsigned int Value) { m_nDestructLvl = Value; }

            void					SetSceneMeshNode(CSceneMeshNode* pScene) { m_pSceneMesh = pScene; }
            CSceneMeshNode*	GetSceneMeshNode() const { return m_pSceneMesh; }

        private:
            virtual void	DoRender();

        public:
            virtual void	SetBoneTransform(unsigned int index, float *pMatrix);

            oes::d3d::CSceneMeshNode	*m_pSceneMesh;

        protected:
            // CSceneMeshNode Nodes replica  
            std::vector<SMeshEffectBank>	m_VMeshEffectBank; // Dstr0[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]],
                                                               // Dstr1[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]],
                                                               // Dstr2[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]], 

            bool m_bShowDebug;
            bool bShowNormals;
            bool bShowTangent;
            bool bShowBinormal;

            bool bCastShadow;
            bool bReciveShadow;

            unsigned int m_nForceLODLvl;
            unsigned int m_nDestructLvl;
        };
    }
}
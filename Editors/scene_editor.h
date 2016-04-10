#pragma once

#include "editor.h"

class SRenderContext;
class CComp_EnvSceneInfo;
class CCamera;

namespace editors
{
    class Scene3DEditor : public EditorBase
    {
    public:
        Scene3DEditor(CActor *actor, ICommandBuffer *buffer);
        virtual ~Scene3DEditor();

    protected:
    private:
        SRenderContext	    *m_pRenderContext;
        CComp_EnvSceneInfo	*m_pEnvSceneInfo;
        CCamera				*m_pCamera;
    };
}

#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "Actor.h"
#include "game_types.h"
#include "transform_traits.h"
#include "transform_history_traits.h"

class CCoreSDK;
class IDrawInterface;

namespace core_sdk_api
{
    template<class TTransformTraits, class TTransformHistory>
    class CORESDK_API ViewportInterface : public TTransformTraits, public TTransformHistory
    {
        struct SController
        {
            Vector displace;
        };

        // shrinked list of selected items & control info
        using TMapSelection = std::map<IDrawInterface*, SController>;

    public:
        ViewportInterface(const CObjectAbstract *pParent);
        virtual ~ViewportInterface();

        void RegisterViewportInterface(const CActor *src);
        void UnregisterViewportInterface();

        const Matrix&	GetViewportViewMatrix() const { return m_ViewMatrix; }
        void			SetViewportViewMatrix(const Matrix &matrix) { m_ViewMatrix = matrix; }

        const Matrix&	GetViewportProjMatrix() const { return m_ProjMatrix; }
        void			SetViewportProjMatrix(const Matrix &matrix) { m_ProjMatrix = matrix; }

        const Vector&	GetViewPoint() const { return m_ViewPoint; }
        void			SetViewPoint(const Vector &Value) { m_ViewPoint = Value; }

        float   GetNearPlane() const { return m_fNearPlane; }
        void    SetNearPlane(float fValue) { m_fNearPlane = fValue; }

        float   GetFarPlane() const { return m_fFarPlane; }
        void    SetFarPlane(float fValue) { m_fFarPlane = fValue; }

        /*!
        * Draw viewport's routine
        */
        virtual void	DrawViewport() const;

        /*!
        * sets selection
        * @selection - vector of pointers to draw interface
        */
        void    SetSelection(const std::vector<IDrawInterface*> &selection);

        /*!
         * adds selection to list
         * @object - pointer to draw interface
        */
        void    SetSelect(IDrawInterface *object);

        /*!
        * removes selection from list
        * @object - pointer to draw interface
        */
        void    DropSelect(IDrawInterface *object);
        
        /*!
        * clears selection list
        */
        void    UnselectAll();
        
        /*!
        * returns selected list
        */
        const TMapSelection& GetSelected() const { return m_SelectedList; }

        /*!
        * process controller input
        * @input - mouse input data
        */
        bool ProcessController(const MouseInputData &input);
       
        /*!
        * process controller input
        * @input - mouse move input data
        */
        bool ProcessController(const MouseMoveInputData &input);

        /*! 
        * release controller processing state
        **/
        void ControllerRelease();

        /*!
        * Process mouse input  
        */
        void ProcessMouseInput(const MouseInputData &input);

        /*!
        * Process mouse move input
        */
        void ProcessMouseInput(const MouseMoveInputData &input);

        /**
        * [Editor] specific movement type.
        */
        EScrObjectEvent	GetControlMode() const { return m_controllerMode; }
        void			SetControlMode(EScrObjectEvent mode) { m_controllerMode = mode; }

        EActorState     GetControllerState() const { return m_controllerState;  }
        void            SetControllerState(EActorState state) { m_controllerState = state; }

        bool    GetMiddleButtonPressed() { return m_bSMiddleButtonPressed; }

    protected:
        void    DrawController(const Vector &pos) const;

        bool    GetControllerPos(Vector &out) const;

        void	ControllerTranslate(const Vector &pos, float k);
        void	ControllerRotateLocal(const MouseMoveInputData &input);
        void	ControllerScaleLocal(const MouseMoveInputData &input);

        Vector  GetIntersectPosition(const MouseMoveInputData &input, EScrObjectEvent mode, float &out_mult) const;
       
        void    UpdateSelectionState(EScrObjectEvent state, const Vector &ctrlPos);

    private:
        Matrix  m_ViewMatrix;
        Matrix  m_ProjMatrix;
        Matrix  m_CameraWTM;
        Vector  m_ViewPoint;
        float	m_fNearPlane;
        float	m_fFarPlane;

        TNodeMap<CActor, ViewportInterface> *m_pNode;

        // shrinked list of selected items & control info
        TMapSelection m_SelectedList;
        
        // controller input state
        EScrObjectEvent  m_controllerMode;
        EActorState	     m_controllerState;

        static Vector	 m_SUserStartMousePosition;
        static Vector	 m_SUserStartMouseDisplace;
        static Vector    m_SUserStartControllerPos;
        static Vector    m_SUserAccumRotation;
        static bool		 m_bSMiddleButtonPressed;
        
        mutable CCoreSDK *m_pCoreSDK;
    };

    typedef ViewportInterface<transform_traits, transform_history_traits> TIViewport;
}

#include "ViewportInterface.ipp"
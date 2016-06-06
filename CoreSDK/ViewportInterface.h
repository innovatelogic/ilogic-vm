#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "Actor.h"
#include "game_types.h"

class CCoreSDK;
class IDrawInterface;

namespace core_sdk_api
{
    class CORESDK_API ViewportInterface
    {
    public:
        ViewportInterface(const CObjectAbstract *pParent);
        virtual ~ViewportInterface();

        void RegisterViewportInterface(const CActor *Src);
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
        void SetSelection(const std::vector<IDrawInterface*> &selection);

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
        const std::list<IDrawInterface*>& GetSelected() const { return m_SelectedList; }

        /*!
        * process controller input
        * @input - mouse input data
        */
        virtual bool ProcessController(const MouseInputData &input);

        /**
        * [Editor] specific movement type.
        */
        EScrObjectEvent		GetControlMode() const { return m_controllerMode; }
        void				SetControlMode(EScrObjectEvent mode) { m_controllerMode = mode; }

    protected:
        void DrawController(const Vector &pos) const;

        bool GetControllerPos(Vector &out) const;

    private:

        Matrix  m_ViewMatrix;
        Matrix  m_ProjMatrix;
        Matrix  m_CameraWTM;
        Vector  m_ViewPoint;
        float	m_fNearPlane;
        float	m_fFarPlane;

        TNodeMap<CActor, ViewportInterface> *m_pNode;

        // shrinked list of selected items
        std::list<IDrawInterface*> m_SelectedList;

        // controller input state
        EScrObjectEvent  m_controllerMode;

        static Vector	 m_SUserStartMousePosition;
        static Vector	 m_SUserStartMouseDisplace;
        static bool		 m_bSMiddleButtonPressed;

        mutable CCoreSDK *m_pCoreSDK;
    };
}
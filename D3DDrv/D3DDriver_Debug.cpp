#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        void D3DDriver::DrawDebugInfo()
        {
            //DrawDebugLines();
            //DrawDebugTriangles();
            //DebugRenderShadowmap();

            DrawFPS();
            DrawPerfInfo();
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DrawDebugLines()
        {
            // render 3D point data
            if (m_3DDotsList.size() > 0)
            {
                size_t num_points = (m_3DDotsList.size() <= MAX_LINE_POINTS) ? m_3DDotsList.size() : MAX_LINE_POINTS;

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixf(GetProjMatrix());

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(GetViewMatrix());

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DDotsList[0].m_Position);

                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DDotsList[0].m_Color);

                glDrawArrays(GL_POINTS, 0, num_points);

                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                m_3DDotsList.clear();
            }

            // z disable
            if (m_3DDotsListNoZ.size() > 0)
            {
                size_t num_points = (m_3DDotsListNoZ.size() <= MAX_LINE_POINTS) ? m_3DDotsListNoZ.size() : MAX_LINE_POINTS;

                glDisable(GL_DEPTH_TEST);
                glMatrixMode(GL_PROJECTION);
                glLoadMatrixf(GetProjMatrix());

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(GetViewMatrix());

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DDotsListNoZ[0].m_Position);

                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DDotsListNoZ[0].m_Color);

                glDrawArrays(GL_POINTS, 0, num_points);

                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);
                glEnable(GL_DEPTH_TEST);

                m_3DDotsListNoZ.clear();
            }

            // render 3D point data
            if (m_3DPointList.size() > 1)
            {
                size_t num_points = (m_3DPointList.size() <= MAX_LINE_POINTS) ? m_3DPointList.size() : MAX_LINE_POINTS;
                size_t num_lines = num_points / 2;

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixf(GetProjMatrix());

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(GetViewMatrix());

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DPointList[0].m_Position);

                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_3DPointList[0].m_Color);

                glDrawArrays(GL_LINES, 0, num_lines * 2);

                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                m_3DPointList.clear();
            }

            // z disable
            if (m_PointListNoZ.size() > 1)
            {
                size_t num_points = (m_PointListNoZ.size() <= MAX_LINE_POINTS) ? m_PointListNoZ.size() : MAX_LINE_POINTS;
                size_t num_lines = num_points / 2;

                glDisable(GL_DEPTH_TEST);

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixf(GetProjMatrix());

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(GetViewMatrix());

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_PointListNoZ[0].m_Position);

                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_PointListNoZ[0].m_Color);

                glDrawArrays(GL_LINES, 0, num_lines * 2);

                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                glEnable(GL_DEPTH_TEST);

                m_PointListNoZ.clear();
            }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DrawDebugTriangles()
        {
            if (m_PointsTriangle.size() >= 3)
            {
                size_t NumTriangles = (m_PointsTriangle.size() <= MAX_DBG_TRIANGLES * 3) ? m_PointsTriangle.size() / 3 : MAX_DBG_TRIANGLES;

                glDisable(GL_DEPTH_TEST);

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixf(GetProjMatrix());

                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(GetViewMatrix());

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_PointsTriangle[0].m_Position);

                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), &m_PointsTriangle[0].m_Color);

                glDrawArrays(GL_TRIANGLES, 0, NumTriangles * 3);

                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                glEnable(GL_DEPTH_TEST);

                m_PointsTriangle.clear();
            }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DrawFPS()
        {

        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DrawPerfInfo()
        {
            /*	RECT rc;

                D3DXMATRIX TxtTransform;
                D3DXMatrixIdentity(&TxtTransform);

                TxtTransform._42 += 100;

                m_pd3dDevice->SetTransform(D3DTS_WORLD, &TxtTransform);
                m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_view);
                m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

                char buffer[256] = {'\0'};

                SetRect( &rc, 0, 0, 0, 0 );

                m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

                sprintf(buffer, " %s \n cps \n event %d \n update %d \n render %d \n timer %d \n serialize %d",
                            (m_bParalell ? "serial" : "parallel"),
                            m_eventCPS,
                            m_updateCPS,
                            m_renderCPS,
                            m_timerCPS,
                            m_streamCPS
                        );
                m_CanvasFont->DrawTextA(m_pTextSprite, buffer, -1, &rc, DT_NOCLIP, 0xffffffff);

                m_pTextSprite->End();*/
        }

    }
}
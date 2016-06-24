#include "coresdkafx.h"
#include "Comp_MeshNode.h"
#include "../D3DDrv/D3DMesh.h"
#include "../D3DDrv/SubMeshNode.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

REGISTER_CLASS(CPolygonMesh, ActorAllocator);

static Vector SwitchedPoint;
static Vector SwitchedNormal;
static Vector SwitchedPlaneDisplace;
static float Sk;

class CUVModificator
{
public:
	void Affect(CPolygonMesh &AffectObject)
	{

	}

//protected:
//private:
	std::vector<size_t> AffectedFaces;
};

//----------------------------------------------------------------------------------------------
CPolygonMesh::CPolygonMesh(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "Comp_StaticMesh", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pMaterialEffect, Comp_MaterialEffect, "Comp_MaterialEffect", this);

	Load("TestPoly.pxml");
}

//----------------------------------------------------------------------------------------------
CPolygonMesh::CPolygonMesh(const CPolygonMesh &Source)
: Super(Source)
, IDrawInterface(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------------------
CPolygonMesh::~CPolygonMesh()
{

}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::Initialize()
{
	Super::Initialize();

	Rebuild();
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::Load(const char * FileName)
{
	bool bResult = false;

	std::string sFilename;  

	if (!FindFile(FileName, &sFilename))
	{
		MessageBoxW(NULL, ConvertStringToWideString(sFilename).c_str(), L"Warning", MB_OK);
		return bResult;
	}

	unsigned int count = 0;

	XML_PARSE_FILE( sFilename.c_str())
	{
		XML_FOR_EACH_TREE(XML_CURRENT_NODE)
		{
			XML_CHECK_NODE("Vertices")
			{
				CStringUtility<float> ParseString(XML_CURRENT_NODE->FirstChild()->Value(), " ");

				size_t Size = ParseString.m_vector.size();

				if (Size > 0 || Size % 3 == 0)
				{
					std::vector<float>::iterator Iter = ParseString.m_vector.begin();

					while (Iter != ParseString.m_vector.end())
					{
						m_Vertices.push_back(Vector(*Iter, *(Iter + 1), *(Iter + 2)));
						Iter += 3;
					}
				}
				else
				{
					MessageBoxW(NULL, L"Vertex data failed", L"Warning", MB_OK);
					return bResult;
				}
			}

			XML_CHECK_NODE("TMaps")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("TMapBase")
					{
						STexMap Map;

						CStringUtility<float> ParseString(XML_CURRENT_NODE->FirstChild()->Value(), " ");

						size_t Size = ParseString.m_vector.size();

						if (Size > 0 || Size % 3 == 0)
						{
							std::vector<float>::iterator Iter = ParseString.m_vector.begin();

							while (Iter != ParseString.m_vector.end())
							{
								m_TexCoords.push_back(Vector(*Iter, *(Iter + 1), *(Iter + 2)));
								Iter += 3;
							}
						}
						else
						{
							MessageBoxW(NULL, L"Texture map data failed", L"Warning", MB_OK);
							return bResult;
						}
					}

					XML_CHECK_NODE("TMapLayers")
					{
						XML_FOR_EACH_TREE(XML_CURRENT_NODE)
						{
							XML_CHECK_NODE("TMapLayer")
							{

							}
						}
					}
				}
			}

			XML_CHECK_NODE("Face")
			{
				SPolygonFace face;

				// TEST CODE
// 				CUVModificator *pUV = new CUVModificator;
// 				pUV->AffectedFaces.push_back(count++);
// 				m_VecModificators.push_back(pUV);
				// TODO REFACTOR

				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("Normal")
					{
						CStringUtility<float> ParseString(XML_CURRENT_NODE->FirstChild()->Value(), " ");
						face.Normal = Vector(ParseString.m_vector[0], ParseString.m_vector[1], ParseString.m_vector[2]);
					}

					XML_CHECK_NODE("Mat")
					{
						if (MaterialEffect *pFx = GetCoreSDK()->GetRenderSDK()->GetFXManager()->GetMaterialInstance(XML_CURRENT_NODE->FirstChild()->Value()))
						{
							m_VecMatInstances.push_back(pFx);
							face.mat_id = m_VecMatInstances.size();
						}
					}

					XML_CHECK_NODE("Indexes")
					{
						CStringUtility<unsigned int> ParseString(XML_CURRENT_NODE->FirstChild()->Value(), " ");
						std::vector<unsigned int>::iterator Iter = ParseString.m_vector.begin();

						while (Iter != ParseString.m_vector.end())
						{
							//face.VertexIndexes.push_back(*Iter);
							face.FaceIndexes.push_back(*Iter);
							++Iter;
						}
					}
				}

				m_Faces.push_back(face);
			}
		}
	}

	bResult = true;
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::Save(const char * FileName)
{
	bool bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::DoDraw()
{
/*	m_pMeshComponent->SetTransformedWTM(WorldMatrixTransform);
	m_pRenderSDK->DrawObject(m_pMeshComponent);

 	if (IsFocused())
 	{
		for (std::vector<SPolygonFace>::const_iterator Iter = m_Faces.begin(); Iter != m_Faces.end(); ++Iter)
 		{
 			for (unsigned int Index = 0; Index < (*Iter).VertexIndexes.size(); ++Index)
 			{
				const Vector &Point = m_Vertices[(*Iter).VertexIndexes[Index]];
 				
				Vector Temp;
 				transform_coord(Temp, Point, WorldMatrixTransform);

				unsigned int Color = 0xff0000ff; // blue

				std::map<int, SSelectInfo>::const_iterator IterFindFace = m_MapSelectInfo.find(Iter - m_Faces.begin());

 				if (IterFindFace != m_MapSelectInfo.end())
 				{
 					if (std::find(IterFindFace->second.PointIndexes.begin(), IterFindFace->second.PointIndexes.end(), Index)
						!= IterFindFace->second.PointIndexes.end())
 					{
 						Color = 0xffff0000; // red one
 					}
 				}
 				GetRenderComponent()->DrawDot(Temp, Color);
 			}
 		}
 	}*/
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::DrawController() const
{
/*	CCoreSDK * pCoreSDK = GetAppMain();

	assert(pCoreSDK);

	if (m_MapSelectInfo.size()) // draw controller-point selected
	{
		Matrix WorldMatrixTransform = GetTransformedWTM();

		Vector Point;
		transform_coord(Point, GetSelectionPivot(), WorldMatrixTransform);

		const CCamera * pCamera = pCoreSDK->GetCameraManager()->GetActiveCamera();

		CRenderSDK * pRenderSDK = pCoreSDK->GetRenderSDK();

		if (pCamera && pRenderSDK)
		{
			EScrObjectEvent Mode = GetControlMode();
			
			Vector CamDir(pCamera->GetDirection());
			Vector Delta(pCamera->GetTransformedWTM().t - Point);

			CamDir.normalize();
			CamDir *= CamDir.Dot(Delta);

			float Length = length(CamDir);
			float k = Length * ::sin(0.1f);
			
			Matrix I;
			const Vector AxisX = I._row0 * k;
			const Vector AxisY = I._row1 * k;
			const Vector AxisZ = I._row2 * k;
			
			pRenderSDK->DrawLine(Point, Point + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED, false);
			pRenderSDK->DrawLine(Point, Point + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE, false);
			pRenderSDK->DrawLine(Point, Point + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN, false);

			// render arrows
			float a, b;
			a = 0.8f; b = 0.1f;

			// X arrow
			pRenderSDK->DrawTriangle(Point + (AxisX * a) + (AxisY * b), Point + (AxisX * a) + (AxisZ * b), Point + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			pRenderSDK->DrawTriangle(Point + (AxisX * a) + (AxisZ * b), Point + (AxisX * a) - (AxisY * b), Point + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			pRenderSDK->DrawTriangle(Point + (AxisX * a) - (AxisY * b), Point + (AxisX * a) - (AxisZ * b), Point + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			pRenderSDK->DrawTriangle(Point + (AxisX * a) - (AxisZ * b), Point + (AxisX * a) + (AxisY * b), Point + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			
			//back
			pRenderSDK->DrawTriangle(Point + (AxisX * a) - (AxisY * b), Point + (AxisX * a) + (AxisZ * b), Point + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);
			pRenderSDK->DrawTriangle(Point + (AxisX * a) - (AxisZ * b), Point + (AxisX * a) - (AxisY * b), Point + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);

			//Y arrow
			pRenderSDK->DrawTriangle(Point + (AxisY * a) + (AxisZ * b), Point + (AxisY * a) + (AxisX * b), Point + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			pRenderSDK->DrawTriangle(Point + (AxisY * a) + (AxisX * b), Point + (AxisY * a) - (AxisZ * b), Point + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			pRenderSDK->DrawTriangle(Point + (AxisY * a) - (AxisZ * b), Point + (AxisY * a) - (AxisX * b), Point + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			pRenderSDK->DrawTriangle(Point + (AxisY * a) - (AxisX * b), Point + (AxisY * a) + (AxisZ * b), Point + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			
			//back
			pRenderSDK->DrawTriangle(Point + (AxisY * a) - (AxisZ * b), Point + (AxisY * a) + (AxisX * b), Point + (AxisY * a) + (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);
			pRenderSDK->DrawTriangle(Point + (AxisY * a) + (AxisZ * b), Point + (AxisY * a) - (AxisX * b), Point + (AxisY * a) - (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);

			//Z arrow
			pRenderSDK->DrawTriangle(Point + (AxisZ * a) + (AxisX * b), Point + (AxisZ * a) + (AxisY * b), Point + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			pRenderSDK->DrawTriangle(Point + (AxisZ * a) + (AxisY * b), Point + (AxisZ * a) - (AxisX * b), Point + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			pRenderSDK->DrawTriangle(Point + (AxisZ * a) - (AxisX * b), Point + (AxisZ * a) - (AxisY * b), Point + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			pRenderSDK->DrawTriangle(Point + (AxisZ * a) - (AxisY * b), Point + (AxisZ * a) + (AxisX * b), Point + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);

			pRenderSDK->DrawTriangle(Point + (AxisZ * a) + (AxisY * b), Point + (AxisZ * a) + (AxisX * b), Point + (AxisZ * a) - (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);
			pRenderSDK->DrawTriangle(Point + (AxisZ * a) - (AxisY * b), Point + (AxisZ * a) - (AxisX * b), Point + (AxisZ * a) + (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);

			//render origin rects
			float c = 0.3f;

			// X-Y
			pRenderSDK->DrawLine(Point + (AxisX * c), Point + (AxisX * c) + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_RED, false);
			pRenderSDK->DrawLine(Point + (AxisX * c) + (AxisY * c), Point + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_BLUE, false);

			// Y-Z
			pRenderSDK->DrawLine(Point + (AxisY * c), Point + (AxisY * c) + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_BLUE, false);
			pRenderSDK->DrawLine(Point + (AxisY * c) + (AxisZ * c), Point + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_GREEN, false);

			// X-Z
			pRenderSDK->DrawLine((Point + (AxisX * c)), (Point + (AxisX * c) + (AxisZ * c)), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_RED, false);
			pRenderSDK->DrawLine((Point + (AxisX * c) + (AxisZ * c)), (Point + (AxisZ * c)), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_GREEN, false);
		}
	}
	else
	{
		IDrawInterface::DrawController();
	}*/
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::Rebuild()
{
	DoRebuildMesh();

	return true;
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::ProcessController(const MouseInputData &InputData)
{
	bool bResult = false;

/*	if (CameraManager * pCamMgr = GetAppMain()->GetCameraManager())
	{
		if (const CCamera * pCamera = pCamMgr->GetActiveCamera())
		{
			Vector2f & ControlBox = GetAppMain()->GetRegistry()->ControlBox;

			const Vector2f ViewportSize((float)GetAppMain()->GetRenderSDK()->GetViewportWidth(),
										(float)GetAppMain()->GetRenderSDK()->GetViewportHeight());

			Matrix ViewMatrix(pCamera->GetViewMatrix());
			Matrix ProjMatrix(pCamera->GetProjMatrix());*/
//			Matrix WorldMatrixTransform = GetTransformedWTM();
			
// 			if (m_MapSelectInfo.size()) // Check controller triangles / process already selected entities
// 			{
// 				Vector vPointOrigin;
// 				Vector vPlaneNormal;
// 
// //				if (m_VecSelectedPoint.size()) // point selected transform_coord(Point, m_Vertices[ m_Faces[m_nSelectedPoly].FaceIndexes[m_nSelectedPoint] ], WorldMatrixTransform);
// //				{
// // 					transform_coord(vPointOrigin, m_Vertices[ m_Faces[m_nSelectedPoly].VertexIndexes[m_nSelectedPoint] ], WorldMatrixTransform);
// // 
// // 					WorldMatrixTransform.t = Vector(0.f, 0.f, 0.f);
// // 					transform_coord(vPlaneNormal, m_Faces[m_nSelectedPoly].Normal, WorldMatrixTransform);
// //				}
// //				else // whole poly selected
// //				{
// 					// get median
// // 					transform_coord(vPointOrigin, m_Vertices[ m_Faces[m_nSelectedPoly].VertexIndexes[m_nSelectedPoint] ], WorldMatrixTransform);
// // 
// // 					WorldMatrixTransform.t = Vector(0.f, 0.f, 0.f);
// // 					transform_coord(vPlaneNormal, m_Faces[m_nSelectedPoly].Normal, WorldMatrixTransform);
// //				}
// 
// 				Matrix InvView, I;
// 				invert(InvView, pCamera->GetViewMatrix());
// 
// 				Vector CamDir(pCamera->GetDirection());
// 				Vector Delta(pCamera->GetTransformedWTM().t - vPointOrigin);
// 				
// 				CamDir.normalize();
// 				CamDir *= CamDir.Dot(Delta);
// 
// 				float Length = length(CamDir);
// 				float k = Length * ::sin(0.1f);
// 
// 				const Vector AxisX = I._row0 * k;
// 				const Vector AxisY = I._row1 * k;
// 				const Vector AxisZ = I._row2 * k;
// 
// 				const Vector2f ViewportSize((float)GetAppMain()->GetRenderSDK()->GetViewportWidth(),
// 											(float)GetAppMain()->GetRenderSDK()->GetViewportHeight());
// 				Vector AX, AY, AZ;
// 				ProjectViewport(AX, vPointOrigin + AxisX, ViewMatrix, ProjMatrix, ViewportSize);
// 				ProjectViewport(AY, vPointOrigin + AxisY, ViewMatrix, ProjMatrix, ViewportSize);
// 				ProjectViewport(AZ, vPointOrigin + AxisZ, ViewMatrix, ProjMatrix, ViewportSize);
// 
// 				Vector ViewDirection = UnprojectViewport(ViewDirection, 
// 															pCamera->GetProjMatrix(), 
// 															pCamera->GetViewMatrix(),
// 															InputData.MousePos,
// 															ViewportSize);
// 
// 				if (!bResult && IsPointInRect(InputData.MousePos.x, InputData.MousePos.y, Vector2f(AX.x, AX.y) - ControlBox, Vector2f(AX.x, AX.y) + ControlBox))
// 				{
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 					Sk = k;
// 
// 					Vector RayIntersect = RayPlaneIntersect(SwitchedPoint, SwitchedNormal, pCamera->GetTransformedWTM().t, ViewDirection);
// 
// 					m_SUserStartMouseDisplace = RayIntersect - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 					
// 					SetControlMode(SOEvent_ControlLockX);
// 					bResult = true;
// 				}
// 				if (!bResult && IsPointInRect(InputData.MousePos.x, InputData.MousePos.y, Vector2f(AY.x, AY.y) - ControlBox, Vector2f(AY.x, AY.y) + ControlBox))
// 				{
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 					
// 					Vector RayIntersect = RayPlaneIntersect(SwitchedPoint, SwitchedNormal, pCamera->GetTransformedWTM().t, ViewDirection);
// 
// 					m_SUserStartMouseDisplace = RayIntersect - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 					Sk = k;
// 
// 					SetControlMode(SOEvent_ControlLockY);
// 					bResult = true;
// 				}
// 				if (!bResult && IsPointInRect(InputData.MousePos.x, InputData.MousePos.y, Vector2f(AZ.x, AZ.y) - ControlBox, Vector2f(AZ.x, AZ.y) + ControlBox))
// 				{
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 
// 					Vector RayIntersect = RayPlaneIntersect(SwitchedPoint, SwitchedNormal, pCamera->GetTransformedWTM().t, ViewDirection);
// 					
// 					m_SUserStartMouseDisplace = RayIntersect - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 					Sk = k;
// 
// 					SetControlMode(SOEvent_ControlLockZ);
// 					bResult = true;
// 				}
// 
// 				// check plane origin hit
// 				/** ray triangle intersection */
// 				float c = 0.3f;
// 				float t = 0.f;
// 
// 				Vector P0 = (vPointOrigin + (AxisX * c));
// 				Vector P1 = (vPointOrigin + (AxisX * c) + (AxisY * c));
// 				Vector P2 = (vPointOrigin + (AxisY * c));
// 
// 				if (!bResult &&
// 					((IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P0, P1, &t) == 1) ||
// 					(IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P1, P2, &t) == 1)))
// 				{
// 					m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM().t + ViewDirection * t) - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 					Sk = k;
// 
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 
// 					SetControlMode(SOEvent_ControlLockXY);
// 					bResult = true;
// 				}
// 
// 				P0 = (vPointOrigin + (AxisZ * c));
// 				P1 = (vPointOrigin + (AxisZ * c) + (AxisY * c));
// 				P2 = (vPointOrigin + (AxisY * c));
// 
// 				if (!bResult &&
// 					((IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P0, P1, &t) == 1) ||
// 					(IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P1, P2, &t) == 1)))
// 				{
// 					m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM().t + ViewDirection * t) - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 					Sk = k;
// 
// 					SetControlMode(SOEvent_ControlLockYZ);
// 					bResult = true;
// 				}
// 
// 				P0 = (vPointOrigin + (AxisX * c));
// 				P1 = (vPointOrigin + (AxisX * c) + (AxisZ * c));
// 				P2 = (vPointOrigin + (AxisZ * c));
// 
// 				if (!bResult &&
// 					((IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P0, P1, &t) == 1) ||
// 					(IntersectRayWithTriangle(pCamera->GetTransformedWTM().t, pCamera->GetTransformedWTM().t + ViewDirection, vPointOrigin, P1, P2, &t) == 1)))
// 				{
// 					m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM().t + ViewDirection * t) - vPointOrigin;
// 					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
// 
// 					SwitchedPoint = vPointOrigin;
// 					SwitchedNormal = vPlaneNormal;
// 					Sk = k;
// 
// 					SetControlMode(SOEvent_ControlLockXZ);
// 					bResult = true;
// 				}
// 
// 				if (!bResult)
// 				{
// 					//m_MapSelectInfo.clear(); // disable selection
// 				}
// 			}
// 			else // check every points
/*			{
				if ((InputData.ModifKey & MK_Control) == 0)
				{
					m_MapSelectInfo.clear(); // disable selection
				}
				
				for (std::vector<SPolygonFace>::iterator IterFace = m_Faces.begin(); IterFace != m_Faces.end(); ++IterFace)
				{
					std::vector<int> PointIndexes;

					for (unsigned int Index = 0; Index < (*IterFace).VertexIndexes.size(); ++Index)
					{
						Vector Temp = transform_coord(Temp, m_Vertices[(*IterFace).VertexIndexes[Index]], WorldMatrixTransform);

						Vector ScrPoint;
						ProjectViewport(ScrPoint, Temp, ViewMatrix, ProjMatrix, ViewportSize);

						if (IsPointInRect(InputData.MousePos.x, InputData.MousePos.y,
							Vector2f(ScrPoint.x, ScrPoint.y) - Vector2f(2.5f, 2.5f),
							Vector2f(ScrPoint.x, ScrPoint.y) + Vector2f(2.5f, 2.5f)))
						{
							PointIndexes.push_back(Index);
							bResult = true;
						}
					}

					if (PointIndexes.size())
					{
						int Index = IterFace - m_Faces.begin();
						
						std::map<int, SSelectInfo>::iterator IterSel = m_MapSelectInfo.find(Index);
						
 						if (IterSel == m_MapSelectInfo.end())
 						{
 							SSelectInfo Info;
 							IterSel = m_MapSelectInfo.insert(std::make_pair(Index, Info)).first;
 						}

 						for (std::vector<int>::iterator IterPoint = PointIndexes.begin(); IterPoint != PointIndexes.end(); ++IterPoint)
 						{
 							std::vector<int>::iterator IterFind = std::find(IterSel->second.PointIndexes.begin(), IterSel->second.PointIndexes.end(), *IterPoint);
 							
 							if (IterFind != IterSel->second.PointIndexes.end())
 							{
 								IterSel->second.PointIndexes.erase(IterFind);

								if (!IterSel->second.PointIndexes.size()){
									m_MapSelectInfo.erase(IterSel);
								}
 							}
 							else
 							{
 								IterSel->second.PointIndexes.push_back(*IterPoint);
 							}
 						}
					}
				}
			}
		}
	}
	*/
return bResult;// ? true : IDrawInterface::ProcessController(InputData); // process default origin controller
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::ProcessController(const MouseMoveInputData &InputData)
{
	bool bResult = false;
/*
	EScrObjectEvent Mode = GetControlMode();

	if (Mode == SOEvent_FreeMove ||
		Mode == SOEvent_ControlLockOrigin ||
		Mode == SOEvent_ControlLockX || 
		Mode == SOEvent_ControlLockY ||
		Mode == SOEvent_ControlLockZ ||
		Mode == SOEvent_ControlLockXY ||
		Mode == SOEvent_ControlLockYZ ||
		Mode == SOEvent_ControlLockXZ)
	{
		if (CameraManager * pCamMgr = GetAppMain()->GetCameraManager())
		{
			if (const CCamera * pCamera = pCamMgr->GetActiveCamera())
			{
				Matrix WorldMatrixTransform = GetTransformedWTM();

				const Vector2f ViewportSize((float)GetAppMain()->GetRenderSDK()->GetViewportWidth(),
											(float)GetAppMain()->GetRenderSDK()->GetViewportHeight());

				Matrix ViewMatrix(pCamera->GetViewMatrix());
				Matrix ProjMatrix(pCamera->GetProjMatrix());

				Vector ViewDirection = UnprojectViewport(ViewDirection, 
					ProjMatrix, 
					ViewMatrix,
					InputData.MousePos,
					ViewportSize);

				if (m_MapSelectInfo.size()) // Check controller triangles / process already selected entities
				{
					Vector vPointOrigin;
					Vector vPlaneNormal;

//					if (m_VecSelectedPoint.size())
//					{
//						transform_coord(vPointOrigin, m_Vertices[ m_Faces[m_nSelectedPoly].VertexIndexes[m_nSelectedPoint] ], WorldMatrixTransform);

// 						Vector CamDir(pCamera->GetDirection());
// 						Vector TransDelta(pCamera->GetTransformedWTM().t - vPointOrigin);
// 
// 						CamDir.normalize();
// 						CamDir *= CamDir.Dot(TransDelta);
// 
// 						float Length = length(CamDir);
// 						float k = Length * ::sin(0.1f);
// 						
// 						Vector Intersect = RayPlaneIntersect(SwitchedPoint, SwitchedNormal, pCamera->GetTransformedWTM().t, ViewDirection);
// 						
// 						Vector D(Intersect - pCamera->GetTransformedWTM().t);
// 						Vector N(SwitchedNormal);
// 
// 						D.normalize();
// 						N.normalize();	
// 						
// 						float fDot = D.Dot(-N);
// 						int hs = GetHalfSpace(pCamera->GetDirection(), pCamera->GetTransformedWTM().t, Intersect);
// 
// 						if (fDot > 0.05f && hs > 0)
// 						{
// 							Matrix IWTM;
// 							invert(IWTM, GetTransformedWTM());
// 
// 							Vector Displace = m_SUserStartMouseDisplace * (k / Sk);
// 							Vector TPos = transform_coord(TPos, Intersect - Displace, IWTM);
// 
// 							float LengthNew = length(pCamera->GetTransformedWTM().t - Intersect);
// 							//if (LengthNew <= 500.f || LengthNew < Length)
// 							{
// 								//m_Vertices[ m_Faces[m_nSelectedPoly].VertexIndexes[m_nSelectedPoint] ] = TPos;
// 							}
// 						}
// 						bResult = true;
// 					}
// 					else // whole poly selected
// 					{
// 						bResult = true;
// 					}
				}
			}
		}
	}
	*/
    return !bResult;// ? IDrawInterface::ProcessController(InputData) : bResult;
}

//----------------------------------------------------------------------------------------------
bool CPolygonMesh::ProcessRelease(const MouseInputData &InputData)
{
	/*if (InputData.event == MOUSE_Released)
	{
		if (GetControlState() == ActorState_Locked)
		{
			if (m_MapSelectInfo.size()) // Check controller triangles / process already selected entities
			{
//				if (m_VecSelectedPoint.size()) // point selected
				{
					Rebuild();
				}
			}
		}
	}
	return IDrawInterface::ProcessRelease(InputData);*/
    return false;
}

//----------------------------------------------------------------------------------------------
size_t CPolygonMesh::GetMaxFaceIndex() const
{
	size_t MaxFaceIdx = 0; // max index value
	size_t FaceNum = GetNumberOfFaces();

	for (size_t Index = 0; Index < FaceNum; ++Index)
	{
		const SPolygonFace* pFace = GetFace(Index);

		for (size_t vIndex = 0; vIndex < pFace->FaceIndexes.size(); ++vIndex)
		{
			if (pFace->FaceIndexes[vIndex] > MaxFaceIdx)
			{
				MaxFaceIdx = pFace->FaceIndexes[vIndex];
			}
		}
	}
	return MaxFaceIdx;
}

//----------------------------------------------------------------------------------------------
size_t CPolygonMesh::GetNumberOfTexVerts(size_t Index) const
{
	return m_TexCoords.size();
}

//----------------------------------------------------------------------------------------------
size_t CPolygonMesh::GetNumberOfColorVerts(size_t Index) const
{
	size_t OutValue = 0;

	return OutValue;
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::BuildFaceTopology(MatFaceMapType &OutMapMatface, TriMapType &OutMapTri, size_t MaxFaceIdx)
{
	for (std::vector<SPolygonFace>::iterator IterFace = m_Faces.begin(); IterFace != m_Faces.end(); ++IterFace)
	{
		size_t Index = IterFace - m_Faces.begin();

		const SPolygonFace *pFace = GetFace(Index);

		bool HasTexVerts = GetNumberOfTexVerts(Index) ? true : false;
		bool HasCVerts = GetNumberOfColorVerts(Index) ? true : false;

		unsigned int smg_id = pFace->smg_id;
		unsigned int mat_id = pFace->mat_id;

		// lets sort by material and find the corresponding mesh_opt
		MatFaceMapIt it_matfacemap = OutMapMatface.find(mat_id);

		mesh_opt * m_opt = NULL;

		if (it_matfacemap == OutMapMatface.end()) // no corresponding mesh, allocate new face holder
		{
			m_opt = new mesh_opt();
			OutMapMatface.insert(MatFaceMapPair(mat_id, m_opt));
		}
		else
		{
			m_opt = (*it_matfacemap).second;
		}

		// iterate through triangles
		for (unsigned int vIndex = 0; vIndex < pFace->FaceIndexes.size(); vIndex += 3)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				vert_opt face;
				unsigned int idx = 0;

				const unsigned int ori_face_idx = pFace->FaceIndexes[vIndex + j]; // get actual index into the vertex array 
				unsigned int face_idx = ori_face_idx;

				bool create_face = false; // false by default

				// build the face as expected
				face.smg_id = 1; //pFace->smg_id; // smooth group

				if (HasTexVerts)
				{
					face.t.x = m_TexCoords[ori_face_idx].x;
					face.t.y = m_TexCoords[ori_face_idx].y;
				}

				if (HasCVerts)
				{
				}
				else
				{
					face.c = Vector4f(1.f, 1.f, 1.f, 1.f);
				}

				face.num_tmaps = 1;

				if (face.num_tmaps)
				{
					face.tmaps = new Vector[face.num_tmaps];
					for (size_t k = 0; k < face.num_tmaps; ++k)
					{
						face.tmaps[k].x = m_TexCoords[ori_face_idx].x;
						face.tmaps[k].y = m_TexCoords[ori_face_idx].y;
						face.tmaps[k].z = m_TexCoords[ori_face_idx].z;
					}
				}

				// try to find in origin array
				FaceMapIt it_face_map = m_opt->face_map.find(face_idx);  // get face map iter

				if (it_face_map == m_opt->face_map.end()) // if not find such create anyway
				{
					create_face = true;
				}
				else
				{
					if (is_matching((*it_face_map).second, face) == false) // check find vertex with similar ID not matching to already stored
					{
						bool found = false; // additional test in multimap

						// process vertex multi map
						std::pair<FaceMMapIt, FaceMMapIt> pair_mmap = m_opt->face_mmap.equal_range(ori_face_idx); // get range

						FaceMMapIt it_face_mmap = pair_mmap.first;

						while (it_face_mmap != pair_mmap.second && found == false) // iterate through range
						{
							idxvert_opt & idxface = (*it_face_mmap).second;

							if (is_matching(idxface.face, face))
							{
								face_idx = idxface.new_idx; // find similar in ext array
								found = true;
							}
							++it_face_mmap;
						}

						if (found == false) // not found - add new
						{
							face_idx = ++MaxFaceIdx; // increment max index and & set index is out of bounds of origin index range
							create_face = true;
						}
					}
				}

				if (create_face)
				{
					face.v = m_Vertices[ori_face_idx];

					face.face_idx = m_opt->count; // add the vertex and store its new idx

					m_opt->face_map.insert(FaceMapPair(face_idx, face)); // store newly created face with actual Idx

					if (ori_face_idx != face_idx)  // store the newly created and duplicated independently
					{
						idxvert_opt idxface(face_idx, face); // store new face
						m_opt->face_mmap.insert(FaceMMapPair(ori_face_idx, idxface)); // but store key as a original
					}

					m_opt->count++; // increment index counter
				}

				// add the face indices...
				// face indexes topology per material type
				TriMapIt it = OutMapTri.find(mat_id);

				if (it != OutMapTri.end())
				{
					(*it).second->push_back(face_idx);
				}
				else
				{
					IdxType * idx_type = new IdxType;
					idx_type->push_back(face_idx);

					OutMapTri.insert(TriMapPair(mat_id, idx_type));
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::BuildMeshTopology(std::vector<m_mesh*> &OutVecMeshes, MatFaceMapType &InMapMatface, TriMapType &InMapTri)
{
	// material <-> triangle indexes topology data
	for (size_t IndexAdd = 0; IndexAdd < InMapTri.size(); ++IndexAdd)
	{
		OutVecMeshes.push_back(new m_mesh()); // allocate mesh pull
	}

	int count = 0; // mesh count

	TriMapIt it = InMapTri.begin(); // iterate through (mat <-> face indexes) topology map

	while (it != InMapTri.end())
	{
		m_mesh & msh = *OutVecMeshes[count];

		msh.num_faces = (*it).second->size() / 3;

		msh.material_id = (*it).first;

		msh.faces_idx = new unsigned int[msh.num_faces * 3];

		for (size_t i = 0; i < msh.num_faces * 3; i += 3) // fill mesh face indexes from list
		{
			int Idx0 = (*it).second->front();
			(*it).second->pop_front();

			int Idx1 = (*it).second->front();
			(*it).second->pop_front();

			int Idx2 = (*it).second->front();
			(*it).second->pop_front();

			msh.faces_idx[i+0] = Idx0;
			msh.faces_idx[i+1] = Idx1;
			msh.faces_idx[i+2] = Idx2;
		}		

		MatFaceMapIt it_mapfacemap = InMapMatface.find((*it).first); // get by mat_id

		assert(it_mapfacemap != InMapMatface.end());

		mesh_opt * m_opt = (*it_mapfacemap).second; // get corresponding mesh

		msh.num_vertices = m_opt->face_map.size();
		msh.vertices = new Vector[msh.num_vertices];
		msh.normals = new Vector[msh.num_vertices];
		msh.colors = new Vector4f[msh.num_vertices];

		bool * faceidx_cache = new bool[msh.num_vertices];
		memset(faceidx_cache, 0, msh.num_vertices * sizeof(bool));

		unsigned int texdim = 0;
		bool alloc_texture = false;

		for (size_t i = 0; i < msh.num_faces * 3; ++i)
		{
			unsigned int face_idx = msh.faces_idx[i]; // get index in stored mesh

			FaceMapIt it_face_map = m_opt->face_map.find(face_idx); // find corresponding in mesh

			assert(it_face_map != m_opt->face_map.end());

			vert_opt face = (*it_face_map).second; // get vertex

			msh.faces_idx[i] = face.face_idx; // get actual local mesh index

			if (faceidx_cache[face.face_idx] == false)
			{
				faceidx_cache[face.face_idx] = true;

				msh.vertices[face.face_idx] = Vector(face.v.x, face.v.y, face.v.z);

				msh.colors[face.face_idx].x = face.c.x;
				msh.colors[face.face_idx].y = face.c.y;
				msh.colors[face.face_idx].z = face.c.z;

				Vector N(face.n.x, face.n.y, face.n.z);
				N.normalize();
				msh.normals[face.face_idx] = N;

				// texture maps
				if (face.num_tmaps && alloc_texture == false)
				{
					alloc_texture = true;
					texdim = 2;
					msh.num_texcoord_sets = face.num_tmaps;
					msh.texcoord_sets = new m_texcoord_set[msh.num_texcoord_sets];

					for (size_t j = 0; j < face.num_tmaps; ++j)
					{
						msh.texcoord_sets[j].dim = texdim;
						msh.texcoord_sets[j].texcoords = new float[msh.num_vertices * texdim];
					}
				}

				for (size_t j = 0; j < face.num_tmaps; ++j)
				{
					msh.texcoord_sets[j].texcoords[face.face_idx * texdim] = face.tmaps[j].x;
					msh.texcoord_sets[j].texcoords[face.face_idx * texdim + 1] = face.tmaps[j].y;
				}
			}
		}

		delete [] faceidx_cache;
		++count;
		++it;
	}
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::ProcessNormals(MatFaceMapType &InMapMatface)
{
	for (std::vector<SPolygonFace>::iterator IterFace = m_Faces.begin(); IterFace != m_Faces.end(); ++IterFace)
	{
		const SPolygonFace *pFace = GetFace(IterFace -  m_Faces.begin());

		// iterate through triangles
		for (unsigned int vIndex = 0; vIndex < pFace->FaceIndexes.size(); vIndex += 3)
		{
			unsigned int smg_id = pFace->smg_id;
			unsigned int mat_id = pFace->mat_id;

			// lets sort by material and find the corresponding mesh_opt
			MatFaceMapIt it_matfacemap = InMapMatface.find(mat_id);
			assert(it_matfacemap != InMapMatface.end());

			mesh_opt * m_opt = (*it_matfacemap).second;

			unsigned int face0_idx = pFace->FaceIndexes[vIndex + 0];
			unsigned int face1_idx = pFace->FaceIndexes[vIndex + 1];
			unsigned int face2_idx = pFace->FaceIndexes[vIndex + 2];

			FaceMapIt it_face0_map = m_opt->face_map.find(face0_idx);
			assert(it_face0_map != m_opt->face_map.end());

			FaceMapIt it_face1_map = m_opt->face_map.find(face1_idx);
			assert(it_face1_map != m_opt->face_map.end());

			FaceMapIt it_face2_map = m_opt->face_map.find(face2_idx);
			assert(it_face2_map != m_opt->face_map.end());

			vert_opt & face0 = (*it_face0_map).second;
			vert_opt & face1 = (*it_face1_map).second;
			vert_opt & face2 = (*it_face2_map).second;

			Vector face_n;
			Vector edge0;
			Vector edge1;

			sub(edge0, face2.v, face0.v);
			sub(edge1, face1.v, face0.v);
			cross(face_n, edge1, edge0);

			float weight = length(face_n);

			if (weight > m_eps){
				scale(face_n, m_one / weight);
			}
			else{
				scale(face_n, float(0.001f));
			}

			if (face0.smg_id == 1/*smg_id*/){
				face0.n += face_n;
			}
			if (face1.smg_id == 1/*smg_id*/){
				face1.n += face_n;
			}
			if (face2.smg_id == 1/*smg_id*/){
				face2.n += face_n;
			}

			std::pair<FaceMMapIt,FaceMMapIt> pair_mmap = m_opt->face_mmap.equal_range(face0_idx);
			FaceMMapIt it_face_mmap = pair_mmap.first;
			while (it_face_mmap != pair_mmap.second)
			{
				idxvert_opt & idxface = (*it_face_mmap).second;
				if (idxface.face.smg_id == smg_id)
				{
					FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
					vert_opt & face = (*it_face_map).second;
					face.n += face_n;
				}
				++it_face_mmap;
			}

			pair_mmap = m_opt->face_mmap.equal_range(face1_idx);
			it_face_mmap = pair_mmap.first;
			while (it_face_mmap != pair_mmap.second)
			{
				idxvert_opt & idxface = (*it_face_mmap).second;
				if (idxface.face.smg_id == smg_id)
				{
					FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
					vert_opt & face = (*it_face_map).second;
					face.n += face_n;
				}
				++it_face_mmap;
			}

			pair_mmap = m_opt->face_mmap.equal_range(face2_idx);
			it_face_mmap = pair_mmap.first;
			while (it_face_mmap != pair_mmap.second)
			{
				idxvert_opt & idxface = (*it_face_mmap).second;
				if (idxface.face.smg_id == smg_id)
				{
					FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
					vert_opt & face = (*it_face_map).second;
					face.n += face_n;
				}
				++it_face_mmap;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void CPolygonMesh::DoRebuildMesh()
{
	if (m_pMeshComponent)
	{
		D3DMesh * dx_mesh = m_pMeshComponent->GetD3DMesh();

		dx_mesh->Clear(true);

		size_t MaxFaceIdx = GetMaxFaceIndex(); // get max persistent index value
		size_t FaceNum = GetNumberOfFaces();

		TriMapType      tri_map;     // <unsigned int, std::list<unsigned int>*>
		MatFaceMapType  matface_map; // int <-> material

		std::vector<m_mesh*> meshes;

		BuildFaceTopology(matface_map, tri_map, MaxFaceIdx);

		ProcessNormals(matface_map);

		BuildMeshTopology(meshes, matface_map, tri_map);

		for (size_t Index = 0; Index < meshes.size(); ++Index)
		{
			const SPolygonFace* pFace = GetFace(Index);

			m_mesh & msh = *meshes[Index];

			Vector * vertices = new Vector[msh.num_vertices];	// array of vertices
			Vector * normals = new Vector[msh.num_vertices];	// array of normals
			Vector4f * colors = new Vector4f[msh.num_vertices];	// array of colors (vertex colors)
			Vector2f * texcoords = new Vector2f[msh.num_vertices];
			unsigned int * Indices = new unsigned int[msh.num_faces * 3];

			for (unsigned int Index2 = 0; Index2 < msh.num_vertices; ++Index2)
			{
				vertices[Index2] = msh.vertices[Index2];
				colors[Index2] = Vector4f(127.f, 127.f, 0.f, 0.f);

				texcoords[Index2] = Vector2f(msh.texcoord_sets[0].texcoords[Index2 * 2],
											 msh.texcoord_sets[0].texcoords[Index2 * 2 + 1]);

				normals[Index2] = msh.normals[Index2];
			}

			int gIndex = 0;
			for (unsigned int Index2 = 0; Index2 < msh.num_faces * 3; ++Index2)
			{
				Indices[Index2] = msh.faces_idx[Index2];
			}

			unsigned int numDistinctBones = 0;
			std::vector<int> VecBoneRemapper;

			SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());

			SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
				(float*)msh.vertices,
				(float*)msh.normals,
				(float*)msh.colors,
				NULL,
				NULL,
				msh.faces_idx,
				msh.num_vertices,
				msh.num_faces,
				(float*)texcoords,
				1,
				false,
				NULL,
				0,
				VecBoneRemapper,
				numDistinctBones);

			BaseMaterial * pEffect =  msh.material_id > 0  ? m_VecMatInstances[msh.material_id - 1] : m_pMaterialEffect->pMaterialEffect;

			// [TEST]
			std::string sFilename;
			if (FindFile(msh.material_id == 1 ? "arab_tex.tga" : "arab_tex.tga", &sFilename)){
				m_VecMatInstances[msh.material_id - 1]->LoadDiffuseMap(sFilename.c_str(), false);
			}
			// [TEST]

			dx_mesh->AddMesh(SMeshEffectPair(SubMesh, pEffect));

			// finally set mesh parameters
			// UGLY need research for correct parameters set
			dx_mesh->SetCastShadows(m_pMeshComponent->GetCastShadows());
			dx_mesh->SetReciveShadows(m_pMeshComponent->GetReciveShadows());

			delete [] texcoords;
			delete [] vertices;
			delete [] normals;
			delete [] colors;
			delete [] Indices;
		}

		// delete meshes
		for (std::vector<m_mesh*>::iterator Iter = meshes.begin(); Iter != meshes.end(); ++Iter)
		{
			DELETEARRAY((*Iter)->vertices);
			DELETEARRAY((*Iter)->normals);
			DELETEARRAY((*Iter)->colors);

			DELETEARRAY((*Iter)->weights);
			DELETEARRAY((*Iter)->bone_idxs);

			DELETEARRAY((*Iter)->texcoord_sets);
			DELETEARRAY((*Iter)->faces_idx);

			DELETESINGLE(*Iter);
		}
	}
}

//----------------------------------------------------------------------------------------------
Vector CPolygonMesh::GetSelectionPivot() const
{
	Vector OutResult(0.f, 0.f, 0.f);

	Bounds3f Bounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX);

	for (std::map<int, SSelectInfo>::const_iterator Iter = m_MapSelectInfo.begin(); Iter != m_MapSelectInfo.end(); ++Iter)
	{
		if (const SPolygonFace* pFace = GetFace((*Iter).first))
		{
			for (unsigned int Index = 0; Index < (*Iter).second.PointIndexes.size(); ++Index)
			{
				const Vector &Point = m_Vertices[pFace->VertexIndexes[(*Iter).second.PointIndexes[Index]]]; // get trough local indexes
		
				// x
				if (Point.x < Bounds.bound_min.x){
					Bounds.bound_min.x = Point.x;
				}

				if (Point.x > Bounds.bound_max.x){
					Bounds.bound_max.x = Point.x;
				}

				// y
				if (Point.y < Bounds.bound_min.y){
					Bounds.bound_min.y = Point.y;
				}

				if (Point.y > Bounds.bound_max.y){
					Bounds.bound_max.y = Point.y;
				}
				// z
				if (Point.z < Bounds.bound_min.z){
					Bounds.bound_min.z = Point.z;
				}

				if (Point.z > Bounds.bound_max.z){
					Bounds.bound_max.z = Point.z;
				}
			}
		}
	}

	if (Bounds.IsValid()){
		OutResult = Bounds.bound_min + (Bounds.bound_max - Bounds.bound_min) * 0.5f;
	}

	return OutResult;
}
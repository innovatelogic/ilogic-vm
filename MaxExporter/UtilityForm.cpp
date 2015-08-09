#include "expstdafx.h"

UtilityForm* UtilityForm::pUtility = NULL;

static UtilityMAXClassDesc UtilityMAXDesc;

ClassDesc2* GetUtilityMAXDesc() { return &UtilityMAXDesc; }

extern bool	bExportSkeleton;
extern bool	bExportModel;
extern bool	bExportAnim;
extern bool bExportCollision;

//----------------------------------------------------------------------------------
BOOL CALLBACK UtilityFormDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_MESH, BM_SETCHECK, bExportSkeleton ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_SKELETON, BM_SETCHECK, bExportSkeleton ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_ANIM, BM_SETCHECK, bExportAnim ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_COLLISION, BM_SETCHECK, bExportCollision ? BST_CHECKED : BST_UNCHECKED, 0);
		break;

	case WM_DESTROY:
		break;

	case WM_COMMAND:
		{
			INT_PTR buttonId = LOWORD(wParam);
			
			if (buttonId == IDC_CHECK_EXPORT_MESH){
				bExportModel = (SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_MESH, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
			else if (buttonId == IDC_CHECK_EXPORT_SKELETON){
				bExportSkeleton = (SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_SKELETON, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
			else if (buttonId == IDC_CHECK_EXPORT_ANIM){
				bExportAnim = (SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_ANIM, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
			else if (buttonId == IDC_CHECK_EXPORT_COLLISION){
				bExportCollision = (SendDlgItemMessage(hWnd, IDC_CHECK_EXPORT_COLLISION, BM_GETCHECK, 0, 0) == BST_CHECKED);
			}
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		UtilityForm::pUtility->ip->RollupMouseMessage(hWnd, msg, wParam, lParam);
		break;

	default:
		return TRUE;
		break;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------
UtilityForm::UtilityForm()
{
	assert(pUtility == NULL);
	pUtility = this;
}

//----------------------------------------------------------------------------------
UtilityForm::~UtilityForm()
{

}

//----------------------------------------------------------------------------------
void UtilityForm::DeleteThis()
{
	pUtility = NULL;
}

//----------------------------------------------------------------------------------
void UtilityForm::BeginEditParams(Interface *_ip, IUtil *iu)
{
	this->ip = _ip;
	
	hPanel = ip->AddRollupPage(hInstance,
								MAKEINTRESOURCE(IDD_DIALOG_EXPORT_OPTIONS),
								UtilityFormDlgProc,
								_T("Export Options"),
								0);
	InvalidateRgn(hPanel, NULL, TRUE);
}

//----------------------------------------------------------------------------------
void UtilityForm::EndEditParams(Interface *_ip, IUtil *iu)
{
	this->ip = NULL;
	_ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}
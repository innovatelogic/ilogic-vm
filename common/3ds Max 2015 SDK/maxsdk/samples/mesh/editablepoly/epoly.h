/**********************************************************************
 *<
	FILE: EPoly.h

	DESCRIPTION:

	CREATED BY: Steve Anderson

	HISTORY:

 *>	Copyright (c) 1999, All Rights Reserved.
 **********************************************************************/

#ifndef __EPOLY__H
#define __EPOLY__H

#include "Max.h"
#include "IParamm2.h"
#include "evrouter.h"
#include "resource.h"
#include "resourceOverride.h"

TCHAR *GetString(int id);
#define BIGFLOAT 9999999.f

extern ClassDesc2 *GetPolySelectDesc();
extern ClassDesc2 *GetEditablePolyDesc();
extern ClassDesc2 *GetEditNormalsDesc();

extern HINSTANCE hInstance;
extern int enabled;

// Edge chamfer types
enum {
	EP_STANDARD_EDGE_CHAMFER = 0,
	EP_QUAD_EDGE_CHAMFER
};

// Edge chamfer smoothing types
enum {
	EP_EDGE_CHAMFER_SMOOTH_ALL = 0,		// Entire object
	EP_EDGE_CHAMFER_SMOOTH_CHAMFER		// Just the chamfers we created
};

// Polymesh selection toolbar icons - used in select and edit tools.
class PolySelImageHandler {
public:
	HIMAGELIST images, hPlusMinus;

	PolySelImageHandler () : images(NULL), hPlusMinus(NULL) { }
	~PolySelImageHandler () { if (images) ImageList_Destroy (images); if (hPlusMinus) ImageList_Destroy (hPlusMinus); }
	HIMAGELIST LoadImages ();
	HIMAGELIST LoadPlusMinus ();
};

extern PolySelImageHandler *GetPolySelImageHandler();
extern bool CheckNodeSelection (Interface *ip, INode *inode);

class RefmsgKillCounter {
private:
	friend class KillRefmsg;
	LONG	counter;

public:
	RefmsgKillCounter() : counter(-1) {}

	bool DistributeRefmsg() { return counter < 0; }
};

class KillRefmsg {
private:
	LONG&	counter;

public:
	KillRefmsg(RefmsgKillCounter& c) : counter(c.counter) { ++counter; }
	~KillRefmsg() { --counter; }
};

// Class used to track the "current" position of the EPoly popup dialogs
class EPolyPopupPosition {
	bool mPositionSet;
	int mPosition[4];
public:
	EPolyPopupPosition() : mPositionSet(false) { }
	bool GetPositionSet () { return mPositionSet; }
	void Scan (HWND hWnd);
	void Set (HWND hWnd);
};

// PolyEdUI.cpp functions:
bool GetCreateShapeName (Interface *ip, TSTR &name, bool &ccSmooth);
bool GetDetachObjectName (Interface *ip, TSTR &name, bool &elem, bool &clone);
BOOL GetCloneObjectName (Interface *ip, TSTR &name);

// For toggling shaded faces with soft selection
// Methods defined in PolyEdOps; used in both Edit Poly and Editable Poly.
class ToggleShadedRestore : public RestoreObj {
	INode *mpNode;
	bool mOldShowVertCol, mOldShadedVertCol, mNewShowVertCol;
	int mOldVertexColorType;

public:
	ToggleShadedRestore (INode *pNode, bool newShow);
	void Restore(int isUndo);
	void Redo();
	int Size() { return sizeof (void *) + 3*sizeof(bool) + sizeof(int); }
	TSTR Description() { return TSTR(_T("ToggleShadedRestore")); }
};

typedef struct GenSoftSelData_tag {
	BOOL useSoftSel; //whether soft selection is active
	BOOL useEdgeDist;
	int edgeIts;
	BOOL ignoreBack;
	float falloff, pinch, bubble;
	GenSoftSelData_tag() //a struct with a constructor :)
	{	useSoftSel=FALSE, useEdgeDist=FALSE, edgeIts=0,
		ignoreBack=FALSE, falloff=20.0f, pinch=bubble=0.0f; }
} GenSoftSelData;

#endif

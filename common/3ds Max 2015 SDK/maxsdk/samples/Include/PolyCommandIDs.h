//**************************************************************************/
// Copyright (c) 1998-2013 Autodesk, Inc.
// All rights reserved.
//
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
#pragma once
#include "../../include/cmdmode.h"

//! Command ID's for Editable Poly and Edit Poly Modifier
enum PolyCommandIDs
{
	CID_POLY_CREATEVERT              = CID_USER + 0x1000,
	CID_POLY_CREATEEDGE              = CID_USER + 0x1001,
	CID_POLY_CREATEFACE              = CID_USER + 0x1002,
	CID_POLY_DIVIDEEDGE              = CID_USER + 0x1018,
	CID_POLY_DIVIDEFACE              = CID_USER + 0x1019,
	CID_POLY_EXTRUDE                 = CID_USER + 0x1020,
	CID_POLY_POLYCHAMFER             = CID_USER + 0x1028,
	CID_POLY_BEVEL                   = CID_USER + 0x102C,
	CID_POLY_INSET                   = CID_USER + 0x102D,
	CID_POLY_OUTLINE_FACE            = CID_USER + 0x102E,
	CID_POLY_CUT                     = CID_USER + 0x1038,
	CID_POLY_QUICKSLICE              = CID_USER + 0x1050,
	CID_POLY_WELD                    = CID_USER + 0x1040,
	CID_POLY_EDITTRI                 = CID_USER + 0x1048, //!< Command Mode ID for edit-tri mode for Edit Poly or Editable Poly
	CID_POLY_EXTRUDE_VERTEX_OR_EDGE  = CID_USER + 0x104A,
	CID_POLY_TURNEDGE                = CID_USER + 0x104E, //!< Command Mode ID for turn-edge mode for Edit Poly or Editable Poly. Used in internal source code.
	CID_POLY_LIFT_FROM_EDGE          = CID_USER + 0x1058,
	CID_POLY_PICK_LIFT_EDGE          = CID_USER + 0x1057,
	CID_POLY_HINGE_FROM_EDGE         = CID_USER + 0x1058,
	CID_POLY_BRIDGE_BORDER           = CID_USER + 0x1060,
	CID_POLY_BRIDGE_POLYGON          = CID_USER + 0x1061,
	CID_POLY_PICK_BRIDGE_1           = CID_USER + 0x1062,
	CID_POLY_PICK_HINGE              = CID_USER + 0x105A, 
	CID_POLY_PICK_BRIDGE_2           = CID_USER + 0x1063,
	CID_POLY_BRIDGE_EDGE             = CID_USER + 0x1064
};



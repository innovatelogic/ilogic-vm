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

//! Command ID's for Editable Mesh and Edit Mesh Modifier
enum MeshCommandIDs
{
	CID_MESH_EXTRUDE    = CID_USER + 972,
	CID_MESH_CREATEVERT = CID_USER + 973,
	CID_MESH_BUILDFACE  = CID_USER + 975,
	CID_MESH_DIVIDEEDGE = CID_USER + 976,
	CID_MESH_TURNEDGE   = CID_USER + 977, //!< Command Mode ID for turn-edge mode for Edit Mesh or Editable Mesh. Used in internal source code.
	CID_MESH_WELDVERT   = CID_USER + 978,
	CID_MESH_DIVIDEFACE = CID_USER + 979,
	CID_MESH_CUTEDGE    = CID_USER + 980,
	CID_MESH_FLIPNORM   = CID_USER + 981,
	CID_MESH_BEVEL      = CID_USER + 982,
	CID_MESH_MCHAMFER   = CID_USER + 983  //!< Command Mode for Mesh Chamfer
};


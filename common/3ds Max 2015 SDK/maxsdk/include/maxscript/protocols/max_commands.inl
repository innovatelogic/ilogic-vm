/*	
 *		MAX_commands.h - defines all the invocable MAX UI commands - for the 'max' scripter construct
 *
 *			Copyright (c) John Wainwright 1996
 *
 */

#pragma once

#include "../../maxcom.h"
#include "../../buildver.h"

#pragma warning(push)
#pragma warning(disable:4100)
 /* the commands... these are the command codes defined in "maxcom.h".  they MUST be in
 * alphabetical order */

	_M("accel pan"),				MAXCOM_ACCEL_PAN,				0,
	_M("acthomegrid"),				MAXCOM_ACTHOMEGRID,				MR_R2,
	_M("activate grid object"),		MAXCOM_ACTGRIDOBJ ,				0,
	_M("activate home grid"),		MAXCOM_ACTHOMEGRID,				0,
	_M("adaptive persp grid"),		MAXCOM_ADAPTIVE_PERSP_GRID_TOGGLE,	0,
	_M("adaptive perspective grid toggle"),	MAXCOM_ADAPTIVE_PERSP_GRID_TOGGLE,	MR_R2,
	_M("align"),					MAXCOM_ALIGN,					0,
	_M("align camera"),				MAXCOM_TOOLS_ALIGNCAMERA,		0,
	_M("align normals"),			MAXCOM_ALIGNNORMALS,			0,
	_M("alignnormals"),				MAXCOM_ALIGNNORMALS,			MR_R2,
	_M("angle snap toggle"),		MAXCOM_ANGLE_SNAP_TOGGLE,		0,
	_M("apply ik"),					MAXCOM_APPLY_IK,				0,
#ifndef NO_TOOL_ARRAY	// russom - 10/22/01
	_M("array"),					MAXCOM_ARRAY,					0,
#endif
	_M("backface"),					MAXCOM_BACKFACE,				MR_R2,
	_M("backface cull toggle"),		MAXCOM_BACKFACE,				0,
	_M("background"),				MAXCOM_BACKGROUND,				MR_R2,
	_M("background display toggle"), MAXCOM_BACKGROUND,				0,
	_M("bind space warp mode"),		MAXCOM_BINDWSM,					0,
	_M("bindwsm"),					MAXCOM_BINDWSM,					MR_R2,
	_M("box mode"),					MAXCOM_BOX_MODE,				MR_R2,
	_M("box mode selected"),		MAXCOM_BOX_MODE,				0,
	_M("box mode toggle"),			MAXCOM_BOX_TOGGLE,				0,
	_M("box toggle"),				MAXCOM_BOX_TOGGLE,				MR_R2,
	_M("configure paths"),			MAXCOM_CONFIGURE_PATHS,			0,
#ifndef NO_CREATE_TASK	// russom - 12/06/01
	_M("create mode"),				MAXCOM_CREATE_MODE,				0,
#endif
#ifndef NO_CUI	// russom - 02/12/02
  _M("customize TP"),				MAXCOM_CUSTOMIZE_CUSTOMIZETP,	0,
	_M("customize UI"),				MAXCOM_CUSTOMIZE_CUSTOMIZEUI,	0,
#endif
	_M("cycle select"),				MAXCOM_CYCLE_SELECT_METHOD ,	0,
	_M("cycle sublevel"),			MAXCOM_CYCLE_SUBLEVEL,			MR_R2,
	_M("cycle subobject level"),	MAXCOM_CYCLE_SUBLEVEL,			0,
	_M("def lgt toggle"),			MAXCOM_DEF_LGT_TOGGLE,			MR_R2,
	_M("default lighting toggle"),	MAXCOM_DEF_LGT_TOGGLE,			0,
	_M("delete"),					MAXCOM_EDIT_DELETE ,			0,
#ifndef NO_TOOL_DISPLAY_FLOATER	// russom - 10/22/01
	_M("display floater"),			MAXCOM_TOOLS_DISPLAYFLOATER,	0,
#endif
#ifndef NO_DISPLAY_TASK	// russom - 12/06/01
	_M("display mode"),				MAXCOM_DISPLAY_MODE,			0,
#endif
	_M("dolly"),					MAXCOM_DOLLY,					MR_R2,
	_M("dolly mode"),				MAXCOM_DOLLY,					0,
	_M("drawing aids"),				MAXCOM_DRAWINGAIDS,				0,
	_M("drawingaids"),				MAXCOM_DRAWINGAIDS,				MR_R2,
	_M("fetch"),					MAXCOM_FETCH,					0,
	_M("file archive"),				MAXCOM_FILE_ARCHIVE,			0,
	_M("file export"),				MAXCOM_FILE_EXPORT,				0,
	_M("file export selected"),		MAXCOM_FILE_EXPORTSELECTED,		0,
	_M("file import"),				MAXCOM_FILE_IMPORT,				0,
	_M("file insert tracks"),		MAXCOM_FILE_INSERTTRACKS,		0,
#ifndef NO_LOAD_SAVE_ANIMATION
	_M("file load animation"),		MAXCOM_FILE_LOADXMLANIMATION,	0,
#endif //NO_LOAD_SAVE_ANIMATION
	_M("file merge"),				MAXCOM_FILE_MERGE,				0,
	_M("file new"),					MAXCOM_FILE_NEW,				0,
	_M("file open"),				MAXCOM_FILE_OPEN,				0,
	_M("file preferences"),			MAXCOM_FILE_PREFERENCES,		0,
	_M("file preferences gamma"),	MAXCOM_API_GAMMA_PREF,			0,
	_M("file replace"),				MAXCOM_FILE_REPLACE,			0,
	_M("file save"),				MAXCOM_FILE_SAVE,				0,
#ifndef NO_LOAD_SAVE_ANIMATION
	_M("file save animation"),		MAXCOM_FILE_SAVEXMLANIMATION,	0,
#endif //NO_LOAD_SAVE_ANIMATION
	_M("file save selected"),		MAXCOM_FILE_SAVESELECTED,		0,
	_M("file saveas"),				MAXCOM_FILE_SAVEAS,				0,
	_M("file savecopyas"),				MAXCOM_FILE_SAVECOPYAS,				0,
	_M("file summary"),				MAXCOM_FILE_SUMMARYINFO,		0,
	_M("file xref object"),			MAXCOM_FILE_XREFOBJECT,			0,
	_M("file xref scene"),			MAXCOM_FILE_XREFSCENE,			0,
	_M("fov"),						MAXCOM_FOV,	0,
	_M("freeze inv"),				MAXCOM_FREEZE_INV,	0,
	_M("freeze selection"),			MAXCOM_FREEZE_SELECTION,	0,
	_M("fullinteract"),				MAXCOM_FULLINTERACT,	0,
	_M("grid nudge down"),			MAXCOM_GRID_NUDGE_DOWN,	0,
	_M("grid nudge up"),			MAXCOM_GRID_NUDGE_UP,	0,
	_M("grid toggle"),				MAXCOM_GRID_TOGGLE,	0,
	_M("grids align"),				MAXCOM_GRIDS_ALIGN,	0,
	_M("group attach"),				MAXCOM_GROUP_ATTACH,	0,
	_M("group close"),				MAXCOM_GROUP_CLOSE,	0,
	_M("group detach"),				MAXCOM_GROUP_DETACH,	0,
	_M("group explode"),			MAXCOM_GROUP_EXPLODE,	0,
	_M("group group"),				MAXCOM_GROUP_GROUP,	0,
	_M("group open"),				MAXCOM_GROUP_OPEN,	0,
	_M("group ungroup"),			MAXCOM_GROUP_UNGROUP,	0,
	_M("help about"),				MAXCOM_HELP_ABOUT,	0,
	_M("hide bone object toggle"),	MAXCOM_HIDE_BONEOBJECT_TOGGLE,	0,
	_M("hide camera toggle"),		MAXCOM_HIDE_CAMERA_TOGGLE,	0,
	_M("hide command panel toggle"), MAXCOM_HIDE_CMD_PAN,	0,
	_M("hide floating toolbars toggle"), MAXCOM_HIDE_FLOATERS,	0,
	_M("hide helper toggle"),		MAXCOM_HIDE_HELPER_TOGGLE,	0,
	_M("hide inv"),					MAXCOM_HIDE_INV,	0,
	_M("hide light toggle"),		MAXCOM_HIDE_LIGHT_TOGGLE,	0,
#ifndef NO_STANDARD_TOOLBAR_OPTIONS	// russom - 02/12/02
	_M("hide main toolbar toggle"),	MAXCOM_HIDE_MAIN_TB,	0,
#endif
	_M("hide object toggle"),		MAXCOM_HIDE_OBJECT_TOGGLE,	0,
	_M("hide selection"),			MAXCOM_HIDE_SELECTION,	0,
	_M("hide shape toggle"),		MAXCOM_HIDE_SHAPE_TOGGLE,	0,
	_M("hide system toggle"),		MAXCOM_HIDE_SYSTEM_TOGGLE,	0,
#if !defined(NO_STANDARD_TOOLBAR_OPTIONS) && !defined(NO_CUI_TABBED_PANELS)	// russom - 02/12/02
	_M("hide tab panel toggle"),	MAXCOM_HIDE_SHELF,	0,
#endif
	_M("hide wsm toggle"),			MAXCOM_HIDE_WSM_TOGGLE,	0,
#ifndef NO_HIERARCHY_TASK  // JP Morel - July 17th 2002
	_M("hierarchy mode"),			MAXCOM_HIERARCHY_MODE,	0,
#endif
	_M("hold"),						MAXCOM_HOLD,	0,
	_M("ik terminator"),			MAXCOM_IK_TERMINATOR,	0,
	_M("ipan"),						MAXCOM_IPAN,	0,
	_M("izoom in"),					MAXCOM_IZOOM_IN,	0,
	_M("izoom out"),				MAXCOM_IZOOM_OUT,	0,
	_M("key mode"),					MAXCOM_KEY_MODE,	0,
	_M("link"),						MAXCOM_LINK,	0,
#ifndef NO_CUI	// russom - 02/12/02
	_M("load custom UI"),			MAXCOM_CUSTOMIZE_LOADCUI,	0,
#endif
	_M("lock UI layout"),			MAXCOM_CUSTOMIZE_LOCKUILAYOUT,	0,
	_M("material browser"),			MAXCOM_TOOLS_MTLMAPBROWSER,	0,
	_M("mirror"),					MAXCOM_MIRROR,	0,
	_M("modify mode"),				MAXCOM_MODIFY_MODE,	0,
#ifndef NO_MOTION_TASK  // JP Morel - July 12th 2002
	_M("motion mode"),				MAXCOM_MOTION_MODE,	0,
#endif
	_M("move"),						MAXCOM_MOVE,	0,
	_M("mtledit"),		 			MAXCOM_MTLEDIT,	0,
	_M("next mod"),					MAXCOM_NEXT_MOD,	0,
	_M("orbit"),				    MAXCOM_ROTATEVIEW,	MR_R2,
	_M("override"),					MAXCOM_OVERRIDE,	0,
	_M("pancamera"),				MAXCOM_PANCAMERA,	0,
	_M("panview"),					MAXCOM_PANVIEW,	0,
	_M("percent snap toggle"),		MAXCOM_PERCENT_SNAP_TOGGLE,	0,
	_M("persp"),					MAXCOM_PERSP,	0,
	_M("place"),						MAXCOM_PLACE,	0,
	_M("place highlight"),			MAXCOM_EDIT_PLACEHIGHLIGHT,	0,
	_M("prev mod"),					MAXCOM_PREV_MOD,	0,
	_M("preview"),					MAXCOM_PREVIEW,	0,
	_M("properties"),				MAXCOM_PROPERTIES,	0,
	_M("quick render"),				MAXCOM_QUICK_RENDER,	0,
	_M("redo"),						MAXCOM_EDIT_REDO ,	0,
	_M("renamepreview"),			MAXCOM_RENAMEPREVIEW,	0,
	_M("render last"),				MAXCOM_RENDER_LAST,	0,
	_M("render scene"),				MAXCOM_RENDER_SCENE,	0,
	_M("reset file"),				MAXCOM_RESET_FILE,	0,
	_M("revert custom UI"),			MAXCOM_CUSTOMIZE_REVERTCUI,	0,
	_M("rns"),						MAXCOM_RNS,	0,
	_M("roll"),						MAXCOM_ROLL,	0,
	_M("rotate"),					MAXCOM_ROTATE,	0,
	_M("rotateview"),				MAXCOM_ROTATEVIEW,	0,
#ifndef NO_SAFE_FRAMES // orb -7-20-2001
	_M("safeframe toggle"),			MAXCOM_SAFEFRAME_TOGGLE,	0,
#endif // NO_SAFE_FRAMES
#ifndef NO_CUI	// russom - 02/12/02
	_M("save custom UI as"),		MAXCOM_CUSTOMIZE_SAVECUIAS,	0,
#endif
	_M("saveplus"),					MAXCOM_SAVEPLUS,	0,
	_M("scale"),					MAXCOM_SCALE,	0,
	_M("scale cycle"),				MAXCOM_SCALE_CYCLE,	0,
	_M("select"),					MAXCOM_SELECT,	0,
	_M("select all"),				MAXCOM_EDIT_SELECTALL ,	0,
	_M("select by color"),			MAXCOM_SELECT_BY_COLOR,	0,
	_M("select child"),				MAXCOM_SELECT_CHILD,	0,
	_M("select invert"),			MAXCOM_EDIT_SELECTINVERT ,	0,
	_M("select next sibling"),		MAXCOM_SELECT_NEXTSIBLING ,	0,
	_M("select none"),				MAXCOM_EDIT_SELECTNONE ,	0,
	_M("select parent"),			MAXCOM_SELECT_PARENT,	0,
	_M("select previous sibling"),	MAXCOM_SELECT_PREVSIBLING ,	0,
	_M("selection floater"),		MAXCOM_TOOLS_SELECTIONFLOATER,	0,

	_M("set key all"),									MAXCOM_TOOL_SETKEY_HOTKEYALL,	0,
	_M("set key all filter"),							MAXCOM_TOOL_SETKEY_ALL,	0,
	_M("set key all position"),							MAXCOM_TOOL_SETKEY_HOTKEYPOS,	0,
	_M("set key all position rotation and scale"),		MAXCOM_TOOL_SETKEY_HOTKEYPRS,	0,
	_M("set key all postion"),							MAXCOM_TOOL_SETKEY_HOTKEYPOS,	0,
	_M("set key all postion rotation and scale"),		MAXCOM_TOOL_SETKEY_HOTKEYPRS,	0,
	_M("set key all rotation"),							MAXCOM_TOOL_SETKEY_HOTKEYROT,	0,
	_M("set key all scale"),							MAXCOM_TOOL_SETKEY_HOTKEYSCALE,	0,
	_M("set key all x position"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSX,	0,
	_M("set key all x postion"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSX,	0,
	_M("set key all x rotation"),						MAXCOM_TOOL_SETKEY_HOTKEYROTX,	0,
	_M("set key all x scale"),							MAXCOM_TOOL_SETKEY_HOTKEYSCALEX,	0,
	_M("set key all y position"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSY,	0,
	_M("set key all y postion"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSY,	0,
	_M("set key all y rotation"),						MAXCOM_TOOL_SETKEY_HOTKEYROTY,	0,
	_M("set key all y scale"),							MAXCOM_TOOL_SETKEY_HOTKEYSCALEY,	0,
	_M("set key all z position"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSZ,	0,
	_M("set key all z postion"),						MAXCOM_TOOL_SETKEY_HOTKEYPOSZ,	0,
	_M("set key all z rotation"),						MAXCOM_TOOL_SETKEY_HOTKEYROTZ,	0,
	_M("set key all z scale"),							MAXCOM_TOOL_SETKEY_HOTKEYSCALEZ,	0,
	_M("set key clear"),								MAXCOM_TOOL_SETKEY_REVERT,	0,
	_M("set key customattributes filter"),				MAXCOM_TOOL_SETKEY_CUSTOMATTRIBUTES,	0,
	_M("set key ikparams filter"),						MAXCOM_TOOL_SETKEY_IK_PARAMS,	0,
	_M("set key keys"),									MAXCOM_TOOL_SETKEY,	0,
	_M("set key materials filter"),						MAXCOM_TOOL_SETKEY_MATERIALS,	0,
	_M("set key mode"),									MAXCOM_TOOL_SETKEYMODE,	0,
	_M("set key modifiers filter"),						MAXCOM_TOOL_SETKEY_MODIFIERS,	0,
	_M("set key objectparams filter"),					MAXCOM_TOOL_SETKEY_OBJECTPARAMS,	0,
	_M("set key on selected"),							MAXCOM_TOOL_SETKEY_SELECTED,	0,
	_M("set key other filter"),							MAXCOM_TOOL_SETKEY_OTHER,	0,	
	_M("set key position filter"),						MAXCOM_TOOL_SETKEY_POSITION,	0,
	_M("set key rotation filter"),						MAXCOM_TOOL_SETKEY_ROTATION,	0,
	_M("set key scale filter"),							MAXCOM_TOOL_SETKEY_SCALE,	0,	

  _M("shade selected"),			MAXCOM_SHADE_SELECTED,	0,
	_M("show last img"),			MAXCOM_SHOW_LAST_IMG,	0,
	_M("show statistics toggle"),	MAXCOM_VPT_STATS_TOGGLE,	0,
	_M("show vfb"),					MAXCOM_SHOW_VFB,	0,
	_M("showaxisicon"),				MAXCOM_SHOWAXISICON,	0,
	_M("showhomegrid"),				MAXCOM_SHOWHOMEGRID,	0,
	_M("snap to frozen object"),	MAXCOM_SNAP_SNAP_TO_FROZEN_OBJ,	0,
	_M("snap toggle"),				MAXCOM_SNAP_TOGGLE,	0,
	_M("snapshot"),					MAXCOM_EDIT_SNAPSHOT,	0,
	_M("spacebar"),					MAXCOM_SPACEBAR,	0,
	_M("spacing tool"),				MAXCOM_TOOLS_SPACINGTOOL,	0,
	_M("spinsnap toggle"),			MAXCOM_SPINSNAP_TOGGLE,	0,
	_M("subobject sel"),			MAXCOM_SUBOBJECT_SEL,	0,
	_M("swap layouts"),				MAXCOM_SWAP_LAYOUTS,	0,
	_M("texture correct"),			MAXCOM_TEXTURE_CORRECT,	0,
	_M("time back"),				MAXCOM_TIME_BACK,	MR_DISABLED,
	_M("time config"),				MAXCOM_TIME_CONFIG,	0,
	_M("time end"),					MAXCOM_TIME_END,	0,
	_M("time forward"),				MAXCOM_TIME_FORWARD,	MR_DISABLED,
	_M("time play"),				MAXCOM_TIME_PLAY,	0,
	_M("time start"),				MAXCOM_TIME_START,	0,
	_M("toggle ik"),				MAXCOM_TOGGLE_IK,	0,
	_M("toggle key mode"),			MAXCOM_KEYMODE_TOGGLE,	0,
	_M("toggle keyboard shortcuts"),MAXCOM_KBDSHORTCUT_TOGGLE,	0,
	_M("toggle sound"),				MAXCOM_TOGGLE_SOUND,	0,
	_M("tool animmode"),			MAXCOM_TOOL_ANIMMODE,	0,
	_M("tool center"),				MAXCOM_TOOL_CENTER,	0,
	_M("tool dualplanes"),			MAXCOM_TOOL_DUALPLANES,	0,
	_M("tool hlist"),				MAXCOM_TOOL_HLIST,	0,
	_M("tool maximize"),			MAXCOM_TOOL_MAXIMIZE,	0,
	_M("tool region toggle"),		MAXCOM_TOOL_REGION_TOGGLE,	0,
	_M("tool walkthrough"),			MAXCOM_TOOL_WALKTHROUGH, 0,
	_M("tool x"),					MAXCOM_TOOL_X,	0,
	_M("tool xy"),					MAXCOM_TOOL_XY,	0,
	_M("tool y"),					MAXCOM_TOOL_Y,	0,
	_M("tool z"),					MAXCOM_TOOL_Z,	0,
	_M("tool zoom"),				MAXCOM_TOOL_ZOOM,	0,
	_M("tool zoomall"),				MAXCOM_TOOL_ZOOMALL,	0,
	_M("tool zoomextents"),			MAXCOM_TOOL_ZOOMEXTENTS,	0,
	_M("tool zoomextents all"),		MAXCOM_TOOL_ZOOMEXTENTS_ALL,	0,
	_M("tool zoomregion"),			MAXCOM_TOOL_ZOOMREGION,	0,
	_M("trajectories"),				MAXCOM_TRAJECTORIES,	0,
	_M("treeview"),					MAXCOM_TREEVIEW,	0,
	_M("truck"),					MAXCOM_TRUCK,	0,
	_M("tti"),						MAXCOM_TTI,	0,
	_M("undo"),						MAXCOM_EDIT_UNDO ,	0,
	_M("unfreeze all"),				MAXCOM_UNFREEZE_ALL,	0,
	_M("unfreeze by hit"),			MAXCOM_UNFREEZE_BY_HIT,	0,
	_M("unfreeze by name"),			MAXCOM_UNFREEZE_BY_NAME,	0,
	_M("unhide all"),				MAXCOM_UNHIDE_ALL,	0,
	_M("unhide by name"),			MAXCOM_UNHIDE_BY_NAME,	0,
	_M("unitsetup"),				MAXCOM_UNITSETUP,	0,
	_M("unlink"),					MAXCOM_UNLINK,	0,
#ifndef NO_UTILITY_TASK	// russom - 12/06/01
	_M("utility mode"),				MAXCOM_UTILITY_MODE,	0,
#endif
	_M("videopost"),				MAXCOM_VIDEOPOST,	0,
	_M("view file"),				MAXCOM_VIEW_FILE,	0,
	_M("view redo"),				MAXCOM_VIEW_REDO,	0,
	_M("viewpreview"),				MAXCOM_VIEWPREVIEW,	0,
	_M("views redraw"),				MAXCOM_VIEWS_REDRAW,	0,
	_M("views undo"),				MAXCOM_VIEWS_UNDO,	0,
	_M("vpt back"),					MAXCOM_VPT_BACK,	0,
	_M("vpt bottom"),				MAXCOM_VPT_BOTTOM,	0,
	_M("vpt camera"),				MAXCOM_VPT_CAMERA,	0,
	_M("vpt disable"),				MAXCOM_VPT_DISABLE,	0,
	_M("vpt front"),				MAXCOM_VPT_FRONT,	0,
	_M("vpt grid"),					MAXCOM_VPT_GRID,	0,
	_M("vpt iso user"),				MAXCOM_VPT_ISO_USER,	0,
	_M("vpt left"),					MAXCOM_VPT_LEFT,	0,
	_M("vpt ortho user"),			MAXCOM_VPT_ISO_USER,	0,
	_M("vpt persp user"),			MAXCOM_VPT_PERSP_USER,	0,
	_M("vpt right"),				MAXCOM_VPT_RIGHT,	0,
	_M("vpt shape"),				MAXCOM_VPT_SHAPE,	0,
	_M("vpt spot"),					MAXCOM_VPT_SPOT,	0,
	_M("vpt tab"),					MAXCOM_VPT_TAB,	0,
	_M("vpt top"),					MAXCOM_VPT_TOP,	0,
	_M("vpt track"),				MAXCOM_VPT_TRACK,	0,
	_M("vptconfig"),				MAXCOM_VPTCONFIG,	0,
	_M("wire facet"),				MAXCOM_WIRE_FACET,	0,
	_M("wire smooth"),				MAXCOM_WIRE_SMOOTH,	0,
	_M("zoom in 2x"),				MAXCOM_ZOOM_IN_2X,	0,
	_M("zoom in 2x all"),			MAXCOM_ZOOM_IN_2X_ALL,	0,
	_M("zoom out 2x"),				MAXCOM_ZOOM_OUT_2X,	0,
	_M("zoom out 2x all"),			MAXCOM_ZOOM_OUT_2X_ALL,	0,
	_M("zoomext sel"),				MAXCOM_ZOOMEXT_SEL,	0,
	_M("zoomext sel all"),			MAXCOM_ZOOMEXT_SEL_ALL,	0,

#pragma warning(pop)


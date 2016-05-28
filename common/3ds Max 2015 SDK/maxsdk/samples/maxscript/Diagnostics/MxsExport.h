
#pragma once

#ifdef MXSDEBUGCRT_EXPORTS
#	define ScripterExport __declspec( dllexport )
#else
#	define ScripterExport __declspec( dllimport )
#endif
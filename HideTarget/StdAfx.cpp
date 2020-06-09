#include "stdafx.h"

#if defined (UNICODE) || defined (_UNICODE)
#pragma comment(linker, "/ENTRY:wmainCRTStartup")
#else
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif // defined (UNICODE) || defined (_UNICODE)
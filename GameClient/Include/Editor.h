#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "GameEngine.h"

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"GameEngine_Debug.lib")

#else
#pragma comment(lib,"GameEngine.lib")

#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"GameEngine32_Debug.lib")

#else
#pragma comment(lib,"GameEngine32.lib")

#endif

#endif

typedef struct _tagTreeItem
{
	CString		strTag;
	HTREEITEM	hItem;
}TreeItem, * PTreeItem;
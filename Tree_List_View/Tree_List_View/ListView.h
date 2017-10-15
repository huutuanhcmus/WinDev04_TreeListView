#pragma once
#include"Tree_List_View.h"
#include"stdafx.h"
#include <Shellapi.h>

LPWSTR Convert(__int64 nSize);
void creatListView(HWND &hList);
void AddItemList(HWND &hList, WCHAR* link, WIN32_FIND_DATA da, int sttdong);
time_t ft_tm(const FILETIME& ft);
LPWSTR GetType(WIN32_FIND_DATA fd);
void AddListToTree(HWND &hList, LPWSTR parent);
void LoadCurSel(HWND &hList, WCHAR * path);

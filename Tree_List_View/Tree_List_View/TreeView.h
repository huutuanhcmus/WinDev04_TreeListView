#pragma once
#include"Tree_List_View.h"
#include"stdafx.h"

void RunFindDisk();
HTREEITEM CreatPC(HWND &hTree);
void CreatTreeViewFromHardDisk(HTREEITEM thispc, HWND &hTree);
HTREEITEM AddItemAfter(WCHAR *s, HTREEITEM a, WCHAR * ht, HWND &hTree);
void CreatTreeView(HWND &hTree);
void DuyetFolder(WCHAR* parent, HTREEITEM a, int flag, HWND &hTree);
int _ExpandEnvironmentStrings(LPTSTR pText);
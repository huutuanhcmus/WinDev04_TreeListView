
#include"stdafx.h"
#include"TreeView.h"

WCHAR odia[26][3];
int demodia;

int Disk[26] = { 0 };


void RunFindDisk() {
	demodia = 0;
	WCHAR s[26 * 4 + 1];
	GetLogicalDriveStrings(26 * 4 + 1, s);
	int dem = 0;
	do
	{
		WCHAR *tam = new WCHAR[3];
		wcsncpy(tam, s + dem, 2);
		tam[2] = 0;
		wcscpy(odia[demodia++], tam);
		dem = dem + 4;
	} while (s[dem] != 0);
}

HTREEITEM CreatPC(HWND &hTree) {
	TV_INSERTSTRUCT tv;
	tv.hInsertAfter = TVI_FIRST;
	tv.item.cchTextMax = 8;
	tv.item.iImage = 0;
	tv.item.iSelectedImage = 0;
	tv.item.lParam = (LPARAM)L"This PC";
	tv.item.pszText = L"This PC";
	tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;
	tv.item.cChildren = 1;
	tv.hParent = TVI_ROOT;
	return TreeView_InsertItem(hTree, &tv);
	//return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&tv);
}

HTREEITEM AddItemAfter(WCHAR *s, HTREEITEM a, WCHAR * ht, HWND &hTree) {
	TVINSERTSTRUCT tv;
	tv.hInsertAfter = a;
	tv.item.cchTextMax = 255;
	tv.item.iImage = 0;
	tv.item.iSelectedImage = 0;
	tv.item.lParam = (LPARAM)s;
	tv.item.pszText = ht;
	tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tv.hParent = a;
	return (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)&tv);
}

void CreatTreeViewFromHardDisk(HTREEITEM thispc, HWND &hTree)
{
	for (int i = 0; i < demodia; i++) {
		HTREEITEM temp = AddItemAfter(odia[i], thispc, odia[i], hTree);
		DuyetFolder(odia[i], temp, 1, hTree);
	}
}

void CreatTreeView(HWND &hTree)
{
	RunFindDisk();
	HTREEITEM thispc = CreatPC(hTree);
	CreatTreeViewFromHardDisk(thispc, hTree);
}

void DuyetFolder(WCHAR* parent, HTREEITEM a, int flag, HWND &hTree)
{
	WIN32_FIND_DATA fd;
	WCHAR folder[260];
	wsprintf(folder, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folder, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{

		do {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L".."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(fd.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, fd.cFileName);
					HTREEITEM temp = AddItemAfter(child, a, fd.cFileName, hTree);	
					if (flag == 1) {
						DuyetFolder(child, temp, 0, hTree);
					}
					/*WCHAR *child = new WCHAR[wcslen(parent) + wcslen(fd.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, fd.cFileName);
					AddItemAfter(child, a, fd.cFileName, hTree);*/
				}
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}


#include"stdafx.h"
#include"ListView.h"
#include"TreeView.h"
#include"time.h"

WCHAR odiaA[26][3] = { 0 };
int demodiaA;

void creatListView(HWND &hList) {
	demodiaA = 0;
	WCHAR s[26 * 4 + 1];
	GetLogicalDriveStrings(26 * 4 + 1, s);
	int dem = 0;
	do
	{
		WCHAR *tam = new WCHAR[3];
		wcsncpy(tam, s + dem, 2);
		tam[2] = 0;
		wcscpy(odiaA[demodiaA++], tam);
		dem = dem + 4;
	} while (s[dem] != 0);
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.pszText = L"Tên";
	col.cx = 200;
	ListView_InsertColumn(hList, 0, &col);
	col.pszText = L"Thời gian chỉnh sửa";
	col.cx = 150;
	ListView_InsertColumn(hList, 1, &col);
	col.pszText = L"Loại";
	col.cx = 150;
	ListView_InsertColumn(hList, 2, &col);
	col.pszText = L"Dung lượng";
	col.cx = 100;
	ListView_InsertColumn(hList, 3, &col);
	for (int i = 0; i < demodiaA; i++) {
		long long nSize = 0;
		SHGetDiskFreeSpaceEx(odiaA[i], NULL, (PULARGE_INTEGER)&nSize, NULL);
		LVITEM m;
		m.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		m.iSubItem = 0;
		m.pszText = odiaA[i];
		m.iImage = 0;
		m.lParam = (LPARAM)odiaA[i];
		m.cchTextMax = 2;
		m.iItem = i;
		ListView_InsertItem(hList, &m);
		ListView_SetItemText(hList, i, 1, L"");
		ListView_SetItemText(hList, i, 2, L"Local Disk");
		ListView_SetItemText(hList, i, 3, Convert(nSize));
		;
	}
}

LPWSTR Convert(__int64 nSize)
{
	int nType = 0;
	while (nSize >= 1048576) //
	{
		nSize /= 1024;
		++nType;
	}
	__int64 nRight;
	if (nSize >= 1024)
	{
		nRight = nSize % 1024;
		while (nRight > 99)
			nRight /= 10;
		nSize /= 1024;
		++nType;
	}
	else
		nRight = 0;
	TCHAR *buffer = new TCHAR[11];
	_itow_s(nSize, buffer, 11, 10);
	if (nRight != 0 && nType > 1)
	{
		wcscat(buffer, L".");
		TCHAR *right = new TCHAR[3];
		_itow_s(nRight, right, 3, 10);
		wcscat(buffer, right);
	}
	if (buffer[0] == L'0')
		return L"";
	switch (nType)
	{
	case 0://Bytes
		wcscat(buffer, L" bytes");
		break;
	case 1:
		wcscat(buffer, L" KB");
		break;
	case 2:
		wcscat(buffer, L" MB");
		break;
	case 3:
		wcscat(buffer, L" GB");
		break;
	case 4:
		wcscat(buffer, L" TB");
		break;
	}
	return buffer;
}

void AddItemList(HWND &hList, WCHAR* link, WIN32_FIND_DATA da, int sttdong)
{
	LVITEM m;
	m.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	m.iSubItem = 0;
	m.pszText = da.cFileName;
	m.iImage = 0;
	m.lParam = (LPARAM)link;
	m.cchTextMax = 100;
	m.iItem = sttdong;
	ListView_InsertItem(hList, &m);
	time_t baygio = ft_tm(da.ftLastWriteTime);
	tm *ltm = localtime(&baygio);
	WCHAR *tam = new WCHAR[20];
	wsprintf(tam, L"%d-%d-%d %d:%d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, 1 + ltm->tm_hour, 1 + ltm->tm_min);
	ListView_SetItemText(hList, sttdong, 1, tam);
	ListView_SetItemText(hList, sttdong, 2, GetType(da));
	ListView_SetItemText(hList, sttdong, 3, Convert(da.nFileSizeLow));
}

time_t ft_tm(const FILETIME& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

LPWSTR GetType(WIN32_FIND_DATA fd)
{
	if (fd.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
		return  L"Foder File";
	int nDotPos = wcsrchr(fd.cFileName, L'.') - fd.cFileName;
	int len = wcslen(fd.cFileName);
	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return L"Không biết";
	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;
	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi
	if (!wcsicmp(szExtension, L".htm") || !wcsicmp(szExtension, L".html"))
		return L"Web page";
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Không biết";
	}
	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Không biết";
	}
	RegCloseKey(hKey);
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Không biết";
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Không biết";
	}
	RegCloseKey(hKey);

	return pszPath;
}

void AddListToTree(HWND &hList, LPWSTR parent)
{
	ListView_DeleteAllItems(hList);
	WIN32_FIND_DATA fd;
	WCHAR folder[MAX_PATH];
	wsprintf(folder, L"%s\\*.*", parent);
	HANDLE hFind = FindFirstFile(folder, &fd);
	int dem = 0;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (!(fd.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN))
			{
				if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L".."))
				{
					WCHAR *child = new WCHAR[wcslen(parent) + wcslen(fd.cFileName) + 2];
					wsprintf(child, L"%s\\%s", parent, fd.cFileName);
					AddItemList(hList, child, fd, dem);
					dem++;
				}
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	
}

void LoadCurSel(HWND &hList, WCHAR * path)
{
	WIN32_FIND_DATA fd;
	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);
	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(hList);
		AddListToTree(hList, path);
	}
	else //Nếu là tập tin thì chạy nó
		ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWNORMAL);
}








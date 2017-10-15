// 1512635.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tree_List_View.h"
#include"TreeView.h"
#include"ListView.h"

#define MAX_LOADSTRING 100

// Global Variables:
HWND hwnd, hTree, hList;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//void DuyetFolder(WCHAR* parent, HTREEITEM a, int flag);
void AddItemList(WCHAR* link, WIN32_FIND_DATA da, int sttdong);
//WCHAR odia[26][3];
//int demodia;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TREE_LIST_VIEW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TREE_LIST_VIEW));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_TREE_LIST_VIEW));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TREE_LIST_VIEW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		InitCommonControls();    // make our tree control to work	
		hTree = CreateWindow(WC_TREEVIEW, TEXT("Tree View"), WS_VISIBLE | WS_CHILD | WS_VSCROLL | TVS_HASBUTTONS |
			WS_BORDER | TVS_HASLINES | TVS_LINESATROOT, 0, 0, 200, 500, hWnd, (HMENU)IDC_TREE1, hInst, NULL);
		hList = CreateWindow(WC_LISTVIEW, L"ListView", WS_CHILD | WS_VISIBLE |
			WS_BORDER | LVS_REPORT | LVS_REPORT | LVS_SINGLESEL | LVS_ICON | WS_VSCROLL
			, 200, 0, 600, 500, hWnd, (HMENU)IDC_LIST1, hInst, NULL);
		CreatTreeView(hTree);
		creatListView(hList);
		addIcon(hTree);
		addIcon(hList);
		
		break;
	case WM_NOTIFY:
	{
		NMHDR *lpnmh = (LPNMHDR)lParam;
		switch (lpnmh->idFrom)
		{
			case IDC_TREE1:
			{
				switch(((LPNMHDR)lParam)->code){
				case NM_DBLCLK:
				case TVN_ITEMEXPANDING:
				//if (((LPNMHDR)lParam)->code == NM_DBLCLK || ((LPNMHDR)lParam)->code == TVN_ITEMEXPANDING) {
					TV_ITEM tvi;
					WCHAR Text[128 + 1];
					HTREEITEM a = (HTREEITEM)TreeView_GetSelection(hTree);
					if (a == NULL)
						break;
					tvi.mask = TVIF_TEXT;
					tvi.pszText = Text;     // Text is the pointer to the text 
					tvi.cchTextMax = 256;   // size of text to retrieve.
					tvi.hItem = a;   // the selected item
					SendMessage(hTree, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi); // get the data!!	
					if (TreeView_GetChild(hTree, a) == false) 												   //ListView_DeleteAllItems(hList);
						DuyetFolder((LPWSTR)tvi.lParam, a, 1, hTree);
					else
						AddListToTree(hList, (LPWSTR)tvi.lParam);
					break;
				}
				break;
			}
			case IDC_LIST1:
			{
				if (((LPNMHDR)lParam)->code == NM_DBLCLK)
				{
					int ItemIndex = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
					LVITEM SelectedItem;
					SelectedItem.iItem = ItemIndex;
					ListView_GetItem(hList, (LVITEM*)&SelectedItem);
					LoadCurSel(hList, (LPWSTR)SelectedItem.lParam);
				}
				break;
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void addIcon(HWND &TreeView)
{
	//HICON hiconItem;     // Icon for list-view items.
						 // Image list for icon view.
	HIMAGELIST hSmall, hImageTree;   // Image list for other views.
	hImageTree = ImageList_Create(24, 24, ILC_COLOR24 | ILC_MASK, 1, 1);

	hSmall = ImageList_Create(16, 16, ILC_COLOR24 | ILC_MASK, 1, 1);

	// Add an icon to each image list.  
	HICON hiconItem1 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	ImageList_AddIcon(hSmall, hiconItem1);
	ImageList_AddIcon(hImageTree, hiconItem1);

	SendMessage(TreeView, TVM_SETIMAGELIST, 0, (LPARAM)hImageTree);
	ListView_SetImageList(TreeView, hSmall, LVSIL_SMALL);

	DestroyIcon(hiconItem1);
}
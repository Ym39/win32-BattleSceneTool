// Win32BattelSceneTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Win32BattelSceneTool.h"
#include"resource.h"
#define MAX_LOADSTRING 100

enum MonsterChangeMode{NONE,M1,M2,M3,M4};
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND menuDlg;
HWND g_MainHwnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

SIZE g_szWndSize;

CSceneInfo MainScene;
MonsterChangeMode mode;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MenuDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32BATTELSCENETOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32BATTELSCENETOOL));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32BATTELSCENETOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = g_MainHwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 670, 710, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int FileOpenDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter, char* strDefExt)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "읽을 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = strDefExt;

	if (GetOpenFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}

int FileSaveDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "저장할 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = "scene";

	if (GetSaveFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rtWnd = { 0,0,670,710 },rtDlg;
	static SIZE szDlgSize;

	static char strFilePath[MAX_PATH], strFileName[100];
	static char strFilter[] = "씬 파일(*.scene)\0*.scene\0";

    switch (message)
    {
	case WM_CREATE:
		menuDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MenuDlgProc);
		ShowWindow(menuDlg, SW_SHOW);
		AdjustWindowRect(&rtWnd, WS_OVERLAPPEDWINDOW, TRUE);
		GetWindowRect(menuDlg, &rtDlg);
		g_szWndSize.cx = rtWnd.right - rtWnd.left;
		g_szWndSize.cy = rtWnd.bottom - rtWnd.top;
		szDlgSize.cx = rtDlg.right - rtDlg.left;
		szDlgSize.cy = rtDlg.bottom - rtDlg.top;
		MoveWindow(hWnd, 100, 100, g_szWndSize.cx, g_szWndSize.cy, TRUE);
		MoveWindow(menuDlg, 100 + g_szWndSize.cx, 100, szDlgSize.cx, szDlgSize.cy, TRUE);
		
		MainScene.SceneInit(hInst);

		return FALSE;
	case WM_MOVE:
		GetWindowRect(hWnd, &rtWnd);
		MoveWindow(menuDlg, LOWORD(lParam) + 670, HIWORD(lParam)-50, szDlgSize.cx, szDlgSize.cy, TRUE);
		break;
	case WM_LBUTTONDOWN:
	{
		if (mode == NONE)
			break;
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		int result = MainScene.SelectRect_ChangeMonster(x, y);

		if (result == -1)
			break;
		else
		{
			switch (mode)
			{
			case M1:
				MainScene.SetMonsterType(result, 0);
				break;
			case M2:
				MainScene.SetMonsterType(result, 1);
				break;
			case M3:
				MainScene.SetMonsterType(result, 2);
				break;
			case M4:
				MainScene.SetMonsterType(result, 3);
				break;
			default:
				break;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_LOAD:
				if (FileOpenDilaog(hWnd, strFilePath, strFileName, strFilter, "scene") != -1)
				{
					MainScene.LoadSceneFile(strFilePath);
					InvalidateRect(hWnd, NULL, TRUE);
				}
                break;
            case ID_SAVE:
				if (FileSaveDilaog(hWnd, strFilePath, strFileName, strFilter) != -1)
				{
					MainScene.SaveSceneFile(strFileName);
				}
				break;
			case ID_EXIT:
				PostQuitMessage(0);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MainScene.BackGroundDraw(hdc);
		MainScene.SceneIndexDraw(hdc);
		MainScene.MonsterDraw(hdc);
		MainScene.GridDraw(hdc);
		EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK MenuDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static int CurMonsterType;
	switch (iMessage)
	{
	case WM_INITDIALOG:
		CurMonsterType = -1;
		mode = NONE;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_PREVBG:
			MainScene.PrevBackGround();
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			break;
		case IDC_BUTTON_NEXTBG:
			MainScene.NextBackGround();
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			break;
		case IDC_BUTTON_SETIDX:
		{
			int sceneIdx = GetDlgItemInt(hDlg, IDC_EDIT_SETIDX, NULL, TRUE);
			if (sceneIdx < 0)
				break;
			MainScene.SetSceneIndex(sceneIdx);
			InvalidateRect(g_MainHwnd, NULL, TRUE);
		}
			break;
		case IDC_BUTTON_ENEMYCOUNTSET:
		{
			int monsterCount = GetDlgItemInt(hDlg, IDC_EDIT_ENEMYCOUNT, NULL, TRUE);
			if (monsterCount < 1 || monsterCount>3)
				break;
			if (MainScene.GetIsBoss() == true)
			{
				MainScene.SwitchingIsBoss();
			}
			MainScene.SetMonsterCount(monsterCount);
			switch (monsterCount)
			{
			case 1:
				MainScene.SetGridRectPos(0, 270, 150, 370, 350);
				break;
			case 2:
				MainScene.SetGridRectPos(0, 200, 150, 300, 350);
				MainScene.SetGridRectPos(1, 400, 150, 500, 350);
				break;
			case 3:
				MainScene.SetGridRectPos(0, 70, 150, 170, 350);
				MainScene.SetGridRectPos(1, 270, 150, 370, 350);
				MainScene.SetGridRectPos(2, 470, 150, 570, 350);
				break;
			default:
				break;
			}
			InvalidateRect(g_MainHwnd, NULL, TRUE);
		}
			break;
		case IDC_BUTTON_SETBOSS:
			MainScene.SetMonsterCount(1);
			MainScene.SetGridRectPos(0, 200, 100, 450, 350);
			MainScene.SwitchingIsBoss();
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			break;
		case IDC_BUTTON_MONSTER1:
			mode = M1;
			break;
		case IDC_BUTTON_MONSTER2:
			mode = M2;
			break;
		case IDC_BUTTON_MONSTER3:
			mode = M3;
			break;
		case IDC_BUTTON_MONSTER4:
			mode = M4;
			break;
		}
		break;
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

/*****************************************************************************
  ��Ȩ���� (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  ��ϵͳ����������������԰�����������������������GNUͨ�ù������֤(GPL)����
  �޸Ļ����·��������֤�����ǵ�2�����µİ汾��

  �����������ϣ�����������棬�����ṩ�κ���ʽ�ĵ�������������ȷ�Ļ��������ģ�
  �����������ڿ����ۺ��ʺ��ض���;��������֤�����GNUͨ�ù������֤: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: egg.c,v 1.18 2005/11/02 03:47:49 sun Exp $ 
*****************************************************************************/

/**
 * Egg���ĳ���
 *
 * ���ļ���Egg�ĺ��ģ�����ϵͳ�ĳ�ʼ�������٣��Լ���Ϣѭ���Ⱥ�������
 */

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <tchar.h>
#include <math.h>
#include "..\egg.h"
#include "egg_priv.h"
#include "graphic.h"
#include "event.h"

//////////////////////////////////////////////////////////////////////////
//
// �����
//
//////////////////////////////////////////////////////////////////////////

/**
 * Egg�������� 
 *
 * Win32�ڽ�����ʱ��Ҫ�����Ķ�������Ȼ�������ô����󣨻����Ҳ�֪���ô���
 */
#define EGG_WINDOW_CLASS_NAME _T("Easy Graphical egG Window")


//////////////////////////////////////////////////////////////////////////
//
// ������Egg����Ч��ȫ�ֱ���
//
//////////////////////////////////////////////////////////////////////////

//!eggͼ�δ��ڵľ��
HWND hwEgg;

//////////////////////////////////////////////////////////////////////////
//
// �����ڴ��ļ�����Ч��ȫ�ֱ���
//
//////////////////////////////////////////////////////////////////////////

//!�ն˴��ڵľ��
static HWND hwConsole;

//!egg�Ƿ��Ѿ����ɹ���ʼ����
static BOOL bInitialized=FALSE;

//!x����ÿ���׵����ص����
static double fXResolution;

//!y����ÿ���׵����ص����
static double fYResolution;

//!�߳̾��
static HANDLE hThread;

//!����Client����С
static SIZE sizeClient;

//////////////////////////////////////////////////////////////////////////
//
// �������ڴ��ļ�����Ч��˽�к���
//
//////////////////////////////////////////////////////////////////////////

//��ʼ��
static HWND FindConsoleWindow(void);
static BOOL CALLBACK EnumWindowsProcById(HWND hwnd, LPARAM lParam);
static BOOL CALLBACK EnumWindowsProcByTitle(HWND hwnd, LPARAM lParam);
static HWND CreateEggWindow(void);
static BOOL InitEggWindow(double fWidth, double fHeight);
static void CalcResolution(void);
static void ChangeClientSizeTo(int nWidth, int nHeight);
static void EggEnd(void);

//����
static LRESULT CALLBACK EggWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static DWORD WINAPI EggThreadProc(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////////
//
// Egg API���壬�ɱ������������
//
//////////////////////////////////////////////////////////////////////////


void EggStart(double fWidth, double fHeight)
{
    HANDLE hEventEggThread;
    DWORD dwRtn;

    if (bInitialized)
    {
        return;
    }

    //��Ҫ�ҵ��ն˴��ڣ����ܿ�������Ҳ���ܰ�Egg�Ĵ�����Ϊ�����Ӵ��ڽ�������
    if ((hwConsole = FindConsoleWindow()) == NULL)
    {
        Sleep(1000);  //��һ���ӣ����ң������õ�һ��Ϊʲô�Ҳ���

        if ((hwConsole = FindConsoleWindow()) == NULL)
        {
            Error(_T("�ն˲����ˣ���Sunnerȥ��ԩ��"));
        }
    }

    hEventEggThread = CreateEvent(NULL, TRUE, FALSE, NULL);
    //����Egg���̣߳������������ڣ�������Ϣѭ��
    hThread = CreateThread(NULL, 0, EggThreadProc, &hEventEggThread, 0, NULL);
    if (hThread == NULL)
    {
        Error(_T("Egg�߳̽�����������ô���ܣ���rpwt��"));
    }

    //���̸߳㶨һ��
    dwRtn = WaitForSingleObject(hEventEggThread, INFINITE);
    if (dwRtn == WAIT_OBJECT_0 && !bInitialized)
    {
        Error(_T("Egg���ڽ���ʧ�ܣ���rpwt��"));
    }

    if (!InitEggWindow(fWidth, fHeight))
    {
        Error(_T("Egg��ʼ��ʧ�ܣ���rpwt��"));
    }

//    ShowConsoleWindow(FALSE);
    ShowWindow(hwEgg, SW_SHOW);
    UpdateWindow(hwEgg);

    //���¡�һ��������ı�����
    if (LayEgg() == NULL)
    {
        Error(_T("�������Ѳ��ˣ���rpwt��"));
    }

    atexit(EggEnd);     //�˳�ʱ���Զ��ͷ�Eggռ�õ���Դ
    CloseHandle(hEventEggThread);
}

void ShowConsoleWindow(int bShow)
{
    assert(hwConsole != NULL);
    ShowWindow(hwConsole, bShow ? SW_SHOW : SW_HIDE);
}

HWND GetEggHwnd(void)
{
    return hwEgg;
}

//////////////////////////////////////////////////////////////////////////
//
// Eggƽ̨�������壬��Eggƽ̨��ģ����Ե���
//
//////////////////////////////////////////////////////////////////////////

void Error(TCHAR* pszErrString)
{
    TCHAR msg[512];

    _stprintf(msg, "%s\nCode:%d", pszErrString, GetLastError());
    MessageBox(NULL, msg, EGG_WINDOW_CLASS_NAME, MB_ICONERROR);
    exit(0);
}

void InitCheck(void)
{
    if (!bInitialized)
    {
        Error(_T("�����ȳɹ�����EggStart()������ʹ��Egg���������ܡ�"));
    }
}

double X_P2CM(int x)
{
    return WidthP2CM(x);
}

double Y_P2CM(int y)
{
    return HeightP2CM(sizeClient.cy - y);
}

int X_CM2P(double x)
{
    return WidthCM2P(x);
}

int Y_CM2P(double y)
{
    return sizeClient.cy - HeightCM2P(y);
}

double WidthP2CM(int nWidth)
{
    assert(fabs(fXResolution - 0.0) >= 0.000001);

    return nWidth/fXResolution;
}

double HeightP2CM(int nHeight)
{
    assert(fabs(fYResolution - 0.0) >= 0.000001);

    return nHeight/fYResolution;
}

int HeightCM2P(double fHeight)
{
    assert(fabs(fYResolution - 0.0) >= 0.000001);

    return (int)floor(fHeight*fYResolution + 0.5);
}

int WidthCM2P(double fWidth)
{
    assert(fabs(fXResolution - 0.0) >= 0.000001);

    return (int)floor(fWidth*fXResolution + 0.5);
}

//////////////////////////////////////////////////////////////////////////
//
// ˽�к������壬���ڴ��ļ�����Ч
//
//////////////////////////////////////////////////////////////////////////

/**
 * ֹͣʹ��Egg
 *
 * �ر�Egg���ڣ��ͷ�Eggռ�õ�������Դ���Ӵ�Egg�����й��ܶ�������ʹ�á�
 *
 * @see EggStart()
 */
static void EggEnd(void)
{
    bInitialized = FALSE;
    
    CleanEggs();
    if (hwEgg)
        DestroyWindow(hwEgg);
}

/**
 * �����ն˴��ڵľ��
 *
 * �ն˴��ھ����Ǹ��ںڵ��ַ����ڡ�<br>
 * ���ý���ID���ң����Ҳ��������ô��ڱ�����ҡ��������еĳ����ý���ID���Ծ�ȷ��
 * ������IDE(VS6.0 sp6)���еĳ���ֻ��ͨ�������ҵ���ͬһ���������ж��ʱ��������
 * ���ҿ��ܳ����⡣
 * <b>Ŀǰ�޷��ҵ������������еĳ�����ն˴��ڡ�</b>
 *
 * @return �ն˴��ھ������û�ҵ�������NULL��
 */
static HWND FindConsoleWindow(void)
{
    HWND hwnd=NULL;
    //��ʼͨ������ID����
    EnumWindows(EnumWindowsProcById, (LPARAM)&hwnd);

    if (hwnd == NULL)  //ͨ��IDû�ҵ�
    {
        //��ͨ������������
        EnumWindows(EnumWindowsProcByTitle, (LPARAM)&hwnd);
    }

    return hwnd;
}

/**
 * Ϊ�õ��ն˴��ھ������Ļص�����
 *
 * �˺������ܱ�ֱ�ӵ��ã�������Ϊ�����ṩ��EnumWindows()��ͨ���ȽϽ���ID
 * �Ƿ�һ��Ѱ���ն˴��ڡ����ҵ����ն˴��ھ��д��lParamָ����ڴ档<br>
 * �μ�msdn�е�EnumWindowsProc()��
 * @see FindConsoleWindow()
 * @see EnumWindowsProcByTitle()
 */
static BOOL CALLBACK EnumWindowsProcById(HWND hwnd, LPARAM lParam)
{
    DWORD dwPID = 0;                    //����hwnd���ڽ��̵�id
    DWORD dwCurrentPID;                 //��ǰ����id
    HWND* phwConsole = (HWND*)lParam;   //ָ��hwConsole��ָ��

    dwCurrentPID = GetCurrentProcessId();
    
    GetWindowThreadProcessId(hwnd, &dwPID);

    if (dwPID == dwCurrentPID)  //������ȣ�˵���ҵ���
    {
        *phwConsole = hwnd;
        return FALSE;   //����������
    }
    else
    {
        return TRUE;    //��������
    }
}

/**
 * Ϊ�õ��ն˴��ھ������Ļص�����
 *
 * �˺������ܱ�ֱ�ӵ��ã�������Ϊ�����ṩ��EnumWindows()��ͨ���Ƚϳ����ļ���
 * �ʹ������Ƿ�һ��Ѱ���ն˴��ڡ����ҵ����ն˴��ھ��д��lParamָ����ڴ档<br>
 * �μ�msdn�е�EnumWindowsProc()��
 * @see FindConsoleWindow()
 * @see EnumWindowsProcById()
 */
static BOOL CALLBACK EnumWindowsProcByTitle(HWND hwnd, LPARAM lParam)
{
    TCHAR szFilePath[FILENAME_MAX]="";  //��ǰִ�еĳ���ȫ·��
    TCHAR szTitle[FILENAME_MAX]="";     //hwnd���ڵı���
    HWND* phwConsole = (HWND*)lParam;   //ָ��hwConsole��ָ��
    
    GetModuleFileName(NULL, szFilePath, FILENAME_MAX-1);
    GetWindowText(hwnd, szTitle, FILENAME_MAX-1);

    if (_tcsstr(szTitle, szFilePath) != 0)  //�ҵ���
    {
        *phwConsole = hwnd;
        return FALSE;   //����������
    }
    else
    {
        return TRUE;    //��������
    }
}

/**
 * ��ʼ��egg����
 *
 * ����egg���ڵĸ��ֳ�ʼ��������������Ϊegg׼����hdcMem����������ϵ���Դ�������
 * Ϊԭ�㣬��λ�����׵�ƽ��ֱ������ϵ��
 * 
 * @param fWidth ������Ч����Ŀ�ȣ����ף�
 * @param fHeight ������Ч����ĸ߶ȣ����ף�
 * @return �Ƿ�ɹ���
 */
static BOOL InitEggWindow(double fWidth, double fHeight)
{
    CalcResolution();

    ChangeClientSizeTo(WidthCM2P(fWidth), HeightCM2P(fHeight));

    return MakePlate();
}

/**
 * ����eggͼ�δ���
 *
 * ����eggͼ�δ��ڡ����ڴ�С�Ȳ���ȱʡֵ�������ٶ�������С���˺����Ĵ���ժ��
 * <em>"Programming Windows��the 5th Edition" by Charles Petzold</em>�������޸�
 *
 * @return ���ھ��
 */
static HWND CreateEggWindow(void)
{
    HWND hwnd;
    WNDCLASS     wndclass;
    
    wndclass.style         = 0;
    wndclass.lpfnWndProc   = EggWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = GetModuleHandle(NULL);
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName  = NULL ;
    wndclass.lpszClassName = EGG_WINDOW_CLASS_NAME ;
    
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, _T("������ֻ����Windows NT��2000��XP��2003����������"),
            EGG_WINDOW_CLASS_NAME, MB_ICONERROR) ;
        return NULL;
    }
    hwnd = CreateWindow (
        EGG_WINDOW_CLASS_NAME,      // window class name
        _T("Easy Graphical egG"),   // window caption
        WS_OVERLAPPED | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,        // window style
        CW_USEDEFAULT,              // initial x position
        CW_USEDEFAULT,              // initial y position
        CW_USEDEFAULT,              // initial x size
        CW_USEDEFAULT,              // initial y size
        hwEgg,                      // parent window handle
        NULL,                       // window menu handle
        GetModuleHandle(NULL),      // program instance handle
        NULL);                      // creation parameters
    
    return hwnd;
}

/**
 * ��Ϣ�������� 
 *
 * 
 */
static LRESULT CALLBACK EggWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    EVENT ev={0,0};

    if (message == WM_PAINT)
    {
        DoUpdate();
        return 0;
    }
    else if(message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
    {
        switch(message)
        {
        case WM_MOUSEMOVE:
            ev.event = MOUSEMOVE;
        	break;
        case WM_LBUTTONDOWN:
            ev.event = KEYDOWN;
            ev.code = VK_LBUTTON;
            bVKDown[ev.code] = TRUE;
            break;
        case WM_RBUTTONDOWN:
            ev.event = KEYDOWN;
            ev.code = VK_RBUTTON;
            bVKDown[ev.code] = TRUE;
            break;
        case WM_MBUTTONDOWN:
            ev.event = KEYDOWN;
            ev.code = VK_MBUTTON;
            bVKDown[ev.code] = TRUE;
            break;
        case WM_LBUTTONUP:
            ev.event = KEYUP;
            ev.code = VK_LBUTTON;
            bVKDown[ev.code] = FALSE;
            break;
        case WM_RBUTTONUP:
            ev.event = KEYUP;
            ev.code = VK_RBUTTON;
            bVKDown[ev.code] = FALSE;
            break;
        case WM_MBUTTONUP:
            ev.event = KEYUP;
            ev.code = VK_MBUTTON;
            bVKDown[ev.code] = FALSE;
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        ptMouse.x = LOWORD(lParam);
        ptMouse.y = HIWORD(lParam);
        AddEvent(ev);
        return 0;
    }
    else if(message >= WM_KEYFIRST && message <= WM_KEYLAST)
    {
        switch(message)
        {
        case WM_KEYDOWN:
            if (!bVKDown[wParam]) 
            {
                bVKDown[wParam] = TRUE;
                ev.event = KEYDOWN;
                ev.code = (unsigned short)wParam;
                AddEvent(ev);
            }
            break;
        case WM_KEYUP:
            bVKDown[wParam] = FALSE;
            ev.event = KEYUP;
            ev.code = (unsigned short)wParam;
            AddEvent(ev);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
        }
        return 0;
    }
    else if (message == WM_TIMER)
    {
        ev.event = TIMER;
        AddEvent(ev);
        return 0;
    }
    else if (message == WM_DESTROY)
    {
        ev.event = EXIT;
        AddEvent(ev);
        PostQuitMessage(0);
        return 0 ;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 * �ı�Client����Ĵ�С 
 *
 * ����Ų������λ�á��´�С��������ȫ�ֱ���sizePlate��
 *
 * @param nWidth Client�����ȣ����أ�
 * @param nHeight Client����߶ȣ����أ�
 */
static void ChangeClientSizeTo(int nWidth, int nHeight)
{
    RECT rectWnd, rectClient;
    SIZE size;
    
    assert(hwEgg != NULL);

    size.cx = sizeClient.cx = nWidth;
    size.cy = sizeClient.cy = nHeight;

    GetWindowRect(hwEgg, &rectWnd);
    GetClientRect(hwEgg, &rectClient);
    
    //���㰴nWidth��nHeight���ú������ڵĴ�С
    size.cx += rectWnd.right - rectWnd.left - rectClient.right;
    size.cy += rectWnd.bottom - rectWnd.top - rectClient.bottom;

    MoveWindow(hwEgg, rectWnd.left, rectWnd.top, size.cx, size.cy, FALSE);
}

/**
 * ����ÿ�����ص���x��y�����Ϊ��������
 *
 * ������ֻ��Ҫִ��һ�Ρ����������fXResolution��fYResolution
 */
static void CalcResolution(void)
{
    HWND hwDesktop;
    HDC hdc;
    int i;

    hwDesktop = GetDesktopWindow();
    hdc = GetDC(hwDesktop);

    i = GetDeviceCaps(hdc, HORZRES);
    i = GetDeviceCaps(hdc, HORZSIZE);
    fXResolution = GetDeviceCaps(hdc, HORZRES) / (0.1 * GetDeviceCaps(hdc, HORZSIZE));
    fYResolution = GetDeviceCaps(hdc, VERTRES) / (0.1 * GetDeviceCaps(hdc, VERTSIZE));

    ReleaseDC(hwDesktop, hdc);
}

static DWORD WINAPI EggThreadProc(LPVOID lpParameter)
{
    MSG msg;
    HANDLE * pEvent = (HANDLE*)lpParameter;

    if ((hwEgg = CreateEggWindow()) == NULL)
    {
        SetEvent(*pEvent);
        return 0;
    }
    
    bInitialized = TRUE;
    SetEvent(*pEvent);

    SetActiveWindow(hwEgg);

    while (GetMessage(&msg, hwEgg, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

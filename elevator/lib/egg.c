/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: egg.c,v 1.18 2005/11/02 03:47:49 sun Exp $ 
*****************************************************************************/

/**
 * Egg核心程序
 *
 * 本文件是Egg的核心，处理系统的初始化和销毁，以及消息循环等核心问题
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
// 定义宏
//
//////////////////////////////////////////////////////////////////////////

/**
 * Egg窗口类名 
 *
 * Win32在建窗口时需要这样的东西，虽然它好像用处不大（或者我不知道用处）
 */
#define EGG_WINDOW_CLASS_NAME _T("Easy Graphical egG Window")


//////////////////////////////////////////////////////////////////////////
//
// 定义在Egg内有效的全局变量
//
//////////////////////////////////////////////////////////////////////////

//!egg图形窗口的句柄
HWND hwEgg;

//////////////////////////////////////////////////////////////////////////
//
// 定义在此文件内有效的全局变量
//
//////////////////////////////////////////////////////////////////////////

//!终端窗口的句柄
static HWND hwConsole;

//!egg是否已经被成功初始化过
static BOOL bInitialized=FALSE;

//!x方向每厘米的象素点个数
static double fXResolution;

//!y方向每厘米的象素点个数
static double fYResolution;

//!线程句柄
static HANDLE hThread;

//!窗口Client区大小
static SIZE sizeClient;

//////////////////////////////////////////////////////////////////////////
//
// 声明仅在此文件内有效的私有函数
//
//////////////////////////////////////////////////////////////////////////

//初始化
static HWND FindConsoleWindow(void);
static BOOL CALLBACK EnumWindowsProcById(HWND hwnd, LPARAM lParam);
static BOOL CALLBACK EnumWindowsProcByTitle(HWND hwnd, LPARAM lParam);
static HWND CreateEggWindow(void);
static BOOL InitEggWindow(double fWidth, double fHeight);
static void CalcResolution(void);
static void ChangeClientSizeTo(int nWidth, int nHeight);
static void EggEnd(void);

//核心
static LRESULT CALLBACK EggWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static DWORD WINAPI EggThreadProc(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////////
//
// Egg API定义，可被其它程序调用
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

    //先要找到终端窗口，才能控制它，也才能把Egg的窗口作为它的子窗口建立起来
    if ((hwConsole = FindConsoleWindow()) == NULL)
    {
        Sleep(1000);  //等一秒钟，再找！鬼晓得第一次为什么找不到

        if ((hwConsole = FindConsoleWindow()) == NULL)
        {
            Error(_T("终端不见了，向Sunner去喊冤！"));
        }
    }

    hEventEggThread = CreateEvent(NULL, TRUE, FALSE, NULL);
    //建立Egg的线程，由它建立窗口，处理消息循环
    hThread = CreateThread(NULL, 0, EggThreadProc, &hEventEggThread, 0, NULL);
    if (hThread == NULL)
    {
        Error(_T("Egg线程建不起来？怎么可能？有rpwt！"));
    }

    //等线程搞定一切
    dwRtn = WaitForSingleObject(hEventEggThread, INFINITE);
    if (dwRtn == WAIT_OBJECT_0 && !bInitialized)
    {
        Error(_T("Egg窗口建立失败，有rpwt！"));
    }

    if (!InitEggWindow(fWidth, fHeight))
    {
        Error(_T("Egg初始化失败，有rpwt！"));
    }

//    ShowConsoleWindow(FALSE);
    ShowWindow(hwEgg, SW_SHOW);
    UpdateWindow(hwEgg);

    //“下”一个最基础的背景蛋
    if (LayEgg() == NULL)
    {
        Error(_T("背景蛋难产了！有rpwt！"));
    }

    atexit(EggEnd);     //退出时，自动释放Egg占用的资源
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
// Egg平台函数定义，仅Egg平台的模块可以调用
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
        Error(_T("必须先成功调用EggStart()，才能使用Egg的其它功能。"));
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
// 私有函数定义，仅在此文件内有效
//
//////////////////////////////////////////////////////////////////////////

/**
 * 停止使用Egg
 *
 * 关闭Egg窗口，释放Egg占用的所有资源，从此Egg的所有功能都不能再使用。
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
 * 返回终端窗口的句柄
 *
 * 终端窗口就是那个黑黑的字符窗口。<br>
 * 先用进程ID查找，如找不到，再用窗口标题查找。单独运行的程序用进程ID可以精确找
 * 到，在IDE(VS6.0 sp6)运行的程序只能通过标题找到。同一个程序运行多次时，按标题
 * 查找可能出问题。
 * <b>目前无法找到从命令行运行的程序的终端窗口。</b>
 *
 * @return 终端窗口句柄。如没找到，返回NULL。
 */
static HWND FindConsoleWindow(void)
{
    HWND hwnd=NULL;
    //开始通过进程ID搜索
    EnumWindows(EnumWindowsProcById, (LPARAM)&hwnd);

    if (hwnd == NULL)  //通过ID没找到
    {
        //再通过程序名查找
        EnumWindows(EnumWindowsProcByTitle, (LPARAM)&hwnd);
    }

    return hwnd;
}

/**
 * 为得到终端窗口句柄而设的回调函数
 *
 * 此函数不能被直接调用，而是作为参数提供给EnumWindows()。通过比较进程ID
 * 是否一致寻找终端窗口。把找到的终端窗口句柄写到lParam指向的内存。<br>
 * 参见msdn中的EnumWindowsProc()。
 * @see FindConsoleWindow()
 * @see EnumWindowsProcByTitle()
 */
static BOOL CALLBACK EnumWindowsProcById(HWND hwnd, LPARAM lParam)
{
    DWORD dwPID = 0;                    //接收hwnd所在进程的id
    DWORD dwCurrentPID;                 //当前进程id
    HWND* phwConsole = (HWND*)lParam;   //指向hwConsole的指针

    dwCurrentPID = GetCurrentProcessId();
    
    GetWindowThreadProcessId(hwnd, &dwPID);

    if (dwPID == dwCurrentPID)  //两者相等，说明找到了
    {
        *phwConsole = hwnd;
        return FALSE;   //不继续搜索
    }
    else
    {
        return TRUE;    //继续搜索
    }
}

/**
 * 为得到终端窗口句柄而设的回调函数
 *
 * 此函数不能被直接调用，而是作为参数提供给EnumWindows()。通过比较程序文件名
 * 和窗口名是否一致寻找终端窗口。把找到的终端窗口句柄写到lParam指向的内存。<br>
 * 参见msdn中的EnumWindowsProc()。
 * @see FindConsoleWindow()
 * @see EnumWindowsProcById()
 */
static BOOL CALLBACK EnumWindowsProcByTitle(HWND hwnd, LPARAM lParam)
{
    TCHAR szFilePath[FILENAME_MAX]="";  //当前执行的程序全路径
    TCHAR szTitle[FILENAME_MAX]="";     //hwnd窗口的标题
    HWND* phwConsole = (HWND*)lParam;   //指向hwConsole的指针
    
    GetModuleFileName(NULL, szFilePath, FILENAME_MAX-1);
    GetWindowText(hwnd, szTitle, FILENAME_MAX-1);

    if (_tcsstr(szTitle, szFilePath) != 0)  //找到了
    {
        *phwConsole = hwnd;
        return FALSE;   //不继续搜索
    }
    else
    {
        return TRUE;    //继续搜索
    }
}

/**
 * 初始化egg窗口
 *
 * 做好egg窗口的各种初始化工作，包括：为egg准备好hdcMem。定义坐标系到以窗口中心
 * 为原点，单位是厘米的平面直角坐标系。
 * 
 * @param fWidth 窗口有效区域的宽度（厘米）
 * @param fHeight 窗口有效区域的高度（厘米）
 * @return 是否成功。
 */
static BOOL InitEggWindow(double fWidth, double fHeight)
{
    CalcResolution();

    ChangeClientSizeTo(WidthCM2P(fWidth), HeightCM2P(fHeight));

    return MakePlate();
}

/**
 * 建立egg图形窗口
 *
 * 建立egg图形窗口。窗口大小先采用缺省值，后面再定义具体大小。此函数的代码摘自
 * <em>"Programming Windows，the 5th Edition" by Charles Petzold</em>，稍作修改
 *
 * @return 窗口句柄
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
        MessageBox(NULL, _T("本程序只能在Windows NT、2000、XP、2003或以上运行"),
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
 * 消息处理中枢 
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
 * 改变Client区域的大小 
 *
 * 并不挪动窗口位置。新大小将保存在全局变量sizePlate中
 *
 * @param nWidth Client区域宽度（像素）
 * @param nHeight Client区域高度（像素）
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
    
    //计算按nWidth和nHeight设置后主窗口的大小
    size.cx += rectWnd.right - rectWnd.left - rectClient.right;
    size.cy += rectWnd.bottom - rectWnd.top - rectClient.bottom;

    MoveWindow(hwEgg, rectWnd.left, rectWnd.top, size.cx, size.cy, FALSE);
}

/**
 * 计算每个象素点在x和y方向各为多少厘米
 *
 * 本函数只需要执行一次。结果保存在fXResolution和fYResolution
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

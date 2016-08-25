/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: graphic.c,v 1.18 2005/11/03 14:28:45 sun Exp $ 
*****************************************************************************/

#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>  //To get FILENAME_MAX
#include "..\egg.h"
#include "egg_priv.h"

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

/**
 * 缺省的画笔颜色
 */
#define DEFAULT_PEN_COLOR   BLACK  //黑色

/**
 * 缺省的画笔宽度
 */
#define DEFAULT_PEN_WIDTH   1

/**
 * 缺省的画刷颜色
 */
#define DEFAULT_BRUSH_COLOR  BLACK  //黑色

/**
 * 缺省的透明色
 */
#define DEFAULT_TRANSPARENT_COLOR  TRANSPARENT_COLOR

/**
 * π
 */
#define PI   3.1415926535

//////////////////////////////////////////////////////////////////////////
//
// 定义数据类型
//
//////////////////////////////////////////////////////////////////////////

/**
 * egg的数据结构
 *
 * egg是Egg平台里的绘图单元。可以针对每个egg进行具体的绘图操作，并可以移动
 * egg的位置。
 */
typedef struct egg
{
    struct egg *pPrev;       /**< 指向前一个被绘制的egg             */

    HDC      hdc;            /**< 内存dc                        */
    HBITMAP  hbm;            /**< 位图                          */
    HPEN     hPen;           /**< 画笔                          */
    HBRUSH   hBrush;         /**< 画刷                          */
    COLORREF rgbTransparent; /**< 透明色                        */

    double   fX;             /**< egg左上角的x坐标（盘子坐标系）    */
    double   fY;             /**< egg左上角的y坐标（盘子坐标系）    */
    double   fPX;            /**< 当前画笔位置的x坐标（egg坐标系）  */
    double   fPY;            /**< 当前画笔位置的y坐标（egg坐标系）  */

    double   fPenWidth;      /**< 画笔粗                            */

    int      bShow;          /**< 是否显示                          */

    struct egg *pNext;       /**< 指向下一个要绘制的egg             */
} egg;

//////////////////////////////////////////////////////////////////////////
//
// 定义在此文件内有效的全局变量
//
//////////////////////////////////////////////////////////////////////////

//!盘子，里面装满彩蛋(egg)！
static egg ePlate;

//!指向最后一个蛋
static egg* pLastEgg = &ePlate;

//!当前的活动egg
static egg* pActiveEgg = NULL;

//!盘子的矩形范围（像素）。每个egg也都是此大小
static RECT rcPlate;

//!盘子背景刷子
static HBRUSH hbrBkgrd;

//!是否自动更新
static int bAutoUpdate = 1;

//!ePlate.hdc临界区
static CRITICAL_SECTION hCriticalSection;


//////////////////////////////////////////////////////////////////////////
//
// 声明仅在此文件内有效的私有函数
//
//////////////////////////////////////////////////////////////////////////

//核心
static BOOL UpdatePlate(const RECT* pRect);

//功能支持
static egg* Valid(HEGG hEgg);
static double Deg2Rad(double deg);
void DeleteEgg(egg* pEgg);

//////////////////////////////////////////////////////////////////////////
//
// Egg API定义，可被其它程序调用
//
//////////////////////////////////////////////////////////////////////////
void MovePen(double x, double y)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    MoveToEx(pActiveEgg->hdc, X_CM2P(pActiveEgg->fPX = x), Y_CM2P(pActiveEgg->fPY = y), NULL);
}

void OffsetPen(double dx, double dy)
{
    MovePen(pActiveEgg->fPX += dx, pActiveEgg->fPY += dy);
}

double GetPenXPos(void)
{
    return pActiveEgg->fPX;
}

double GetPenYPos(void)
{
    return pActiveEgg->fPY;
}

void DrawLine(double dx, double dy)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    LineTo(pActiveEgg->hdc, 
           X_CM2P(pActiveEgg->fPX += dx),
           Y_CM2P(pActiveEgg->fPY += dy));

    UpdatePlate(&rcPlate);
}

void DrawString(char* pString)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    TextOut(pActiveEgg->hdc, X_CM2P(pActiveEgg->fPX), Y_CM2P(pActiveEgg->fPY), pString, _tcslen(pString));
    pActiveEgg->fPX += GetStringWidth(pString);
    MovePen(pActiveEgg->fPX, pActiveEgg->fPY);

    UpdatePlate(&rcPlate);
}

double GetStringWidth(char* pString)
{
    SIZE size;
    InitCheck();
    assert(pActiveEgg != NULL);

    GetTextExtentPoint(pActiveEgg->hdc, pString, _tcslen(pString), &size);
    return WidthP2CM(size.cx);
}

void DrawArc(double r, double dStart, double dSweep)
{
    DrawEllipticalArc(r, r, dStart, dSweep);
}

void DrawEllipticalArc(double rx, double ry, double dStart, double dSweep)
{
    double fCenterX, fCenterY;
    double fDstX, fDstY;

    InitCheck();
    assert(pActiveEgg != NULL);

    fCenterX = pActiveEgg->fPX + rx * cos(Deg2Rad(dStart + 180));
    fCenterY = pActiveEgg->fPY + ry * sin(Deg2Rad(dStart + 180));

    fDstX = fCenterX + rx * cos(Deg2Rad(dStart + dSweep));
    fDstY = fCenterY + ry * sin(Deg2Rad(dStart + dSweep));

    if (dSweep > 0)
    {
        Arc(pActiveEgg->hdc, 
              X_CM2P(fCenterX - rx),
              Y_CM2P(fCenterY + ry),
              X_CM2P(fCenterX + rx),
              Y_CM2P(fCenterY - ry),
              X_CM2P(pActiveEgg->fPX), 
              Y_CM2P(pActiveEgg->fPY),
              X_CM2P(fDstX),
              Y_CM2P(fDstY));
    }
    else
    {
        Arc(pActiveEgg->hdc, 
              X_CM2P(fCenterX - rx),
              Y_CM2P(fCenterY + ry),
              X_CM2P(fCenterX + rx),
              Y_CM2P(fCenterY - ry),
              X_CM2P(fDstX),
              Y_CM2P(fDstY),
              X_CM2P(pActiveEgg->fPX), 
              Y_CM2P(pActiveEgg->fPY));
    }

    pActiveEgg->fPX = fDstX;
    pActiveEgg->fPY = fDstY;

    MoveToEx(pActiveEgg->hdc, X_CM2P(pActiveEgg->fPX), Y_CM2P(pActiveEgg->fPY), NULL);
    
    UpdatePlate(&rcPlate);
}

void DrawBitmap(char* lpszFilename)
{
	HBITMAP hBitmap = NULL;
    HDC hdcTemp;
    HBITMAP hOldBitmap;
    BITMAP bm;

    InitCheck();
    assert(pActiveEgg != NULL);

	hBitmap = (HBITMAP)LoadImage(NULL, lpszFilename, IMAGE_BITMAP, 0, 0,
	                LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if (hBitmap == NULL)
    {
        char tmpStr[FILENAME_MAX];
        _snprintf(tmpStr, FILENAME_MAX, _T("位图文件\"%s\"不存在。"), lpszFilename);
        Error(tmpStr);
    }

    hdcTemp = CreateCompatibleDC(pActiveEgg->hdc);
    hOldBitmap = SelectObject(hdcTemp, hBitmap);

	GetObject( hBitmap, sizeof( bm ), &bm );

    BitBlt(pActiveEgg->hdc, X_CM2P(pActiveEgg->fPX), Y_CM2P(pActiveEgg->fPY), 
           bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, SRCCOPY);

    DeleteObject(SelectObject(hdcTemp, hOldBitmap));
    DeleteDC(hdcTemp);

    OffsetPen(X_P2CM(bm.bmWidth), 0);
    
    UpdatePlate(&rcPlate);
}

HEGG LayEgg(void)
{
    //TODO: 更好的异常处理
    HBRUSH hbrTransparent;
    egg* pEgg = NULL;

    //建立egg
    pEgg = (egg*)calloc(sizeof(egg), 1);
    assert(pEgg != NULL);

    EnterCriticalSection(&hCriticalSection);

    pEgg->hdc = CreateCompatibleDC(ePlate.hdc);
    if (pEgg->hdc == NULL) 
    {
        Error(_T("CreateCompatibleDC Error!"));
    }

    pEgg->hbm = CreateCompatibleBitmap(ePlate.hdc,
                                       rcPlate.right,
                                       rcPlate.bottom);

    LeaveCriticalSection(&hCriticalSection);

    assert(pEgg->hbm != NULL);
    
    SelectObject(pEgg->hdc, pEgg->hbm);

    //用缺省颜色建立画笔
    SetTextColor(pEgg->hdc, DEFAULT_PEN_COLOR);
    pEgg->hPen = CreatePen(PS_SOLID, DEFAULT_PEN_WIDTH, DEFAULT_PEN_COLOR);
    SelectObject(pEgg->hdc, pEgg->hPen);

    //透明色绘制背景
    pEgg->rgbTransparent = DEFAULT_TRANSPARENT_COLOR;
    SetBkColor(pEgg->hdc, pEgg->rgbTransparent);
    hbrTransparent = CreateSolidBrush(pEgg->rgbTransparent);
    FillRect(pEgg->hdc, &rcPlate, hbrTransparent);
    DeleteObject(hbrTransparent);

    //初始画笔位置和初始egg位置都在左上角
    pEgg->fX = pEgg->fPX = X_P2CM(rcPlate.left);
    pEgg->fY = pEgg->fPY = Y_P2CM(rcPlate.top);

    //放入egg链表
    pLastEgg->pNext = pEgg;
    if (pLastEgg == &ePlate) //ePlate只做头指针用，不参与链表的其它事情
        pEgg->pPrev = NULL;
    else
        pEgg->pPrev = pLastEgg;
    pLastEgg = pEgg;
    pLastEgg->pNext = NULL;

    pActiveEgg = pEgg;
    pEgg->bShow = 1;
    
    return pEgg;
}

void EatEgg(void)
{
    egg* pEgg;
    InitCheck();
    assert(pActiveEgg != NULL);

/*    for (pEgg = ePlate.pNext;
         pEgg != NULL;
         pEgg = pEgg->pNext)
    {
        if (pEgg->pNext == pActiveEgg) 
        {
            pEgg->pNext = pActiveEgg->pNext;
            DeleteEgg(pActiveEgg);
            SetActiveEgg(pEgg->pNext);
        }
    }*/

    if (pActiveEgg->pPrev != NULL || pActiveEgg->pNext != NULL)  //最后仅存一个egg不能删除
    {

        if (pActiveEgg->pPrev != NULL)
            pActiveEgg->pPrev->pNext = pActiveEgg->pNext;
        else
            ePlate.pNext = pActiveEgg->pNext;

        if (pActiveEgg->pNext != NULL)
            pActiveEgg->pNext->pPrev = pActiveEgg->pPrev;
        else
            pLastEgg = pActiveEgg->pPrev;

        //下一个活动egg
        pEgg = pActiveEgg->pNext;

        DeleteEgg(pActiveEgg);
        SetActiveEgg(pEgg);
        UpdatePlate(&rcPlate);
    }
}

void SetActiveEgg(HEGG hEgg)
{
    egg* pEgg;

    InitCheck();

    if (hEgg == NULL)
    {
        //参数给NULL，就使用第一个egg
        pActiveEgg = ePlate.pNext;
    }
    else
    {
        for (pEgg = ePlate.pNext; 
             pEgg != hEgg && pEgg != NULL; 
             pEgg = pEgg->pNext)
            ;

        if (pEgg)
        {
            pActiveEgg = pEgg;
        }
        else
        {
            MessageBox(NULL, 
                       _T("向SetActiveEgg()提供的hEgg参数有误\n程序将退出"), 
                       _T("错误使用Egg"), 
                       MB_OK|MB_ICONSTOP);
            exit(0);
        }
    }
}

HEGG GetActiveEgg(void)
{
    return pActiveEgg;
}

void MoveEgg(double x, double y)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    pActiveEgg->fX = x;
    pActiveEgg->fY = y;

    UpdatePlate(&rcPlate);
}

void OffsetEgg(double dx, double dy)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    pActiveEgg->fX += dx;
    pActiveEgg->fY += dy;

    UpdatePlate(&rcPlate);
}

double GetEggXPos(void)
{
    return pActiveEgg->fX;
}

double GetEggYPos(void)
{
    return pActiveEgg->fY;
}

void ShowEgg(int bShow)
{
    InitCheck();
    assert(pActiveEgg != NULL);

    pActiveEgg->bShow = bShow;

    UpdatePlate(&rcPlate);
}

void ChangeEggLayer(int nOffset)
{
    egg* pEgg;  //被挪动的egg
    int i;

    InitCheck();
    assert(pActiveEgg != NULL);

    pEgg = pActiveEgg;
    
    //如果不挪，或者只有一个egg，走人
    if (nOffset == 0 || (pActiveEgg->pNext == NULL && pActiveEgg->pPrev == NULL ))
        return;
    
    for (i = 0; i < abs(nOffset); i++)
    {
        pEgg = (nOffset > 0) ? pEgg->pNext : pEgg->pPrev;
        if (pEgg == NULL)
            break;  //到顶或到底了
    }

    //把pActiveEgg摘出来
    if (pActiveEgg->pNext == NULL)
    {
        //这是最后一个
        pActiveEgg->pPrev->pNext = NULL;
        pLastEgg = pActiveEgg->pPrev;
    }
    else if (pActiveEgg->pPrev == NULL) 
    {
        //这是第一个
        pActiveEgg->pNext->pPrev = NULL;
        ePlate.pNext = pActiveEgg->pNext;
    }
    else
    {
        pActiveEgg->pPrev->pNext = pActiveEgg->pNext;
        pActiveEgg->pNext->pPrev = pActiveEgg->pPrev;
    }

    //开始挪位置

    //如向前挪，则在多向前找一个egg，这样就都是把pActiveEgg插到pEgg之后
    if (nOffset < 0 && pEgg != NULL)   
        pEgg = pEgg->pPrev;

    if (pEgg != NULL) //向后接
    {
        pActiveEgg->pNext = pEgg->pNext;
        pEgg->pNext->pPrev = pActiveEgg;
        
        pActiveEgg->pPrev = pEgg;
        pEgg->pNext = pActiveEgg;
    }
    else if (nOffset > 0) //放尾巴上
    {
        assert(pLastEgg->pNext == NULL);
        pLastEgg->pNext = pActiveEgg;
        pActiveEgg->pPrev = pLastEgg;
        pLastEgg = pActiveEgg;
        pActiveEgg->pNext = NULL;
    }
    else    //放头上
    {
        assert(ePlate.pNext->pPrev == NULL);
        pActiveEgg->pNext = ePlate.pNext;
        ePlate.pNext->pPrev = pActiveEgg;
        pActiveEgg->pPrev = NULL;
        ePlate.pNext = pActiveEgg;
    }
}

void SetPen(unsigned long color, double fWidth)
{
    HPEN hPen;

    InitCheck();
    assert(pActiveEgg != NULL);
    
    SetTextColor(pActiveEgg->hdc, color);

    pActiveEgg->fPenWidth = fWidth;
    hPen = CreatePen(PS_SOLID, WidthCM2P(fWidth), color);
    DeleteObject(SelectObject(pActiveEgg->hdc, hPen));
}

void SetPenColor(unsigned long color)
{
    SetPen(color, pActiveEgg->fPenWidth);
}

void SetPenWidth(double fWidth)
{
    SetPen(GetTextColor(pActiveEgg->hdc), fWidth);
}

void SetAutoUpdate(int bAuto)
{
    bAutoUpdate = bAuto;
}

void Update(void)
{
    egg *pEgg;
    BOOL rval;
//    HBITMAP bm;

    EnterCriticalSection(&hCriticalSection);

    FillRect(ePlate.hdc, &rcPlate, hbrBkgrd);

    for (pEgg = ePlate.pNext; pEgg != NULL; pEgg = pEgg->pNext)
    {
        if (pEgg->bShow) 
        {
            rval = TransparentBlt(ePlate.hdc, 
                X_CM2P(pEgg->fX), Y_CM2P(pEgg->fY),
                rcPlate.right, rcPlate.bottom,
                pEgg->hdc, 
                0, 0, 
                rcPlate.right, rcPlate.bottom,
                pEgg->rgbTransparent);
            assert(rval);
        }
    }
    LeaveCriticalSection(&hCriticalSection);

    InvalidateRect(hwEgg, &rcPlate, FALSE);
}
//////////////////////////////////////////////////////////////////////////
//
// Egg平台函数定义，仅Egg平台的模块可以调用
//
//////////////////////////////////////////////////////////////////////////

void DoUpdate(void)
{
    HDC         hdc;
    PAINTSTRUCT ps;

    assert(ePlate.hdc != NULL);

    hdc = BeginPaint(hwEgg, &ps);

    EnterCriticalSection(&hCriticalSection);

    BitBlt(hdc, 0, 0, rcPlate.right, rcPlate.bottom,
           ePlate.hdc, 0, 0, SRCCOPY);

    LeaveCriticalSection(&hCriticalSection);
    
    EndPaint(hwEgg, &ps);
}

void DeleteEgg(egg* pEgg)
{
    if (pEgg->hdc)
        DeleteDC(pEgg->hdc);
    if (pEgg->hbm)
        DeleteObject(pEgg->hbm);
    if (pEgg->hPen)
        DeleteObject(pEgg->hPen);
    free(pEgg);
}

void CleanEggs(void)
{
    egg *pEgg, *pTemp;

    for (pEgg = ePlate.pNext;
         pEgg != NULL;
         pEgg = pTemp)
    {
        pTemp = pEgg->pNext;
        DeleteEgg(pEgg);
    }
}

BOOL MakePlate(void)
{
    HDC hdc=NULL;        //窗口dc
    BOOL rval;

    assert(hwEgg != NULL);

    GetClientRect(hwEgg, &rcPlate);

    hdc = GetDC(hwEgg);
    if (hdc == NULL)
    {
        goto Error;
    }

    ePlate.hdc = CreateCompatibleDC(hdc);
    if (ePlate.hdc == NULL)
    {
        goto Error;
    }
    
    ePlate.hbm = CreateCompatibleBitmap(hdc, rcPlate.right, rcPlate.bottom);
    if (ePlate.hbm == NULL)
    {
        goto Error;
    }

    SelectObject(ePlate.hdc, ePlate.hbm);
    hbrBkgrd = GetStockObject(WHITE_BRUSH);

    InitializeCriticalSection(&hCriticalSection);

    rval = TRUE;
    goto Exit;  //一切正常
Error:
    if (ePlate.hdc) 
        DeleteDC(ePlate.hdc);
    if (ePlate.hbm) 
        DeleteObject(ePlate.hbm);
    rval = FALSE;
Exit:
    if (hdc)
        ReleaseDC(hwEgg, hdc);

    return rval;
}


//////////////////////////////////////////////////////////////////////////
//
// 私有函数定义，仅在此文件内有效
//
//////////////////////////////////////////////////////////////////////////

/**
 * 重新向盘子上绘制所有egg
 *
 * @param pRect 需要重新绘制的区域。如果要重新绘制整个窗口，用&rcPlate（废弃）
 * @return 是否成功
 */
static BOOL UpdatePlate(const RECT* pRect)
{
    pRect = NULL;  //仅为去掉警告。此参数作废后，删除
    if (bAutoUpdate) 
    {
        Update();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * 转度数为弧度
 *
 * @param deg 度数
 * @return 弧度
 */
static double Deg2Rad(double deg)
{
    return (deg * PI / 180);
}

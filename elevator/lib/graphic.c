/*****************************************************************************
  ��Ȩ���� (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  ��ϵͳ����������������԰�����������������������GNUͨ�ù������֤(GPL)����
  �޸Ļ����·��������֤�����ǵ�2�����µİ汾��

  �����������ϣ�����������棬�����ṩ�κ���ʽ�ĵ�������������ȷ�Ļ��������ģ�
  �����������ڿ����ۺ��ʺ��ض���;��������֤�����GNUͨ�ù������֤: 

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
// �����
//
//////////////////////////////////////////////////////////////////////////

/**
 * Egg�������� 
 *
 * Win32�ڽ�����ʱ��Ҫ�����Ķ�������Ȼ�������ô����󣨻����Ҳ�֪���ô���
 */
#define EGG_WINDOW_CLASS_NAME _T("Easy Graphical egG Window")

/**
 * ȱʡ�Ļ�����ɫ
 */
#define DEFAULT_PEN_COLOR   BLACK  //��ɫ

/**
 * ȱʡ�Ļ��ʿ��
 */
#define DEFAULT_PEN_WIDTH   1

/**
 * ȱʡ�Ļ�ˢ��ɫ
 */
#define DEFAULT_BRUSH_COLOR  BLACK  //��ɫ

/**
 * ȱʡ��͸��ɫ
 */
#define DEFAULT_TRANSPARENT_COLOR  TRANSPARENT_COLOR

/**
 * ��
 */
#define PI   3.1415926535

//////////////////////////////////////////////////////////////////////////
//
// ������������
//
//////////////////////////////////////////////////////////////////////////

/**
 * egg�����ݽṹ
 *
 * egg��Eggƽ̨��Ļ�ͼ��Ԫ���������ÿ��egg���о���Ļ�ͼ�������������ƶ�
 * egg��λ�á�
 */
typedef struct egg
{
    struct egg *pPrev;       /**< ָ��ǰһ�������Ƶ�egg             */

    HDC      hdc;            /**< �ڴ�dc                        */
    HBITMAP  hbm;            /**< λͼ                          */
    HPEN     hPen;           /**< ����                          */
    HBRUSH   hBrush;         /**< ��ˢ                          */
    COLORREF rgbTransparent; /**< ͸��ɫ                        */

    double   fX;             /**< egg���Ͻǵ�x���꣨��������ϵ��    */
    double   fY;             /**< egg���Ͻǵ�y���꣨��������ϵ��    */
    double   fPX;            /**< ��ǰ����λ�õ�x���꣨egg����ϵ��  */
    double   fPY;            /**< ��ǰ����λ�õ�y���꣨egg����ϵ��  */

    double   fPenWidth;      /**< ���ʴ�                            */

    int      bShow;          /**< �Ƿ���ʾ                          */

    struct egg *pNext;       /**< ָ����һ��Ҫ���Ƶ�egg             */
} egg;

//////////////////////////////////////////////////////////////////////////
//
// �����ڴ��ļ�����Ч��ȫ�ֱ���
//
//////////////////////////////////////////////////////////////////////////

//!���ӣ�����װ���ʵ�(egg)��
static egg ePlate;

//!ָ�����һ����
static egg* pLastEgg = &ePlate;

//!��ǰ�Ļegg
static egg* pActiveEgg = NULL;

//!���ӵľ��η�Χ�����أ���ÿ��eggҲ���Ǵ˴�С
static RECT rcPlate;

//!���ӱ���ˢ��
static HBRUSH hbrBkgrd;

//!�Ƿ��Զ�����
static int bAutoUpdate = 1;

//!ePlate.hdc�ٽ���
static CRITICAL_SECTION hCriticalSection;


//////////////////////////////////////////////////////////////////////////
//
// �������ڴ��ļ�����Ч��˽�к���
//
//////////////////////////////////////////////////////////////////////////

//����
static BOOL UpdatePlate(const RECT* pRect);

//����֧��
static egg* Valid(HEGG hEgg);
static double Deg2Rad(double deg);
void DeleteEgg(egg* pEgg);

//////////////////////////////////////////////////////////////////////////
//
// Egg API���壬�ɱ������������
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
        _snprintf(tmpStr, FILENAME_MAX, _T("λͼ�ļ�\"%s\"�����ڡ�"), lpszFilename);
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
    //TODO: ���õ��쳣����
    HBRUSH hbrTransparent;
    egg* pEgg = NULL;

    //����egg
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

    //��ȱʡ��ɫ��������
    SetTextColor(pEgg->hdc, DEFAULT_PEN_COLOR);
    pEgg->hPen = CreatePen(PS_SOLID, DEFAULT_PEN_WIDTH, DEFAULT_PEN_COLOR);
    SelectObject(pEgg->hdc, pEgg->hPen);

    //͸��ɫ���Ʊ���
    pEgg->rgbTransparent = DEFAULT_TRANSPARENT_COLOR;
    SetBkColor(pEgg->hdc, pEgg->rgbTransparent);
    hbrTransparent = CreateSolidBrush(pEgg->rgbTransparent);
    FillRect(pEgg->hdc, &rcPlate, hbrTransparent);
    DeleteObject(hbrTransparent);

    //��ʼ����λ�úͳ�ʼeggλ�ö������Ͻ�
    pEgg->fX = pEgg->fPX = X_P2CM(rcPlate.left);
    pEgg->fY = pEgg->fPY = Y_P2CM(rcPlate.top);

    //����egg����
    pLastEgg->pNext = pEgg;
    if (pLastEgg == &ePlate) //ePlateֻ��ͷָ���ã��������������������
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

    if (pActiveEgg->pPrev != NULL || pActiveEgg->pNext != NULL)  //������һ��egg����ɾ��
    {

        if (pActiveEgg->pPrev != NULL)
            pActiveEgg->pPrev->pNext = pActiveEgg->pNext;
        else
            ePlate.pNext = pActiveEgg->pNext;

        if (pActiveEgg->pNext != NULL)
            pActiveEgg->pNext->pPrev = pActiveEgg->pPrev;
        else
            pLastEgg = pActiveEgg->pPrev;

        //��һ���egg
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
        //������NULL����ʹ�õ�һ��egg
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
                       _T("��SetActiveEgg()�ṩ��hEgg��������\n�����˳�"), 
                       _T("����ʹ��Egg"), 
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
    egg* pEgg;  //��Ų����egg
    int i;

    InitCheck();
    assert(pActiveEgg != NULL);

    pEgg = pActiveEgg;
    
    //�����Ų������ֻ��һ��egg������
    if (nOffset == 0 || (pActiveEgg->pNext == NULL && pActiveEgg->pPrev == NULL ))
        return;
    
    for (i = 0; i < abs(nOffset); i++)
    {
        pEgg = (nOffset > 0) ? pEgg->pNext : pEgg->pPrev;
        if (pEgg == NULL)
            break;  //�����򵽵���
    }

    //��pActiveEggժ����
    if (pActiveEgg->pNext == NULL)
    {
        //�������һ��
        pActiveEgg->pPrev->pNext = NULL;
        pLastEgg = pActiveEgg->pPrev;
    }
    else if (pActiveEgg->pPrev == NULL) 
    {
        //���ǵ�һ��
        pActiveEgg->pNext->pPrev = NULL;
        ePlate.pNext = pActiveEgg->pNext;
    }
    else
    {
        pActiveEgg->pPrev->pNext = pActiveEgg->pNext;
        pActiveEgg->pNext->pPrev = pActiveEgg->pPrev;
    }

    //��ʼŲλ��

    //����ǰŲ�����ڶ���ǰ��һ��egg�������Ͷ��ǰ�pActiveEgg�嵽pEgg֮��
    if (nOffset < 0 && pEgg != NULL)   
        pEgg = pEgg->pPrev;

    if (pEgg != NULL) //����
    {
        pActiveEgg->pNext = pEgg->pNext;
        pEgg->pNext->pPrev = pActiveEgg;
        
        pActiveEgg->pPrev = pEgg;
        pEgg->pNext = pActiveEgg;
    }
    else if (nOffset > 0) //��β����
    {
        assert(pLastEgg->pNext == NULL);
        pLastEgg->pNext = pActiveEgg;
        pActiveEgg->pPrev = pLastEgg;
        pLastEgg = pActiveEgg;
        pActiveEgg->pNext = NULL;
    }
    else    //��ͷ��
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
// Eggƽ̨�������壬��Eggƽ̨��ģ����Ե���
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
    HDC hdc=NULL;        //����dc
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
    goto Exit;  //һ������
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
// ˽�к������壬���ڴ��ļ�����Ч
//
//////////////////////////////////////////////////////////////////////////

/**
 * �����������ϻ�������egg
 *
 * @param pRect ��Ҫ���»��Ƶ��������Ҫ���»����������ڣ���&rcPlate��������
 * @return �Ƿ�ɹ�
 */
static BOOL UpdatePlate(const RECT* pRect)
{
    pRect = NULL;  //��Ϊȥ�����档�˲������Ϻ�ɾ��
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
 * ת����Ϊ����
 *
 * @param deg ����
 * @return ����
 */
static double Deg2Rad(double deg)
{
    return (deg * PI / 180);
}

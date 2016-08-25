/*****************************************************************************
  ��Ȩ���� (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  ��ϵͳ����������������԰�����������������������GNUͨ�ù������֤(GPL)����
  �޸Ļ����·��������֤�����ǵ�2�����µİ汾��

  �����������ϣ�����������棬�����ṩ�κ���ʽ�ĵ�������������ȷ�Ļ��������ģ�
  �����������ڿ����ۺ��ʺ��ض���;��������֤�����GNUͨ�ù������֤: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: event.c,v 1.6 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

/**
 * �¼�����ģ��
 *
 * ������ꡢ���̺Ͷ�ʱ�¼�
 */

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include "..\egg.h"
#include "event.h"
#include "egg_priv.h"

//////////////////////////////////////////////////////////////////////////
//
// �����
//
//////////////////////////////////////////////////////////////////////////

/**
 * �����ֵ����
 */
#define VIRTUAL_KEY_NUMBER  256

/**
 * �¼���������С
 */
#define EVENT_BUFFER_COUNT  128

/**
 * ��1���ӵ�EVENT_BUFFER_COUNT�Զ�����
 * @param x ���ӵĶ���
 */
#define CYCLE_INCREASE(x)  ( (x+1) % EVENT_BUFFER_COUNT )

//////////////////////////////////////////////////////////////////////////
//
// ������Egg����Ч��ȫ�ֱ���
//
//////////////////////////////////////////////////////////////////////////

POINT ptMouse;
char bVKDown[VIRTUAL_KEY_NUMBER];

//////////////////////////////////////////////////////////////////////////
//
// �����ڴ��ļ�����Ч��ȫ�ֱ���
//
//////////////////////////////////////////////////////////////////////////

//!��ʱ����ʶ��
static UINT_PTR uTimer;

//!�¼������ٽ���
static CRITICAL_SECTION hCriticalSection;

//!�¼������źŵ�
static HANDLE hSmphEventList;

//!�¼�����
static EVENT aEvents[EVENT_BUFFER_COUNT];

//!�����¼����
static UINT uFirst;

//!��β��һ�����¼����
static UINT uLast;

//!�������ļ�ֵ
static UINT uStruckKey;

//////////////////////////////////////////////////////////////////////////
//
// �������ڴ��ļ�����Ч��˽�к���
//
//////////////////////////////////////////////////////////////////////////

static void EventStart(void);
static EVENT GetEvent(void);
static void CleanPastEvents(void);

//////////////////////////////////////////////////////////////////////////
//
// Egg API���壬�ɱ������������
//
//////////////////////////////////////////////////////////////////////////

EVENT_TYPE WaitForEvent(void)
{
    DWORD rval;
    EVENT ev = {0,0};

    if (hSmphEventList == NULL)
    {
        EventStart();
    }

    rval = WaitForSingleObject(hSmphEventList, INFINITE);
    if (rval == WAIT_OBJECT_0)
    {
        EnterCriticalSection(&hCriticalSection);

        ev = aEvents[uFirst];
        uFirst = CYCLE_INCREASE(uFirst);

        LeaveCriticalSection(&hCriticalSection);

        uStruckKey = ev.code;
        return ev.event;
    }
    else
    {
        Error(_T("�ȴ��¼��źŵ�ʧ��"));
    }
    
    assert(FALSE);
    return ev.event;  //��Զ����ִ�е�����
}

void WaitForExit(void)
{
    EVENT_TYPE ev;

    ev = WaitForEvent();
    while (ev != EXIT)
    {
         ev = WaitForEvent();
    }

    assert(ev == EXIT);
}

int WaitFor(unsigned int uMillisecond)
{
    EVENT_TYPE ev;

    StartTimer(uMillisecond);

    ev = WaitForEvent();
    while (ev != TIMER && ev != EXIT)
    {
         ev = WaitForEvent();
    }

    StopTimer();

    assert(ev == TIMER || ev == EXIT);
    return (ev==TIMER);
}

int IsKeyDown(unsigned int uVKCode)
{
    return bVKDown[uVKCode];
}

unsigned int GetStruckKey(void)
{
    return uStruckKey;
}

double GetMouseX(void)
{
    return X_P2CM(ptMouse.x);
}

double GetMouseY(void)
{
    return Y_P2CM(ptMouse.y);
}

void StartTimer(unsigned int uMillisecond)
{
    uTimer = SetTimer(hwEgg, 1, uMillisecond, NULL);
}

void StopTimer(void)
{
    KillTimer(hwEgg, uTimer);
}
//////////////////////////////////////////////////////////////////////////
//
// Eggƽ̨�������壬��Eggƽ̨��ģ����Ե���
//
//////////////////////////////////////////////////////////////////////////

void AddEvent(EVENT ev)
{
    if (hSmphEventList == NULL)
    {
        return;
    }
    
    EnterCriticalSection(&hCriticalSection);

    aEvents[uLast] = ev;
    uLast = CYCLE_INCREASE(uLast);

    if (!ReleaseSemaphore(hSmphEventList, 1, NULL))
    {
        //������������Ҫ����aEvents[uFirst]
        uFirst = CYCLE_INCREASE(uFirst);
        assert(uFirst == uLast);
    }

    LeaveCriticalSection(&hCriticalSection);
}

//////////////////////////////////////////////////////////////////////////
//
// ˽�к������壬���ڴ��ļ�����Ч
//
//////////////////////////////////////////////////////////////////////////

/**
 * ��ʼ���¼�ģ��
 *
 * @see EventEnd()
 */
static void EventStart(void)
{
    if (hSmphEventList != NULL)
    {
        return;
    }

    hSmphEventList = CreateSemaphore(NULL, 0, EVENT_BUFFER_COUNT, NULL);
    if (hSmphEventList == NULL)
    {
        Error(_T("�¼��źŵƽ���ʧ�ܣ�����rpwt��"));
    }

    InitializeCriticalSection(&hCriticalSection);

    CleanPastEvents();
}

/**
 * ��������е������¼�
 *
 * �˺���Ҳ���Կ�����ΪEgg API��һ���֡�
 */
void CleanPastEvents(void)
{
    EnterCriticalSection(&hCriticalSection);
    
    while (WaitForSingleObject(hSmphEventList,0) != WAIT_TIMEOUT)
        ;

    uFirst = uLast = 0;

    LeaveCriticalSection(&hCriticalSection);
}

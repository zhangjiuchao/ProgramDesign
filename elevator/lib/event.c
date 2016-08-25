/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: event.c,v 1.6 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

/**
 * 事件处理模块
 *
 * 处理鼠标、键盘和定时事件
 */

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include "..\egg.h"
#include "event.h"
#include "egg_priv.h"

//////////////////////////////////////////////////////////////////////////
//
// 定义宏
//
//////////////////////////////////////////////////////////////////////////

/**
 * 虚拟键值个数
 */
#define VIRTUAL_KEY_NUMBER  256

/**
 * 事件缓冲区大小
 */
#define EVENT_BUFFER_COUNT  128

/**
 * 加1，加到EVENT_BUFFER_COUNT自动回零
 * @param x 被加的对象
 */
#define CYCLE_INCREASE(x)  ( (x+1) % EVENT_BUFFER_COUNT )

//////////////////////////////////////////////////////////////////////////
//
// 定义在Egg内有效的全局变量
//
//////////////////////////////////////////////////////////////////////////

POINT ptMouse;
char bVKDown[VIRTUAL_KEY_NUMBER];

//////////////////////////////////////////////////////////////////////////
//
// 定义在此文件内有效的全局变量
//
//////////////////////////////////////////////////////////////////////////

//!定时器标识符
static UINT_PTR uTimer;

//!事件队列临界区
static CRITICAL_SECTION hCriticalSection;

//!事件队列信号灯
static HANDLE hSmphEventList;

//!事件队列
static EVENT aEvents[EVENT_BUFFER_COUNT];

//!队首事件编号
static UINT uFirst;

//!队尾第一个空事件编号
static UINT uLast;

//!被按键的键值
static UINT uStruckKey;

//////////////////////////////////////////////////////////////////////////
//
// 声明仅在此文件内有效的私有函数
//
//////////////////////////////////////////////////////////////////////////

static void EventStart(void);
static EVENT GetEvent(void);
static void CleanPastEvents(void);

//////////////////////////////////////////////////////////////////////////
//
// Egg API定义，可被其它程序调用
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
        Error(_T("等待事件信号灯失败"));
    }
    
    assert(FALSE);
    return ev.event;  //永远不会执行到这里
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
// Egg平台函数定义，仅Egg平台的模块可以调用
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
        //队列已满，需要挤出aEvents[uFirst]
        uFirst = CYCLE_INCREASE(uFirst);
        assert(uFirst == uLast);
    }

    LeaveCriticalSection(&hCriticalSection);
}

//////////////////////////////////////////////////////////////////////////
//
// 私有函数定义，仅在此文件内有效
//
//////////////////////////////////////////////////////////////////////////

/**
 * 初始化事件模块
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
        Error(_T("事件信号灯建立失败，出现rpwt！"));
    }

    InitializeCriticalSection(&hCriticalSection);

    CleanPastEvents();
}

/**
 * 清除缓存中的所有事件
 *
 * 此函数也可以考虑作为Egg API的一部分。
 */
void CleanPastEvents(void)
{
    EnterCriticalSection(&hCriticalSection);
    
    while (WaitForSingleObject(hSmphEventList,0) != WAIT_TIMEOUT)
        ;

    uFirst = uLast = 0;

    LeaveCriticalSection(&hCriticalSection);
}

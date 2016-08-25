/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: event.h,v 1.3 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

#ifndef _EVENT_SUNNER_HIT
#define _EVENT_SUNNER_HIT

//!鼠标指针当前位置（像素）
extern POINT ptMouse;

//!记录每个虚拟键是否正在被按下
extern char bVKDown[];

/**
 * 事件
 *
 * @see WaitForEvent()
 */
typedef struct EVENT
{
    EVENT_TYPE event;       /**< 事件类型 */
    unsigned short code;    /**< 对事件的具体说明，event不同，含义不同 */
} EVENT;


/**
 * 向事件队列添加一个事件
 *
 * 如果队列满，则挤出最早进入的事件。
 * @param ev 被添加的事件
 * @see GetEvent()
 */
void AddEvent(EVENT ev);

#endif //_EVENT_SUNNER_HIT

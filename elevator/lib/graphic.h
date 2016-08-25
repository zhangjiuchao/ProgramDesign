/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: graphic.h,v 1.3 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

/**
 * Egg图形模块的内部头文件
 *
 * 只有Egg的其它模块才需要使用此头文件
 */

#ifndef _GRAPHIC_SUNNER_HIT
#define _GRAPHIC_SUNNER_HIT

#include <windows.h>
#include "..\egg.h"

/**
 * 生成盘子
 *
 * 生成作为背景的盘子
 *
 * @return 成功返回TRUE；失败返回FALSE
 */
BOOL MakePlate(void);

/**
 * 刷新窗口内容
 *
 * 响应WM_PAINT消息，把ePlate显示到窗口
 *
 */
void DoUpdate(void);

/**
 * 释放所有egg占用的资源
 */
void CleanEggs(void);

#endif //_GRAPHIC_SUNNER_HIT

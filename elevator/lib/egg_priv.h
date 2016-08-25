/*****************************************************************************
  版权所有 (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  该系统是自由软件；您可以按照自由软件基金会所公布的GNU通用公共许可证(GPL)进行
  修改或重新发布；许可证可以是第2版或更新的版本。

  发布该软件是希望它有用有益，但不提供任何形式的担保，不论是明确的还是隐含的，
  包括但不限于可销售和适合特定用途的隐含保证。详见GNU通用公共许可证: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: egg_priv.h,v 1.2 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

/**
 * Egg核心的内部头文件
 *
 * 只有Egg的其它模块才需要使用此头文件
 */

#ifndef _EGG_PRIVATE_SUNNER_HIT
#define _EGG_PRIVATE_SUNNER_HIT

extern HWND hwEgg;

/**
 * 转换像素X坐标到厘米笛卡尔X坐标
 *
 * @param x 像素X坐标值
 * @return 转换后的厘米笛卡尔X坐标
 * @see X_P2CM(), Y_P2CM()
 */
double X_P2CM(int x);

/**
 * 转换像素Y坐标到厘米笛卡尔Y坐标
 *
 * @param y 像素Y坐标值
 * @return 转换后的厘米笛卡尔Y坐标
 * @see CoordinateCM2P()
 */
double Y_P2CM(int y);

/**
 * 转换厘米笛卡尔X坐标到像素X坐标
 *
 * @param x 厘米笛卡尔X坐标
 * @return 转换后的像素X坐标
 * @see CoordinateCM2P()
 */
int X_CM2P(double x);

/**
 * 转换像素Y坐标到厘米笛卡尔Y坐标
 *
 * @param y 像素Y坐标值
 * @return 转换后的厘米笛卡尔Y坐标
 * @see CoordinateCM2P()
 */
int Y_CM2P(double y);

/**
 * 转换像素宽度到厘米宽度
 *
 * @param nWidth 像素宽度
 * @return 转换后的厘米宽度
 */
double WidthP2CM(int nWidth);

/**
 * 转换像素高度到厘米高度
 *
 * @param nHeight 像素高度
 * @return 转换后的厘米高度
 */
double HeightP2CM(int nHeight);

/**
 * 转换厘米高度到像素高度
 *
 * @param fHeight 厘米高度
 * @return 转换后的像素高度
 * @see WidthCM2P()
 */
int HeightCM2P(double fHeight);

/**
 * 转换厘米宽度到像素宽度
 *
 * @param fWidth 厘米宽度
 * @return 转换后的像素宽度
 * @see HeightCM2P()
 */
int WidthCM2P(double fWidth);

/**
 * 提示错误并退出程序
 *
 * 本函数弹出一个消息框，在其中显示pszErrString。待用户点击“确定”后，退出程序。
 * <P> <B>说明：</B>作为一个函数库，应该保持“沉默”的原则，即只通过返回值报错，
 * 用错误日志记录错误，不可向界面输出错误信息，更不能退出程序。错误的处理工作应
 * 该由调用者完成。Egg没有遵守这个原则，因为它是为初学者设计的，以能帮助初学者
 * 最快地掌握程序设计的基本技术为目标。这样不“沉默”的表现，可以让初学者最便捷
 * 地知晓程序的问题在哪里。
 * @param pszErrString 错误信息
 */
void Error(TCHAR* pszErrString);

/**
 * 检查Egg是否成功初始化过
 *
 * 如未曾成功初始化，调用 Error()，反之，返回。
 */
void InitCheck(void);

#endif //_EGG_PRIVATE_SUNNER_HIT

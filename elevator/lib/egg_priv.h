/*****************************************************************************
  ��Ȩ���� (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  ��ϵͳ����������������԰�����������������������GNUͨ�ù������֤(GPL)����
  �޸Ļ����·��������֤�����ǵ�2�����µİ汾��

  �����������ϣ�����������棬�����ṩ�κ���ʽ�ĵ�������������ȷ�Ļ��������ģ�
  �����������ڿ����ۺ��ʺ��ض���;��������֤�����GNUͨ�ù������֤: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: egg_priv.h,v 1.2 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

/**
 * Egg���ĵ��ڲ�ͷ�ļ�
 *
 * ֻ��Egg������ģ�����Ҫʹ�ô�ͷ�ļ�
 */

#ifndef _EGG_PRIVATE_SUNNER_HIT
#define _EGG_PRIVATE_SUNNER_HIT

extern HWND hwEgg;

/**
 * ת������X���굽���׵ѿ���X����
 *
 * @param x ����X����ֵ
 * @return ת��������׵ѿ���X����
 * @see X_P2CM(), Y_P2CM()
 */
double X_P2CM(int x);

/**
 * ת������Y���굽���׵ѿ���Y����
 *
 * @param y ����Y����ֵ
 * @return ת��������׵ѿ���Y����
 * @see CoordinateCM2P()
 */
double Y_P2CM(int y);

/**
 * ת�����׵ѿ���X���굽����X����
 *
 * @param x ���׵ѿ���X����
 * @return ת���������X����
 * @see CoordinateCM2P()
 */
int X_CM2P(double x);

/**
 * ת������Y���굽���׵ѿ���Y����
 *
 * @param y ����Y����ֵ
 * @return ת��������׵ѿ���Y����
 * @see CoordinateCM2P()
 */
int Y_CM2P(double y);

/**
 * ת�����ؿ�ȵ����׿��
 *
 * @param nWidth ���ؿ��
 * @return ת��������׿��
 */
double WidthP2CM(int nWidth);

/**
 * ת�����ظ߶ȵ����׸߶�
 *
 * @param nHeight ���ظ߶�
 * @return ת��������׸߶�
 */
double HeightP2CM(int nHeight);

/**
 * ת�����׸߶ȵ����ظ߶�
 *
 * @param fHeight ���׸߶�
 * @return ת��������ظ߶�
 * @see WidthCM2P()
 */
int HeightCM2P(double fHeight);

/**
 * ת�����׿�ȵ����ؿ��
 *
 * @param fWidth ���׿��
 * @return ת��������ؿ��
 * @see HeightCM2P()
 */
int WidthCM2P(double fWidth);

/**
 * ��ʾ�����˳�����
 *
 * ����������һ����Ϣ����������ʾpszErrString�����û������ȷ�������˳�����
 * <P> <B>˵����</B>��Ϊһ�������⣬Ӧ�ñ��֡���Ĭ����ԭ�򣬼�ֻͨ������ֵ����
 * �ô�����־��¼���󣬲�����������������Ϣ���������˳����򡣴���Ĵ�����Ӧ
 * ���ɵ�������ɡ�Eggû���������ԭ����Ϊ����Ϊ��ѧ����Ƶģ����ܰ�����ѧ��
 * �������ճ�����ƵĻ�������ΪĿ�ꡣ����������Ĭ���ı��֣������ó�ѧ������
 * ��֪����������������
 * @param pszErrString ������Ϣ
 */
void Error(TCHAR* pszErrString);

/**
 * ���Egg�Ƿ�ɹ���ʼ����
 *
 * ��δ���ɹ���ʼ�������� Error()����֮�����ء�
 */
void InitCheck(void);

#endif //_EGG_PRIVATE_SUNNER_HIT

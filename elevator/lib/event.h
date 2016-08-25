/*****************************************************************************
  ��Ȩ���� (C) 2005-2005  Sun Zhigang  (http://egg.sunner.cn)

  ��ϵͳ����������������԰�����������������������GNUͨ�ù������֤(GPL)����
  �޸Ļ����·��������֤�����ǵ�2�����µİ汾��

  �����������ϣ�����������棬�����ṩ�κ���ʽ�ĵ�������������ȷ�Ļ��������ģ�
  �����������ڿ����ۺ��ʺ��ض���;��������֤�����GNUͨ�ù������֤: 

  http://www.gnu.org/copyleft/gpl.html

  $Id: event.h,v 1.3 2005/10/28 14:45:31 sun Exp $ 
*****************************************************************************/

#ifndef _EVENT_SUNNER_HIT
#define _EVENT_SUNNER_HIT

//!���ָ�뵱ǰλ�ã����أ�
extern POINT ptMouse;

//!��¼ÿ��������Ƿ����ڱ�����
extern char bVKDown[];

/**
 * �¼�
 *
 * @see WaitForEvent()
 */
typedef struct EVENT
{
    EVENT_TYPE event;       /**< �¼����� */
    unsigned short code;    /**< ���¼��ľ���˵����event��ͬ�����岻ͬ */
} EVENT;


/**
 * ���¼��������һ���¼�
 *
 * ������������򼷳����������¼���
 * @param ev ����ӵ��¼�
 * @see GetEvent()
 */
void AddEvent(EVENT ev);

#endif //_EVENT_SUNNER_HIT

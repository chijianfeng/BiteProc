#pragma  once

#ifndef _IEEG_VIEWER_
#define _IEEG_VIEWER_

#include "Structdefine.h"
#include "ManagedData.h"
#include <iostream>
using namespace std;

DECLEAR_INTERFACE(IEEGViewer)

//�ڴ���ӽӿں���,�����Ǵ��麯��

virtual void SetEEGData( ManagedData& data,  ManagedData& color) = 0;	//��������
virtual void SetMaxMin(const int Maxv, const int Minv) = 0;				//���������Сֵ����������
virtual void GetCurMaxMin(int& Maxv, int& Minv) = 0;					//��ȡ��ǰ�������Сֵ
virtual void AutoMaxMin(bool b) = 0;									//�Զ����������Сֵ

virtual void UpdateAll() = 0;											//ȫ���ػ�
virtual void SetClientSize(const CRect) = 0;							//��ȡ���������С
virtual void SetChannelNum(const size_t num) = 0;						//����ͨ����
//virtual void SetState(bool b) = 0;                                      //����flage��״̬

END_DECLEAR_INTERFACE

#endif
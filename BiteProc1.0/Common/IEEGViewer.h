#pragma  once

#ifndef _IEEG_VIEWER_
#define _IEEG_VIEWER_

#include "Structdefine.h"
#include "ManagedData.h"
#include <iostream>
using namespace std;

DECLEAR_INTERFACE(IEEGViewer)

//在此添加接口函数,必须是纯虚函数

virtual void SetEEGData( ManagedData& data,  ManagedData& color) = 0;	//传递数据
virtual void SetMaxMin(const int Maxv, const int Minv) = 0;				//设置最大最小值，用于缩放
virtual void GetCurMaxMin(int& Maxv, int& Minv) = 0;					//获取当前的最大最小值
virtual void AutoMaxMin(bool b) = 0;									//自动调整最大最小值

virtual void UpdateAll() = 0;											//全部重绘
virtual void SetClientSize(const CRect) = 0;							//获取绘制区域大小
virtual void SetChannelNum(const size_t num) = 0;						//设置通道数
//virtual void SetState(bool b) = 0;                                      //设置flage打开状态

END_DECLEAR_INTERFACE

#endif
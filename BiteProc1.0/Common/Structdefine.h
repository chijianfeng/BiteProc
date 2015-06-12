/*
the file is in order to define the structs.
*/
#ifndef _STRUCT_DEFINE_
#define _STRUCT_DEFINE_

#include <iostream>
#include <afx.h>
using namespace std;
#define MYDELETE(p) {if(NULL!=p)delete p;p=NULL;}
#define M_STOP 4

#define UPDATACONTROLTOVAL UpdateData(TRUE)
#define UPDATAVALTOCONTROL UpdateData(FALSE)

typedef struct _FILT
{
	FLOAT fu;    //lower border frequency of filter
	FLOAT fo;    //upper border frequency of filter
	FLOAT fs;     //sample rate;
	FLOAT type;   //filter type 2 - CHEBYSHEV  1 - BUTTERWORTH
	FLOAT order;   //filter order.
}FILT;

typedef struct
{
	FLOAT version;
	char* info;
	
}Device_info;
enum ConnetState{CONNECTED=1,DISCONNECTED,CONNECTING};


#define MYCOLOR_SNOW            RGB(255,250,250)
#define MYCOLOR_GAINSBORO       RGB(220,220,220)
#define MYCOLOR_OLDLACE         RGB(253,245,230)
#define MYCOLOR_BLACK           RGB(0,0,0)
#define MYCOLOR_DARKSLAKEGRAY   RGB(49,79,79)
#define MYCOLOR_CYAN1           RGB(0,255,255)
#define MYCOLOR_CYAN2           RGB(0,238,238)
#define MYCOLOR_CYAN3           RGB(121,205,205)
#define MYCOLOR_DIMGRAY         RGB(105,105,105)
#define MYCOLOR_DARKSEAGREEN    RGB(105,139,105)
#define MYCOLOR_MIDNIGHTBLUE    RGB(25,25,112)
#define MYCOLOR_DARKSLATEBLUE   RGB(72,61,139)
#define MYCOLOR_STEELBLUE       RGB(70,130,180)
#define MYCOLOR_KHAKI           RGB(139,134,78)
#define MYCOLOR_DARKOLIVERGREEN RGB(85,107,47)
#define MYCOLOR_LIGGHTGOLDEN    RGB(205,190,112)
#define MYCOLOR_GOLD            RGB(238,201,0)
#define MYCOLOR_GOLD2           RGB(139,117,0)
#define MYCOLOR_YELLOW          RGB(255,255,0)
#define MYCOLOR_DARKGOLDEN      RGB(205,149,12)
#define MYCOLOR_ROSYBROWN       RGB(188,143,143)
#define MYCOLOR_INDIANRED       RGB(205,92,92)
#define MYCOLOR_SIENNA          RGB(205,104,57)
#define MYCOLOR_FIREBIRCK       RGB(178,34,34)
#define MYCOLOR_BURYWOOD        RGB(139,115,85)
#define MYCOLOR_RED             RGB(255,0,0)
#define MYCOLOR_DEEPPINK        RGB(255,20,147)
#define MYCOLOR_DARKORCHID      RGB(153,50,204)
#define MYCOLOR_LAWGREEN		RGB(134,252,0)
#define MYCOLOR_DARKGREEN		RGB(0,128,0)
#define MYCOLOR_WHITE			RGB(255,255,255)
#define MYCOLOR_GREEN			RGB(0,255,0)
#define MYCOLOR_LIGHTGREY		RGB(208,208,208)

// MODES
#define M_NORMAL		0
#define	M_IMPEDANCE		1
#define	M_CALIBRATE		2
#define M_COUNTER		3

//type and macro define
#define  TOLSTRING(S) _T(#S)
#define  TOSTRING(S)	(#S)

#define  Equalstr(str1 , str2) (str1.compare(str2)==0)
#define  Equalwstr(str1 , str2) (str1.Compare(str2)==0)
//用于接口定义宏
#define Interface class

#define implements public

#define DECLEAR_INTERFACE(classname) Interface classname{\
public:virtual ~classname(){}

#define END_DECLEAR_INTERFACE };

//参数文件定义：
#define CONFIGFILE "Config.txt"
#define DEVICEFILE "Device.txt"
#define VIEWFILE "ViewConfig.txt"
#define APPFILE	"AppConfig.txt"
#define TLLEFTPARAM "TL_Left_param.txt"
#define TLRIGHTPARAM "TL_Right_param.txt"
#define TLBOTHPARAM "TL_Both_param.txt"

const COLORREF Bite = RGB(255, 0, 0);		//定义咬牙颜色 red
const COLORREF NBite = RGB(0, 0, 0);		//非咬牙颜色

//咬牙命令定义
#define LEFTONELIGHTBITE  1
#define LEFTONEHEAVYBITE  2
#define LEFTTWOBITE		  3
#define LEFTLONGBITE	  4
#define RIGHTONELIGHTBITE 5
#define RIGHTONEHEAVYBITE 6
#define RIGHTTWOBITE	  7
#define RIGHTLONGBITE	  8
#define BOTHONELIGHTBITE  9
#define BOTHONEHEAVYBITE  10
#define BOTHTWOBITE       11
#define BOTHLONGBITE	  12

//0.5秒的咬牙数据
#define BITELENGTH 0.5
#define ONEBITELENGTH 0.35

//滤波参数定义：
/*3~100HZ */
static double a_3_100[] = { 1.0000, -1.1689, -0.8881, 0.7217, 0.7770, -0.2404, -0.1978 };
static double b_3_100[] = { 0.4459, 0, -1.3377, 0, 1.3377, 0, -0.4459 };
/*添加其他频道参数，可通过matlab进行计算*/


//用户自定义消息规则定义：
/*
	MYWM_xxxTRAINWND		:	来自训练窗口的消息,其值范围：WM_USER+0x01~WM_USER+0xFF
	TRAINWD_XXX				:	表示训练窗口实际完成的消息，以WM_USER+0xFFXX以上值
*/

#define MYWM_TRAINWMD WM_USER+0x0F0
#define TRAINWND_BITEDONE WM_USER+0x0FF01
#define TRAINWND_BITECANCLE	WM_USER+0x0FF02
#define TRAINWND_CLASSIFYDONE WM_USER+0x0FF07
#define TRAINWND_CLASSIFYCANCLE WM_USER+0x0FF08
#define TRAINWND_HLDONE	WM_USER+0x0FF09
#define TRAINWND_HLCANCLE WM_USER+0x0FF0A
#define TRAINWND_TLDONE	WM_USER+0x0FF0B
#define TRAINWND_TLCANCLE WM_USER+0x0FF0C

//定义训练模块与主模块之间测试消息
#define TRAINWND_TEST	WM_USER+0x0FF0D
#define TRAINWND_TESTFINISH WM_USER+0x0FF0F
#define TRAINWND_PROCESSRESULT WM_USER+0x0FF0E


//线程与训练窗体消息
#define MYWM_THREADMSG	WM_USER+0x0F1
#define THREADMSG_ALLFINISH WM_USER+0x0FF03
#define THREADMSG_PROCESSFINISH	WM_USER+0x0FF04
#define THREADMSG_LOADFINISH WM_USER+0x0FF05
#define THREADMSG_SAVEFINISH WM_USER+0x0FF06


#endif
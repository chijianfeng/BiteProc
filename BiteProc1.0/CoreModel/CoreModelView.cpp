
// CoreModelView.cpp : CCoreModelView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CoreModel.h"
#endif

#include "CoreModelDoc.h"
#include "CoreModelView.h"
#include "OutputWnd.h"
#include "AppConfig.h"
#include"../AppWheelChair/WheelChairShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCoreModelView

IMPLEMENT_DYNCREATE(CCoreModelView, CView)

BEGIN_MESSAGE_MAP(CCoreModelView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_MESSAGE(MYWM_TRAINWMD , OnRcvTrainMsg)
END_MESSAGE_MAP()

// CCoreModelView 构造/析构

CCoreModelView::CCoreModelView()
{
	// TODO:  在此处添加构造代码

}

CCoreModelView::~CCoreModelView()
{
}

BOOL CCoreModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCoreModelView 绘制

void CCoreModelView::OnDraw(CDC* /*pDC*/)
{
	CCoreModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CCoreModelView::SendDatatoWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBaseTrainWnd* pWnd = pDoc->GetBaseTrainWnd();				//获取当前的训练窗口
	if (!pWnd)													//窗口无效，不发送
		return;
	ManagedData data ,color;
	pDoc->GetNewData(data, color);
	pWnd->SetData(data);										//发送给窗口
}

void CCoreModelView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCoreModelView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCoreModelView 诊断

#ifdef _DEBUG
void CCoreModelView::AssertValid() const
{
	//CView::AssertValid();
}

void CCoreModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCoreModelDoc* CCoreModelView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoreModelDoc)));
	return (CCoreModelDoc*)m_pDocument;
}
#endif //_DEBUG


// CCoreModelView 消息处理程序


void CCoreModelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 初始化，用于加载训练模块以及应用模块
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	LoadBiteTrainWnd();
}

void CCoreModelView::LoadBiteTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//加载算法配置路径
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//加载参数至内存
	{
		m_wndOutput << "加载阈值训练窗口失败：无法读取参数文件" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetBiteModulePath();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "加载阈值训练窗口失败：模块路径不存在" << m_EndOut;
		return;
	}
	if (!config.GetBitestate())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//仅仅加载模型
		{
			m_wndOutput << "加载阈值模型失败" << m_EndOut;
			return;
		}
		m_wndOutput << "加载阈值模型成功" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_BITEDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "加载阈值训练窗口失败：窗口注册失败" << m_EndOut;
		return;
	}
	m_wndOutput << "加载阈值训练窗口模块成功" << m_EndOut;
}

void CCoreModelView::LoadClassifyTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//加载算法配置路径
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//加载参数至内存
	{
		m_wndOutput << "加载分类训练窗口失败：无法读取参数文件" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetClassifyModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "加载分类训练窗口失败：模块路径不存在" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYCANCLE);
		return;
	}

	if (!config.GetClassifystate()&&!config.IsRetrainClassify())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//仅仅加载模型
		{
			m_wndOutput << "加载分类模型失败" << m_EndOut;
			return;
		}
		m_wndOutput << "加载分类模型成功" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_CLASSIFYDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "加载分类训练窗口失败：窗口注册失败" << m_EndOut;
		return;
	}
	m_wndOutput << "加载分类训练窗口模块成功" << m_EndOut;

}

void CCoreModelView::LoadHLTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//加载算法配置路径
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//加载参数至内存
	{
		m_wndOutput << "加载轻重训练窗口失败：无法读取参数文件" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetHLModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "加载轻重训练窗口失败：模块路径不存在" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_HLCANCLE);
		return;
	}

	if (!config.GetHLState()&&!config.GetHLReTrain())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//仅仅加载模型
		{
			m_wndOutput << "加载轻重模型失败" << m_EndOut;
			return;
		}
		m_wndOutput << "加载轻重模型成功" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_HLDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "加载轻重训练窗口失败：窗口注册失败" << m_EndOut;
		return;
	}
	m_wndOutput << "加载轻重训练窗口模块成功" << m_EndOut;
}

void CCoreModelView::LoadTLTrainWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString algpath = pDoc->GetCurUser() + "\\" + CONFIGFILE;	//加载算法配置路径
	CAlgParamConfig config(algpath);
	if (!config.LoadParam())									//加载参数至内存
	{
		m_wndOutput << "加载长咬、两次训练窗口失败：无法读取参数文件" << m_EndOut;
		config.CloseParmFile();
		return;
	}
	else
		config.CloseParmFile();
	//check the dll module path.
	CString dllpath = config.GetTLModule();
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "加载长咬、两次训练窗口失败：模块路径不存在" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_TLCANCLE);
		return;
	}

	if (!config.GetTLTrainState()&&!config.GetTLReTrain())
	{
		if (!pDoc->AppendAlgModel(dllpath))						//仅仅加载模型
		{
			m_wndOutput << "加载长咬、两次模型失败" << m_EndOut;
			return;
		}
		m_wndOutput << "加载长咬、两次模型成功" << m_EndOut;
		this->SendMessage(MYWM_TRAINWMD, TRAINWND_TLDONE);
		return;
	}
	if (!pDoc->RegTrainModule(dllpath, this))
	{
		m_wndOutput << "加载长咬、两次训练窗口失败：窗口注册失败" << m_EndOut;
		return;
	}
	m_wndOutput << "加载长咬、两次训练窗口模块成功" << m_EndOut;
}

void CCoreModelView::LoadAppWnd()
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CString Apppath = pDoc->GetCurUser() + "\\" + APPFILE;	//加载算法配置路径
	CAppConfig config(Apppath);
	if (!config.IsOpen())
	{
		m_wndOutput << "加载应用窗口失败：无法读取参数文件" << m_EndOut;
		return;
	}
	config.CloseFile();
	CString dllpath = config.GetModulePath();				//获取模块路径
	if (dllpath.GetLength() <= 0)
	{
		m_wndOutput << "加载应用窗口失败：模块路径不存在" << m_EndOut;
		return;
	}
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = config.GetWidth();							//获取参数
	rect.bottom = config.GetHeight();

	if (!pDoc->RegAppModule(dllpath, this, rect))
	{
		m_wndOutput << "加载应用窗口失败：窗口注册失败" << m_EndOut;
		return;
	}
	m_wndOutput << "加载应用窗口模块成功" << m_EndOut;
	//启动处理线程
	if (!pDoc->StartPrcessThread())
	{
		m_wndOutput << "无法启动处理线程，无法实现数据处理" << m_EndOut;
	}
	else
		m_wndOutput << "启动处理线程成功" << m_EndOut;
}

int CCoreModelView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//add init once code here

	return 0;
}

void CCoreModelView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* pHint)
{
	if (pHint)
	{
		if (pHint->IsKindOf(RUNTIME_CLASS(UpdateRng)))
		{
			UpdateRng* rng = (UpdateRng*)pHint;
			if (rng->m_bUpdataFrame)
			{
				//训练或应用数据更新
				rng->m_bUpdataFrame = FALSE;
				//
				SendDatatoWnd();
				return;
			}
			if (rng->m_bUpDateEEG)
				return;
		}
	}
	Invalidate(TRUE);
}

LRESULT CCoreModelView::OnRcvTrainMsg(WPARAM wparam, LPARAM lparam)
{
	CCoreModelDoc* pDoc = GetDocument();
	if (!pDoc)
		return 1;
	switch (wparam)
	{
	case TRAINWND_BITEDONE:
		if (!pDoc->CloseTrainModule())
			m_wndOutput << "移除训练模块失败" << m_EndOut;
		LoadClassifyTrainWnd();										//加载下一个训练窗口
		break;
	case TRAINWND_BITECANCLE:										//处理取消训练
		m_wndOutput << "取消阈值模型训练，程序将自动加载原先模型" << m_EndOut;
		LoadClassifyTrainWnd();
		break;
	case TRAINWND_CLASSIFYDONE:
		if (!pDoc->CloseTrainModule())							//关闭当前模型窗口
			m_wndOutput << "移除分类训练模块失败" << m_EndOut;
		LoadHLTrainWnd();										//加载下一个训练窗口
		break;
	case TRAINWND_CLASSIFYCANCLE:
		m_wndOutput << "取消分类模型训练，程序将自动加载原先模型" << m_EndOut;
		LoadHLTrainWnd();
		break;
	case TRAINWND_HLDONE:
		if (!pDoc->CloseTrainModule())							//关闭当前模型窗口
			m_wndOutput << "移除轻重训练模块失败" << m_EndOut;
		LoadTLTrainWnd();
		break;
	case TRAINWND_HLCANCLE:
		m_wndOutput << "取消轻重模型训练，程序将自动加载原先模型" << m_EndOut;
		LoadTLTrainWnd();
		break;
	case TRAINWND_TLDONE:
		if (!pDoc->CloseTrainModule())							//关闭当前模型窗口
			m_wndOutput << "移除长咬、两次训练模块失败" << m_EndOut;
		LoadAppWnd();
		break;
	case TRAINWND_TLCANCLE:
		m_wndOutput << "取消长咬、两次模型训练，程序将自动加载原先模型" << m_EndOut;
		LoadAppWnd();
	case  TRAINWND_TEST:
		//当前工作模块，进入测试模式
		if (!pDoc->StartPrcessThread())
			m_wndOutput << "无法启动数据处理线程" << m_EndOut;
		else
			m_wndOutput << "启动数据处理线程成功" << m_EndOut;
		break;
	case TRAINWND_TESTFINISH:
		if(!pDoc->SuspendProcessThread())
			m_wndOutput << "无法停止数据处理线程" << m_EndOut;
		else
			m_wndOutput << "停止数据处理线程成功" << m_EndOut;
		break;
	default:
		break;
	}
	return 0;
}


BOOL CCoreModelView::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)						//模拟VK 按下
	{
		CCoreModelDoc* pDoc = GetDocument();
		if (!pDoc)
			return TRUE;
		CBaseAppWnd* pApp = pDoc->GetCurApp();
		if (!pApp)
		{
			m_wndOutput << "应用窗体无效，无法操作" << m_EndOut;
			return TRUE;
		}
		pApp->PreTranslateMessage(pMsg);
		//return TRUE;
	}

	return CView::PreTranslateMessage(pMsg);
}

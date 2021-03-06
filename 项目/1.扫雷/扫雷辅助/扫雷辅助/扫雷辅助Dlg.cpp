
// 扫雷辅助Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "扫雷辅助.h"
#include "扫雷辅助Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//菜单类ID
//#define start 0x208
#define diji 0x209
#define zhongji 0x20A
#define gaoji 0x20B
#define zidingyi 0x20C
//#define 标记 20F
//#define 颜色 211
//#define 声音 20E
//#define 英雄榜 210
//#define 退出 200
//#define 目录 24E
//#define 帮助主题 24F
//#define 使用帮助 250
//#define 关于扫雷 251

//CALL
#define qipanCall  0x1005361	//棋盘call
#define heightCall  0x1005338	//高度call
#define biaozhiCall  0x1005194	//标志旗帜call

#define gameStartX  21	//初始X坐标
#define gameStartY  62	//初始Y坐标

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C扫雷辅助Dlg 对话框



C扫雷辅助Dlg::C扫雷辅助Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C扫雷辅助Dlg::IDD, pParent)
	, m_editBase(0)
	, m_gameData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C扫雷辅助Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editBase);
	DDX_Text(pDX, IDC_EDIT2, m_gameData);
}

BEGIN_MESSAGE_MAP(C扫雷辅助Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHUJI_GAME, &C扫雷辅助Dlg::OnBnClickedButtonChujiGame)
	ON_BN_CLICKED(IDC_BUTTON_ZHONGJI_GAME, &C扫雷辅助Dlg::OnBnClickedButtonZhongjiGame)
	ON_BN_CLICKED(IDC_BUTTON_READ_FLAG, &C扫雷辅助Dlg::OnBnClickedButtonReadFlag)
	ON_BN_CLICKED(IDC_BUTTON_GAOJI_GAME, &C扫雷辅助Dlg::OnBnClickedButtonGaojiGame)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM, &C扫雷辅助Dlg::OnBnClickedButtonCustom)
	ON_BN_CLICKED(IDC_BUTTONC_CLICK, &C扫雷辅助Dlg::OnBnClickedButtoncClick)
	ON_BN_CLICKED(IDC_BUTTON_READ, &C扫雷辅助Dlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_ALL_KILL, &C扫雷辅助Dlg::OnBnClickedButtonAllKill)
END_MESSAGE_MAP()

//提升权限
bool enableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

// C扫雷辅助Dlg 消息处理程序

BOOL C扫雷辅助Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C扫雷辅助Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C扫雷辅助Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C扫雷辅助Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//创建一个通用的传ID进来即可
void C扫雷辅助Dlg::OnBaseOpenMenu(int ID)
{
	HWND h = ::FindWindow(NULL, L"扫雷");
	if (h == 0){
		::MessageBox(0, L"扫雷没打开", 0, MB_OK);
		return;
	}
	::PostMessage(h, WM_COMMAND, ID, 0);
}

//进行初级游戏
void C扫雷辅助Dlg::OnBnClickedButtonChujiGame()
{
	this->OnBaseOpenMenu(diji);
}

//进行中级游戏
void C扫雷辅助Dlg::OnBnClickedButtonZhongjiGame()
{
	this->OnBaseOpenMenu(zhongji);
}

//读取标记数据
void C扫雷辅助Dlg::OnBnClickedButtonReadFlag()
{
	HWND h = ::FindWindow(NULL, L"扫雷");
	if (h == 0){
		::MessageBox(0, L"扫雷没打开", 0, MB_OK);
		return;
	}
	DWORD pid;
	GetWindowThreadProcessId(h, &pid); //获取进程ID并保存
	enableDebugPriv();
	HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, pid);	//打开一个已存在的进程对象
	if (hp == NULL)
	{
		::MessageBox(0, L"打开进程出错", 0, MB_OK);
		return;
	}
	ReadProcessMemory(hp, (LPCVOID)biaozhiCall, &m_editBase, 4, &pid);	//根据进程句柄读入该进程的某个内存空间
	UpdateData(false);
}

//高级游戏
void C扫雷辅助Dlg::OnBnClickedButtonGaojiGame()
{
	this->OnBaseOpenMenu(gaoji);
}

//自定义
void C扫雷辅助Dlg::OnBnClickedButtonCustom()
{
	this->OnBaseOpenMenu(zidingyi);
}

//模拟单击
void C扫雷辅助Dlg::OnBnClickedButtoncClick()
{
	WORD yx[2];
	HWND h = ::FindWindow(NULL, L"扫雷");
	if (h == 0){
		::MessageBox(0, L"扫雷没打开", 0, MB_OK);
		return;
	}
	yx[0] = 21;
	yx[1] = 62;
	::SendMessage(h, WM_LBUTTONDOWN, 1, *(int*)yx);
	::SendMessage(h, WM_LBUTTONUP, 0, *(int*)yx);
}

unsigned char gamebase[24][32]; //ReadProcessMemory 棋盘数据缓冲区

//读取数据
void C扫雷辅助Dlg::OnBnClickedButtonRead()
{
	HWND h = ::FindWindow(NULL, L"扫雷");
	if (h == 0)
	{
		::MessageBox(0, L"游戏未打开", 0, MB_OK);
		return;
	}
	DWORD pid;
	GetWindowThreadProcessId(h, &pid);
	enableDebugPriv();
	HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	//
	if (hp == NULL)
	{
		::MessageBox(0, L"打开进程出错", 0, MB_OK);
		return;
	}
	//读取数据 
	ReadProcessMemory(hp, (LPCVOID)qipanCall, gamebase, 32 * 24, &pid);
	int high;
	//读取高度
	ReadProcessMemory(hp, (void*)heightCall, &high, 4, &pid);
	m_gameData.Empty();
	CString s1;
	//WORD yx[2];
	for (int y = 0; y < high; y++){
		for (int x = 0; x < 32; x++){
			s1.Format(L"%x,", gamebase[y][x]);
			if (gamebase[y][x] == 0x10)
			{
				break;
			}
			if (wcslen(s1) == 2)
			{
				s1 = L"0" + s1;
			}
			m_gameData += s1;
		}
		m_gameData +="\r\n";
	}
	UpdateData(false);
}


void C扫雷辅助Dlg::OnBnClickedButtonAllKill()
{
	HWND h = ::FindWindow(NULL, L"扫雷");
	if (h == 0)
	{
		::MessageBox(0, L"游戏未打开", 0, MB_OK);
		return;
	}
	DWORD pid;
	GetWindowThreadProcessId(h, &pid);
	enableDebugPriv();
	HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	//
	if (hp == NULL)
	{
		::MessageBox(0, L"打开进程出错", 0, MB_OK);
		return;
	}
	//读取数据 
	ReadProcessMemory(hp, (LPCVOID)qipanCall, gamebase, 32 * 24, &pid);
	int high;
	//读取高度
	ReadProcessMemory(hp, (void*)heightCall, &high, 4, &pid);
	CString s1;
	WORD yx[2];
	for (int y = 0; y < high; y++){
		for (int x = 0; x < 32; x++){
			if (gamebase[y][x] != 0x8f)
			{
				//计算雷所在位置坐标
				yx[0] = gameStartX + x * 16;
				yx[1] = gameStartY + y * 16;
				//模拟鼠标单击
				::PostMessage(h, WM_LBUTTONDOWN, 1, *(int*)yx);
				::PostMessage(h, WM_LBUTTONUP, 0, *(int*)yx);
			}
		}
	}
	UpdateData(false);
}

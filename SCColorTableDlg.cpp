
// SCColorTableDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SCColorTable.h"
#include "SCColorTableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSCColorTableDlg 대화 상자



CSCColorTableDlg::CSCColorTableDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCCOLORTABLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSCColorTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RGBA, m_ip_rgba);
	DDX_Control(pDX, IDC_EDIT_ARGB, m_edit_argb);
	DDX_Control(pDX, IDC_EDIT_RGBA, m_edit_rgba);
	DDX_Control(pDX, IDC_EDIT_INT, m_edit_int);
	DDX_Control(pDX, IDC_LIST, m_list0);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}

BEGIN_MESSAGE_MAP(CSCColorTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSCColorTableDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSCColorTableDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	//ON_NOTIFY(IPN_FIELDCHANGED, IDC_RGBA, &CSCColorTableDlg::OnIpnFieldchangedRgba)
	//ON_EN_CHANGE(IDC_EDIT_ARGB, &CSCColorTableDlg::OnEnChangeEditArgb)
	//ON_EN_CHANGE(IDC_EDIT_RGBA, &CSCColorTableDlg::OnEnChangeEditRgba)
	//ON_EN_CHANGE(IDC_EDIT_INT, &CSCColorTableDlg::OnEnChangeEditInt)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CSCColorTableDlg::OnLvnItemChangedList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CSCColorTableDlg::OnNMClickList)
END_MESSAGE_MAP()


// CSCColorTableDlg 메시지 처리기

BOOL CSCColorTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_LIST, 0, 0, 50, 100);
	m_resize.Add(IDC_LIST1, 50, 0, 50, 100);

	init_list();

	RestoreWindowPosition(&theApp, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSCColorTableDlg::init_list()
{
	m_list0.set_headings(_T("Index,40;Name,200;Hex,60;Decimal,100;Value,100"));
	m_list0.load_column_width(&theApp, _T("color table list"));
	m_list0.set_font_size(8);
	m_list0.set_header_height(28);
	m_list0.set_line_height(30);

	m_list1.set_headings(_T("Index,40;Name,200;Hex,60;Decimal,100;Value,100"));
	m_list1.load_column_width(&theApp, _T("color table list"));
	m_list1.set_font_size(8);
	m_list1.set_header_height(28);
	m_list1.set_line_height(30);

	m_cr_list = CSCColorList::get_color_list();

	int index;
	Gdiplus::Color cr;
	CString hex;
	CString rgb;

	for (int i = 0; i < m_cr_list.size(); i++)
	{
		cr = m_cr_list[i].second;
		hex = get_color_string(cr.ToCOLORREF());
		rgb.Format(_T("%d, %d, %d"), cr.GetR(), cr.GetG(), cr.GetB());

		if (i < 66)
		{
			index = m_list0.insert_item(i, -1, i2S(i), CString(m_cr_list[i].first.c_str()), hex, rgb, i2S(cr.GetValue(), true));
			m_list0.set_back_color(index, -1, m_cr_list[i].second);
			m_list0.set_text_color(index, -1, get_distinct_gcolor(m_cr_list[i].second));
		}
		else
		{
			index = m_list1.insert_item(i - 66, -1, i2S(i), CString(m_cr_list[i].first.c_str()), hex, rgb, i2S(cr.GetValue(), true));
			m_list1.set_back_color(index, -1, m_cr_list[i].second);
			m_list1.set_text_color(index, -1, get_distinct_gcolor(m_cr_list[i].second));
		}
	}
}

void CSCColorTableDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSCColorTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSCColorTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSCColorTableDlg::OnBnClickedOk()
{
	CWnd* pWnd = GetFocus();

	if (pWnd == &m_ip_rgba)
		TRACE(_T("m_ip_rgba\n"), pWnd);
	else if (pWnd == &m_edit_argb)
		TRACE(_T("m_edit_argb\n"), pWnd);
	else if (pWnd == &m_edit_rgba)
		TRACE(_T("m_edit_rgba\n"), pWnd);
	else if (pWnd == &m_edit_int)
		TRACE(_T("m_edit_int\n"), pWnd);
}

void CSCColorTableDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CSCColorTableDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

BOOL CSCColorTableDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN :
				OnBnClickedOk();
				return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//a, r, g, b 값을 받아 4개의 폼에 맞게 변형하여 UI를 갱신시키고 해당 색을 리스트에서 찾아서 선택 상태로 표시한다.
void CSCColorTableDlg::fill_color_values(int a, int r, int g, int b)
{
	CString str;

	m_ip_rgba.SetAddress(a, r, g, b);

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_rgba.set_text(str);

	str.Format(_T("%02X%02X%02X%02X"), a, r, g, b);
	m_edit_argb.set_text(str);

	str.Format(_T("%lu"), Gdiplus::Color(a, r, g, b).GetValue());
	m_edit_int.set_text(str);

	//select_color_item()
	//std::string cr_name = CSCColorList::get_color_name(Gdiplus::Color(a, r, g, b));

}

void CSCColorTableDlg::OnIpnFieldchangedRgba(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE r, g, b, a;
	m_ip_rgba.GetAddress(r, g, b, a);

	CString str;

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_rgba.set_text(str);

	str.Format(_T("%02X%02X%02X%02X"), a, r, g, b);
	m_edit_argb.set_text(str);

	str.Format(_T("%d"), Gdiplus::Color(a, r, g, b).GetValue());
	m_edit_int.set_text(str);

	*pResult = 0;
}

void CSCColorTableDlg::OnEnChangeEditArgb()
{
	CString argb = m_edit_argb.get_text();

	if (argb.GetLength() > 1 && argb.Left(1) == _T("#"))
		argb = argb.Mid(1);

	if (argb.GetLength() > 2 && argb.Left(2) == _T("0x"))
		argb = argb.Mid(2);

	if (argb.GetLength() != 8)
		return;

	CString str;
	int a = _ttoi(argb.Left(2));
	int r = _ttoi(argb.Mid(2, 2));
	int g = _ttoi(argb.Mid(4, 2));
	int b = _ttoi(argb.Mid(6, 2));

	m_ip_rgba.SetAddress(a, r, g, b);

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_rgba.set_text(str);

	str.Format(_T("%d"), Gdiplus::Color(a, r, g, b).GetValue());
	m_edit_int.set_text(str);
}

void CSCColorTableDlg::OnEnChangeEditRgba()
{
	CString rgba = m_edit_rgba.get_text();

	if (rgba.GetLength() > 1 && rgba.Left(1) == _T("#"))
		rgba = rgba.Mid(1);

	if (rgba.GetLength() > 2 && rgba.Left(2) == _T("0x"))
		rgba = rgba.Mid(2);

	if (rgba.GetLength() != 8)
		return;

	CString str;
	int r = _ttoi(rgba.Left(2));
	int g = _ttoi(rgba.Mid(2, 2));
	int b = _ttoi(rgba.Mid(4, 2));
	int a = _ttoi(rgba.Mid(6, 2));

	m_ip_rgba.SetAddress(a, r, g, b);

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_argb.set_text(str);

	str.Format(_T("%d"), Gdiplus::Color(a, r, g, b).GetValue());
	m_edit_int.set_text(str);
}

void CSCColorTableDlg::OnEnChangeEditInt()
{
	Gdiplus::ARGB value = _ttoi(m_edit_int.get_text());
	Gdiplus::Color cr(value);

	int r = cr.GetR();
	int g = cr.GetG();
	int b = cr.GetB();
	int a = cr.GetA();

	m_ip_rgba.SetAddress(a, r, g, b);

	CString str;

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_argb.set_text(str);

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_argb.set_text(str);
}

void CSCColorTableDlg::OnLvnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int item = pNMLV->iItem;
	CString text = m_list0.get_text(item, col_value);

	text.Replace(_T(","), _T(""));

	Gdiplus::ARGB value = _ttoi64(text);
	Gdiplus::Color cr(value);
	fill_color_values(cr.GetA(), cr.GetR(), cr.GetG(), cr.GetB());

	*pResult = 0;
}

void CSCColorTableDlg::OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//int item = pNMItemActivate->iItem;
	//CString text = m_list.get_text(item, col_value);
	//text.Replace(_T(","), _T(""));

	//Gdiplus::ARGB value = _ttoi64(text);
	//Gdiplus::Color cr(value);
	//fill_color_values(cr.GetA(), cr.GetR(), cr.GetG(), cr.GetB());

	*pResult = 0;
}

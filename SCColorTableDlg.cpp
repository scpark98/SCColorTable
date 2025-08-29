
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

#define SECOND_LIST_INDEX	66

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
	DDX_Control(pDX, IDC_EDIT_COLOR, m_edit_color);
}

BEGIN_MESSAGE_MAP(CSCColorTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSCColorTableDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSCColorTableDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_RGBA, &CSCColorTableDlg::OnIpnFieldchangedRgba)
	//ON_EN_CHANGE(IDC_EDIT_ARGB, &CSCColorTableDlg::OnEnChangeEditArgb)
	//ON_EN_CHANGE(IDC_EDIT_RGBA, &CSCColorTableDlg::OnEnChangeEditRgba)
	//ON_EN_CHANGE(IDC_EDIT_INT, &CSCColorTableDlg::OnEnChangeEditInt)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CSCColorTableDlg::OnLvnItemChangedList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSCColorTableDlg::OnLvnItemChangedList1)
	ON_REGISTERED_MESSAGE(Message_CSCIPAddressCtrl, &CSCColorTableDlg::on_message_CSCIPAddressCtrl)
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
	m_resize.Add(IDC_EDIT_COLOR, 0, 0, 100, 0);
	m_resize.Add(IDC_LIST, 0, 0, 50, 100);
	m_resize.Add(IDC_LIST1, 50, 0, 50, 100);
	m_resize.SetMinimumTrackingSize(CSize(880, 400));

	m_edit_argb.SetLimitText(8);
	m_edit_rgba.SetLimitText(8);

	//선택된 컬러 이름과 색상을 표시하는 CSCEdit이지만 readonly 속성이고
	//readonly 속성이라도 readonly 색상을 사용하지 않고 지정된 cr_back을 사용해서 표현해야 한다.
	m_edit_color.set_use_readonly_color(false);

	//m_static_color.draw_border(true, 1, Gdiplus::Color::Black);
	//m_edit_color.set_line_align()에서는 height가 달라지지 않는데도 RestoreWindowPosition() 앞에서 호출하면 적용되지 않는다.
	//m_edit_color.set_text_color(gRGB(0, 0, 255));
	//m_edit_color.set_back_color(Gdiplus::Color::RosyBrown);

	//m_edit_color.set_line_align(DT_CENTER | DT_VCENTER);

	init_list();

	RestoreWindowPosition(&theApp, this);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSCColorTableDlg::init_list()
{
	m_list0.SetExtendedStyle(LVS_EX_FULLROWSELECT);// | LVS_EX_DOUBLEBUFFER | LVS_EX_FLATSB);
	m_list0.set_headings(_T("Index,40;Name,200;Hex,60;Decimal,100;Value,100"));
	m_list0.load_column_width(&theApp, _T("color table list"));
	m_list0.set_font_size(8);
	m_list0.set_header_height(28);
	m_list0.set_line_height(30);
	m_list0.set_fixed_width_column(col_name);
	m_list0.set_draw_selected_border(true, 0, 2, Gdiplus::DashStyleDash);
	m_list0.set_use_distinct_border_color();
	m_list0.allow_sort(false);
	m_list0.m_theme.cr_back_selected = Gdiplus::Color::Transparent;

	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);// | LVS_EX_DOUBLEBUFFER | LVS_EX_FLATSB);
	m_list1.set_headings(_T("Index,40;Name,200;Hex,60;Decimal,100;Value,100"));
	m_list1.load_column_width(&theApp, _T("color table list"));
	m_list1.set_font_size(8);
	m_list1.set_header_height(28);
	m_list1.set_line_height(30);
	m_list1.set_fixed_width_column(col_name);
	m_list1.set_draw_selected_border(true, 0, 2, Gdiplus::DashStyleDash);
	m_list1.set_use_distinct_border_color();
	m_list1.allow_sort(false);
	m_list1.m_theme.cr_back_selected = Gdiplus::Color::Transparent;

	m_cr_list = CSCColorList::get_color_list();

	int index;
	Gdiplus::Color cr;
	CString hex;
	CString rgb;

	//총 140개의 컬러를 2개의 리스트에 나눠서 표시한다.
	for (int i = 0; i < m_cr_list.size(); i++)
	{
		cr = m_cr_list[i].second;
		hex = get_color_string(cr.ToCOLORREF());
		rgb.Format(_T("%d, %d, %d"), cr.GetR(), cr.GetG(), cr.GetB());

		if (i < SECOND_LIST_INDEX)
		{
			index = m_list0.insert_item(i, -1, i2S(i), CString(m_cr_list[i].first.c_str()), hex, rgb, i2S(cr.GetValue(), true));
			m_list0.set_back_color(index, -1, m_cr_list[i].second);
			m_list0.set_text_color(index, -1, get_distinct_color(m_cr_list[i].second));
		}
		else
		{
			index = m_list1.insert_item(i - SECOND_LIST_INDEX, -1, i2S(i), CString(m_cr_list[i].first.c_str()), hex, rgb, i2S(cr.GetValue(), true));
			m_list1.set_back_color(index, -1, m_cr_list[i].second);
			m_list1.set_text_color(index, -1, get_distinct_color(m_cr_list[i].second));
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
	Gdiplus::Color cr = Gdiplus::Color::Transparent;

	//IPControl에서 return키를 쳐도 여기에서 걸러지지 않는다. pWnd != &m_ip_rgba로 나온다. 메시지로 처리한다.
	if (pWnd == &m_ip_rgba)
		TRACE(_T("m_ip_rgba\n"));
	else if (pWnd == &m_edit_argb)
	{
		TRACE(_T("m_edit_argb\n"));
		cr.SetValue(_tcstoui64(m_edit_argb.get_text(), NULL, 16));
	}
	else if (pWnd == &m_edit_rgba)
	{
		TRACE(_T("m_edit_rgba\n"));
		CString text = m_edit_rgba.get_text();
		CString alpha = text.Right(2);
		text = alpha + text.Left(6);
		cr.SetValue(_tcstoui64(text, NULL, 16));
	}
	else if (pWnd == &m_edit_int)
	{
		TRACE(_T("m_edit_int\n"));
		cr.SetValue(_ttoi64(m_edit_int.get_text()));
	}

	fill_color_values(cr.GetR(), cr.GetG(), cr.GetB(), cr.GetA(), true);
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
				CSCColorTableDlg::OnBnClickedOk();
				return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//a, r, g, b 값을 받아 4개의 폼에 맞게 변형하여 UI를 갱신시키고 해당 색을 리스트에서 찾아서 선택 상태로 표시한다.
void CSCColorTableDlg::fill_color_values(int r, int g, int b, int a, bool find_list)
{
	CString str;

	m_ip_rgba.SetAddress(r, g, b, a);

	str.Format(_T("%02X%02X%02X%02X"), r, g, b, a);
	m_edit_rgba.set_text(str);

	str.Format(_T("%02X%02X%02X%02X"), a, r, g, b);
	m_edit_argb.set_text(str);

	str.Format(_T("%lu"), Gdiplus::Color(a, r, g, b).GetValue());
	m_edit_int.set_text(str);

	//select_color_item()
	CString cr_name(CSCColorList::get_color_name(Gdiplus::Color(a, r, g, b), false).c_str());
	//TRACE(_T("name = %s\n"), cr_name);

	m_edit_color.set_color(get_distinct_color(Gdiplus::Color(a, r, g, b)), Gdiplus::Color(a, r, g, b));
	m_edit_color.set_text(cr_name);

	if (!find_list)
		return;

	//유사한 색인 경우는 EnsureVisible()만 해주고
	int pos = cr_name.Find(_T("near : "));
	int index;

	if (pos == 0)
	{
		cr_name.Replace(_T("near : "), _T(""));
		index = m_list0.find(cr_name, NULL, 0, -1, true, true, false, 1);

		if (index >= 0)
		{
			m_list0.EnsureVisible(index, FALSE);
		}
		else
		{
			index = m_list1.find(cr_name, NULL, 0, -1, true, true, false, 1);

			if (index >= 0)
				m_list1.EnsureVisible(index, FALSE);
		}
	}
	//리스트에 존재하는 색인 경우는 선택상태로 표시한다.
	else
	{
		index = m_list0.find(cr_name, NULL, 0, -1, true, true, false, 1);
		if (index >= 0)
		{
			m_list1.select_item(-1, false, true, false);
			m_list0.select_item(index, true, true);
			m_list0.SetFocus();
		}
		else
		{
			index = m_list1.find(cr_name, NULL, 0, -1, true, true, false, 1);
			if (index >= 0)
			{
				m_list0.select_item(-1, false, true);
				m_list1.select_item(index, true, true);
				m_list1.SetFocus();
			}
		}
	}
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

	str.Format(_T("%u"), Gdiplus::Color(a, r, g, b).GetValue());
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
	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int item = pNMLV->iItem;
		CString text = m_list0.get_text(item, col_value);

		text.Replace(_T(","), _T(""));

		Gdiplus::ARGB value = _ttoi64(text);
		Gdiplus::Color cr(value);
		fill_color_values(cr.GetR(), cr.GetG(), cr.GetB(), cr.GetA(), false);
	}

	*pResult = 0;
}

void CSCColorTableDlg::OnLvnItemChangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int item = pNMLV->iItem;
		CString text = m_list1.get_text(item, col_value);

		text.Replace(_T(","), _T(""));

		Gdiplus::ARGB value = _ttoi64(text);
		Gdiplus::Color cr(value);
		fill_color_values(cr.GetR(), cr.GetG(), cr.GetB(), cr.GetA(), false);
	}

	*pResult = 0;
}

LRESULT CSCColorTableDlg::on_message_CSCIPAddressCtrl(WPARAM wParam, LPARAM lParam)
{
	int message = (int)wParam;

	if (message == WM_KEYDOWN)
	{
		TRACE(_T("key down message on CSCIPAdddressCtrl\n"));
	}
	else if (message == WM_KILLFOCUS)
	{
		TRACE(_T("WM_KILLFOCUS message on CSCIPAdddressCtrl\n"));
	}
	else if (message == IPN_FIELDCHANGED)
	{
		TRACE(_T("IPN_FIELDCHANGED message on CSCIPAdddressCtrl\n"));
		return 0;
	}

	BYTE r, g, b, a;
	m_ip_rgba.GetAddress(r, g, b, a);
	fill_color_values(r, g, b, a, true);

	return 0;
}
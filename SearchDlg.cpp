// SearchDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SCColorTable.h"
#include "afxdialogex.h"
#include "SearchDlg.h"


// CSearchDlg 대화 상자

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCH, pParent)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_KEYWORD, m_combo_search);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSearchDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSearchDlg 메시지 처리기

BOOL CSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_combo_search.set_line_height(12);
	m_combo_search.load_history(&theApp, _T("CSearchDlg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSearchDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_text = m_combo_search.get_text();
	m_combo_search.add(m_text);
	CDialogEx::OnOK();
}

void CSearchDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_search.save_history(&theApp, _T("CSearchDlg"));
	CDialogEx::OnCancel();
}

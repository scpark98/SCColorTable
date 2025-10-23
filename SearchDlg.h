#pragma once
#include "afxdialogex.h"

#include "Common/CComboBox/SCComboBox/SCComboBox.h"

// CSearchDlg 대화 상자

class CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSearchDlg();

	CString		m_text;
	CString		get_text() { return m_text; }

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CSCComboBox m_combo_search;
};

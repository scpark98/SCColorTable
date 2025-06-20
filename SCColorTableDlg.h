
// SCColorTableDlg.h: 헤더 파일
//

#pragma once

#include "../Common/ResizeCtrl.h"
#include "../Common/CEdit/SCEdit/SCEdit.h"
#include "../Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"

// CSCColorTableDlg 대화 상자
class CSCColorTableDlg : public CDialogEx
{
// 생성입니다.
public:
	CSCColorTableDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCCOLORTABLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	enum LIST_COLUMN_ID
	{
		col_index = 0,
		col_name,
		col_hex,
		col_decimal,
		col_value,
	};

	CResizeCtrl				m_resize;
	std::vector<std::pair<std::string, Gdiplus::Color>> m_cr_list;

	void					init_list();


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl m_ip_rgba;
	CSCEdit m_edit_argb;
	CSCEdit m_edit_rgba;
	CSCEdit m_edit_int;
	CVtListCtrlEx m_list;
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnIpnFieldchangedRgba(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditArgb();
	afx_msg void OnEnChangeEditRgba();
	afx_msg void OnEnChangeEditInt();
};

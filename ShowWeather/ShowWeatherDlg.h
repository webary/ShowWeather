
// ShowWeatherDlg.h : 头文件
//

#pragma once


// CShowWeatherDlg 对话框
class CShowWeatherDlg : public CDialogEx
{
// 构造
public:
	CShowWeatherDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWWEATHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    bool finished;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    void prase();
    CString m_show;
    CComboBox m_cmb;
};

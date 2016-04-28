
// ShowWeatherDlg.h : 头文件
//

#pragma once
#include <string>
#include <map>
#include "afxwin.h"
using std::string;
using std::map;

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

    void prase(int city_id);
    void loadCityIdFromFile(const string & file);
    inline int getCityId(const string & _provice, const string & _city_ch);
    inline void joinURL(int city_id);

    map<string, map<string, int> > city_cn_id_Map;
    CString m_url;
    CString m_show;
    CComboBox m_cmb_city;
    CComboBox m_cmb_provice;    // 省份
    afx_msg void OnCbnSelChangeComboProvice();
};

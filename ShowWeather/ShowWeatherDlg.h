
// ShowWeatherDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <map>
#include "afxwin.h"
using std::string;
using std::map;

// CShowWeatherDlg �Ի���
class CShowWeatherDlg : public CDialogEx
{
// ����
public:
	CShowWeatherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWWEATHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    bool finished;

	// ���ɵ���Ϣӳ�亯��
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
    CComboBox m_cmb_provice;    // ʡ��
    afx_msg void OnCbnSelChangeComboProvice();
};

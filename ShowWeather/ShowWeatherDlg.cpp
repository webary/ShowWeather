
// ShowWeatherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowWeather.h"
#include "ShowWeatherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <atlbase.h>
#include <afxinet.h>
#include <wininet.h>
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <string>
using namespace std;
#pragma comment(lib,"WinInet.lib")


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum {
        IDD = IDD_ABOUTBOX
    };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CShowWeatherDlg 对话框



CShowWeatherDlg::CShowWeatherDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_SHOWWEATHER_DIALOG, pParent)
    , m_show(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowWeatherDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_show);
    DDX_Control(pDX, IDC_COMBO1, m_cmb);
}

BEGIN_MESSAGE_MAP(CShowWeatherDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CShowWeatherDlg::OnBnClickedOk)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CShowWeatherDlg 消息处理程序

BOOL CShowWeatherDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_cmb.AddString("上海市");
    m_cmb.SetCurSel(0);
    SetTimer(0, 1, NULL);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShowWeatherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShowWeatherDlg::OnPaint()
{
    if (IsIconic()) {
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
    else {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CShowWeatherDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CShowWeatherDlg::OnBnClickedOk()
{
    m_show = "正在查询，请稍后...";
    UpdateData(FALSE);
    prase();
}


void CShowWeatherDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent) {
        case 0:
            KillTimer(0);
            prase();
            break;
    }
    CDialogEx::OnTimer(nIDEvent);
}

void CShowWeatherDlg::prase()
{
    CInternetSession session;
    CString strServer, strObject;
    INTERNET_PORT wPort;
    DWORD dwType;
    if (!AfxParseURL(_T("http://api.openweathermap.org/data/2.5/forecast?id=1796236&mode=xml&APPID=aba637fa40511e6c11f801b29414a4ab"), dwType, strServer, strObject, wPort))
        return;//URL解析错误
    CHttpConnection* pHttpConnection = session.GetHttpConnection(strServer, wPort);
    CHttpFile *pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
    pHttpFile->SendRequest();
    DWORD dwRet;
    pHttpFile->QueryInfoStatusCode(dwRet);
    CString str = "";
    if (dwRet == HTTP_STATUS_OK) {
        CString tmp;
        while (pHttpFile->ReadString(tmp) > 0) {
            str += tmp;
        }
    }

    map<string, double> date_tp;
    map<string, double> date_tp_max;
    map<string, double> date_tp_min;
    tinyxml2::XMLDocument doc;
    doc.Parse(str.GetBuffer());
    tinyxml2::XMLElement* node_forcast = doc.FirstChildElement()->FirstChildElement("forecast");
    tinyxml2::XMLElement* node_time = node_forcast->FirstChildElement();
    string lastDate;
    int cnt = 0;
    while (1) {
        string date = node_time->Attribute("from");
        date = date.substr(0, 10);
        if (date_tp.empty())
            lastDate = date;
        tinyxml2::XMLElement* tpElem = node_time->FirstChildElement("temperature");
        double tp;
        tpElem->QueryAttribute("value", &tp);
        if (date_tp.find(date) != date_tp.end()) { //找到
            date_tp[date] += tp;
            ++cnt;
            if (tp > date_tp_max[date])
                date_tp_max[date] = tp;
            else if (tp < date_tp_min[date])
                date_tp_min[date] = tp;
        }
        else {
            if (lastDate != date)
                date_tp[lastDate] = (int)(date_tp[lastDate] / cnt); //上一天的平均气温
            date_tp[date] = date_tp_max[date] = date_tp_min[date] = tp;
            cnt = 1;
        }
        lastDate = date;
        if (node_time == node_forcast->LastChildElement())
            break;
        else
            node_time = node_time->NextSiblingElement();
    }
    date_tp[lastDate] = (int)(date_tp[lastDate] / cnt); //上一天的平均气温

    m_show = "日期\t\t平均温度℃\t最低气温\t最高气温";
    for (auto &elem : date_tp) {
        auto date = elem.first;
        m_show.Format("%s\r\n%s\t%d\t\t%d\t%d", m_show, date.c_str(), (int)elem.second, (int)date_tp_min[date], (int)date_tp_max[date]);
    }
    UpdateData(FALSE);
}

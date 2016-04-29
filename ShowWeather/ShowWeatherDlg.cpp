
// ShowWeatherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowWeather.h"
#include "ShowWeatherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <afxinet.h>
#include <iostream>
#include <fstream>
#include <cstring>

#include "tinyxml2/tinyxml2.h"
#include "rapidjson/document.h"

using namespace std;
#pragma comment(lib,"WinInet.lib")


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
    {
    }
#ifdef AFX_DESIGN_TIME
    enum {
        IDD = IDD_ABOUTBOX
    };
#endif
};

// CShowWeatherDlg 对话框

CShowWeatherDlg::CShowWeatherDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_SHOWWEATHER_DIALOG, pParent)
    , m_show("日期\t\t平均温度℃　　最低气温\t最高气温")
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowWeatherDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_show);
    DDX_Control(pDX, IDC_COMBO_CITY, m_cmb_city);
    DDX_Control(pDX, IDC_COMBO_PROVICE, m_cmb_provice);
}

BEGIN_MESSAGE_MAP(CShowWeatherDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDOK, &CShowWeatherDlg::OnBnClickedOk)
    ON_CBN_SELCHANGE(IDC_COMBO_PROVICE, &CShowWeatherDlg::OnCbnSelChangeComboProvice)
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

    SetTimer(1, 1, NULL);
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CShowWeatherDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CShowWeatherDlg::OnBnClickedOk()
{
    CString provice, city;
    m_cmb_provice.GetLBText(m_cmb_provice.GetCurSel(), provice);
    m_cmb_city.GetLBText(m_cmb_city.GetCurSel(), city);
    SetDlgItemText(IDC_STATIC_TIPS, "正在查询该城市的天气情况...");
    UpdateData(FALSE);
    int city_id = getCityId(provice.GetBuffer(), city.GetBuffer());
    if (city_id == 0) {
        ofstream ferr("err.log", std::ios::app);
        ferr << CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S\t").GetBuffer()
            << "provice: " << provice.GetBuffer() << "\tcity: " << city.GetBuffer()
            << endl;
        ferr.close();
        MessageBox("访问服务器出错，请稍后重试！");
    }
    else {
        prase(city_id);
    }
    SetDlgItemText(IDC_STATIC_TIPS, "该城市最近14天内的天气情况如下：");
}

void CShowWeatherDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent) {
        case 1:
            KillTimer(1);
            loadCityIdFromFile("city_cn_id.txt");
            for (auto& pro : city_cn_id_Map)
                m_cmb_provice.AddString(pro.first.c_str());
            m_cmb_provice.SetCurSel(11);
            CString provice;
            m_cmb_provice.GetLBText(m_cmb_provice.GetCurSel(), provice);
            auto& it = city_cn_id_Map.find(provice.GetBuffer());
            for (auto& city : it->second)
                m_cmb_city.AddString(city.first.c_str());
            m_cmb_city.SetCurSel(6);
            OnBnClickedOk();
            break;
    }
    CDialogEx::OnTimer(nIDEvent);
}

inline void CShowWeatherDlg::joinURL(int city_id)
{
    /*
    @id是要查询的城市的id，可在http://bulk.openweathermap.org/sample/中获取
    @cnt传递我们需要获取最近多少天的天气情况
    @mode：服务器回传的数据格式
    @units：温度的单位
    @APPID：个人注册后生成的id，用于用户授权和权限控制
    */
    m_url.Format("http://api.openweathermap.org/data/2.5/forecast/daily?id=%d&cnt=%d&mode=xml&units=Metric&APPID=aba637fa40511e6c11f801b29414a4ab", city_id, 14);
}

void CShowWeatherDlg::prase(int city_id)
{
    joinURL(city_id);
    CInternetSession session1;
    CStdioFile *pFile = session1.OpenURL(m_url); //访问指定的网址
    CString strHtml = "", strTemp;
    while (pFile->ReadString(strTemp)) //获取返回的结果代码
        strHtml += strTemp;

    map<string, int> date_tp, date_tp_max, date_tp_min;
    tinyxml2::XMLDocument doc;
    doc.Parse(strHtml.GetBuffer()); //解析该xml格式内容
    tinyxml2::XMLElement* node_forcast = doc.FirstChildElement()->FirstChildElement("forecast");
    tinyxml2::XMLElement* node_time = node_forcast->FirstChildElement();
    while (1) {
        string date = node_time->Attribute("day");
        tinyxml2::XMLElement* tpElem = node_time->FirstChildElement("temperature");
        double tp;
        tpElem->QueryAttribute("day", &tp);
        date_tp[date] = int(tp + .5); //四舍五入得到一个整型的温度
        tpElem->QueryAttribute("min", &tp);
        date_tp_min[date] = int(tp + .5);
        tpElem->QueryAttribute("max", &tp);
        date_tp_max[date] = int(tp + .5);
        if (node_time == node_forcast->LastChildElement())
            break;
        node_time = node_time->NextSiblingElement();
    }

    m_show = "日期\t\t平均温度℃　　最低气温\t最高气温";
    for (auto& elem : date_tp) {
        auto& date = elem.first;
        m_show.Format("%s\r\n%s\t%d\t　　　　%d\t 　%d", m_show, date.c_str(), elem.second, date_tp_min[date], date_tp_max[date]);
    }
    UpdateData(FALSE);
}

void CShowWeatherDlg::loadCityIdFromFile(const string& file)
{
    ifstream fin(file.c_str()); //中文城市名~id
    if (fin.is_open()) {
        string provice, city;
        int id;
        while (fin >> city >> id) {
            if (city[0] == '=') {
                city = city.substr(1);
                provice = city;
            }
            else
                city_cn_id_Map[provice][city] = id;
        }
        fin.close();
    }
}

inline int CShowWeatherDlg::getCityId(const string& _provice, const string& _city_ch)
{
    if (city_cn_id_Map.find(_provice) != city_cn_id_Map.end() &&
        city_cn_id_Map[_provice].find(_city_ch) != city_cn_id_Map[_provice].end())
        return city_cn_id_Map[_provice][_city_ch];
    return 0;
}


void CShowWeatherDlg::OnCbnSelChangeComboProvice()
{
    CString provice;
    m_cmb_provice.GetLBText(m_cmb_provice.GetCurSel(), provice);
    auto& it = city_cn_id_Map.find(provice.GetBuffer());
    m_cmb_city.ResetContent();
    for (auto& city : it->second)
        m_cmb_city.AddString(city.first.c_str());
    m_cmb_city.SetCurSel(0);
}

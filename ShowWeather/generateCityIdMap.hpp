#ifndef _GENERATE_CITY_ID_HPP_
#define _GENERATE_CITY_ID_HPP_

/*
 @author    : webary (https://github.com/webary/)
 @time      : 2016-4-29 13:35
 @usage     : 使用rapidjson开源库(https://github.com/webary/rapidjson)解析json文件
              "city.list.json",并依赖于"city_cn.txt"文件中的中文城市名和对应的拼音
              合并之后生成一个中文城市名到对应ID的映射表,映射表先找到省份,再访问该
              省份中的所有城市的ID.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "rapidjson/document.h"
using std::string;
using std::map;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::cin;
using std::cout;

typedef map<string, map<string, int> > City_Id_Type;

//将一个string对象内容全部变为小写
inline string tolower(const string& _str)
{
    string low = _str;
    for (auto& elem : low)
        elem = tolower(elem);
    return low;
}

/**
将[英文城市名~id]文件和[中文城市名~拼音]文件合并得到城市中文名和id的映射表,
并返回该表. 在读取[英文城市名~id]文件时需要调用rapidjson开源库解析该json文件.
本函数需读取文件"city.list.json"和"city_cn.txt",并生成"city_cn_id.txt"
**/
inline City_Id_Type generateCityIdMap()
{
    rapidjson::Document doc;
    map<string, int> city_en_id; //<城市名(英文|拼音),id>映射表
    ifstream fin1("city.list.json"); //所有的城市名(英文|拼音)~id
    if (fin1.is_open()) {
        string line;
        while (getline(fin1, line)) { //成功读取到一行
            if (line.find("\"country\":\"CN\"") != line.npos) { //查找中国的城市
                doc.Parse(line.c_str()); //解析json内容
                city_en_id[tolower(doc["name"].GetString())] = doc["_id"].GetInt();
            }
        }
        fin1.close();
    }

    City_Id_Type city_cn_id;//<中文城市名,id>映射表
    ifstream fin2("city_cn.txt"); //中文城市名~拼音
    if (fin2.is_open()) {
        string cn, en, provice;
        while (fin2 >> cn >> en) {
            if (cn[0] == '=') {
                cn = cn.substr(1);
                provice = cn;
            }
            en = tolower(en);
            if (city_en_id.find(en) != city_en_id.end()) { //找到这个城市的id
                city_cn_id[provice][cn] = city_en_id[en];
            } else if (city_en_id.find(en + " sheng") != city_en_id.end()) {
                city_cn_id[provice][cn] = city_en_id[en + " sheng"];
            }
        }
        fin2.close();
    }

    ofstream fout("city_cn_id.txt"); //将中文城市名~id映射存储到文件
    for (auto& it : city_cn_id) {
        fout << "=" << it.first << "\t" << it.second.size() << endl;
        for (auto& e : it.second)
            fout << "  " << e.first << "\t" << e.second << endl;
    }
    fout.close();

    return city_cn_id;
}

//从文件中加载城市中文名和id的映射表,并返回该表
inline City_Id_Type loadCityIdFromFile(const string& file)
{
    City_Id_Type city_cn_id;
    ifstream fin(file.c_str()); //中文城市名~id
    if (fin.is_open()) {
        string provice, city;
        int id;
        while (fin >> city >> id) {
            if (city[0] == '=') {
                city = city.substr(1);
                provice = city;
            } else
                city_cn_id[provice][city] = id;
        }
        fin.close();
    }
    return city_cn_id;
}

//在给定map中查找指定省份的指定城市的id
inline int getCityId(City_Id_Type& _map, const string& _provice, const string& _city)
{
    if (_map.find(_provice) != _map.end() &&
            _map[_provice].find(_city) != _map[_provice].end())
        return _map[_provice][_city];
    return 0;
}

//对上面几个函数的简单测试
inline void test()
{
    City_Id_Type city_cn_id1 = generateCityIdMap();
    City_Id_Type city_cn_id2 = loadCityIdFromFile("city_cn_id.txt");
#ifndef __AFXWIN_H__
    cout << "已存储完毕，请输入省份和城市验证id: " << endl;
    string provice, city;
    while (cin >> provice >> city )
        cout << getCityId(city_cn_id2, provice, city) << endl;
#endif // __AFXWIN_H__
}

#endif // !_GENERATE_CITY_ID_HPP_

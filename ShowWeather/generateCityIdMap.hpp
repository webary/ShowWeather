#ifndef _GENERATE_CITY_ID_HPP_
#define _GENERATE_CITY_ID_HPP_

/*
 @author    : webary (https://github.com/webary/)
 @time      : 2016-4-29 13:35
 @usage     : ʹ��rapidjson��Դ��(https://github.com/webary/rapidjson)����json�ļ�
              "city.list.json",��������"city_cn.txt"�ļ��е����ĳ������Ͷ�Ӧ��ƴ��
              �ϲ�֮������һ�����ĳ���������ӦID��ӳ���,ӳ������ҵ�ʡ��,�ٷ��ʸ�
              ʡ���е����г��е�ID.
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

//��һ��string��������ȫ����ΪСд
inline string tolower(const string& _str)
{
    string low = _str;
    for (auto& elem : low)
        elem = tolower(elem);
    return low;
}

/**
��[Ӣ�ĳ�����~id]�ļ���[���ĳ�����~ƴ��]�ļ��ϲ��õ�������������id��ӳ���,
�����ظñ�. �ڶ�ȡ[Ӣ�ĳ�����~id]�ļ�ʱ��Ҫ����rapidjson��Դ�������json�ļ�.
���������ȡ�ļ�"city.list.json"��"city_cn.txt",������"city_cn_id.txt"
**/
inline City_Id_Type generateCityIdMap()
{
    rapidjson::Document doc;
    map<string, int> city_en_id; //<������(Ӣ��|ƴ��),id>ӳ���
    ifstream fin1("city.list.json"); //���еĳ�����(Ӣ��|ƴ��)~id
    if (fin1.is_open()) {
        string line;
        while (getline(fin1, line)) { //�ɹ���ȡ��һ��
            if (line.find("\"country\":\"CN\"") != line.npos) { //�����й��ĳ���
                doc.Parse(line.c_str()); //����json����
                city_en_id[tolower(doc["name"].GetString())] = doc["_id"].GetInt();
            }
        }
        fin1.close();
    }

    City_Id_Type city_cn_id;//<���ĳ�����,id>ӳ���
    ifstream fin2("city_cn.txt"); //���ĳ�����~ƴ��
    if (fin2.is_open()) {
        string cn, en, provice;
        while (fin2 >> cn >> en) {
            if (cn[0] == '=') {
                cn = cn.substr(1);
                provice = cn;
            }
            en = tolower(en);
            if (city_en_id.find(en) != city_en_id.end()) { //�ҵ�������е�id
                city_cn_id[provice][cn] = city_en_id[en];
            } else if (city_en_id.find(en + " sheng") != city_en_id.end()) {
                city_cn_id[provice][cn] = city_en_id[en + " sheng"];
            }
        }
        fin2.close();
    }

    ofstream fout("city_cn_id.txt"); //�����ĳ�����~idӳ��洢���ļ�
    for (auto& it : city_cn_id) {
        fout << "=" << it.first << "\t" << it.second.size() << endl;
        for (auto& e : it.second)
            fout << "  " << e.first << "\t" << e.second << endl;
    }
    fout.close();

    return city_cn_id;
}

//���ļ��м��س�����������id��ӳ���,�����ظñ�
inline City_Id_Type loadCityIdFromFile(const string& file)
{
    City_Id_Type city_cn_id;
    ifstream fin(file.c_str()); //���ĳ�����~id
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

//�ڸ���map�в���ָ��ʡ�ݵ�ָ�����е�id
inline int getCityId(City_Id_Type& _map, const string& _provice, const string& _city)
{
    if (_map.find(_provice) != _map.end() &&
            _map[_provice].find(_city) != _map[_provice].end())
        return _map[_provice][_city];
    return 0;
}

//�����漸�������ļ򵥲���
inline void test()
{
    City_Id_Type city_cn_id1 = generateCityIdMap();
    City_Id_Type city_cn_id2 = loadCityIdFromFile("city_cn_id.txt");
#ifndef __AFXWIN_H__
    cout << "�Ѵ洢��ϣ�������ʡ�ݺͳ�����֤id: " << endl;
    string provice, city;
    while (cin >> provice >> city )
        cout << getCityId(city_cn_id2, provice, city) << endl;
#endif // __AFXWIN_H__
}

#endif // !_GENERATE_CITY_ID_HPP_

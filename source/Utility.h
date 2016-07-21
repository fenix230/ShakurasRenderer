#pragma once
#include <vector>
#include <string>
#include <memory>
#include <array>


#define SHAKURAS_BEGIN namespace shakuras {
#define SHAKURAS_END }


SHAKURAS_BEGIN;


#define SHAKURAS_SHARED_PTR(C) typedef std::shared_ptr<C> C##Ptr


template<class T>
void unstable_erase(std::vector<T>& vec, size_t pos, size_t count) {
	size_t len = vec.size();
	size_t bpos = len - count;
	std::swap_ranges(vec.begin() + pos, vec.begin() + pos + count, vec.begin() + bpos);
	vec.erase(vec.begin() + bpos, vec.end());
}


typedef void(*pfnprogress)(int v);//0~100


//�ж��ض���ͷ��β
bool startwith(const std::string& str, const std::string& s);
bool endswith(const std::string& str, const std::string& e);


//ȥ����ͷ
std::string& rmstart(std::string& str, std::string s);
std::string rmstartcopy(std::string str, std::string s);
std::string& trimstart(std::string& str, char c);
//ȥ����β
std::string& rmends(std::string& str, std::string e);
std::string rmendscopy(std::string str, std::string e);
std::string& trimends(std::string& str, char c);


//�����һ��б�߷ֿ������ڷ���Ŀ¼���ļ�
std::array<std::string, 2> pathsplit(std::string path);
//���
std::string pathjoin(std::string dn, std::string fn);


//�����һ��.�ֿ�
std::array<std::string, 2> dotsplit(std::string path);
//���
std::string dotjoin(std::string fn, std::string ext);


//�����ļ����ļ���
bool pathexist(std::string path);
bool isfile(std::string path);
bool isdir(std::string path);


//��·�������ļ���
void makedirs(std::string path);
//�ݹ�ɾ���ļ�������
void removedirs(std::string dn);


//�����ļ�
void copyfile(std::string src, std::string dest);
//�ݹ鸴���ļ���
void copydir(std::string src, std::string dest, pfnprogress pfn = NULL);


//�о��ļ����µ��ļ�ȫ·��
void getfiles(std::string dn, std::string filter, std::vector<std::string>& fps);
//�ݹ��о��ļ����µ��ļ�ȫ·��
void getfilesrec(std::string dn, std::string filter, std::vector<std::string>& fps);
//�о��ļ����µ��ļ���ȫ·��
void getfolders(std::string dn, std::vector<std::string>& fps);


//תСд�ҡ�\\��->'/'
void makestdpath(std::string& fp);


//ȡ���·��
std::string relpath(std::string prefix, std::string fp);


SHAKURAS_END;
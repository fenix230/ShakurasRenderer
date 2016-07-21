#include "Utility.h"
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>



SHAKURAS_BEGIN;


bool startwith(const std::string& str, const std::string& s)
{
	if (str.size() >= s.size())
	{
		return str.substr(0, s.size()) == s;
	}
	return false;
}
bool endswith(const std::string& str, const std::string& e)
{
	if (str.size() >= e.size())
	{
		return str.substr(str.size()-e.size()) == e;
	}
	return false;
}


std::string& rmstart(std::string& str, std::string s)
{
	while (!str.empty() && !s.empty() && str.front() == s.front())
	{
		str.erase(str.begin());
		s.erase(s.begin());
	}
	return str;
}
std::string rmstartcopy(std::string str, std::string s)
{
	return rmstart(str, s);
}
std::string& trimstart(std::string& str, char c)
{
	while (!str.empty() && str.front() == c)
	{
		str.erase(str.begin());
	}
	return str;
}


std::string& rmends(std::string& str, std::string e)
{
	while (!str.empty() && !e.empty() && str.back() == e.back())
	{
		str.pop_back();
		e.pop_back();
	}
	return str;
}
std::string rmendscopy(std::string str, std::string e)
{
	return rmends(str, e);
}
std::string& trimends(std::string& str, char c)
{
	while (!str.empty() && str.back() == c)
	{
		str.pop_back();
	}
	return str;
}


bool pathexist(std::string path)
{
	return (_access(path.c_str(), 4) == 0);
}

bool isfile(std::string path)
{
	struct _stat st;
	int res = _stat(path.c_str(), &st);
	return (res == 0 && _S_IFREG & st.st_mode);
}

bool isdir(std::string path)
{
	struct _stat st;
	int res = _stat(path.c_str(), &st);
	return (res == 0 && _S_IFDIR & st.st_mode);
}


void makedirs(std::string path)
{
	std::string sect;
	for (size_t i = 0; i != path.size(); i++)
	{
		sect.push_back(path[i]);
		if ((path[i] == '\\' || path[i] == '/') && !pathexist(sect))
		{
			_mkdir(sect.c_str());
		}
	}
}


void removedirs(std::string dn)
{
	if (_access(dn.c_str(), 4) != 0) return;

	rmends(dn, "\\");
	rmends(dn, "/");

	char dn2[FILENAME_MAX];
	sprintf_s(dn2, "%s/*", dn.c_str());

	//��һ��
	_finddata_t fb;
	intptr_t handle = _findfirst(dn2, &fb); 
	if (handle != 0 && handle != -1L)
	{ 
		int res = 0;
		while (0 == res) 
		{ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0)
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib != 16) 
			{ 
				remove(path);
			} 

			res = _findnext(handle, &fb);
		} 

		_findclose(handle); 
	}

	//����
	sprintf_s(dn2, "%s/*", dn.c_str());

	handle = _findfirst(dn2, &fb); 
	if (handle != 0 && handle != -1L)
	{ 
		int res = 0;
		while (0 == res) 
		{ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0)
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib == 16) 
			{ 
				removedirs(path);
			} 

			res = _findnext(handle, &fb);
		} 

		_findclose(handle); 
	}

	_rmdir(dn.c_str());
}


void copyfile(std::string src, std::string dest)
{
	FILE* fsrc = NULL;
	errno_t e = fopen_s(&fsrc, src.c_str(), "rb"); //�Զ�ȡ�����Ƶķ�ʽ��Դ�ļ�
	if (fsrc == NULL)
	{
		return;
	}

	makedirs(dest);
	FILE* fdest = NULL;
	e = fopen_s(&fdest, dest.c_str(), "wb"); // //��д������Ƶķ�ʽ��Ŀ���ļ�

	int c = 0;
	while((c=fgetc(fsrc)) != EOF)
	{
		fputc(c, fdest);
	}

	fclose(fsrc); //�ر��ļ�ָ�룬�ͷ��ڴ�
	fclose(fdest);
}


void copydir(std::string src, std::string dest, pfnprogress pfn)
{
	std::replace(src.begin(), src.end(), '\\', '/');
	rmends(src, "/");
	src += "/";

	std::replace(dest.begin(), dest.end(), '\\', '/');
	rmends(dest, "/");
	dest += "/";

	std::vector<std::string> fps;
	getfilesrec(src, "*.*", fps);

	int nprog = -1;
	for (size_t i = 0; i != fps.size(); i++)
	{
		std::string fn = fps[i];
		rmstart(fn, src);
		std::string fpdst = dest+fn;
		copyfile(fps[i], fpdst);

		int nprog_i = (int)(100*double(i)/double(fps.size()));
		if (pfn && nprog_i > nprog)
		{
			pfn(nprog_i);
			nprog = nprog_i;
		}
	}
}


void getfiles(std::string dn, std::string filter, std::vector<std::string>& fps)
{
	if (_access(dn.c_str(), 4) != 0) return;

	rmends(dn, "\\");
	rmends(dn, "/");

	char dn2[FILENAME_MAX];
	sprintf_s(dn2, "%s/%s", dn.c_str(), filter.c_str());

	_finddata_t fb; //������ͬ�����ļ��Ĵ洢�ṹ�� 
	intptr_t handle = _findfirst(dn2, &fb); 
	//�ҵ���һ��ƥ����ļ� 
	if (handle != 0 && handle != -1L) 
	{ 
		//�����Լ����ҵ�ƥ����ļ�������ִ�� 
		int res = 0;
		while (0 == res) 
		{ 
			//windows�£����и�ϵͳ�ļ�����Ϊ�� ..�� ,������������ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0) 
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib == 16) 
			{ 
				res = _findnext(handle, &fb);
				continue;
			} 
			//���ļ��е��ļ�
			else 
			{ 
				fps.push_back(path); 
			} 

			res = _findnext(handle, &fb);
		} 
		//�ر��ļ��У�ֻ�йر��˲���ɾ����������������˺ܾã���׼c���õ���closedir 
		//������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ����� 
		_findclose(handle); 
	}
}


void getfolders(std::string dn, std::vector<std::string>& fps)
{
	if (_access(dn.c_str(), 4) != 0) return;

	rmends(dn, "\\");
	rmends(dn, "/");

	char dn2[FILENAME_MAX];
	sprintf_s(dn2, "%s/*.*", dn.c_str());

	_finddata_t fb; //������ͬ�����ļ��Ĵ洢�ṹ�� 
	intptr_t handle = _findfirst(dn2, &fb); 
	//�ҵ���һ��ƥ����ļ� 
	if (handle != 0 && handle != -1L) 
	{ 
		//�����Լ����ҵ�ƥ����ļ�������ִ�� 
		int res = 0;
		while (0 == res) 
		{ 
			//windows�£����и�ϵͳ�ļ�����Ϊ�� ..�� ,������������ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0) 
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib == 16) 
			{ 
				fps.push_back(path); 
			} 
			//���ļ��е��ļ�
			else 
			{ 
				res = _findnext(handle, &fb);
				continue;
			} 

			res = _findnext(handle, &fb);
		} 
		//�ر��ļ��У�ֻ�йر��˲���ɾ����������������˺ܾã���׼c���õ���closedir 
		//������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ����� 
		_findclose(handle); 
	}
}

void getfilesrec(std::string dn, std::string filter, std::vector<std::string>& fps)
{
	if (_access(dn.c_str(), 4) != 0) return;

	rmends(dn, "\\");
	rmends(dn, "/");

	char dn2[FILENAME_MAX];
	sprintf_s(dn2, "%s/%s", dn.c_str(), filter.c_str());

	//��һ��
	_finddata_t fb;
	intptr_t handle = _findfirst(dn2, &fb); 
	if (handle != 0 && handle != -1L)
	{ 
		int res = 0;
		while (0 == res) 
		{ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0)
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib != 16) 
			{ 
				fps.push_back(path); 
			} 

			res = _findnext(handle, &fb);
		} 

		_findclose(handle); 
	}

	//����
	sprintf_s(dn2, "%s/*", dn.c_str());

	handle = _findfirst(dn2, &fb); 
	if (handle != 0 && handle != -1L)
	{ 
		int res = 0;
		while (0 == res) 
		{ 
			if (strcmp(fb.name, "..") == 0 || strcmp(fb.name, ".") == 0)
			{
				res = _findnext(handle, &fb);
				continue;
			}

			//��������·�� 
			char path[FILENAME_MAX];
			sprintf_s(path, "%s/%s", dn.c_str(), fb.name);
			//����ֵΪ16����˵�����ļ���
			if (fb.attrib == 16) 
			{ 
				getfilesrec(path, filter, fps);
			} 

			res = _findnext(handle, &fb);
		} 

		_findclose(handle); 
	}
}

std::array<std::string, 2> pathsplit( std::string path )
{
	std::array<std::string, 2> rv;

	size_t p1 = path.rfind('/');
	size_t p2 = path.rfind('\\');
	size_t p = std::string::npos;
	if (p1 == std::string::npos)
	{
		p = p2;
	}
	else if (p2 == std::string::npos)
	{
		p = p1;
	}
	else
	{
		p = (std::max)(p1, p2);
	}
	
	if (p != std::string::npos && p+1 < path.size())
	{
		rv[0] = path.substr(0, p);
		rv[1] = path.substr(p+1);
	}
	else
	{
		rv[1] = path;
	}

	return rv;
}


std::string pathjoin(std::string dn, std::string fn)
{
	trimends(dn, '\\');
	trimends(dn, '/');

	trimstart(fn, '\\');
	trimstart(fn, '/');

	return dn+"/"+fn;
}


std::array<std::string, 2> dotsplit(std::string path)
{
	std::array<std::string, 2> rv;

	size_t p = path.rfind('.');

	rv[0] = path.substr(0, p);
	if (p != std::string::npos && p+1 < path.size())
	{
		rv[1] = path.substr(p+1);
	}

	return rv;
}


std::string dotjoin(std::string fn, std::string ext)
{
	trimends(fn, '.');
	trimstart(ext, '.');

	return fn+"."+ext;
}


std::string relpath( std::string prefix, std::string fp )
{
	makestdpath(prefix);
	makestdpath(fp);

	if (fp.find(prefix) != 0)
	{
		return fp;
	}

	if (fp.size() < prefix.size()+1)
	{
		return fp;
	}

	return fp.substr(prefix.size()+1);
}

void makestdpath( std::string& fp )
{
	std::replace(fp.begin(), fp.end(), '\\', '/');
	std::transform(fp.begin(), fp.end(), fp.begin(), ::tolower);
	trimends(fp, '/');
	trimstart(fp, '/');
}


SHAKURAS_END;
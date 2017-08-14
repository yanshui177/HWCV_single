/*
ͷ�ļ���path.h ��·����ز����ĺ���ͷ�ļ��Լ�����ԭ�� 
*/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cv.h> 
#include <cv.h> 
#include <highgui.h> 
#include <direct.h>
#include <process.h> 
#include <io.h>
#include <stdio.h>
#include <string.h>
using namespace std;


extern int    g_bi_threshold;		/*ȫ�ֱ��� ��ֵ����ֵ*/
extern double g_std_kesa[50][50];	/*ȫ�ֱ��� ��׼������*/
extern float  g_doubt_threshold;	/*ȫ�ֱ��� ����������ֵ*/
extern string g_dir;				/*ȫ�ֱ��� ��·����Ŀ¼*/
extern int    g_conti;				/*ȫ�ֱ��� �Ƚϱ�׼*/

extern string g_db_hostName;		/*ȫ�ֱ��� ������ip������*/	
extern string g_db_dBName;			/*ȫ�ֱ��� ������ODBC����Դ*/
extern string g_db_userName;		/*ȫ�ֱ��� �������û���*/	
extern string g_db_password;		/*ȫ�ֱ��� ����������*/	

extern char g_log_adr[50];			/*ȫ�ֱ��� ������־�洢��ַ*/
extern char g_log_rec[500];			/*ȫ�ֱ��� ������־ר�ñ���*/
		/*ȫ�ֱ��� ����*/
		/*ȫ�ֱ��� ����*/

extern string g_db_qurry_start;		/*ȫ�ֱ��� ���ݿ��ѯ_��ʼ����*/
extern string g_db_qurry_end;		/*ȫ�ֱ��� ���ݿ��ѯ_��������*/
extern string g_db_qurry_zone;		/*ȫ�ֱ��� ���ݿ��ѯ_�ض�����*/
extern string g_db_qurry_stu_num;	/*ȫ�ֱ��� ���ݿ��ѯ_�ض�����*/
extern bool g_db_qurry_all;			/*ȫ�ֱ��� ���ݿ��ѯ_��ѯȫ�����*/
extern string g_db_hoster_zk;



extern	time_t g_ltime;
extern	char *g_srcTime;
extern	char g_timeNow[32];
extern  char g_msg[100];

/**
��ȡ�����ص�ǰʱ��
 */
char* GetTime();


/**
����ѧ����Ϣ�����ļ�·���������ļ���ȡ

@���� date ��������
@���� subject ���Կ�Ŀ
@���� stuNum ����
@����ֵ �������ɵ��ļ�·��
*/
string CrPath(string date, string subject, string stuNum);


/**
���ܣ���ȡ��׼���ļ�
@���� filesname �ļ���
@���� col ����
@���� _vector ��ȡ���ı�׼��浽vector��
@����ֵ �ɹ�1ʧ��0
*/
int ReadScanf(const string &filename, const int &cols, vector<double *> &_vector);


/**
��ȡ�����ļ��������ø������

@���� filename �����ļ���·��
@����ֵ �ɹ�1  ʧ��0
*/
int ReadConfig(char *filename);


/**
�������ܣ��洢�������ݵ�txt�ļ�

@������ record �洢�����
@���� g_txt_file_path �洢��λ��
@����ֵ 1�ɹ� 0ʧ��
*/
int SaveLog(char *record, string txt_file_path, char *type);


/**
���ܣ������м����ͼ��
@����
@����
@����ֵ
*/
int SaveImg(IplImage *img, string date, string subject, string stuNum);


/**
���ܣ���ȡ�ļ����µ�����ͼ�񣬲��ѵ�ַ���ص�һ��vector��
@����ֵ ���صı���
*/
vector<string> GetImgsPath(string dir_path);


/*
���ܣ���ȡ�ļ����µ������ļ��У����ѵ�ַ���ص�һ��vector��
@����ֵ ���صı���
*/
vector<string> GetDirPath(string dir_path);
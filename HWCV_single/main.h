/*
ͷ�ļ���segmentation.h ������ͷ�ļ�
*/
#pragma once
#ifdef WIN32                            //����VC6��STL��һЩ����ȫ֧�����
#pragma warning (disable: 4514 4786)
#endif
#include "StdAfx.h"
#include "path.h"
#include "Point.h"
#include "process.h"
#include "windows.h"

using namespace std;

/*����ȫ�ֱ���*/

int G_SLEEP_TIME = 1;
int		g_bi_threshold = 230;				/* ȫ�ֱ��� ��ֵ����ֵ*/
double  g_std_kesa[50][50];					/* ȫ�ֱ��� ��׼������*/
float   g_doubt_threshold = 0.12;			/* ȫ�ֱ��� ����������ֵ*/
string  g_dir = "Y:\\";						/* ȫ�ֱ��� ��·����Ŀ¼*/
int		g_cheat_num_threshold = 0;			/* ȫ�ֱ��� Ĭ��������ֵ*/
int		g_conti = 1;						/* ȫ�ֱ��� Ĭ�����ױȽϵ�ͼƬ*/

int g_all_img_num = 0;				/* ȫ�ֱ��� �Ѽ�����ȫ��ͼƬ����*/
int g_doubt_img_num = 0;			/* ȫ�ֱ��� �Ѽ������ɵ�ͼƬ����*/
int g_all_stu_num = 0;				/* ȫ�ֱ��� �Ѽ�����ȫ��ѧ������*/
int g_doubt_stu_num = 0;			/* ȫ�ֱ��� �Ѽ������ɵ�ѧ������*/

//string g_db_hostName = "192.168.200.97";	/*ȫ�ֱ��� ������ip������*/
//string g_db_dBName = "purple";			/*ȫ�ֱ��� ������ODBC����Դ*/
//string g_db_userName = "BJSH";			/*ȫ�ֱ��� �������û���*/
//string g_db_password = "bjshadmin";		/*ȫ�ֱ��� ����������*/
////string g_db_hsotName = "localhost";		/*ȫ�ֱ��� ������ip������*/
////string g_db_dBName = "orcl123";			/*ȫ�ֱ��� ������ODBC����Դ*/
////string g_db_userName = "yannsy";		/*ȫ�ֱ��� �������û���*/
////string g_db_password = "123456";		/*ȫ�ֱ��� ����������*/


string g_db_hostName = "192.168.200.97";	//ȫ�ֱ��� ������ip������
string g_db_dBName = "purple";			//ȫ�ֱ��� ������ODBC����Դ
string g_db_userName = "BJSH";			//ȫ�ֱ��� �������û���
string g_db_password = "bjshadmin";		//ȫ�ֱ��� ����������
/*
string g_db_hostName = "localhost";		//ȫ�ֱ��� ������ip������
string g_db_dBName = "orcl123";			//ȫ�ֱ��� ������ODBC����Դ
string g_db_userName = "yannsy";		//ȫ�ֱ��� �������û���
string g_db_password = "123456";		//ȫ�ֱ��� ����������
*/


string	g_db_qurry_start = "200906";		/*ȫ�ֱ��� ���ݿ��ѯ_��ʼ����*/
string	g_db_qurry_end	 = "201710";		/*ȫ�ֱ��� ���ݿ��ѯ_��������*/
string	g_db_qurry_zone  = "0";				/*ȫ�ֱ��� ���ݿ��ѯ_�ض�����*/
string	g_db_qurry_stu_num = "0";			/*ȫ�ֱ��� ���ݿ��ѯ_�ض�����*/
bool	g_db_qurry_all   = true;			/*ȫ�ֱ��� ���ݿ��ѯ_��ѯȫ�����*/
string  g_db_hoster_zk = "ZK";				/*ȫ�ֱ��� ���ݿ��û���zk������Ժ��zk�����ص�yannsy��*/


bool g_output_cmd_config = false;	/*ȫ�ֱ��� �����������*/
bool g_output_txt_config = false;	/*ȫ�ֱ��� ����м��ļ�ѡ��*/
char g_log_adr[50] = "D:/HWCV/log_ori.csv";	/*ȫ�ֱ��� ������־�洢��ַ*/
char g_log_rec[500] = { 0 };		/*ȫ�ֱ��� ������־ר�ñ���*/
/*ȫ�ֱ��� ���txt����ļ�*/
/*ȫ�ֱ��� ���txt����ļ���ַ*/


time_t g_ltime;
char *g_srcTime=NULL;
char g_timeNow[32]={0};
char g_msg[100]={0};
int g_stu_sus;


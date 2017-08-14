/*
实现文件：path.cpp 路径操作实现文件
*/
#include "path.h"


/**
获取并返回当前时间
 */
char* GetTime()
{
	time(&g_ltime);
	g_srcTime = ctime(&g_ltime);
	strncpy(g_timeNow, g_srcTime, strlen(g_srcTime) - 5);  //不拷贝换行
	g_timeNow[strlen(g_srcTime) - 1] = '\0';   //加结束符'\0'

	char showTime[30];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);

	timeinfo = localtime(&rawtime);
	strftime(showTime, 100, "%Y-%m-%d ", timeinfo);
	strcat(showTime, g_timeNow);
	return showTime;
}


/**
根据学生考试信息生成笔迹卡图像路径，用于图像文件的读取

@变量 date 考试日期
@变量 subject 考试科目
@变量 stuNum 考号
@返回值 返回生成的文件路径
*/
string CrPath(string date, string subject, string stuNum)
{
	string temp = g_dir; temp += date; temp += "\\";
	temp += subject.substr(0, 4); temp += "\\";
	temp += stuNum; temp += ".jpg";

	return temp;
}


/*
功能：读取标准差文件
@输入 filesname	文件名
	  col		行数
	  _vector	读取到的标准差存到vector中
@返回值 成功1失败0
*/
int ReadScanf(const string &filename, const int &cols, vector<double *> &_vector)
{
	// 功能：将filename 中的数据（共cols列）读取到_vector中，_vector可视为二维数组
	FILE *fp = fopen(filename.c_str(), "r");//打开并读取文件
	bool flag = true;
	int i = 0;

	//	printf("--read_scanf--");
	if (!fp){ return 0; }
	while (flag){
		double *point = new double[cols];
		for (i = 0; i<cols; i++){ //读取数据，存在_vector[cols]中
			if (EOF == fscanf(fp, "%lf", &point[i])) {
				flag = false; break;
			}
			if (EOF == fgetc(fp)) {
				flag = false; i++; break;
			}
		}
		if (cols == i)
			_vector.push_back(point);
	}
	fclose(fp);

	return 1;
}


/**
读取配置文件，并配置各项参数

@变量 filename 配置文件的路径
@返回值 成功1  失败0
*/
int ReadConfig(char *filename)
{
	ifstream file(filename);

	if (!file)/*"配置文件不存在！"*/
	{
		/*写入时间*/
		memset(g_log_rec, 0, sizeof(g_log_rec));
//		cout<<"read"<<endl;

		strcat(g_log_rec, ",--ERR:配置文件不存在!");
		SaveLog(g_log_rec, g_log_adr, "a");

		return 0;
	}

	/*步骤：开始读取信息*/
	/*仅用作过滤字符*/
	string temp;
	file >> temp >> temp;
	/*---此行6个，参考配置信息：图片对比参数*/
	file >> temp >> temp >> temp >> temp >> temp >> temp;

	file  >> g_dir >> temp >> temp >> temp;
	string g_log_adr_t;
	file  >> g_log_adr_t >> temp;

	memset(g_log_adr, 0, sizeof(g_log_adr));
	strcpy(g_log_adr, (char*)g_log_adr_t.c_str());
	printf("输出的日志路径为：%s\n", g_log_adr);
	//if (g_bi_threshold < 0 || g_bi_threshold >255)/*输出到日志错误信息*/{		
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	//	strcat(g_log_rec, "--MSG:配置参数有误-笔迹图像对比参数!");
	//	strcat(g_log_rec, g_dir.c_str());
	//	SaveLog(g_log_rec, g_err_adr, "a");

	//	return 0;
	//}


	/*---此行6个，参考配置信息：网络配置参数*/
	file >> temp >> temp >> g_db_hostName >> temp >> g_db_dBName >> temp >> g_db_userName >> temp >> g_db_password >> temp >> g_db_hoster_zk >> temp;

	///*检验参数*/
	////if (strcmp(g_db_hostName.c_str(), "") || g_bi_threshold >255 || g_bi_threshold < 1)/*输出到日志错误信息*/{
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	//	/*time_t timer;
	//	struct tm *tblock;
	//	timer = time(NULL);
	//	tblock = localtime(&timer);
	//	strcat(g_log_rec, asctime(tblock));*/
	//	strcat(g_log_rec, "--MSG:-网络配置参数!");
	//	strcat(g_log_rec, g_db_password.c_str());
	//	strcat(g_log_rec, "\n");

	//	SaveLog(g_log_rec, g_err_adr, "a");

//		return 0;
//	}


	/*---此行5个，参考配置信息：控制参数*/
	//备用
	file >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp;

	///*检验参数*/
	//if (0)/*输出到日志错误信息*/{
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	///*	time_t timer;
	//	struct tm *tblock;
	//	timer = time(NULL);
	//	tblock = localtime(&timer);
	//	strcat(g_log_rec, asctime(tblock));*/
	//	strcat(g_log_rec, "--ERR:配置参数有误-控制参数!");
	//	SaveLog(g_log_rec, g_err_adr, "a");

	//	return 0;
	//}

	/*---此行5个，参考配置信息：数据库查询参数*/
	file >> temp >> temp >> g_db_qurry_start >> temp >> g_db_qurry_end >> temp >> g_db_qurry_zone >> temp >> g_db_qurry_stu_num >> temp >> g_db_qurry_all >> temp;

	///*检验参数*/
	//if (0)/*输出到日志错误信息*/{
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	//	/*time_t timer;
	//	struct tm *tblock;
	//	timer = time(NULL);
	//	tblock = localtime(&timer);
	//	strcat(g_log_rec, asctime(tblock));*/
	//	strcat(g_log_rec, "--ERR:配置参数有误-数据库查询参数!");
	//	SaveLog(g_log_rec, g_err_adr, "a");

	//	return 0;
	//}



	//if (g_doubt_threshold < 0.01)
	//{
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	//	/*time_t timer;
	//	struct tm *tblock;
	//	timer = time(NULL);
	//	tblock = localtime(&timer);
	//	strcat(g_log_rec, asctime(tblock));*/
	//	strcat(g_log_rec, "--ERR:配置参数有误-笔迹图像对比参数!");
	//	SaveLog(g_log_rec, g_err_adr, "a");
	//
	//	return 0;
	//}

	//if (g_conti < 0 || g_conti > 10 || g_conti == NULL)
	//{
	//	memset(g_log_rec, 0, sizeof(g_log_rec));
	///*	time_t timer;
	//	struct tm *tblock;
	//	timer = time(NULL);
	//	tblock = localtime(&timer);
	//	strcat(g_log_rec, asctime(tblock));*/
	//	strcat(g_log_rec, "--ERR:配置参数有误-笔迹图像对比参数!\n\n");
	//	SaveLog(g_log_rec, g_err_adr, "a");

	//	return 0;
	//}
	file.close();/*关闭文件句柄*/

	return 1;
}


/**
函数功能：存储过程数据到txt文件

@变量： record 存储的语句
@变量 g_txt_file_path 存储的位置
@返回值 1成功 0失败
*/
int SaveLog(char *txt, string txt_file_path, char *type)
{
	FILE* fpzz = fopen(txt_file_path.c_str(), type);  //创建文件	
	if (!fpzz)
	{
		cout << "打开文件日志文件" << txt_file_path.c_str() << "失败，无法保存日志！！！" << endl;
		return 0;
	}
	//要返回错误代码
	fprintf(fpzz, txt); //从控制台中读入并在文本输出
	fclose(fpzz);
	fpzz = NULL;//需要指向空，否则会指向原打开文件地址  

	return 1;
}


/*
功能：保存中间鉴定图像(不实现，没有必要)
@变量
@变量
@返回值
*/
int SaveImg(IplImage *img, string date, string subject, string stuNum)
{
	string save_path = "D:\\new_img\\";
	save_path += stuNum;

	//创建文件夹
	_mkdir((char*)save_path.c_str());

	save_path += "\\";
	save_path += date; 
	save_path += "_";
	save_path += subject.substr(0, 4); save_path += ".jpg";

	cout << "Path:savepath" << save_path << endl;
	
	/*string cop = "copy " + source + " /a " + save_path;
	cout << "Path:saveImgage:cop:" << cop << endl;;*/
	cvSaveImage((char*)save_path.c_str(), img);
	//system(cop.c_str());

	return 1;
}


/*
功能：读取文件夹下的所有图像，并把地址返回到一个vector中
@返回值 返回的变量
*/
vector<string> GetImgsPath(string dir_path)
{
	vector<string> paths;
	using namespace std;//引入整个名空间
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(dir_path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  

			if (!(fileinfo.attrib &  _A_SUBDIR))  // 如果是文件，则直接加到向量中
			{
				paths.push_back(p.assign(dir_path).append("/").append(fileinfo.name));
				//cout << "\t\tpath-GetImgsPath：图像文件：" <<fileinfo.name << endl;
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	return paths;
}


/*
功能：读取文件夹下的所有文件夹，并把地址返回到一个vector中
@返回值 返回的变量
*/
vector<string> GetDirPath(string dir_path)
{
	vector<string> paths;
	using namespace std;//引入整个名空间
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(dir_path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  // 如果是目录，则直接加到向量中
			{
				paths.push_back(p.assign(dir_path).append("/").append(fileinfo.name));
				cout << "待鉴定文件夹：" << p.assign(dir_path).append("/").append(fileinfo.name) << endl;
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

	return paths;
}
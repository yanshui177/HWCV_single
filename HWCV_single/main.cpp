/*
主函数文件：segmentation.cpp 主函数的实现文件
*/
#include "main.h"
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )//没有界面运行


/*
计算图像特征，并把特征计算得到wcd，最终使用的是scd，即比较距离
@ 输入：
	files		比较的文件路径，<vector(string)>
	flag_exist	考生学号有笔迹卡存在的的路径索引
	bzckesa		标准差表
	wcd			计算的距离（相当于输出的）
	conti		比较的笔迹卡索引
@ 输出：
*/
int ComputeFeature(vector<string> files, double bzckesa[50][50], double *wcd, int conti, bool flagprint)
{
	if (flagprint) printf("\t第%d次计算图像特征\n", conti);
	int i, ii, jj, size_exist;
	double	bzcu[50][50] = { 0 };			//标准差中的u
	double	featurep[50][50][30] = { 0 };	//所有图像的轮廓方向特征初始化//干什么	//30
	int		feature[50][50][30] = { 0 };	//所有图像的特征值
	int		featx[50][50] = { 0 };			//循环赋值的feature
	int		featureall;					//图像特征值和				//做什么用

	size_exist = files.size();
	for (i = 0; i < size_exist; i++)
	{
		memset(featx, 0, sizeof(featx));
		//cout << "\t\t图像地址为:" << (char*)files[i].c_str() << "\t";
		//检查文件存在与否

		singlefeature((char*)files[i].c_str(), featx, flagprint);				//featx[][50]
		featureall = 0;                                    //图像特征值和的初始化
		for (ii = 0; ii < 48; ii++)
		{  //将featx存起来,回头看能不能用函数换掉
			for (jj = ii + 1; jj < 47; jj++)
			{
				feature[ii][jj][i] = featx[ii][jj];
				featureall = featureall + featx[ii][jj];
			}
		}
		//求轮廓方向特征featurep，式(5)  与标准差中的u的和
		for (ii = 0; ii < 48; ii++)
		{
			for (jj = ii + 1; jj < 47; jj++)
			{
				featurep[ii][jj][i] = (double)featx[ii][jj] / featureall;
				bzcu[ii][jj] += (double)featx[ii][jj] / featureall * 1000;     //标准差的值过小,进行放大（1）
			}
		}
	}
	//处理完一个人的每一张图片后
	for (ii = 0; ii < 48; ii++)//求标准差中的u
		for (jj = ii + 1; jj < 47; jj++)
			bzcu[ii][jj] = bzcu[ii][jj] / size_exist;
	//求相似性就是带权卡方wcd
	for (int wcd_i = 0; wcd_i < size_exist; wcd_i++)
		for (ii = 0; ii < 48; ii++)
			for (jj = ii + 1; jj < 47; jj++)
				if (featurep[ii][jj][wcd_i] * featurep[ii][jj][conti] != 0 && bzckesa[ii][jj] != -1)
					wcd[wcd_i] += pow((featurep[ii][jj][wcd_i] - featurep[ii][jj][conti]), 2) / ((featurep[ii][jj][wcd_i] + featurep[ii][jj][conti])*bzckesa[ii][jj]);

	memset(feature, 0, sizeof(feature));
	memset(featurep, 0, sizeof(featurep));

	return 1;
}


/*主函数*/
int main()
{
	bool flagprint = true;
	int num_img = 0;
	int num_stu = 0;
	int susnum_img = 0;  //嫌疑图像数量
	int susnum_stu = 0;  // 嫌疑学生数量

	int		i, ii, jj;
	double  bzckesa[50][50] = { 0 };
	double  wcd[30] = { 0 };
	int		featdif[30] = { 0 };
	float	maxx = 0;			//最大特征值的标号与值
	int		xyimgnum = 0;				//嫌疑图片的数目

	vector<string> suspict;		//记录嫌疑图片地址
	vector<float> suspict_wcd;	//嫌疑图片的wcd值
	vector<string> files;		//存储该生所有考试文件路径
	vector<string> flagVec;//记录查到的学生的所有考试信息

	//读取配置文件，并配置各项参数
	if (!ReadConfig("D:/HWCV/config/configure.cfg"))
	{
		printf("MainErr: 读取配置失败！！\n");
		Sleep(G_SLEEP_TIME);
		return 0;
	}cout << "Main:读取配置文件成功！" << endl;

	//读取标准差文件
	int bzccolumns = 47;//txt文件中有47列
	vector<double *> output_bzc;
	if (!ReadScanf("D:\\HWCV\\config\\stdfile.db", bzccolumns, output_bzc))
	{
		printf("MainErr:没找到标准差文件！！！\n");
		Sleep(G_SLEEP_TIME);
		return 0;
	}
	for (ii = 0; ii < 48; ii++)//output_vector可视为二维数组;输出数组元素：
		for (jj = ii + 1; jj < 47; jj++)
			bzckesa[ii][jj] = output_bzc[ii][jj];
	cout << "Main:读取标准差文件成功！" << endl;

	// 载入图像路径
	string path = "D:/img/HWCV图片";
	vector<string> Dirs=GetDirPath(path);  //Dirs是所有考生（342名）图像所在的文件夹路径
	printf("Main：当前共有%d名考生\nMain：阈值大小为：%g\n\n\n", Dirs.size(), g_doubt_threshold);

	// 针对每一个考生，进行鉴定
	for (int iterdir = 2; iterdir < Dirs.size(); iterdir++)
	{
		if(flagprint) printf("\n\n当前考生：%s\n", Dirs[iterdir]);
		if (flagprint) for (int iterprint = 0; iterprint < 100; iterprint++) printf("=");

		files = GetImgsPath(Dirs[iterdir]);  //获取此考生的所有图像路径
		if (flagprint) cout << "\nMain：当前学号：" << Dirs[iterdir] << "， 文件个数：" << files.size() << endl;
		if (flagprint) if (files.size() < 2){ printf("Main：此考生只有一个笔迹卡，无法比较！！！\n"); continue; }

		/* 开始对每一张图片进行处理 循环n次，确定最好的结果*/
		if (flagprint) printf("Main：计算相似度\n");
		for (int r = 0; r < files.size(); r++)
		{
			memset(wcd, 0, sizeof(wcd));
			ComputeFeature(files, bzckesa, wcd, r, flagprint);

			xyimgnum = 0;
			//疑似作弊
			for (i = 0; i < files.size(); i++)
			{
				if (flagprint) printf("\t\t图像地址：%s，卡方距离为：%g", files[i].c_str(), wcd[i]);
				if (wcd[i] > g_doubt_threshold)
				{
					xyimgnum++;
					suspict.push_back(files[i].c_str());
					suspict_wcd.push_back(wcd[i]);
					if (flagprint) printf("\t*X*\n");
				}
				else
				{
					if (flagprint) printf("\n");
				}
			}

			// 验证作弊数量
			if (xyimgnum < files.size()/2)
			{
				break;
			}
			if (r < files.size() - 1)
			{
				suspict.clear();
				suspict_wcd.clear();
				xyimgnum = 0;
				flagVec.clear();
			}
		}
		if (xyimgnum > 0)
		{
			susnum_img += xyimgnum;
			susnum_stu += 1;
			if (flagprint) printf("\nMain：有作弊嫌疑\n");
		}
		else if (flagprint) printf("Main：鉴定无作弊作弊嫌疑\n");

		// 统计已鉴定的图像数量
		num_img += files.size();
		num_stu += 1;
		if (!flagprint)printf("Main：鉴别数量：%d，当前考生：%s, 文件个数：%d\n\t\t\t总数量：%d，嫌疑：%d，比例：%g\n\t\t\t图像数量：%d，嫌疑：%d，比例：%g\n\n", 
			iterdir, Dirs[iterdir].c_str(), files.size(), (int)num_stu, (int)susnum_stu, (float)susnum_stu/num_stu, 
			(int)num_img, (int)susnum_img, (float)susnum_img / num_img);
		// 清理数据
		suspict.clear();
		suspict_wcd.clear();
		memset(wcd, 0, sizeof(wcd));
		files.clear();
	}


	printf("执行完毕\n全部学生数量：%d，作弊学生数量：%d，比例%g\n全部图像数量：%d，作弊嫌疑图像数量%d，比例%g", 
		(int)num_stu, (int)susnum_stu, (float)susnum_stu / num_stu, (int)num_img, (int)susnum_img, (float)susnum_img / num_img);
	/*善后*/
	output_bzc.clear();
	memset(g_log_rec, 0, sizeof(g_log_rec));
	memset(bzckesa, 0, sizeof(bzckesa));
	memset(featdif, 0, sizeof(featdif));
	/*返回值*/

	system("pause");
	return 0;
}



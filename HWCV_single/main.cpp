/*
�������ļ���segmentation.cpp ��������ʵ���ļ�
*/
#include "main.h"
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )//û�н�������


/*
����ͼ��������������������õ�wcd������ʹ�õ���scd�����ȽϾ���
@ ���룺
	files		�Ƚϵ��ļ�·����<vector(string)>
	flag_exist	����ѧ���бʼ������ڵĵ�·������
	bzckesa		��׼���
	wcd			����ľ��루�൱������ģ�
	conti		�Ƚϵıʼ�������
@ �����
*/
int ComputeFeature(vector<string> files, double bzckesa[50][50], double *wcd, int conti, bool flagprint)
{
	if (flagprint) printf("\t��%d�μ���ͼ������\n", conti);
	int i, ii, jj, size_exist;
	double	bzcu[50][50] = { 0 };			//��׼���е�u
	double	featurep[50][50][30] = { 0 };	//����ͼ�����������������ʼ��//��ʲô	//30
	int		feature[50][50][30] = { 0 };	//����ͼ�������ֵ
	int		featx[50][50] = { 0 };			//ѭ����ֵ��feature
	int		featureall;					//ͼ������ֵ��				//��ʲô��

	size_exist = files.size();
	for (i = 0; i < size_exist; i++)
	{
		memset(featx, 0, sizeof(featx));
		//cout << "\t\tͼ���ַΪ:" << (char*)files[i].c_str() << "\t";
		//����ļ��������

		singlefeature((char*)files[i].c_str(), featx, flagprint);				//featx[][50]
		featureall = 0;                                    //ͼ������ֵ�͵ĳ�ʼ��
		for (ii = 0; ii < 48; ii++)
		{  //��featx������,��ͷ���ܲ����ú�������
			for (jj = ii + 1; jj < 47; jj++)
			{
				feature[ii][jj][i] = featx[ii][jj];
				featureall = featureall + featx[ii][jj];
			}
		}
		//��������������featurep��ʽ(5)  ���׼���е�u�ĺ�
		for (ii = 0; ii < 48; ii++)
		{
			for (jj = ii + 1; jj < 47; jj++)
			{
				featurep[ii][jj][i] = (double)featx[ii][jj] / featureall;
				bzcu[ii][jj] += (double)featx[ii][jj] / featureall * 1000;     //��׼���ֵ��С,���зŴ�1��
			}
		}
	}
	//������һ���˵�ÿһ��ͼƬ��
	for (ii = 0; ii < 48; ii++)//���׼���е�u
		for (jj = ii + 1; jj < 47; jj++)
			bzcu[ii][jj] = bzcu[ii][jj] / size_exist;
	//�������Ծ��Ǵ�Ȩ����wcd
	for (int wcd_i = 0; wcd_i < size_exist; wcd_i++)
		for (ii = 0; ii < 48; ii++)
			for (jj = ii + 1; jj < 47; jj++)
				if (featurep[ii][jj][wcd_i] * featurep[ii][jj][conti] != 0 && bzckesa[ii][jj] != -1)
					wcd[wcd_i] += pow((featurep[ii][jj][wcd_i] - featurep[ii][jj][conti]), 2) / ((featurep[ii][jj][wcd_i] + featurep[ii][jj][conti])*bzckesa[ii][jj]);

	memset(feature, 0, sizeof(feature));
	memset(featurep, 0, sizeof(featurep));

	return 1;
}


/*������*/
int main()
{
	bool flagprint = true;
	int num_img = 0;
	int num_stu = 0;
	int susnum_img = 0;  //����ͼ������
	int susnum_stu = 0;  // ����ѧ������

	int		i, ii, jj;
	double  bzckesa[50][50] = { 0 };
	double  wcd[30] = { 0 };
	int		featdif[30] = { 0 };
	float	maxx = 0;			//�������ֵ�ı����ֵ
	int		xyimgnum = 0;				//����ͼƬ����Ŀ

	vector<string> suspict;		//��¼����ͼƬ��ַ
	vector<float> suspict_wcd;	//����ͼƬ��wcdֵ
	vector<string> files;		//�洢�������п����ļ�·��
	vector<string> flagVec;//��¼�鵽��ѧ�������п�����Ϣ

	//��ȡ�����ļ��������ø������
	if (!ReadConfig("D:/HWCV/config/configure.cfg"))
	{
		printf("MainErr: ��ȡ����ʧ�ܣ���\n");
		Sleep(G_SLEEP_TIME);
		return 0;
	}cout << "Main:��ȡ�����ļ��ɹ���" << endl;

	//��ȡ��׼���ļ�
	int bzccolumns = 47;//txt�ļ�����47��
	vector<double *> output_bzc;
	if (!ReadScanf("D:\\HWCV\\config\\stdfile.db", bzccolumns, output_bzc))
	{
		printf("MainErr:û�ҵ���׼���ļ�������\n");
		Sleep(G_SLEEP_TIME);
		return 0;
	}
	for (ii = 0; ii < 48; ii++)//output_vector����Ϊ��ά����;�������Ԫ�أ�
		for (jj = ii + 1; jj < 47; jj++)
			bzckesa[ii][jj] = output_bzc[ii][jj];
	cout << "Main:��ȡ��׼���ļ��ɹ���" << endl;

	// ����ͼ��·��
	string path = "D:/img/HWCVͼƬ";
	vector<string> Dirs=GetDirPath(path);  //Dirs�����п�����342����ͼ�����ڵ��ļ���·��
	printf("Main����ǰ����%d������\nMain����ֵ��СΪ��%g\n\n\n", Dirs.size(), g_doubt_threshold);

	// ���ÿһ�����������м���
	for (int iterdir = 2; iterdir < Dirs.size(); iterdir++)
	{
		if(flagprint) printf("\n\n��ǰ������%s\n", Dirs[iterdir]);
		if (flagprint) for (int iterprint = 0; iterprint < 100; iterprint++) printf("=");

		files = GetImgsPath(Dirs[iterdir]);  //��ȡ�˿���������ͼ��·��
		if (flagprint) cout << "\nMain����ǰѧ�ţ�" << Dirs[iterdir] << "�� �ļ�������" << files.size() << endl;
		if (flagprint) if (files.size() < 2){ printf("Main���˿���ֻ��һ���ʼ������޷��Ƚϣ�����\n"); continue; }

		/* ��ʼ��ÿһ��ͼƬ���д��� ѭ��n�Σ�ȷ����õĽ��*/
		if (flagprint) printf("Main���������ƶ�\n");
		for (int r = 0; r < files.size(); r++)
		{
			memset(wcd, 0, sizeof(wcd));
			ComputeFeature(files, bzckesa, wcd, r, flagprint);

			xyimgnum = 0;
			//��������
			for (i = 0; i < files.size(); i++)
			{
				if (flagprint) printf("\t\tͼ���ַ��%s����������Ϊ��%g", files[i].c_str(), wcd[i]);
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

			// ��֤��������
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
			if (flagprint) printf("\nMain������������\n");
		}
		else if (flagprint) printf("Main��������������������\n");

		// ͳ���Ѽ�����ͼ������
		num_img += files.size();
		num_stu += 1;
		if (!flagprint)printf("Main������������%d����ǰ������%s, �ļ�������%d\n\t\t\t��������%d�����ɣ�%d��������%g\n\t\t\tͼ��������%d�����ɣ�%d��������%g\n\n", 
			iterdir, Dirs[iterdir].c_str(), files.size(), (int)num_stu, (int)susnum_stu, (float)susnum_stu/num_stu, 
			(int)num_img, (int)susnum_img, (float)susnum_img / num_img);
		// ��������
		suspict.clear();
		suspict_wcd.clear();
		memset(wcd, 0, sizeof(wcd));
		files.clear();
	}


	printf("ִ�����\nȫ��ѧ��������%d������ѧ��������%d������%g\nȫ��ͼ��������%d����������ͼ������%d������%g", 
		(int)num_stu, (int)susnum_stu, (float)susnum_stu / num_stu, (int)num_img, (int)susnum_img, (float)susnum_img / num_img);
	/*�ƺ�*/
	output_bzc.clear();
	memset(g_log_rec, 0, sizeof(g_log_rec));
	memset(bzckesa, 0, sizeof(bzckesa));
	memset(featdif, 0, sizeof(featdif));
	/*����ֵ*/

	system("pause");
	return 0;
}



/*
*����������������������������������������������������������������������������������������������������
*������    ��������ԭ���д����ʵ�ָ�˹ģ��													   ��
*������    �ߣ�Ĳ����|BestBlade																	   ��
*������    ����1.0																			   	   ��
*��������ʱ�䣺2020.07.11																		   ��
*����������������������������������������������������������������������������������������������������
*/

#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>																				/*	exp()����	*/

using namespace std;
using namespace cv;

int Gaussian_blur(Mat origin_pic, Mat& result_pic, Size ksize, double sigma)
{
	int kw = ksize.width;
	int kh = ksize.height;

	result_pic = origin_pic.clone();

	int row = origin_pic.rows;
	int col = origin_pic.cols;
																								/*	���ж�ͼƬ�����Ƿ���ȷ	*/
	if ((row < kw) && (col < kh))
	{
		return -1;																				/*	ͼƬ�ߴ��С	*/
	}
	else
		if ((kw % 2 == 0) || (kh % 2 == 0))
		{
			return -2;																			/*	ƽ�����ڳߴ����	*/
		}
		else
		{
			int row_start = kw / 2;
			int col_start = kh / 2;

			for (int i = row_start; i < row - row_start; i++)
			{
				for (int j = col_start; j < col - col_start; j++)								/*	ѭ��ͼ���(i,j)������	*/
				{
					double channel_r = { 0 };
					double channel_g = { 0 };
					double channel_b = { 0 };
					double wij_sum	 = { 0 };													/*	����Ȩ���ܺ�	*/

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start; q++)					/*	�ڵ�(i,j)������Χ��ksize(weight,height)�������м���Ȩ���ܺ�	*/
						{
							int wi = p - i;														/*	���㵱ǰ���ض����ĵ�(i,j)��������꣬����ͬ��	*/
							int wj = q - j;

							double wij = exp(-(wi * wi + wj * wj) / (2 * sigma * sigma));		/*	����ԭʼ��˹Ȩ��	*/

							wij_sum += wij;														/*	�õ�Ȩ���ܺ�	*/
						}		
					}

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start; q++)					/*	�ڵ�(i,j)������Χ��ksize(weight,height)�������м����ܺ�	*/
						{
							int wi = p - i;														
							int wj = q - j;

							double w_ij = exp(-(wi * wi + wj * wj) / (2 * sigma * sigma)) / wij_sum;	/*	��ÿ�����Ȩ�س���Ȩ���ܺ��Թ�һ��	*/

							Vec3b pq = origin_pic.at<Vec3b>(p, q);
							channel_r += pq[2] * w_ij;											/*	��ͨ�����ϸ�˹����Ȩ��	*/
							channel_g += pq[1] * w_ij;
							channel_b += pq[0] * w_ij;
						}
					}
					result_pic.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(channel_b);			/*	��saturate_cast���������Ա������ݲ����	*/
					result_pic.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(channel_g);			/*	����	*/
					result_pic.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(channel_r);
				}
			}
			return 0;
		}
}

int main()
{


	Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/�ͻ��˿���.JPG");
	Mat result_pic;
	Size ksize(3, 3);
	double sigma = 1;

	Gaussian_blur(origin_pic, result_pic, ksize, sigma);										/*	�Զ����˹ģ������	*/

	imshow("ԭͼ", origin_pic);
	imshow("3x3,sigma = 1,ƽ�����ͼ", result_pic);

	Mat result_pic_opencv_blur;
	GaussianBlur(origin_pic, result_pic_opencv_blur, ksize, sigma, BORDER_DEFAULT);				/*	OPENCV�Դ���˹ģ��	*/
	imshow("OPENCV GaussianBlur()���ͼ", result_pic_opencv_blur);

	waitKey(0);
	//return 0;
}

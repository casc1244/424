#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
int main(int argc, const char * argv[]) {


	Mat src1 = imread("D:\\360downloads\\16.jpg");
	Mat src2 = imread("D:\\360downloads\\15.jpg");
	Mat dsp = imread("D:\\360downloads\\14.jpg");
	Mat gx, gy;
	Mat mag, angle;
	Sobel(dsp, gx, CV_32F, 1, 0, 1);
	Sobel(dsp, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	//图像划分为多个16×16的cell
	int cellSize = 16;
	int nX = dsp.cols / cellSize;
	int nY = dsp.rows / cellSize;
	int cellnum = nX * nY;
	int bins = cellnum * 8;
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	int cell = 0;
	for (int j = 0; j < nY; j++)
	{
		
		for (int i = 0; i < nX; i++)
		{
			
			for (int y = j * cellSize; y < (j + 1) * cellSize; y++)
			{
				
				for (int x = i * cellSize; x < (i + 1) * cellSize; x++)
				{
					int intangle = 0;
					
					float fangle = angle.at<float>(y, x);
					
					intangle = fangle / 45;
					
					float magnitude = mag.at<float>(y, x);
					
					ref_hist[intangle + cell * 8] += magnitude;
				}
			}
			cell++;
		}
	}
	//所有像素计算梯度和角度方向
    Mat gx1, gy1;
	Mat mag1, angle1;
	Sobel(src1, gx1, CV_32F, 1, 0, 1);
	Sobel(src1, gy1, CV_32F, 0, 1, 1);
	cartToPolar(gx1, gy1, mag1, angle1, true);
	//重复操作

	nX = src1.cols / cellSize;
	nY = src1.rows / cellSize;
	cellnum = nX * nY;
	bins = cellnum * 8;
	float* ref_hist1 = new float[bins];
	memset(ref_hist1, 0, sizeof(float) * bins);
	cell = 0;
	for (int j = 0; j < nY; j++)
	{
		for (int i = 0; i < nX; i++)
		{
			for (int y = j * cellSize; y < (j + 1) * cellSize; y++)
			{
				for (int x = i * cellSize; x < (i + 1) * cellSize; x++)
				{
					int intangle = 0;
					float fangle = angle1.at<float>(y, x);
					intangle = fangle / 45;
					float magnitude = mag1.at<float>(y, x);
					ref_hist1[intangle + cell * 8] += magnitude;
				}
			}
			cell++;
		}
	}

	Mat gx2, gy2;
	Mat mag2, angle2;
	Sobel(src2, gx2, CV_32F, 1, 0, 1);
	Sobel(src2, gy2, CV_32F, 0, 1, 1);
	cartToPolar(gx2, gy2, mag2, angle2, true);

	nX = src2.cols / cellSize;
	nY = src2.rows / cellSize;
	cellnum = nX * nY;
	bins = cellnum * 8;
	float* ref_hist2 = new float[bins];
	memset(ref_hist2, 0, sizeof(float) * bins);
	cell = 0;
	for (int j = 0; j < nY; j++)
	{
		for (int i = 0; i < nX; i++)
		{
			for (int y = j * cellSize; y < (j + 1) * cellSize; y++)
			{
				for (int x = i * cellSize; x < (i + 1) * cellSize; x++)
				{
					int intangle = 0;
					float fangle = angle2.at<float>(y, x);
					intangle = fangle / 45;
					float magnitude = mag2.at<float>(y, x);
					ref_hist2[intangle + cell * 8] += magnitude;
				}
			}
			cell++;
		}
	}


	int r1 = 0;
	int r2 = 0;
	int result1 = 0;
	int result2 = 0;
	for (int i = 0; i < bins; i++)
	{
		r1 += (ref_hist[i] - ref_hist1[i])*(ref_hist[i] - ref_hist1[i]);
		r2 += (ref_hist[i] - ref_hist2[i])*(ref_hist[i] - ref_hist2[i]);

	}
	result1 = sqrt(r1);
	result2 = sqrt(r2);
	cout << result1 << endl;
	cout << result2 << endl;
	if (result1 < result2)
	{
		cout << "src1与原图更像" << endl;
	}
	else
	{
		cout << "src2与原图更像" << endl;
	}
	waitKey(0);
	delete[] ref_hist;
	delete[] ref_hist1;
	delete[] ref_hist2;

	return 0;
}
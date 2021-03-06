#include "led.h"
#include <cstdlib>
#include <cmath>

#if 1 //トンネルチャレンジ
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <math.h>

void Anglar_Fisheye(char *file_name)
{

	IplImage *img = 0;
	IplImage *out_img = 0;

	double r;	//魚眼レンズの半径
	int w;	//重み
	int vx, vy;
	double rp;


	img = cvLoadImage(file_name, CV_LOAD_IMAGE_COLOR);
	out_img = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);

	w = 40;
	r = ((img->width) > (img->height) ? (img->width) : (img->height)) / 2;

	for (int y = 0;y<img->height;y++) {
		for (int x = 0;x<img->width;x++) {
			rp = sqrt((double)(w * w + pow((double)(x - img->width / 2), 2) + pow((double)(y - img->height / 2), 2)));

			vx = (int)((rp * (x - img->width / 2)) / r + img->width / 2);
			vy = (int)((rp * (y - img->height / 2)) / r + img->height / 2);

			if (0 <= vx && vx < img->width && 0 <= vy && vy < img->height) {
				out_img->imageData[out_img->widthStep * y + x * 3] = img->imageData[img->widthStep * vy + vx * 3];
				out_img->imageData[out_img->widthStep * y + x * 3 + 1] = img->imageData[img->widthStep * vy + vx * 3 + 1];
				out_img->imageData[out_img->widthStep * y + x * 3 + 2] = img->imageData[img->widthStep * vy + vx * 3 + 2];
			}

		}
	}

	cvNamedWindow("Fish-Eye", CV_WINDOW_AUTOSIZE);
	cvShowImage("Fish-Eye", out_img);
	cvWaitKey(0);

	cvDestroyWindow("Fish-Eye");
	cvReleaseImage(&img);
	cvReleaseImage(&out_img);
}

#include <iostream>

int main()
{
	char *filename = "C:\\Users\\nabe\\Desktop\\ledsim\\dot_ori.jpg";
	char *tempname = "C:\\Users\\nabe\\Desktop\\ledsim\\temp.jpg";
	IplImage *img = 0;

	//読み込み
	cv::Mat src_img = cv::imread(filename);
	img = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR);
	if (src_img.empty()) return -1;
	cv::Mat tmp_img = src_img.clone();


	//切り取り
	cv::Mat roi1(tmp_img, cv::Rect(0, 0, img->width, img->height - (img->height / 10)));
	cv::Mat roi2(tmp_img, cv::Rect(0, img->height - (img->height / 10), img->width, img->height / 10));

	//貼り付け
	roi2.copyTo(tmp_img);

	cv::imshow("Original", src_img);
	cv::imshow("test", tmp_img);


	cv::waitKey(0);

	Anglar_Fisheye(filename);
	return 0;
}
#endif

#if 0 //bitmap 積層
#include "bitmap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define X_N 16
#define X_LENGTH 4.0
#define DX  X_LENGTH / (X_N-1)
#define Y_N 32
#define Y_LENGTH 4.0
#define DY  Y_LENGTH / (Y_N-1)

int main()
{
	using namespace makerfaire::fxat;
	Led led;

	bitmap bmpData[8];
	char filename[8][255] = { "C:\\Users\\nabe\\Desktop\\ledsim\\pika01.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika02.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika03.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika04.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika05.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika06.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika07.bmp",
			"C:\\Users\\nabe\\Desktop\\ledsim\\pika08.bmp" };

	for (int i = 0;i < 8;i++) {
		if (bmpData[i].read_bmp(filename[i]) == false) return -1;
	}

	for(int h = 0;;h++){
		for (int k = 0;k<8;k++) {
			//BitmapファイルのRGB情報は左下から右へ、下から上に並んでいる
			for (unsigned int i = 0; i<bmpData[k].height; i++) {
				for (unsigned int j = 0; j<bmpData[k].width; j++) {
					int x = j;
					int y = bmpData[k].height - i - 1;
					int z = k%8;
					int index = (bmpData[k].height - i - 1)*bmpData[k].width + j;

					if (bmpData[k].Rdata[index] == 255 &&
						bmpData[k].Gdata[index] == 255 &&
						bmpData[k].Bdata[index] == 255) {
						//白は透明色
						continue;
					}
					int rgb = (bmpData[k].Rdata[index] << 16) + 
						(bmpData[k].Gdata[index] << 8) + 
						bmpData[k].Bdata[index];
					led.SetLed(x, y, z, rgb);
				}
			}
		}
		led.Show();
		Wait(250);
		led.Clear();
	}

	return 0;
}
#endif

#if 0 // bitmap(1層+波)
#include "bitmap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define X_N 16
#define X_LENGTH 4.0
#define DX  X_LENGTH / (X_N-1)
#define Y_N 32
#define Y_LENGTH 4.0
#define DY  Y_LENGTH / (Y_N-1)

int main()
{
	using namespace makerfaire::fxat;
	Led led;

	bitmap bmpData;
	char *filename="C:\\Users\\nabe\\Desktop\\ledsim\\gunma16x32.bmp";	
	
	if (bmpData.read_bmp(filename) == false) return -1;

	for (int k = 0;;k++){
		//BitmapファイルのRGB情報は左下から右へ、下から上に並んでいる
		for (unsigned int i = 0; i<bmpData.height; i++){
			for (unsigned int j = 0; j<bmpData.width; j++){
				int x = j;
				int y = bmpData.height-i-1;
				
				//int z = k%8; //単純移動
				
				//ガウシアン(波っぽい)チャレンジ
				//float r2 = (j*DX - X_LENGTH / 2.0)*(j*DX - X_LENGTH / 2.0)
				//	+ (i*DY - Y_LENGTH / 2.0)*(i*DY - Y_LENGTH / 2.0);
				//int z = 8.0*exp(-r2 / (2 * 2.0*2.0));
				
				int t = k % 8;
				int z = 8*sin(k+x /3.14)*sin(k+x/3.14);

				int rgb = (bmpData.Rdata[(bmpData.height - i - 1)*bmpData.width + j] << 16) + (bmpData.Gdata[(bmpData.height - i - 1)*bmpData.width + j] << 8) + bmpData.Bdata[(bmpData.height - i - 1)*bmpData.width + j];
				led.SetLed(x, y, z, rgb);
			}
		}

		led.Show();
		Wait(250);
		led.Clear();
	}
	return 0;
}
#endif

#if 0 //random
int main()
{
	using namespace makerfaire::fxat;
	Led led;
	for (int i = 0;; ++i){
		if (0 == (i % 128)){
			led.Clear();
		}
		else{
			int x = rand() % Led::Width;
			int y = rand() % Led::Height;
			int z = rand() % Led::Depth;
			int r = rand() & 0xFF;
			int g = rand() & 0xFF;
			int b = rand() & 0xFF;
			int rgb = (r << 16) + (g << 8) + b;
			led.SetLed(x, y, z, rgb);
		}
		led.Show();
		Wait(50);
	}
	return 0;
}
#endif

#if 0 // circle and square
int N = 100;
int get_power(int ix)
{
	int fade = N / 5;
	if (ix < fade) {
		return ix * 255 / fade;
	}
	else if (ix < N - fade) {
		return 255;
	}
	else {
		return (N - ix) * 255 / fade;
	}
}

template< typename t >
t max3(t const & a, t const & b, t const & c)
{
	auto ab = a < b ? b : a;
	return ab < c ? c : ab;
}

template< typename proc_t >
void concentric(makerfaire::fxat::Led & led, proc_t const & proc)
{
	using makerfaire::fxat::Led;
	for (int ix = 0; ix<N; ++ix) {
		int cx = Led::Width / 2;
		int cy = Led::Height / 2;
		int cz = Led::Depth / 2;
		led.Clear();
		int power = get_power(ix);
		for (int x = 0; x < Led::Width; ++x) {
			for (int y = 0; y < Led::Height; ++y) {
				for (int z = 0; z < Led::Depth; ++z) {
					double d = proc(x - cx, y - cy, z - cz);
					int col0 = static_cast<int>(std::round(ix - d)) % 64;
					int col = (col0 & 7) == 0 ? col0 / 8 : 0;
					int r = (col & 1) ? power : 0;
					int g = (col & 2) ? power : 0;
					int b = (col & 4) ? power : 0;
					int rgb = (r << 16) + (g << 8) + b;
					led.SetLed(x, y, z, rgb);
				}
			}
		}
		led.Show();
		makerfaire::fxat::Wait(50);
	}
}

void cube(makerfaire::fxat::Led & led)
{
	concentric(led, [](int dx, int dy, int dz)->double {
		return max3(std::abs(dx), std::abs(dy), std::abs(dz));
	});
}

void sphere(makerfaire::fxat::Led & led)
{
	concentric(led, [](int dx, int dy, int dz)->double {
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	});
}

int main(int argc, const char* argv[])
{
	static_cast<void>(argc); // unused
	static_cast<void>(argv); // unused
	using namespace makerfaire::fxat;
	Led led;
	for (;;) {
		cube(led);
		sphere(led);
	}
	return 0;
}

#endif
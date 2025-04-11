#include <QString>
#include <QtWidgets/QWidget>
#include <opencv2/opencv.hpp>
#include <vector>
#include "func.h"
#include <cstdlib>
#include <opencv2\imgproc\types_c.h>

using namespace cv;
using namespace std;

void Func::canny(Mat img1, Mat& out)
{
	Mat gray, edge;
	cvtColor(img1, gray, COLOR_RGB2GRAY);
	blur(gray, edge, Size(3, 3));
	Canny(edge, out, 100, 50);
}
void Func::enhance(cv::Mat img1, cv::Mat& img_out)
{
	cv::Mat enhancedImage;
	cv::pow(img1 / 255, 2.0, enhancedImage);
	enhancedImage *= 255.0;
	img_out = enhancedImage.clone();
}
void Func::Circular_GIF(Mat img, Mat seg_img, Mat origin_img, int x_center, int y_center, string outpath, string basename)
{
	Mat imgadd = img.clone();
	Size size = Size(imgadd.cols, imgadd.rows);
	VideoWriter writer;
	string path = "tmp.avi";
	writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, size);
	Point2f center(x_center, y_center);
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0);
		Mat rotatedImage;
		warpAffine(img, rotatedImage, rotationMatrix, img.size());
		imgadd = (imgadd + rotatedImage);

		Mat temp_img = seg_img + imgadd;
		Mat result_img;
		addforeground(temp_img, seg_img, origin_img, result_img);
		writer.write(result_img);
	}
	writer.release();
	string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Circular_" + basename + ".gif";
	system(ffmpegCommand.c_str());
	remove("tmp.avi");
}
void Func::Radiation_GIF(Mat img, Mat seg_img, Mat origin_img, int x_center, int y_center, string outpath, string basename)
{
	Mat imgadd = img.clone();
	Size size = Size(imgadd.cols, imgadd.rows);
	VideoWriter writer;
	string path = "tmp.avi";
	writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, size);
	Point2f center(x_center, y_center);
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		Mat rotationMatrix = getRotationMatrix2D(center, 0, 1.0 + angle / 70);
		Mat rotatedImage;
		warpAffine(img, rotatedImage, rotationMatrix, img.size());
		imgadd = (imgadd + rotatedImage);

		Mat temp_img = seg_img + imgadd;
		Mat result_img;
		addforeground(temp_img, seg_img, origin_img, result_img);
		writer.write(result_img);
	}
	writer.release();
	string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Radiation_" + basename + ".gif";
	system(ffmpegCommand.c_str());
	remove("tmp.avi");
}
void Func::Spiral_GIF(Mat img, Mat seg_img, Mat origin_img, int x_center, int y_center, string outpath, string basename)
{
	Mat imgadd = img.clone();
	Size size = Size(imgadd.cols, imgadd.rows);
	VideoWriter writer;
	string path = "tmp.avi";
	writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, size);
	Point2f center(x_center, y_center);
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0 + angle / 70);
		Mat rotatedImage;
		warpAffine(img, rotatedImage, rotationMatrix, img.size());
		imgadd = (imgadd + rotatedImage);

		Mat temp_img = seg_img + imgadd;
		Mat result_img;
		addforeground(temp_img, seg_img, origin_img, result_img);
		writer.write(result_img);
	}
	writer.release();
	string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Spiral_" + basename + ".gif";
	system(ffmpegCommand.c_str());
	remove("tmp.avi");
}
/*
void Func::Rotation(Mat img1, Mat& img_out, int x_center, int y_center)
{
	uchar* d_img1, * d_img_out, * d_output1, * d_rotat;
	int size = img1.cols * img1.rows;
	cudaMalloc((void**)&d_img1, size * sizeof(uchar));
	cudaMalloc((void**)&d_img_out, size * sizeof(uchar));

	cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
	cudaMemcpy(d_img_out, img_out.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

	dim3 blockSize(32, 32);
	dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
	for (float angle = 0.0; angle < 240.0; angle += 0.5)
	{
		rotateandscale<< <gridSize, blockSize>> >(d_img1, d_rotat, img1.rows, img1.cols, angle);
	}
	uchar* rotated_image = new uchar[size];
	cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
	Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
	img_out = rotated_mat.clone();
}
*/
void Func::Rotation2(Mat img1, Mat& img_out, int x_center, int y_center)
{
	Mat imgadd = img1.clone();
	Point2f center(x_center, y_center);
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0 + angle / 70);
		Mat rotatedImage;
		warpAffine(img1, rotatedImage, rotationMatrix, img1.size());
		imgadd = (imgadd + rotatedImage);
	}
	img_out = imgadd.clone();
}
void Func::Rotationline(Mat img1, Mat& img_out, int x_center, int y_center)
{
	Mat imgadd = img1.clone();
	Point2f center(x_center, y_center);
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		Mat rotationMatrix = getRotationMatrix2D(center, 0, 1.0 + angle / 70);
		Mat rotatedImage;
		warpAffine(img1, rotatedImage, rotationMatrix, img1.size());
		imgadd = (imgadd + rotatedImage);
	}
	img_out = imgadd.clone();
}
void Func::addforeground(cv::Mat img1, cv::Mat img_back, cv::Mat img_src, cv::Mat& img_out)
{
	Mat img = img1.clone();
	for (int i = 0; i < img_src.rows; i++)
	{
		for (int j = 0; j < img_src.cols; j++)
		{
			if (img_back.at<Vec3b>(i, j)[0] == 0 && img_back.at<Vec3b>(i, j)[1] == 0 && img_back.at<Vec3b>(i, j)[2] == 0)
			{
				for (int k = 0; k < 3; k++)
				{
					img.at<Vec3b>(i, j)[k] = img_src.at<Vec3b>(i, j)[k];
				}
			}
		}
	}
	img_out = img.clone();
}
void Func::calculateStar(Mat image, Point& max_star) 
{
	//start
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(grayImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	cout << contours.size() << "顆" << endl;

	double max = -1;
	vector<StarInfo> stars;
	for (size_t i = 0; i < contours.size(); ++i) {

		StarInfo star;
		size_t size;

		Mat mask = Mat::zeros(image.size(), CV_8UC1);
		drawContours(mask, contours, static_cast<int>(i), Scalar(255), FILLED);

		Scalar meanColor = mean(image, mask);

		double brightness;
		brightness = (meanColor[0] + meanColor[1] + meanColor[2]) / 3;

		size = 0;
		Point center(0, 0);

		//計算星星的中心座標和大小
		for (int y = 0; y < mask.rows; ++y) {
			for (int x = 0; x < mask.cols; ++x) {
				if (mask.at<uchar>(y, x) != 0) {
					center.x += x;
					center.y += y;
					size++;
				}
			}
		}

		if (size > 0) {
			center.x /= size;
			center.y /= size;
		}

		star.brightness = brightness;
		star.size = size;
		star.center = center;

		if (star.size * star.brightness > max) {
			max = star.size * star.brightness;
			max_star = star.center;
		}

		stars.push_back(star);
	}
}
void Func::powerLawTransform(Mat image, double gamma, Mat& output) 
{
	Mat imgNormalized, imgPowerLaw;
	normalize(image, imgNormalized, 0, 1, NORM_MINMAX, CV_32F);
	pow(imgNormalized, gamma, imgPowerLaw);
	normalize(imgPowerLaw, imgPowerLaw, 0, 255, NORM_MINMAX, CV_8U);
	output = imgPowerLaw.clone();
}
void Func::filterAurora_1(Mat image, Mat& output) 
{
	Mat blurredImage;
	GaussianBlur(image, blurredImage, Size(61, 61), 0);
	Mat highPassImage = image - blurredImage;
	/*
	Mat hsvImage;
	cvtColor(highPassImage, hsvImage, COLOR_BGR2HSV);
	// (sat)
	
	int saturationThreshold = 60;
	for (int i = 0; i < hsvImage.rows; ++i) {
		for (int j = 0; j < hsvImage.cols; ++j) {
			if (hsvImage.at<Vec3b>(i, j)[1] > saturationThreshold) {
				hsvImage.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat filteredImage;
	cvtColor(hsvImage, filteredImage, COLOR_HSV2BGR);
	*/
	Mat grayImage;
	cvtColor(highPassImage, grayImage, COLOR_BGR2GRAY);

	// Power-law ，增強暗部細節
	double gamma = 0.25;
	Mat enhancedImage;
	powerLawTransform(grayImage, gamma, enhancedImage);

	Mat binaryImage;
	threshold(enhancedImage, binaryImage, 190, 255, THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	for (size_t i = 0; i < contours.size(); ++i) {
		drawContours(mask, contours, static_cast<int>(i), Scalar(255), FILLED);
	}

	// 保留星星，其他是0
	Mat result;
	image.copyTo(result, binaryImage);
	output = result.clone();
}
void Func::background(cv::Mat img1, cv::Mat& img_out)
{
	Mat result, bg, fg;
	Rect rect(0, 0, img1.cols, img1.rows* 0.8);
	grabCut(img1, result, rect, bg, fg, 1, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);
	Mat fore(img1.size(), CV_8UC3, Scalar(0, 0, 0));
	img1.copyTo(fore, result);
	img_out = fore.clone();
}
void Func::foreground(cv::Mat img1, cv::Mat img_back, cv::Mat& img_out)
{
	Mat result;
	subtract(img1, img_back, result);
	img_out = result.clone();
}
void Func::Resize(cv::Mat img, cv::Mat& out_img, double threshold)
{
	if (img.rows > threshold)
	{
		int ocol = img.cols / (double)(img.rows / (double)threshold);
		cv::resize(img, out_img, cv::Size(ocol, threshold));// img.cols, img.rows
	}
	else {
		out_img = img.clone();
	}
}
void Func::reResize(cv::Mat img, cv::Mat& out_img, int x, int y)
{
	if (img.rows != x)
	{
		cv::resize(img, out_img, cv::Size(y, x));
	}
	else {
		out_img = img.clone();
	}
}
void Func::getMask(Mat img, Mat& img_out)
{
	Mat result, bg, fg;
	Rect rect(0, 0, img.cols, img.rows - 100);
	grabCut(img, result, rect, bg, fg, 2, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);
	Mat fore(img.size(), CV_8UC3, Scalar(0, 0, 0));
	img_out = result.clone();
}
void Func::showImg(cv::Mat img)
{
	Mat out;
	cv::resize(img, out, Size(img.cols / 4, img.rows / 4));
	imshow("display", out);
	waitKey(0);
	//destroyAllWindows();
}
Func::Func(QWidget* parent) : QWidget(parent)
{

}
void Func::Homono_filter(cv::Mat img_in, cv::Mat& img_out)
{
	Mat image_gray, image_output, image_transform;;
	cvtColor(img_in, image_gray, COLOR_BGR2GRAY);
	Mat image_gray2(image_gray.size(), CV_32F);
	for (int i = 0; i < image_gray.rows; i++)
	{
		for (int j = 0; j < image_gray.cols; j++)
		{
			image_gray2.at<float>(i, j) = log(image_gray.at<uchar>(i, j) + 0.1);
		}
	}

	My_DFT(image_gray2, image_output, image_transform);

	Mat planes[] = { Mat_<float>(image_output), Mat::zeros(image_output.size(),CV_32F) };
	split(image_transform, planes);
	Mat image_transform_real = planes[0];
	Mat image_transform_imag = planes[1];

	int core_x = image_transform_real.rows / 2;
	int core_y = image_transform_real.cols / 2;
	int d0 = 10;//*image_transform_real.rows/ image_transform_real.cols;
	float h;
	float rh = 3;// * image_transform_real.rows / (float)image_transform_real.cols;
	float rl = 7;//* image_transform_real.rows / (float)image_transform_real.cols;
	float c = 40;
	for (int i = 0; i < image_transform_real.rows; i++)
	{
		for (int j = 0; j < image_transform_real.cols; j++)
		{
			h = (rh - rl) * (1 - exp(-c * ((i - core_x) * (i - core_x) + (j - core_y) * (j - core_y)) / (d0 * d0))) + rl;
			image_transform_real.at<float>(i, j) = image_transform_real.at<float>(i, j) * h;
			image_transform_imag.at<float>(i, j) = image_transform_imag.at<float>(i, j) * h;

		}
	}
	planes[0] = image_transform_real;
	planes[1] = image_transform_imag;
	Mat image_transform_ilpf;
	merge(planes, 2, image_transform_ilpf);

	Mat iDft[] = { Mat_<float>(image_output), Mat::zeros(image_output.size(),CV_32F) };
	idft(image_transform_ilpf, image_transform_ilpf);
	split(image_transform_ilpf, iDft);
	magnitude(iDft[0], iDft[1], iDft[0]);
	normalize(iDft[0], iDft[0], 0, 1, NORM_MINMAX);
	exp(iDft[0], iDft[0]);
	normalize(iDft[0], iDft[0], 0, 1, NORM_MINMAX);
	iDft[0].convertTo(iDft[0], CV_8U, 255 / 1.0, 0);
	img_out = iDft[0].clone();
	cv::resize(img_out, img_out, img_in.size());
}
void Func::My_DFT(Mat input_image, Mat& output_image, Mat& transform_image)
{
	//int m = getOptimalDFTSize(input_image.rows);
	//int n = getOptimalDFTSize(input_image.cols);
	//copyMakeBorder(input_image, input_image, 0, m - input_image.rows, 0, n - input_image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(input_image), Mat::zeros(input_image.size(), CV_32F) };

	merge(planes, 2, transform_image);

	dft(transform_image, transform_image);
	split(transform_image, planes);
	Mat transform_image_real = planes[0];
	Mat transform_image_imag = planes[1];

	magnitude(planes[0], planes[1], output_image);
	output_image += Scalar(1);
	log(output_image, output_image);
	normalize(output_image, output_image, 0, 1, NORM_MINMAX);

	output_image = output_image(Rect(0, 0, output_image.cols & -2, output_image.rows & -2));

	int cx = output_image.cols / 2;
	int cy = output_image.rows / 2;
	Mat q0(output_image, Rect(0, 0, cx, cy));
	Mat q1(output_image, Rect(cx, 0, cx, cy));
	Mat q2(output_image, Rect(0, cy, cx, cy));
	Mat q3(output_image, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp); q3.copyTo(q0); tmp.copyTo(q3);
	q1.copyTo(tmp); q2.copyTo(q1); tmp.copyTo(q2);


	Mat q00(transform_image_real, Rect(0, 0, cx, cy));
	Mat q01(transform_image_real, Rect(cx, 0, cx, cy));
	Mat q02(transform_image_real, Rect(0, cy, cx, cy));
	Mat q03(transform_image_real, Rect(cx, cy, cx, cy));
	q00.copyTo(tmp); q03.copyTo(q00); tmp.copyTo(q03);
	q01.copyTo(tmp); q02.copyTo(q01); tmp.copyTo(q02);

	Mat q10(transform_image_imag, Rect(0, 0, cx, cy));
	Mat q11(transform_image_imag, Rect(cx, 0, cx, cy));
	Mat q12(transform_image_imag, Rect(0, cy, cx, cy));
	Mat q13(transform_image_imag, Rect(cx, cy, cx, cy));
	q10.copyTo(tmp); q13.copyTo(q10); tmp.copyTo(q13);
	q11.copyTo(tmp); q12.copyTo(q11); tmp.copyTo(q12);

	planes[0] = transform_image_real;
	planes[1] = transform_image_imag;
	merge(planes, 2, transform_image);
}
void Func::mask_generate(cv::Mat img_in, cv::Mat& img_out, cv::Mat& tmp_mask, int row, int col)
{
	Mat img = img_in.clone();
	for (int i = 0; i < img_in.cols; i++)
	{
		for (int j = 0; j < 0.3 * img_in.rows; j++)
		{
			img.at<uchar>(j, i) = 0;
		}
	}
	for (int i = 0; i < img_in.cols; i++)
	{
		unsigned char flag = 0;
		for (int j = 0; j < img_in.rows; j++)
		{
			if (img.at<uchar>(j, i) != 0)
			{
				flag = 255;
			}
			if (flag == 0)
			{
				img.at<uchar>(j, i) = 255;
			}
			else
			{
				img.at<uchar>(j, i) = 0;
			}
		}
	}
	/*
	imshow("Canny1", img);
	waitKey(0);
	for (int i = 0; i < img_in.cols; i++)
	{
	 for (int j = 0; j < 0.3 * img_in.rows; j++)
	 {
	  img.at<uchar>(j, i) = 255;
	 }
	}
	*/
	//imshow("Canny2", img);
	//waitKey(0);
	Mat img2, img3, img4, img5;
	//col:640
	int c1 = round(row * 60 / (float)col);//open
	int c2 = round(row * 30 / (float)col);
	int c3 = round(row * 40 / (float)col);//close
	int c4 = round(row * 15 / (float)col);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(c1, c2), Point(-1, -1));//open
	Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(c3, c4), Point(-1, -1));//close
	Mat kernel3 = getStructuringElement(MORPH_ELLIPSE, Size(25, 25), Point(-1, -1));//close


	morphologyEx(img, img2, CV_MOP_CLOSE, kernel3);
	//imshow("Canny2.5", img2);
	//waitKey(0);
	morphologyEx(img2, img2, CV_MOP_OPEN, kernel);
	//imshow("Canny3", img2);
	//waitKey(0);
	morphologyEx(img2, img3, CV_MOP_CLOSE, kernel2);
	//imshow("Canny4", img3);
	//waitKey(0);

	Mat tmp, tmpp, tmpp1, tmpp2, tmpp3;
	Mat Struct = getStructuringElement(MORPH_ELLIPSE, Size(16, 16));
	Mat Struct_1 = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
	Mat Struct_2 = getStructuringElement(MORPH_ELLIPSE, Size(32, 32));
	//dilate(img3, img4, Struct);
	//erode(img3, tmpp, Struct);
	erode(img3, tmpp, Struct_2);
	erode(tmpp, tmpp1, Struct_2);
	erode(tmpp1, tmp, Struct_2);

	Mat translationMat = (Mat_<float>(2, 3) << 1, 0, 0, 0, 1, -65);
	warpAffine(tmp, tmp, translationMat, tmp.size());

	//cv::resize(img4, img_out, Size(row, col), 0, 0, INTER_LINEAR);
	cv::resize(img3, img_out, Size(row, col), 0, 0, INTER_LINEAR);
	cv::resize(tmp, tmp_mask, Size(row, col), 0, 0, INTER_LINEAR);

	//imshow("img_out", img_out);
	//waitKey(0);
	//imshow("tmp_mask", tmp_mask);
	//waitKey(0);
}
void Func::newbackground(cv::Mat img1, cv::Mat& output, cv::Mat& mask_img, cv::Mat& tmp_mask)
{
	Mat img2, img_out, img_out2, img_out3, img_out4, img_out5, img_out6, img_BGR;
	cv::resize(img1, img2, Size(640, img1.rows * 640 / img1.cols), 0, 0, INTER_LINEAR);
	//imshow("ORIN_IMG", img2);

	Mat hsvImage, aurora_mask;
	cvtColor(img2, hsvImage, COLOR_BGR2HSV);

	Mat auroraMask;
	int c1 = round(img1.rows * 10 / (float)img1.cols);//close
	int c2 = round(img1.rows * 10 / (float)img1.cols);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(c1, c2), Point(-1, -1));//close
	Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));//close
	Mat kernel3 = getStructuringElement(MORPH_ELLIPSE, Size(15, 15), Point(-1, -1));//open

	inRange(hsvImage, Scalar(0, 0, 230), Scalar(0, 60, 255), auroraMask);
	bitwise_not(auroraMask, auroraMask);

	for (int i = 0; i < hsvImage.rows; ++i) {
		for (int j = 0; j < hsvImage.cols; ++j) {

			if (hsvImage.at<Vec3b>(i, j)[0] > 30 && hsvImage.at<Vec3b>(i, j)[0] < 130 && hsvImage.at<Vec3b>(i, j)[1] > 64 && hsvImage.at<Vec3b>(i, j)[1] + hsvImage.at<Vec3b>(i, j)[2] > 260) {
				auroraMask.at<uchar>(i, j) = 0;//blue
			}
			if (hsvImage.at<Vec3b>(i, j)[0] > 30 && hsvImage.at<Vec3b>(i, j)[0] < 130 && hsvImage.at<Vec3b>(i, j)[1] > 64 && hsvImage.at<Vec3b>(i, j)[1] < 128 && hsvImage.at<Vec3b>(i, j)[2] >64) {
				auroraMask.at<uchar>(i, j) = 0;//gray blue green
			}
			if (hsvImage.at<Vec3b>(i, j)[0] > 0 && hsvImage.at<Vec3b>(i, j)[0] < 35 && hsvImage.at<Vec3b>(i, j)[1] < 80 && hsvImage.at<Vec3b>(i, j)[2]>50 && hsvImage.at<Vec3b>(i, j)[2]>192) {
				auroraMask.at<uchar>(i, j) = 0;//orange
			}
			if (hsvImage.at<Vec3b>(i, j)[0] > 120 && hsvImage.at<Vec3b>(i, j)[1] > 90 && hsvImage.at<Vec3b>(i, j)[2] > 180 && hsvImage.at<Vec3b>(i, j)[1] + hsvImage.at<Vec3b>(i, j)[2] > 300) {
				auroraMask.at<uchar>(i, j) = 0;//purple
			}
			if (hsvImage.at<Vec3b>(i, j)[0] < 35 && hsvImage.at<Vec3b>(i, j)[1] < 160 && hsvImage.at<Vec3b>(i, j)[2] > 85 && hsvImage.at<Vec3b>(i, j)[2] < 200) {
				auroraMask.at<uchar>(i, j) = 0;//gray
			}
			if (hsvImage.at<Vec3b>(i, j)[0] > 100 && hsvImage.at<Vec3b>(i, j)[0] < 130 && hsvImage.at<Vec3b>(i, j)[1] < 170 && hsvImage.at<Vec3b>(i, j)[2] > 85 && hsvImage.at<Vec3b>(i, j)[2] < 200) {
				auroraMask.at<uchar>(i, j) = 0;//gray2
			}
		}
	}

	Mat Struct = getStructuringElement(MORPH_ELLIPSE, Size(c1, c2));

	morphologyEx(auroraMask, auroraMask, CV_MOP_CLOSE, kernel2);
	morphologyEx(auroraMask, auroraMask, CV_MOP_OPEN, kernel);
	erode(auroraMask, auroraMask, Struct);

	//mask up shift
	Mat translationMat = (Mat_<float>(2, 3) << 1, 0, 0, 0, 1, -0);
	Mat auroraMaskout;
	auroraMaskout.setTo(Scalar(255));
	warpAffine(auroraMask, auroraMask, translationMat, auroraMask.size());
	//erode(auroraMask, auroraMask, Struct);
	//morphologyEx(auroraMask, auroraMask, CV_MOP_CLOSE, kernel);
	//imshow("auroraMask_close", auroraMask);
	//waitKey(0);

	Mat img_m;

	int k = round(10 * img2.rows / (float)img2.cols);
	if (k % 2 == 0)k += 1;

	//Homono_filter
	medianBlur(img2, img_m, k);
	Homono_filter(img_m, img_out);
	medianBlur(img_out, img_out2, k);

	//imshow("medium", img_out2);
	//waitKey(0);

	///edge detect
	Canny(img_out2, img_out3, 25, 10);

	//imshow("Canny", img_out3);
	//waitKey(0);

	//auro mask
	img_out3.copyTo(img_out4, auroraMask);
	//imshow("auroraMask", img_out4);
	//waitKey(0);

	mask_generate(img_out4, mask_img, tmp_mask, img1.cols, img1.rows);
	cvtColor(mask_img / 255, img_BGR, COLOR_GRAY2BGR);
	mask_img = img_BGR.clone();
	multiply(img_BGR, img1, img_out5);

	//Mat tmp;
	//cv::resize(tmp_mask, tmp, Size(img1.cols, img1.rows), 0, 0, INTER_LINEAR);
	//tmp_mask = tmp.clone();
	output = img_out5.clone();
}
void Func::starSky_mask_generate(cv::Mat img_in, cv::Mat& img_out, cv::Mat& tmp_mask, int row, int col)
{
	Mat img = img_in.clone();
	for (int i = 0; i < img_in.cols; i++)
	{
		for (int j = 0; j < 0.1 * img_in.rows; j++)
		{
			img.at<uchar>(j, i) = 0;
		}
	}
	for (int i = 0; i < img_in.cols; i++)
	{
		unsigned char flag = 0;
		for (int j = 0; j < img_in.rows; j++)
		{
			if (img.at<uchar>(j, i) != 0)
			{
				flag = 255;
			}
			if (flag == 0)
			{
				img.at<uchar>(j, i) = 255;
			}
			else
			{
				img.at<uchar>(j, i) = 0;
			}
		}
	}
	Mat img2, img3, img4, img5;
	int c1 = (round(col / 1000) + 1) * 10;
	int c2 = (round(row / 1000) + 1) * 10;
	int c3 = (round(col / 1000) + 1) * 18;
	int c4 = (round(row / 1000) + 1) * 18;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(c1, c2), Point(-1, -1));
	Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(c3, c4), Point(-1, -1));
	morphologyEx(img, img2, CV_MOP_CLOSE, kernel2);
	morphologyEx(img2, img3, CV_MOP_OPEN, kernel);
	Mat tmp, tmpp;
	Mat Struct = getStructuringElement(MORPH_ELLIPSE, Size(16, 16));
	Mat Struct_1 = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
	Mat Struct_2 = getStructuringElement(MORPH_ELLIPSE, Size(32, 32));
	erode(img3, tmpp, Struct_2);
	erode(tmpp, tmp, Struct_1);
	cv::resize(img3, img_out, Size(row, col), 0, 0, INTER_LINEAR);
	cv::resize(tmp, tmp_mask, Size(row, col), 0, 0, INTER_LINEAR);
}
void Func::starSky_newbackground(cv::Mat img1, cv::Mat& output, cv::Mat& mask_img, cv::Mat& tmp_mask)
{
	Mat img2, img_out, img_out2, img_out3, img_out4, img_out5, img_out6, img_BGR, img_m;
	cv::resize(img1, img2, Size(640, img1.rows * 640 / img1.cols), 0, 0, INTER_LINEAR);
	medianBlur(img2, img_m, 9);
	medianBlur(img_m, img_out2, 13);
	Canny(img_out2, img_out3, 43, 20);
	starSky_mask_generate(img_out3, mask_img, tmp_mask, img1.cols, img1.rows);
	cvtColor(mask_img / 255, img_BGR, COLOR_GRAY2BGR);
	multiply(img_BGR, img1, img_out5);
	output = img_out5.clone();
}
void Func::restore_startrail(Mat& image, Mat& out) 
{
	Mat t;
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	Mat edges;
	double lowThreshold = 50;
	double highThreshold = 150;

	Canny(grayImage, edges, lowThreshold, highThreshold);//找邊緣
	vector<vector<Point>> contours;
	findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat filledImage = Mat::zeros(edges.size(), CV_8UC1); // 用于填充的空白图像
	for (size_t i = 0; i < contours.size(); ++i) {
		drawContours(filledImage, contours, static_cast<int>(i), Scalar(255), FILLED);
	}
	imwrite("edge_fill.jpg", filledImage);


	Mat kernel2 = getStructuringElement(MORPH_CROSS, Size(2, 2));
	morphologyEx(filledImage, filledImage, MORPH_CLOSE, kernel2);
	imwrite("close.jpg", filledImage);


	findContours(filledImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// 填充輪廓
	filledImage = Mat::zeros(edges.size(), CV_8UC1);
	for (size_t i = 0; i < contours.size(); ++i) {
		drawContours(filledImage, contours, static_cast<int>(i), Scalar(255), FILLED);
	}

	//imwrite("filled_again.jpg", filledImage);


	vector<Vec4i> hierarchy;
	findContours(filledImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat starMask = Mat::zeros(image.size(), CV_8UC1);

	for (size_t i = 0; i < contours.size(); i++) {
		size_t size = 0;
		Point center(0, 0);
		double maxDistance = 0.0;

		//計算星星的中心座標和大小
		for (size_t j = 0; j < contours[i].size(); j++) {

			center.x += contours[i][j].x;
			center.y += contours[i][j].y;
			size++;

			double distance;
			Point pt1 = contours[i][j];

			for (size_t k = j + 1; k < contours[i].size(); k++) {
				Point pt2 = contours[i][k];

				distance = norm(pt1 - pt2);

				if (distance > maxDistance) {
					maxDistance = distance;

				}
			}

		}

		if (size > 0) {
			center.x /= size;
			center.y /= size;
		}
		maxDistance = maxDistance * 0.97;//cos(13)
		double width = contours[i].size() / maxDistance;
		int radius = static_cast<int>(ceil(width * 0.5));

		if (radius > 0)
			circle(starMask, center, radius, Scalar(255), -1);


	}

	// 保留星星部分
	Mat result;
	image.copyTo(result, starMask);

	Mat hsvImage;
	cvtColor(result, hsvImage, COLOR_BGR2HSV);

	for (int i = 0; i < hsvImage.rows; ++i) {
		for (int j = 0; j < hsvImage.cols; ++j) {
			hsvImage.at<Vec3b>(i, j)[1] *= 2;
			hsvImage.at<Vec3b>(i, j)[2] *= 2;
		}
	}
	Mat filteredImage;
	cvtColor(hsvImage, result, COLOR_HSV2BGR);

	Mat skyMask = 255 - starMask;
	Mat sky;
	image.copyTo(sky, skyMask); // 提取天空
	medianBlur(sky, sky, 121);//低通 

	Mat finalImage;
	add(result, sky, finalImage);
	out = finalImage.clone();


}

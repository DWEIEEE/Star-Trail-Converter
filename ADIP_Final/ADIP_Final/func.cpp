#include "func.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace MyFunction {
	Mat loadImage(const string& filePath) {
		Mat image = cv::imread(filePath);
		if (image.empty()) {
			cerr << "Error loading image: " << filePath << endl;
		}
		return image;
	}
	void Myclass::loadData()
	{
		vector<Mat> images;

		images.push_back(loadImage("../../Dataset/light_foreground_1.jpg"));
		images.push_back(loadImage("../../Dataset/star_trail_2.jpeg"));

		for (size_t i = 0; i < images.size(); i++) {
			imshow("Image", images[i]);
			waitKey(0);
		}
	}
}
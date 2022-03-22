#define IMAGES_NUMBER 9
#define CSV_DATA_OFFSET 1

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#include "Typedefs.h"


void findCheckerboard(ReferenceRegionsTypes reg, Mat& srcImage, vector<Point2f>& corners) {
	corners.clear();
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_RGB2GRAY);
	Size patternsize(CheckerboardInnerWidth[reg], CheckerboardInnerHeight[reg]);
	corners.reserve(patternsize.width * patternsize.height);

	bool found = findChessboardCorners(grayImage, patternsize, corners,
		CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
		+ CALIB_CB_FAST_CHECK);

	if (!found) {
		cout << "Stosowny komunikat" << endl;
	}

	if (corners.front().x > corners[CheckerboardInnerWidth[reg] - 1].x)
		reverse(corners.begin(), corners.end());

	for (auto c : corners) {
		circle(srcImage, c, 10, Scalar(0, 255, 0));
	}
}


void calc_dist(std::stringstream& ss, float& dist_sum, std::vector<cv::Point2f>& corners, int index, Mat img)
{
	string temp_x, temp_y;
	float x, y;

	ss >> temp_x >> temp_y;
	std::stringstream(temp_x) >> x;
	std::stringstream(temp_y) >> y;
	dist_sum += sqrt(pow(corners[index].x - x, 2) + pow(corners[index].y - y, 2));
	//circle(img, Point(x, y), 3, Scalar(0, 0, 255));

	line(img, Point(x, y), corners[index], Scalar(0, 0, 255), 2);
	circle(img, corners[index], 3, Scalar(0, 255, 0));
}



int main()
{

	const float circle_radius = 3;
	const Scalar cicrcle_color = Scalar(0, 0, 255);
	double avg = 0;
	Point p1(10, 10), p2(100, 100);
	Rect r(p1, p2);
	Mat srcImage;
	Mat images[IMAGES_NUMBER];
	vector<Point2f> corners;


	std::string windowName = "Piotr Sienski 184297";
	std::string imagesDir = "C:/Users/PCu/Desktop/Images/";
	std::string idx = "184297_", ext = ".jpg";
	std::string img_path, line;
	std::stringstream ss;
	float x, y;

	std::ifstream file(imagesDir + idx + ".csv");



	// pominiêcie pierwszych linii pliku CSV
	for (int i = 0; i < CSV_DATA_OFFSET; i++)
		std::getline(file, line);


	for (int i = 0; i < IMAGES_NUMBER; i++) {

		// wczytywanie zdjêæ
		char c = 'A' + i; // zmienna oznaczaj¹ca etykietê wczytywanego zdjêcia 
		img_path = imagesDir + idx + c;
		images[i] = imread(img_path + ext);

		if (!images[i].data) {
			std::cout << "Blad odczytu" << std::endl;
			return -1;
		}

		// pobranie linii z pliku csv
		std::getline(file, line);
		//wyszukanie pierwszej wspó³rzêdnej i przyciêcie ³añcucha znaków do tego miejsca
		std::size_t fpos = line.find(';');
		line = line.substr(fpos);


		// zamiana ';' na ' ' pozwala na wczytywanie ze strumienia
		std::replace(line.begin(), line.end(), ';', ' ');
		// dane zapisane z przecinkami zamiast kropek dla danych zmiennoprzecinkowych, nale¿y to zmieniæ
		std::replace(line.begin(), line.end(), ',', '.');
		//wczytanie linii do strumienia 
		ss << line;
		float dist_sum = 0;
		int count = 0;

		for (int a = 0; a < 4; a++) {
			findCheckerboard(ReferenceRegionsTypes(a), images[i], corners);
			int w = CheckerboardInnerWidth[a], h = CheckerboardInnerHeight[a];
			calc_dist(ss, dist_sum, corners, 0, images[i]);
			calc_dist(ss, dist_sum, corners, w - 1, images[i]);
			calc_dist(ss, dist_sum, corners, (h * w) - 1, images[i]);
			calc_dist(ss, dist_sum, corners, w * (h - 1), images[i]);
			corners.clear();
		}

		avg += dist_sum / 16;

		ss.str(std::string()); // wyczyszczenie strumienia



		//imshow(windowName, images[i]);
		imwrite(to_string(i) + ".jpg", images[i]);
		images[i].release();
		waitKey(0);
	}

	avg /= IMAGES_NUMBER;
	cout << "\n\n\nSrednia roznica : " << avg << endl;

	return 0;
}

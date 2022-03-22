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


void findCheckerboard(ReferenceRegionsTypes reg, Mat& srcImage, vector<Point2f>& pts, vector<Point2f>& templatePts, float reverseScale) {

	Size patternsize(CheckerboardInnerWidth[reg], CheckerboardInnerHeight[reg]);
	vector<Point2f> corners;
	corners.reserve(patternsize.width * patternsize.height);
	bool found = findChessboardCorners(srcImage, patternsize, corners);

	if (!found) {
		cout << "Stosowny komunikat" << endl;
	}
	cornerSubPix(srcImage, corners, patternsize, patternsize, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
	drawChessboardCorners(srcImage, patternsize, corners, true);


	if (corners.front().y > corners.back().y || corners.front().x > corners.back().x)
		reverse(corners.begin(), corners.end());

	for (auto corner : corners) {
		pts.push_back(corner);
	}

	Point2f coord;

	for (int row = 0; row < patternsize.height; row++) {
		for (int col = 0; col < patternsize.width; col++) {
			coord = CheckerboardLTCoordinatesWithMargin[reg] + Point2f(CHECKERBOARD_FIELD_SIZE * col, CHECKERBOARD_FIELD_SIZE * row);
			templatePts.push_back(coord * reverseScale);
		}

	}
}



void calc_color_diff(std::vector<cv::Point2f>& dewrapedCorners, cv::Mat& dewrapedROI, cv::Scalar& colorDif, cv::Scalar  refColors[24])
{
	// wektor zawieraj�cy �rednie warto�ci p�l color chceckera 
	vector<Scalar> means;

	Point2f diff_a = dewrapedCorners[0] - dewrapedCorners[1];
	// d�ugo�� boku kolorowego kwadratu
	float len_a = sqrt(pow(diff_a.x, 2) + pow(diff_a.y, 2));
	Point2f diff_b = dewrapedCorners[1] - dewrapedCorners[2];
	// odleg�o�� mi�dzy kwadratami 
	float len_b = sqrt(pow(diff_b.x, 2) + pow(diff_b.y, 2));
	float len = len_a + len_b;
	Point2f beg = dewrapedCorners[0]; // lewy g�rny r�g pierwszego kwadratu 
	Point2f d;
	Rect r;
	Point2f margin(5, 5);

	// iterujemy po wszystkich 24 polach color checkera 
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 6; col++) {
			// odleg�o�� od pocz�tku 
			d = Point(len * col, len * row);
			r = Rect(beg + d + margin, beg + d + Point2f(len_a, len_a) - margin); // wsp�rz�dne prostok�ta z kt�rego b�dzie brana �rednia
			means.push_back(mean(dewrapedROI(r))); // obliczenie �redniej warto�ci pikseli  i push tej warto�ci do wektora 
			rectangle(dewrapedROI, r, Scalar(255, 255, 255)); // narysowanie prostok�ta w miejscu z kt�rego by�a brana �rednia warto�� pikseli
		}
	}


	// obliczenie sumy r�nic
	for (int count = 0; count < means.size(); count++) {
		colorDif += refColors[count] - means[count];
	}
	// podzielenie sumy r�nic przez ilo�� pr�bek
	for (int count = 0; count < 3; count++) {
		colorDif[count] /= means.size();
	}
}




int main()
{
	// referencyjne warto�ci kolor�w color checkera
	Scalar refColors[] = { Scalar(68,82,115),Scalar(130,150,194),Scalar(157,122,98),Scalar(67,108,87),Scalar(177,128,133),Scalar(170,189,103),
							Scalar(44,126,214),Scalar(166,91,80),Scalar(99,90,193),Scalar(108,60,94),Scalar(64,188,157),Scalar(46,163,224),
							Scalar(150,61,56),Scalar(73,148,70),Scalar(60,54,175),Scalar(31,199,231),Scalar(149,86,187),Scalar(161,133,8),
							Scalar(242,243,243),Scalar(200,200,200),Scalar(160,160,160),Scalar(121,122,122),Scalar(85,85,85),Scalar(52,52,52) };

	vector<Point2f> colorCorners;
	Mat images[IMAGES_NUMBER];

	std::string windowName = "Piotr Sienski 184297";
	std::string imagesDir = "C:/Users/PCu/Desktop/Images/";
	std::string idx = "184297_", ext = ".jpg";
	std::string img_path, line;
	std::string temp_x, temp_y;
	std::stringstream ss;
	float x, y;

	std::ifstream file(imagesDir + idx + ".csv");


	// pomini�cie pierwszych linii pliku CSV
	for (int i = 0; i < CSV_DATA_OFFSET; i++)
		std::getline(file, line);

	for (int i = 0; i < IMAGES_NUMBER; i++) {

		// wczytywanie zdj��
		char c = 'A' + i; // zmienna oznaczaj�ca etykiet� wczytywanego zdj�cia 
		img_path = imagesDir + idx + c;
		images[i] = imread(img_path + ext);

		if (!images[i].data) {
			std::cout << "Blad odczytu" << std::endl;
			return -1;
		}

		// pobranie linii z pliku csv
		std::getline(file, line);
		//wyszukanie pierwszej wsp�rz�dnej i przyci�cie �a�cucha znak�w do tego miejsca
		std::size_t fpos = line.find(';');
		line = line.substr(fpos);


		// zamiana ';' na ' ' pozwala na wczytywanie ze strumienia
		std::replace(line.begin(), line.end(), ';', ' ');
		// dane zapisane z przecinkami zamiast kropek dla danych zmiennoprzecinkowych, nale�y to zmieni�
		std::replace(line.begin(), line.end(), ',', '.');
		//wczytanie linii do strumienia 
		//ss << line;
		ss.str(line);
		//pomini�cie danych o szachownicy
		for (int a = 0; a < 16; a++) {
			ss >> temp_x >> temp_y;
		}

		while (!ss.eof()) {

			ss >> temp_x >> temp_y;

			if (!(std::stringstream(temp_x) >> x) || !(std::stringstream(temp_y) >> y))
				std::cout << " Niepoprawne koordynaty " << std::endl;
			colorCorners.push_back(Point(x, y)); // dodanie punktu wczytanego z pliku .csv do wektora 
			
		}
		std::stringstream().swap(ss); // zamiana strumienia wejsciowego na pusty strumien (wyczyszczenie zawartosci + reset flag)
		vector<Point2f> corners, templateCorners, dewrapedCorners;
		Mat srcImage, greyImage;
		float reverseScale = 25.0f;
		srcImage = images[i];
		cvtColor(srcImage, greyImage, COLOR_RGB2GRAY); 

		findCheckerboard(TOP, greyImage, corners, templateCorners, reverseScale); // znalezienie szachownic i wpisanie ich do wektora corners
		findCheckerboard(LEFT, greyImage, corners, templateCorners, reverseScale);
		findCheckerboard(BOTTOM, greyImage, corners, templateCorners, reverseScale);
		findCheckerboard(RIGHT, greyImage, corners, templateCorners, reverseScale);


		// znalezienie homografii i wyprostowanie zdj�cia
		Mat homography = findHomography(corners, templateCorners, RANSAC);
		Mat dewrapedImage(srcImage.rows, srcImage.cols, srcImage.type());
		warpPerspective(srcImage, dewrapedImage, homography, srcImage.size());


		// wyci�cie ROI z wyprostowanego zdj�cia
		Mat dewrapedROI = dewrapedImage(Rect(reverseScale * TemplateLT, reverseScale * TemplateRB));
		perspectiveTransform(colorCorners, dewrapedCorners, homography);
		Scalar colorDif = (0, 0, 0); // �rednia r�nica warto�ci ka�dego kana�u od warto�ci referencyjnej

		calc_color_diff(dewrapedCorners, dewrapedROI, colorDif, refColors);

		cout << "\n\n\n PRZED : " << colorDif << endl;
		// zapisanie zdj�cia przed transformacj� 
		imwrite(to_string(i) + "_before.jpg", dewrapedROI);

		//iteracja po ka�dym pikselu zdj�cia i dodanie do niego r�nicy mi�dzy �redni� warto�ci� a warto�ci� referencyjn� dla ka�dego kana�u
		for (int row = 0; row < dewrapedROI.rows; row++) {
			for (int col = 0; col < dewrapedROI.cols; col++) {
				Vec3b& color = dewrapedROI.at<Vec3b>(Point(col, row));
				for (int g = 0; g < 3; g++) {
					color[g] += colorDif[g];
					if (color[g] > 255)
						color[g] = 255;
					else if (color[g] < 0)
						color[g] = 0;
				}
				dewrapedROI.at<Vec3b>(Point(col, row)) = color;
			}
		}

		// zapisanie zdj�cia po transformacji 
		imwrite(to_string(i) + "_after.jpg", dewrapedROI);
		calc_color_diff(dewrapedCorners, dewrapedROI, colorDif, refColors);

		cout << "\n PO : " << colorDif << endl;
		//zwolnienie pami�ci
		images[i].release();
		colorCorners.clear();

	}

	return 0;
}

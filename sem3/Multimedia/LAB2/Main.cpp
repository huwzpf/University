#include "opencv2/opencv.hpp"
#include "opencv2/core/types_c.h"
//#include "opencv2/calib3d/calib3d_c.h"

using namespace cv;
using namespace std;


#include "Typedefs.h"


void showImg(string s, int x, int y, Mat img) {
	namedWindow(s);
	moveWindow(s, x, y);
	imshow(s, img);

}

void findCheckerboard(ReferenceRegionsTypes reg, Mat& srcImage, vector<Point2f> &pts, vector<Point2f> &templatePts, float reverseScale) {

	Size patternsize(CheckerboardInnerWidth[reg], CheckerboardInnerHeight[reg]);

	vector<Point2f> corners;
	corners.reserve(patternsize.width * patternsize.height);
	bool found = findChessboardCorners(srcImage, patternsize, corners);

	if (!found) {
		cout << "Stosowny komunikat" << endl;
	}
	cornerSubPix(srcImage, corners, patternsize, patternsize, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
	drawChessboardCorners(srcImage, patternsize, corners, true);
	

	Point diff = corners[0] - corners[1];
	float distance = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
	cout << distance << " - " <<reg << endl;

	if (corners.front().y > corners.back().y || corners.front().x > corners.back().x)
		reverse(corners.begin(), corners.end());

	int i = 0;
	for (auto corner : corners) {

		putText(srcImage, to_string(i), corner, CV_FONT_NORMAL, 0.3, (255, 255, 255));
		i++;
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

int main()
{
	vector<Point2f> corners, templateCorners;
	Mat srcImage, greyImage;
	float reverseScale = 25.0f;
	srcImage = imread("184297_G.jpg");
	cvtColor(srcImage, greyImage, COLOR_RGB2GRAY);

	findCheckerboard(TOP, greyImage, corners, templateCorners, reverseScale);
	findCheckerboard(LEFT, greyImage, corners, templateCorners, reverseScale);
	findCheckerboard(BOTTOM, greyImage, corners, templateCorners, reverseScale);
	findCheckerboard(RIGHT, greyImage, corners, templateCorners, reverseScale);
	imwrite("184297_G_ChessboardFound.jpg", greyImage);



	Mat homography = findHomography(corners, templateCorners, CV_RANSAC);
	Mat dewrapedImage(srcImage.rows, srcImage.cols, srcImage.type());
	warpPerspective(srcImage, dewrapedImage, homography, srcImage.size());


	//imwrite("184297_G_Dewraped.jpg", dewrapedImage);
	//showImg("1", 0, 0, dewrapedImage);
	

	Mat dewrapedROI = dewrapedImage(Rect(reverseScale * TemplateLT, reverseScale * TemplateRB));
	Mat debugImage = dewrapedROI.clone();
	vector<Point2f> dewrapedCorners;
	dewrapedCorners.reserve(corners.size());

	perspectiveTransform(corners, dewrapedCorners, homography);
	
	for (auto c : dewrapedCorners) {
		circle(debugImage, c, 4, (0, 0, 255), 2);
	}
	
	imwrite("184297_G_Dewraped_ROI.jpg", debugImage);


	srcImage.release();

	return(0);
}

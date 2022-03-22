#pragma once

// checkerboards types and number
enum ReferenceRegionsTypes { TOP, LEFT, BOTTOM, RIGHT, CHECKERBOARD_NUM };

// checkerboards locations. The counterclockwise order is required to start searching from the biggest one !!!!
const int CheckerboardInnerWidth[CHECKERBOARD_NUM] = { 13, 3, 11, 3 }; // inne corners
const int CheckerboardInnerHeight[CHECKERBOARD_NUM] = { 3, 12, 3, 10 };

const float CHECKERBOARD_FIELD_SIZE = 0.8f; // size of all checkerboard fields
const float CheckerboardsMargin = 1.6f; // outer margin around checkerboard

// Coordinates of the output region of interest 
const Point2f TemplateLT = Point2f(0.0f, 0.0f); // left-top corner
const Point2f TemplateRB = Point2f(20.0f, 20.0f); // right-bottom corner

const Point2f CheckerboardLTCoordinatesWithMargin[CHECKERBOARD_NUM] =
{ // coordinates of outer left-top corner
	Point2f(4.4f, 0.5f), // TOP
	Point2f(0.5f, 4.8f), // LEFT
	Point2f(5.2f, 16.3f), // BOTTOM
	Point2f(16.3f, 5.6f) // RIGHT
};
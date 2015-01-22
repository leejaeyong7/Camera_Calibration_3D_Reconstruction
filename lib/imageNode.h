#ifndef _IMAGENODE_H_
#define _IMAGENODE_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class imageNode
{
	public:
	imageNode()
	{
		prev = NULL;
		next = NULL;
	}
	~imageNode()
	{
		prev = NULL;
		next = NULL;
	}
	imageNode(Mat * img)
	{
		prev = NULL;
		next = NULL;
		img->copyTo(data);
	}
	imageNode(Mat img)
	{
		prev = NULL;
		next = NULL;
		img.copyTo(data);
	}
	imageNode(const imageNode & rhs)
	{
		if(this==&rhs)
			return;
		prev = rhs.prev;
		next = rhs.next;
		data = rhs.data;
		pointBuf = rhs.pointBuf;
		corners = rhs.corners;
		
	}
	imageNode& operator=(const imageNode & rhs)
	{
		if(this==&rhs)
			return *this;
		prev = NULL;
		next = NULL;
		data = rhs.data;
		pointBuf = rhs.pointBuf;
		corners = rhs.corners;
		return *this;
	}
	imageNode * prev;
	imageNode * next;
	Mat data;
	vector<Point2f> pointBuf;
	vector<Point3f> corners;
	float color[3];
};
#endif

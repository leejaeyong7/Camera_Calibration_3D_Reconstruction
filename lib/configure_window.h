#ifndef _CONFIGURE_WINDOW_H_
#define _CONFIGURE_WINDOW_H_

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include <lib/imageNode.h>

using namespace cv;
using namespace std;

class Configure_Window : public Fl_Window
{
	public:
		Configure_Window()
		:Fl_Window(600,0,350,200,"Configure Window")
		{
			Fl_Box * capture_Title = new Fl_Box(0,0,120,20,"Calibration Menu");
			Fl_Box * capture_Frame = new Fl_Box(FL_DOWN_BOX,5,20,340,70,"");
			
			Fl_Box * calib_Title = new Fl_Box(0,100,130,20,"Save & Load Menu");
			Fl_Box * calib_Frame = new Fl_Box(FL_DOWN_BOX,5,120,340,70,"");
			
			Fl_Box * slider_Title = new Fl_Box(10,30,140,20,"Capture per Frame: ");
				fpsSlider  = new Fl_Value_Slider(150, 30, 180, 20);
				fpsSlider->Fl_Widget::type(FL_HOR_NICE_SLIDER);
				fpsSlider->bounds(1, 120);
				fpsSlider->scrollvalue(30, 1, 1, 120);
				add(fpsSlider);
			
			
				cameraMatrix = new Mat();
				distCoeffs = new Mat();
				calibration_Calculated = false;
				image_captured = false;
			
				imageNamePrefix = "./img/img_";
		}
	
		Fl_Slider * fpsValue() {return fpsSlider;}
		void set_Image_Pointers(imageNode *h, imageNode*t, int* nN) { head = h; tail =t; nodeNumber = nN;}
	
		void save_Captured_Images();
		void load_Captured_Images();
		void calculate_Calibration();
		void save_Coefficients();
		void load_Coefficients();
	
		Mat * cameraMatrix;
		Mat * distCoeffs;
		vector<Mat> rvecs;
		vector<Mat> tvecs;
		bool calibration_Calculated;
		bool image_captured;
	
	private:
		Fl_Value_Slider * fpsSlider;
	
		vector<vector<Point3f>> object_points;
		vector<vector<Point2f>> image_points;
	
		string imageNamePrefix;
		cv::Size IMAGE_SIZE;
	
		imageNode * head;
		imageNode * tail;
		int * nodeNumber;
		double rmsValue;
};

#endif
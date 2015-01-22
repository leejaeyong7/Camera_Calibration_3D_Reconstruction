#ifndef _CAMERA_WINDOW_H_
#define _CAMERA_WINDOW_H_
#include <FL/Fl.H>
#include <FL/Gl.H>

#include <FL/Fl_Slider.H>
#include <FL/Fl_gl_Window.H>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include <lib/imageNode.h>
using namespace cv;
using namespace std;


class Camera_Window : public Fl_Gl_Window
{
	public:
		Camera_Window(int wx, int wy, int wW, int wH, bool * cbv, bool *ubv,const char* name = 0)
		:Fl_Gl_Window(wx, wy, wW, wH, name)
		{
			cap.open(0);
			dInit = false;
			capture_Button_Value = cbv;
			undistort_Button_Value = ubv;
			Fl::add_timeout(1.0/30.0, Timer_CB, (void*)this);
			head = new imageNode();
			tail = new imageNode();
			
			head->next = tail;
			head->prev = NULL;
			
			tail->next= NULL;
			tail->prev = head;
			
			fps_Count = 0;
			imageNodeNum = new int(0);
		}
		~Camera_Window()
		{
			cap.release();
			removeNodes();
		}
	
		void set_Fps(Fl_Slider * fpsPtr) {fps_Value = fpsPtr;}
		void set_Calibration_Coeffs(Mat * cM, Mat * dC) { cameraMatrix = cM; distCoeffs = dC;}
		int handle(int event);
		void draw();
	
		void add_Node(Mat * videoImage);
		void removeNodes();
	
		imageNode * hP() {return head;}
		imageNode * tP() {return tail;}
		void setCurr(imageNode * nCurr) {curr = nCurr;}
		int* nN(){return imageNodeNum;}
	
	private:
		static void Timer_CB(void *userdata)
		{
			Fl_Window *win = (Fl_Window*)userdata;
			win->redraw();
			Fl::repeat_timeout(1.0/30.0, Timer_CB, userdata);
		}
		VideoCapture cap;
		Mat * video;
	
		bool dInit;
		bool * capture_Button_Value;
		bool * undistort_Button_Value;
	
		Fl_Slider * fps_Value;
		int fps_Count;
	
	
		GLuint cameraImageTextureID;
		Mat * cameraMatrix;
		Mat * distCoeffs;
		imageNode * head;
		imageNode * tail;
		imageNode * curr;
	
		int * imageNodeNum;
		int i,j;
};


#endif
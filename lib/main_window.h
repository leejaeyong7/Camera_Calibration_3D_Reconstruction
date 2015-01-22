#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 500
#define WINDOW_TITLE "Camera calibrator"
#define BOARDSIZE_WIDTH 8
#define BOARDSIZE_HEIGHT 6

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>


#include <math.h>
#include <iostream>
#include <vector>
#include <thread>



#include <lib/3D_reconstruction.h>
#include <lib/camera_window.h>
#include <lib/configure_window.h>

using namespace std;

class Main_Window : public Fl_Window
{
	public:
		Main_Window()
		:Fl_Window(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
		{
			//control logic values
			capture_Button_Value = new bool(false);
			undistort_Button_Value = new bool(false);
			
			begin();
			cameraWindow = new Camera_Window(0,25,WINDOW_WIDTH,WINDOW_HEIGHT-100,capture_Button_Value,undistort_Button_Value, "camera window");
			end();
			
			
		}
		void runProgram();
		void set_Fps(Fl_Slider * fpsS) {cameraWindow->set_Fps(fpsS);}
		void set_Calibration_Coeffs(Mat * cameraMatrix, Mat * distortCoeffs) { cameraWindow->set_Calibration_Coeffs(cameraMatrix,distortCoeffs);}
		imageNode* hP(){return cameraWindow->hP();}
		imageNode* tP(){return cameraWindow->tP();}
		void setCurr(imageNode * nCurr) {cameraWindow->setCurr(nCurr);}
		int* nN(){return cameraWindow->nN();}
	
		bool * capture_Button_Value;
		bool * undistort_Button_Value;
	private:
		Camera_Window * cameraWindow;
		Fl_Button * capture_Button;
	
	
};

#endif
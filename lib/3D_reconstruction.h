#ifndef _3D_RECON_H_
#define _3D_RECON_H_
#include <FL/Fl.H>
#include <FL/Gl.H>
#include <vector>
#include <opencv2/opencv.hpp>
#include <lib/imageNode.h>
#include <FL/Fl_Gl_Window.H>
using namespace std;
using namespace cv;

class Reconstruction : public Fl_Gl_Window
{
	public:
		Reconstruction()
		:Fl_Gl_Window(0, 0, 600, 600, "3D Reconstruction")
		{
			dInit =  false;
			rvecs = NULL;
			tvecs = NULL;
			//mode(FL_SINGLE);
			Board = imread("../Data/Board.png");
			hasBeenPushed = false;
			
			xAngle = 0;
			yAngle = 0;
			zAngle = 0;
			xTrans = 0;
			yTrans = 0;
			zScale = 1;
		}
		void draw();
		int handle(int);
	
		void getvectorptrs(vector<Mat> &r, vector<Mat> &t, imageNode * h){rvecs = &r; tvecs = &t; head = h;}

		void Origin();
		void CheckerBoard();
	private:
		bool dInit;
	
		GLfloat xAngle;
		GLfloat yAngle;
		GLfloat zAngle;
		GLfloat xTrans;
		GLfloat yTrans;
		GLfloat zScale;
		GLuint boardImageTextureID;
		Mat Board;
	
		int xpos;
		int ypos;
		bool hasBeenPushed;
		bool dragevent;
	

	
		vector<Mat> *rvecs;
		vector<Mat> *tvecs;
		imageNode * curr;
		imageNode * head;
	Mat *voidmat;
};


#endif
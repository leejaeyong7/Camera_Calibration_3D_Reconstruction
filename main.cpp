#include <lib/main_window.h>
#include <FL/fl_ask.H>
Main_Window * mw;
Reconstruction * reCon;
Configure_Window * configureWindow;
Fl_Menu_Bar * menuBar;

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---------------------------configure button callbacks------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
void open_Configure_Callback(Fl_Widget * obj, void * v)
{
		configureWindow->show();
}

void about_Callback(Fl_Widget * obj, void * v)
{
	
}

void calculate_Calibration_Callback(Fl_Widget * obj, void * v)
{
	configureWindow->calculate_Calibration();
}

void save_Image_Callback(Fl_Widget * obj, void * v)
{
	if (!(configureWindow->calibration_Calculated))
	{
		int ans = fl_ask("would you like to calculate Calibration first?");
		if (1)
		{
			
			//configureWindow->calculate_Calibration();
			if (!(configureWindow->image_captured))
			{
				fl_alert("No images to calculate... Capture valid images first.");
				return;
			}
			configureWindow->calculate_Calibration();
			configureWindow->save_Captured_Images();
			return;
		}
		else
			fl_alert("You must calculate calibration before saving images!");
	}
	if (!(configureWindow->image_captured))
	{
		fl_alert("No images to save... Capture valid images first.");
		return;
	}
	configureWindow->save_Captured_Images();
	
}

void load_Image_Callback(Fl_Widget * obj, void * v)
{
	if ((configureWindow->image_captured))
	{
		if(fl_ask("discard captured images in memory and replace them with saved one?"))
		{
			configureWindow->load_Captured_Images();
			configureWindow->image_captured = true;
		}
	}
	else
	{
		configureWindow->load_Captured_Images();
		configureWindow->image_captured = true;
	}
}
void run_Reconstruction_Callback(Fl_Widget * obj, void * v)
{
	if (!(configureWindow->calibration_Calculated))
	{
		cout<<"calibration not calculated yet"<<endl;
		return;
	}
	reCon->show();
}

void save_Coefficients_Callback(Fl_Widget * obj, void * v)
{
	configureWindow->save_Coefficients();
}

void load_Coefficients_Callback(Fl_Widget * obj, void * v)
{
	configureWindow->load_Coefficients();
}

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-------------------------------Main window callbacks-------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
void capture_Button_Callback(Fl_Widget * obj)
{
	*(mw->capture_Button_Value) = !(*(mw->capture_Button_Value));
	if(*(mw->capture_Button_Value) && !configureWindow->image_captured)
	{
		obj->label("Stop Capturing");
		configureWindow->image_captured = true;
	}
	else if(*(mw->capture_Button_Value) && configureWindow->image_captured)
	{
		mw->setCurr(mw->tP()->prev);
		obj->label("Stop Capturing");
	}
	else
	{
		obj->label("Start Capturing");
	}
}
void undistort_Camera_Callback(Fl_Widget * obj)
{
	if (!(configureWindow->calibration_Calculated))
	{
		cout<<"run calibration calculation first!!"<<endl;
		return;
	}
	*(mw->undistort_Button_Value) = !(*(mw->undistort_Button_Value));
	if(*(mw->undistort_Button_Value))
	{
		obj->label("Unfix distortion");
	}
	else
	{
		obj->label("Fix distortion");
	}
}
Fl_Menu_Item menuitems[] =
{
	{"&Configure", 0,0,0, FL_SUBMENU },
		{"Open Configure &Menu", FL_ALT + 'c', (Fl_Callback *)open_Configure_Callback},
//		{"Start Capturing", FL_ALT + 's', (Fl_Callback *)capture_Button_Callback},
		{0},
/*	{"&Help", 0,0,0, FL_SUBMENU },
		{"&About" , FL_ALT + 'a', (Fl_Callback*)about_Callback},
		{0},*/
	{0}
};

//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//

int main(int argv, char **argc)
{
	mw = new Main_Window();
	configureWindow = new Configure_Window();
	mw->begin();
		mw->set_Fps(configureWindow->fpsValue());
			menuBar = new Fl_Menu_Bar(0,0,800,25);
			menuBar->menu(menuitems);
		mw->add(menuBar);
	
		Fl_Button * run_Capture = new Fl_Button(110,450, 180, 25, "Start Capturing");
		run_Capture->shortcut(FL_Enter);
		run_Capture->callback(capture_Button_Callback);
	
	
		Fl_Button * undistort_Camera = new Fl_Button(310,450, 180, 25, "Fix distortion");
		undistort_Camera->callback(undistort_Camera_Callback);
	
		mw->set_Calibration_Coeffs(configureWindow->cameraMatrix, configureWindow->distCoeffs);
	
	mw->end();
	
	reCon = new Reconstruction();
	configureWindow->begin();
	
		reCon->getvectorptrs(configureWindow->rvecs, configureWindow->tvecs, mw->hP());
	

		Fl_Button * calculate_Calibration = new Fl_Button(20,60, 150, 20, "Calculate Calibration");
		calculate_Calibration->callback(calculate_Calibration_Callback);
		configureWindow->add(calculate_Calibration);
	
		Fl_Button * save_Image = new Fl_Button(20,160, 150, 20,  "Save Captured Images");
		save_Image->callback(save_Image_Callback);
		configureWindow->add(save_Image);
	
		Fl_Button * load_Image = new Fl_Button(180,160, 150, 20,  "Load Captured Images");
		load_Image->callback(load_Image_Callback);
		configureWindow->add(load_Image);
		
		Fl_Button * save_Coefficients = new Fl_Button(20,130, 150, 20,"Save Coefficients");
		save_Coefficients->callback(save_Coefficients_Callback);
		configureWindow->add(save_Coefficients);
	
		Fl_Button * load_Coefficients = new Fl_Button(180,130, 150, 20, "Load Coefficients");
		load_Coefficients->callback(load_Coefficients_Callback);
		configureWindow->add(load_Coefficients);
	
	
		configureWindow->set_Image_Pointers(mw->hP(),mw->tP(), mw->nN());
	
	
	
		Fl_Button * run_Reconstruction = new Fl_Button(180,60, 150, 20, "Run 3D Reconstruction");
		run_Reconstruction->callback(run_Reconstruction_Callback);
		configureWindow->add(run_Reconstruction);
	
	
	configureWindow->end();
	
	mw->show(argv,argc);
	mw->runProgram();
	
	return 0;
}

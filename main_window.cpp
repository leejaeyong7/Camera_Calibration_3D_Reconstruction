#include <lib/main_window.h>

void Main_Window::runProgram()
{
	//cout<<"program running"<<endl;
	if(!(child(0)->as_gl_window()->valid()))
	{
		child(0)->show();
	}
	while(shown())
	{
		Fl::wait();
	}
	return;
}
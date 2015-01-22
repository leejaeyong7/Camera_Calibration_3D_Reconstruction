#include <lib/camera_window.h>

void Camera_Window::draw()
{
	if (!dInit)
	{
		//cap >> video;
		glClearDepth(1.0f);
		glClearColor(0.75, 0.75, 0.75, 0);
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		dInit = true;
		video =  new Mat();
		j = 30;
		
		i =0;
	}
	if (!cap.isOpened()) {
		cout<<"camera not opened..."<<endl;
	}
	/*string imageName = "./img/img_";
	imageName+=to_string(i);
	imageName+=".png";
	(*video) = imread(imageName);*/
	
	cap >> (*video);
	if (*capture_Button_Value)
	{
		if(!fps_Count)
		{
			add_Node(video);
			fps_Count = fps_Value->value();
			bitwise_not ( *video, *video );
		}
		fps_Count--;
	}
/*	if (!j)
	{
		i++;
		if (i == *imageNodeNum)
		{
			i = 0;
		}
		j=30;
	}*/
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (*undistort_Button_Value)
	{
		Mat retval;
		/*Mat R;
		Mat nCM;
		Size IMAGE_SIZ = video->size();
		IMAGE_SIZ.width=752/16*20;
		IMAGE_SIZ.height=480/16*20;
		Mat map1;
		Mat map2;
		initUndistortRectifyMap(*cameraMatrix, *distCoeffs, R, nCM, IMAGE_SIZ, CV_32FC1,  map1,  map2);
		
		remap(*video, retval, map1, map2,INTER_LINEAR,BORDER_TRANSPARENT);*/
		undistort(*video, retval, *cameraMatrix, *distCoeffs);
		*video = retval;
		/*
		string imageName = "./img2/img_";
		imageName+=to_string(i);
		imageName+=".png";
		imwrite(imageName,(*video));
		
		j--;*/
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if( (video->rows > 0) && (video->cols > 0) )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, cameraImageTextureID);
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video->cols, video->rows, 0, GL_BGR, GL_UNSIGNED_BYTE, video->data);
		glBindTexture(GL_TEXTURE_2D, cameraImageTextureID);
		glColor3f(1,1,1);
		
		glBegin(GL_QUADS);
		
		float x = 1;
		float y = 1;
		
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-x,y, 0.0f);
		glTexCoord2f(1.0f, 0.0f);glVertex3f( x, y, 0.0f);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( x, -y, 0.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex3f(-x, -y, 0.0f);
		
		glEnd();
		
	}
	else
	{
		cout<<"no video..."<<endl;
	}
	glDisable(GL_TEXTURE_2D);

}

int Camera_Window::handle(int event)
{
	switch (event)
	{
		default:
			break;
	}
	return event;
}

void Camera_Window::add_Node(Mat * videoImage)
{
	//cout<<"came into addnode"<<endl;
	if(*imageNodeNum>100)
	{
		cout<<"too many images.. not taking new images"<<endl;
	}
	if(head->next==tail)
	{
		curr = new imageNode(videoImage);
		head->next = curr;
		curr->prev = head;
		
		tail->prev = curr;
		curr->next = tail;
	}
	else
	{
		imageNode * save = curr;
		curr = new imageNode(videoImage);
		save->next = curr;
		curr->prev = save;
		
		tail->prev = curr;
		curr->next = tail;
	}
	(*imageNodeNum)++;
}

void Camera_Window::removeNodes()
{
	curr = head->next;
	while(curr!=tail)
	{
		imageNode * save = curr->next;
		delete curr;
		curr=NULL;
		curr = save;
	}
}

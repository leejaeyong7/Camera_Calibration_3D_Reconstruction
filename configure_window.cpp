#include <lib/configure_window.h>
void Configure_Window::save_Captured_Images()
{
	imageNode * curr =head->next;
	
	for(int i= 0; i<*nodeNumber; i++)
	{
		string imageName = imageNamePrefix;
		imageName+=to_string(i);
		imageName+=".png";
		imwrite(imageName,curr->data);
		curr = curr->next;
	}
	string filename = "./Data/iN.xml";
	FileStorage fs(filename, FileStorage::WRITE);
	fs << "numImage" << *nodeNumber;
	fs.release();
}
void Configure_Window::load_Captured_Images()
{
	if (head->next !=tail)
	{
		int i = 0;
		imageNode * temp = head->next;
		while (temp !=tail)
		{
			imageNode * temp2 = temp->next;
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
			temp = temp2;
		}
	}
	
	FileStorage fs("./Data/iN.xml", FileStorage::READ);
	fs["numImage"] >> *nodeNumber;
	fs.release();

	imageNode * save;
	if (!(*nodeNumber))
	{
		fl_alert("no images to be loaded");
		return;
	}
	save = head;
	for (int i = 0; i < *nodeNumber; i++)
	{
		
		string imageName = imageNamePrefix;
		imageName+=to_string(i);
		imageName+=".png";
		imageNode* curr = new imageNode();
		curr->data = imread(imageName);
		if(curr->data.empty())
		{
			(*nodeNumber)--;
			delete curr;
			continue;
		}
		if (head ==save)
		{
			curr->prev = head;
			head->next = curr;
		}
		else
		{
			curr->prev = save;
			save->next = curr;
		}
		save = curr;
	}
	save->next = tail;
	tail->prev = save;
}
void Configure_Window::save_Coefficients()
{
	if (!calibration_Calculated)
	{
		cout<<"calculate calibration first!!"<<endl;
		return;
	}
	string filename = "./Data/Data.xml";
	FileStorage fs(filename, FileStorage::WRITE);
	fs << "cameraMatrix" << *cameraMatrix;
	fs << "distCoeffs" << *distCoeffs;
	fs << "size" << IMAGE_SIZE;
	fs << "rvecs" <<rvecs;
	fs << "tvecs" <<tvecs;
	fs.release();
}

void Configure_Window::load_Coefficients()
{
	FileStorage fs("./Data/Data.xml", FileStorage::READ);
	if(!fs.isOpened())
	{
		cout<<"fail reading"<<endl;
		return;
	}
	fs["cameraMatrix"] >> *cameraMatrix;
	fs["distCoeffs"] >> *distCoeffs;
	fs["size"] >> IMAGE_SIZE;
	fs["rvecs"] >> rvecs;
	fs["tvecs"] >> tvecs;
	
	fs.release();
	calibration_Calculated = true;
		
}
void Configure_Window::calculate_Calibration()
{
	imageNode * curr =head->next;
	int i = 0;
	IMAGE_SIZE = curr->data.size();
	cout<<IMAGE_SIZE<<endl;
	
	while (curr!=tail)
	{
		
		Size boardSize(8,6);
		bool found = findChessboardCorners( curr->data, boardSize, curr->pointBuf,
										   CV_CALIB_CB_ADAPTIVE_THRESH |
										   CV_CALIB_CB_FAST_CHECK |
										   CV_CALIB_CB_NORMALIZE_IMAGE);
		
		if(!found)
		{
			//cout<<"no checkerboard found"<<endl;
			imageNode * save = curr->next;
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			delete curr;
			curr = save;
			(*nodeNumber)--;
			continue;
		}
		if(curr->data.empty())
		{
			//cout<<"not a valid image.."<<endl;
			imageNode * save = curr->next;
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			delete curr;
			curr = save;
			(*nodeNumber)--;
			continue;
		}
		Mat viewGray;
		cvtColor(curr->data, viewGray, CV_BGR2GRAY);
		
		cornerSubPix( viewGray, curr->pointBuf, Size(11,11), Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
		
		
		vector<Point3f> corner;
		for(int j=0;j<48;j++)
			corner.push_back(Point3f(j/8, j%8, 0.0f));
		curr->corners = corner;
		Mat curdata;
		curr->data.copyTo(curdata);
		drawChessboardCorners( curdata, boardSize, Mat(curr->pointBuf), found );
		
		object_points.push_back(curr->corners);
		image_points.push_back(curr->pointBuf);
		
		curr = curr->next;
	}
	
	*cameraMatrix = Mat::eye(3, 3, CV_64F);
		
	cameraMatrix->at<double>(0,0) = 1.0;
		
	*distCoeffs = Mat::zeros(8, 1, CV_64F);
	
	if (object_points.size() ==0)
	{
		cout<<"no images for calibration"<<endl;
		return;
	}
	rmsValue = calibrateCamera(object_points, image_points, IMAGE_SIZE, *cameraMatrix, *distCoeffs, rvecs, tvecs);
	
	cout<<"average Error: "<<rmsValue<<endl;
	cout<<"total number of Images used for calculation: "<<*nodeNumber<<endl;
	
	calibration_Calculated = true;
	if (head->next!=tail) {
		image_captured = true;
	}
	else
		image_captured = false;

}



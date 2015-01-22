#include <lib/3D_reconstruction.h>

void Reconstruction::draw()
{
	
	if(!valid())
	{
		cout<<"initializing...3D"<<endl;
		dInit = true;
		glClearColor(0.8f, 0.9f, 0.9f, 1.0);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);

		
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	if(rvecs->size() == 0)
	{
		return;
	}
	
	glLoadIdentity();
	glOrtho(-13.0, 13.0, 13.0,-13.0,-3000.0,3000.0);
	glTranslatef(-7, 7, 0);
	glTranslatef(xTrans,yTrans,0);
	glScalef(0.7*zScale,0.7*zScale,0.7*zScale);
	glRotatef(-15+xAngle*0.3, 1,0,0);
	glRotatef(-15+yAngle, 0,1,0);
	glRotatef(180, 0,1,0);
	glRotatef(zAngle, 0,0,1);
	
	glPushMatrix();
		Origin();
	glPopMatrix();
	
	glPushMatrix();
		glLineWidth(0.5);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
			for(int i = -100; i<100; i++)
			{
				glVertex3f(-3000.0, 0.0, i*25);
				glVertex3f(3000.0, 0, i*25);
				
				glVertex3f(i*25, 0.0, -3000.0);
				glVertex3f(i*25, 0, 3000);
			}
		glEnd();
	
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(0.0, -3000.0, 0.0);
			glVertex3f(0.0, 3000, 0.0);
		glEnd();
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, boardImageTextureID);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Board.rows, Board.cols, 0, GL_BGR, GL_UNSIGNED_BYTE, Board.data);
	glBindTexture(GL_TEXTURE_2D, boardImageTextureID);
	
	glPushMatrix();

		//curr = head->next;
		for(int i = 0; i<rvecs->size(); i++)
		{
			glPushMatrix();
				//translation
				glTranslatef((*tvecs)[i].at<double>(0,0), (*tvecs)[i].at<double>(0,1) , (*tvecs)[i].at<double>(0,2));
				
				//rotation
				GLfloat theta = sqrt(pow((*rvecs)[i].at<double>(0,0),2)+pow((*rvecs)[i].at<double>(0,1),2)+pow((*rvecs)[i].at<double>(0,2),2));
				GLfloat xR = (*rvecs)[i].at<double>(0,0)/theta;
				GLfloat yR = (*rvecs)[i].at<double>(0,1)/theta;
				GLfloat zR = (*rvecs)[i].at<double>(0,2)/theta;
				
				
				glRotatef(theta*180/3.141592, xR, yR, zR);
				
				//curr = curr->next;
				CheckerBoard();
			
			glPopMatrix();
		}
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	return;
}
int Reconstruction::handle(int event)
{
	switch(event)
	{
		case FL_KEYDOWN:
			int key;
			key = Fl::event_key();
			switch(key)
			{
				case 97: //a
					yAngle+=2;
					break;
				case 100://d
					yAngle-=2;
					break;
				case 119://w
					zScale*=1.1;//zAngle+=2;
					break;
				case 115://s
					zScale*=1/(1.1);//zAngle-=2;
					break;
				case 113://q
					xAngle+=2;
					break;
				case 101://e
					xAngle-=2;
					break;
				case 122://z
					xAngle=0;
					yAngle=0;
					zAngle=0;
					zScale = 1;
					break;
			}
			redraw();
			break;
		case FL_PUSH:
			hasBeenPushed = true;
			xpos = Fl::event_x();
			ypos = Fl::event_y();
		case FL_DRAG:
			if (hasBeenPushed)
			{
				xTrans-=0.1*(xpos - Fl::event_x());
				yTrans-=0.1*(ypos - Fl::event_y());
				xpos = Fl::event_x();
				ypos = Fl::event_y();
				redraw();
			}
			
			
			break;
		case FL_RELEASE:
			
			hasBeenPushed = false;
			redraw();
			break;
		default:
			redraw();
	}
	return Fl_Gl_Window::handle(event);
}


void Reconstruction::Origin()
{

	glColor3f( 0.0f, 0.0f, 0.0f );
	glScalef(2.0,1.0,1.0);
		glBegin(GL_QUADS);
		glVertex3f( 0.5f, 1.0f, -1.0f);
		glVertex3f( 0.5f, -1.0f, -1.0f);
		glVertex3f( -0.5f, -1.0f, -1.0f);
		glVertex3f( -0.5f, 1.0f, -1.0f);
	
		glVertex3f( 0.5f, 1.0f, 1.0f);
		glVertex3f( 0.5f, 1.0f, -1.0f);
		glVertex3f( -0.5f, 1.0f, -1.0f);
		glVertex3f( -0.5f, 1.0f, 1.0f);
	glEnd();
}
void Reconstruction::CheckerBoard()
{
	glColor3f(1,1,1);
	


	glBegin( GL_QUADS );
		glScalef(1.0,1.0,1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f( 1.0f, 2.0f, 0.1f );
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, -2.0f, 0.1f );
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f, -2.0f, 0.1f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 2.0f, 0.1f);
		
		glVertex3f( 1.0f, 2.0f, -0.1f);
		glVertex3f( 1.0f, -2.0f, -0.1f);
		glVertex3f( -1.0f, -2.0f, -0.1f);
		glVertex3f( -1.0f, 2.0f, -0.1f);
		
		glVertex3f( 1.0f, 2.0f, 0.1f);
		glVertex3f( 1.0f, 2.0f, -0.1f);
		glVertex3f( -1.0f, 2.0f, -0.1f);
		glVertex3f( -1.0f, 2.0f, 0.1f);
		
		glVertex3f( 1.0f, -2.0f, 0.1f);
		glVertex3f( 1.0f, -2.0f, -0.1f);
		glVertex3f( -1.0f, -2.0f, -0.1f);
		glVertex3f( -1.0f, -2.0f, 0.1f);
		
		glVertex3f( 1.0f, 2.0f, 0.1f);
		glVertex3f( 1.0f, 2.0f, -0.1f);
		glVertex3f( 1.0f, -2.0f, -0.1f);
		glVertex3f( 1.0f, -2.0f, 0.1f);
		
		glVertex3f( -1.0f, 2.0f, 0.1f);
		glVertex3f( -1.0f, 2.0f, -0.1f);
		glVertex3f( -1.0f, -2.0f, -0.1f);
		glVertex3f( -1.0f, -2.0f, 0.1f);
	
	glEnd();
	
}
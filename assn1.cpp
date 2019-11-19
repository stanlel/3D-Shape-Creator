//		          Programming Assignment #1
//
//			        Victor Zordan
//
//
//
/***************************************************************************/

/* Include needed files */
//modified to work on my mac
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>



#define WIDTH 500
#define HEIGHT 500

using namespace std;

struct Point
{
  int xPoint;
  int yPoint;
};

vector<Point> pointVector;

void write_pixel(int x, int y, double intensity);
void lineSegment (int x1, int y1, int x2, int y2);
void drawFaces();
void translation(unsigned char key);
void matrixMul(char rotateType, double theta);
void rotation(unsigned char key);
void scale(unsigned char key);





int x_last,y_last;
bool leftClicked;
bool somethingToggled;
bool lineToggled;
bool triToggled;
bool recToggled;
bool circToggled;
bool elipToggled;
bool colorToggled;
bool transformToggled;

struct Vertex
{
  double xPoint;
  double yPoint;
  double zPoint;
};

struct faceIndex
{
  int vert1;
  int vert2;
  int vert3;
};

ifstream inFile;
vector<Vertex> vertexVector;
vector<faceIndex> faceVector;

bool WASDtoggled;
bool translationToggled;
bool rotationToggled;
bool scaleToggled;
//double theta;



// //window size
//int xWindow = 500;
int yWindow = 500;



int input;

/***************************************************************************/
void colorPicker()
{
}

/***************************************************************************/
//turns off all toggles
void clearToggles()
{
  cout << "Clearing Toggles" << endl;
  somethingToggled = false;
  lineToggled = false;
  triToggled = false;
  recToggled = false;
  elipToggled = false;
}

void clearTransformToggles()
{
  cout << "Clearing transform toggles " << endl;
  //WASDtoggled = false;
  translationToggled = false;
  rotationToggled = false;
  scaleToggled = false;
  transformToggled = false;
}

/***************************************************************************/

void init_window()
/* Clear the image area, and set up the coordinate system */
{
        /* Clear the window */
        glClearColor(0.0,0.0,0.0,0.0);
	      glShadeModel(GL_SMOOTH);
        glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
}

/***************************************************************************/

void write_pixel(int x, int y, double intensity)
/* Turn on the pixel found at x,y */
{
        glColor3f (intensity, intensity, intensity);
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();
}

//***************************************************************************/

void display ( void )   // Create The Display Function
{
  if(transformToggled == true){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawFaces();
  }
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen
  write_pixel(x_last,y_last,1.0);//<-you can get rid of this call if you like
  glutSwapBuffers();                                      // Draw Frame Buffer
  drawFaces();

}

/***************************************************************************/

//creates line segment using midpoint function
void lineSegment (int x1, int y1, int x2, int y2)
{
  int temp1 = x1;
  int temp2 = y1;
  //flip coordinates if x1 is greater than x2
  if(x1 > x2)
  {
    x1 = x2;
    y1 = y2;
    x2 = temp1;
    y2 = temp2;
  }

  float slope;
  float dy;
  float dx;

  //calculate slope
  dy = (y2 - y1);
  dx = (x2 - x1);
  slope = dy / dx;

  int a,b,d;
  int x, y;

  //shallow positive lines
  if(slope <= 1 && slope >= 0)
  {
    x = x1;
    y = y1;
    //dy
    a = y2 - y1;
    b = -(x2 - x1);
    d = a + b/2;
    //draw slope
    for(int i = x1; i < x2; i++){
      write_pixel(i, y, 1);
      if(d < 0){
      //  i = i + 1;
        d = d + a;
      }
      else{
      //  i = i + 1;
        y = y + 1;
        d = d + a + b;
      }
    }
  }
  //steep positive line
  else if(slope > 1)
  {
    x = x1;
    y = y1;
    //dy
    a = y2 - y1;
    b = -(x2 - x1);
    d = b + a/2;
    //draw slope
    for(int i = y1; i < y2; i++){
      write_pixel(x, i, 1);
      if(d > 0){
      //  i = i + 1;
        d = d + b;
      }
      else{
      //  i = i + 1;
        x = x + 1;
        d = d + a + b;
      }
    }
  }
  //shallow negative lines
  else if(slope < 0 && slope > -1)
  {
    x = x1;
    y = y1;
    //dy
    a = y2 - y1;
    b = x2 - x1;
    d = a + b/2;
    //draw slope
    for(int i = x1; i < x2; i++){
      write_pixel(i, y, 1);
      if(d > 0){
      //  i = i + 1;
        d = d + a;
      }
      else{
      //  i = i + 1;
        y = y - 1;
        d = d + a + b;
      }
    }
  }
  //steep negtive line
  else if(slope <= -1)
  {
    int temp1 = x1;
    int temp2 = y1;
    //flip coordinates if x1 is greater than x2
    if(y1 > y2)
    {
      x1 = x2;
      y1 = y2;
      x2 = temp1;
      y2 = temp2;
    }
    //printf("NEGTIVE SLOPE DOG\n" );
    x = x1;
    y = y1;
    //dy
    a = -(y2 - y1);
    b = -(x2 - x1);
    d = b + a/2;
    //draw slope
    for(int i = y1; i < y2; i++){
      write_pixel(x, i, 1);
      if(d < 0){
      //  i = i + 1;
        d = d + b;
      }
      else{
      //  i = i + 1;
        x = x - 1;
        d = d + a + b;
      }
    }
  }


  //testing
  //printf("this is working FUNCTION: ");
  //printf("%d , %d | %d , %d\n",x1,y1,x2,y2);
  //printf("%f , %f \n",dx,dy);
  //printf("slope is %f \n", slope);


}

/***************************************************************************/
//creates rectangle using linesegment function
void rectangle(int x1, int y1, int x2, int y2)
{
  lineSegment(x1, y1, x1, y2);
  lineSegment(x1, y1, x2, y1);
  lineSegment(x2, y1, x2, y2);
  lineSegment(x2, y2, x1, y2);
}

/***************************************************************************/
//creates triangle using linesegment function
void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
  lineSegment(x1, y1, x2, y2);
  lineSegment(x2, y2, x3, y3);
  lineSegment(x3, y3, x1, y1);
}

/***************************************************************************/
//copies pixels across quadrants for an elipsis
void copyPoints(int x, int y, int a, int b)
{
  write_pixel(x + a, y + b, 1);
  write_pixel(-x + a, y + b, 1);
  write_pixel(x + a, -y + b, 1);
  write_pixel(-x + a, -y + b, 1);
}

/***************************************************************************/
//copies pixels across quadrants for a circle
void circlePoints(int x, int y, int a, int b)
{
  write_pixel(x + a, y + b, 1);
  write_pixel(y + a, x + b, 1);
  write_pixel(y + a, -x + b, 1);
  write_pixel(x + a, -y + b, 1);

  write_pixel(-x + a, -y + b, 1);
  write_pixel(-y + a, -x + b, 1);
  write_pixel(-y + a, x + b, 1);
  write_pixel(-x + a, y + b, 1);
}

/***************************************************************************/
//creates a circle using midpoint
void circle(int x1, int y1, int x2, int y2)
{
  int dx, dy, distx, disty, dist, x, y, d;

  //distance formula
  dx = x2 - x1;
  dy = y2 - y1;
  distx = pow(dx, 2);
  disty = pow(dy, 2);

  dist = sqrt(distx + disty);

  x = dist;
  y = 0;

  d = 5/4 - dist;

  while(x > y)
  {
    circlePoints(x,y, x1, y1);
    if(d < 0)
    {
      y = y + 1;
      d = d + 2 * y + 3;
    }
    else
    {
      x = x - 1;
      y = y + 1;
      d = d + 2 * (y - x) + 5;
    }
  }

}

/***************************************************************************/

//ELIPSIS
void elipsis(int x1, int y1, int x2, int y2)
{
  int x, y, a, b, d;
  //get center of elipsis
  int dx, dy;
  dx = x2 - x1;
  dy = y2 - y1;

  //shifting variables
  int e, f;
  e = (x1 + x2)/2;
  f = (y1 + y2)/2;

  a = abs(dx/2);  //x axis radius
  b = abs(dy/2);   //y axis radius

  x = a;
  y = 0;
  dy = x * b * b;
  dx = 0;
  d = a * a + b * b * (0.25 - a);

  //Bottom quadrant of elipsis
  while (dy > dx)
  {
    //printf("this is working FUNCTION: ELIP1");
    copyPoints(x, y, e, f);
    y++;
    dx = y * a * a;
    if(d < 0)
    {
      d = d + (a * a * (2 * y + 3));
    }
    else
    {
      x--;
      dy = x * b * b;
      d = d + (a * a * (2 * y + 3)) + (2 * b * b * (1-x));
    }
  }

  //reset values
  x = 0;
  y = b;
  dy = 0;
  dx = y * a * a;
  d = b * b + a * a * (0.25 - b);

  //Upper quadrant of elipsis
  while(dy < dx)
  {
    //printf("this is working FUNCTION: ELIP2");
    copyPoints(x,y,e,f);
    x++;
    dy = x * b *b;
    if(d < 0)
    {
      d = d + (b * b * (2 * x + 3));
    }
    else
    {
      y--;
      dx = y * a * a;
      d = d + (b * b * (2 * x + 3)) + (2 * a * a * (1 - y));
    }
  }
}
/***************************************************************************/

void mouse(int button, int state, int x, int y)
{
/* This function I finessed a bit, the value of the printed x,y should
   match the screen, also it remembers where the old value was to avoid multiple
   readings from the same mouse click.  This can cause problems when trying to
   start a line or curve where the last one ended */
  static int oldx = 0;
  static int oldy = 0;

	int mag;

  //only registers the left click when pressed down
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
	   y *= -1;  //align y with mouse
	   y += 500; //ignore
	   mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
     if (mag > 20)
     {
       printf(" x,y is (%d,%d)\n", x,y);
	   }

     oldx = x;
     oldy = y;

	   x_last = x;
	   y_last = y;

  }

  //line toggle
  if(lineToggled == true && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point oldPoint;
    oldPoint.xPoint = oldx;
    oldPoint.yPoint = oldy;
    //adds points to vector
    pointVector.push_back(oldPoint);

    input++;

    //when two points are found, even numbers
    if(input == 2)
    {
      lineSegment(pointVector[pointVector.size()-2].xPoint, pointVector[pointVector.size()-2].yPoint,
                  pointVector[pointVector.size()-1].xPoint, pointVector[pointVector.size()-1].yPoint);
      input = 0;

      if(input == 2)
        pointVector.clear();
    }
  }

  if(recToggled == true && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point oldPoint;
    oldPoint.xPoint = oldx;
    oldPoint.yPoint = oldy;
    //adds points to vector
    pointVector.push_back(oldPoint);

    input++;

    //when two points are found, even numbers
    if(input == 2)
    {
      rectangle(pointVector[pointVector.size()-2].xPoint, pointVector[pointVector.size()-2].yPoint,
                pointVector[pointVector.size()-1].xPoint, pointVector[pointVector.size()-1].yPoint);
      input = 0;

      if(input == 2)
        pointVector.clear();
    }
  }

  if(triToggled == true && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point oldPoint;
    oldPoint.xPoint = oldx;
    oldPoint.yPoint = oldy;
    //adds points to vector
    pointVector.push_back(oldPoint);
    input++;
    //when three points are found
    if(input == 3)
    {
      triangle( pointVector[pointVector.size()-3].xPoint, pointVector[pointVector.size()-3].yPoint,
                pointVector[pointVector.size()-2].xPoint, pointVector[pointVector.size()-2].yPoint,
                pointVector[pointVector.size()-1].xPoint, pointVector[pointVector.size()-1].yPoint);
      input = 0;

      if(input == 3)
        pointVector.clear();
    }
  }

  if(circToggled == true && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point oldPoint;
    oldPoint.xPoint = oldx;
    oldPoint.yPoint = oldy;
    //adds points to vector
    pointVector.push_back(oldPoint);
    input++;
    //when two points are found, even numbers
    if(input == 2)
    {
      circle(pointVector[pointVector.size()-2].xPoint, pointVector[pointVector.size()-2].yPoint,
                  pointVector[pointVector.size()-1].xPoint, pointVector[pointVector.size()-1].yPoint);
      input = 0;
      if(input == 2)
        pointVector.clear();
    }
  }

  if(elipToggled == true && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point oldPoint;
    oldPoint.xPoint = oldx;
    oldPoint.yPoint = oldy;
    //adds points to vector
    pointVector.push_back(oldPoint);
    input++;
    //when two points are found, even numbers
    if(input == 2)
    {
      elipsis(pointVector[pointVector.size()-2].xPoint, pointVector[pointVector.size()-2].yPoint,
                  pointVector[pointVector.size()-1].xPoint, pointVector[pointVector.size()-1].yPoint);
      input = 0;
      if(input == 2)
        pointVector.clear();
    }
  }
}

/***************************************************************************/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key )
  {
		case 27:              // When Escape Is Pressed...
    {
			exit ( 0 );   // Exit The Program
			break;
    }
	  case '1':
    {             // stub for new screen
    printf("New screen\n");
    //init_window();
			break;
    }
    case 'd':
    {             // stub for new screen
      printf("New screen\n");
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      break;
    }
    //line segment
    case 'l':
    {
      if(lineToggled == true)
      {
        printf("Line mode already toggled on\n");
      }
      else if(somethingToggled == true)
      {
        clearToggles();
        printf("Clearing toggles\n");
        lineToggled = true;
        somethingToggled = true;
        printf("Line Mode Toggled [ON]\n");
      }
      else
      {
        lineToggled = true;
        somethingToggled = true;
        printf("Line Mode Toggled [ON]\n");
      }
      break;
    }
    //create rectangle
    case 'r':
    {
      if(recToggled == true)
      {
        printf("Rectangle mode already toggled on\n");
      }
      else if(somethingToggled == true)
      {
        clearToggles();
        printf("Clearing toggles\n");
        recToggled = true;
        somethingToggled = true;
        printf("Rectangle Mode Toggled [ON]\n");
      }
      else
      {
        recToggled = true;
        somethingToggled = true;
        printf("Rectangle Mode Toggled [ON]\n");
      }
      break;
    }
    case 't':
    {
      if(triToggled == true)
      {
        printf("Triangle mode already toggled on\n");
      }
      else if(somethingToggled == true)
      {
        clearToggles();
        printf("Clearing toggles\n");
        triToggled = true;
        somethingToggled = true;
        printf("Triangle Mode Toggled [ON]\n");
      }
      else
      {
        triToggled = true;
        somethingToggled = true;
        printf("Triangle Mode Toggled [ON]\n");
      }
      break;
    }

    case 'e':
    {
      if(elipToggled == true)
      {
        printf("Elipsis mode already toggled on\n");
      }
      else if(somethingToggled == true)
      {
        clearToggles();
        printf("Clearing toggles\n");
        elipToggled = true;
        somethingToggled = true;
        printf("Elipsis Mode Toggled [ON]\n");
      }
      else
      {
        elipToggled = true;
        somethingToggled = true;
        printf("Elipsis Mode Toggled [ON]\n");
      }
      break;
    }

    case 'c':
    {
      if(circToggled == true)
      {
        printf("Elipsis mode already toggled on\n");
      }
      else if(somethingToggled == true)
      {
        clearToggles();
        printf("Clearing toggles\n");
        circToggled = true;
        somethingToggled = true;
        printf("Circle Mode Toggled [ON]\n");
      }
      else
      {
        circToggled = true;
        somethingToggled = true;
        printf("Circle Mode Toggled [ON]\n");
      }
      break;
    }
    //print to test
    case 'P':
    {
      for(int i = 0; i < pointVector.size(); i++)
      {
        printf("pointVector: %d , %d\n", pointVector[i].xPoint, pointVector[i].yPoint);
      }

      //TESTING
      cout << endl << "Output of begin and end VIEWSPACED: " << endl;
      for(int i = 0; i < vertexVector.size(); i++)
      {
        cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint <<  ", " <<  vertexVector[i].zPoint << " " << endl;
      }
      break;
    }

    /************************************************************/

    //TRANSLATION
    case 'T':
    {
      //WASDtoggled = true;
      //transformToggled = true;

      if(translationToggled == true)
      {
        cout << "Translation mode already on " << endl;
      }
      else if(transformToggled == true)
      {
        clearTransformToggles();
        transformToggled = true;
        translationToggled = true;
        cout << "[Translation Enabled]" << endl;
      }
      else
      {
        cout << "[Translation Enabled]" << endl;
        transformToggled = true;
        translationToggled = true;
      }
      break;

    }
    //ROTATION
    case 'R':
    {
      // WASDtoggled = true;
      // rotationToggled = true;

      if(rotationToggled == true)
      {
        cout << "Roation mode already on " << endl;
      }
      else if(transformToggled == true)
      {
        clearTransformToggles();
        cout << "Clearing transform toggles " << endl;
        transformToggled = true;
        rotationToggled = true;
        cout << "[Rotation Enabled]" << endl;
      }
      else
      {
        cout << "[Rotation Enabled]" << endl;
        transformToggled = true;
        rotationToggled = true;
      }
      break;
    }
    // //SCALE
    case 'E':
    {
      if(scaleToggled == true)
      {
        cout << "Scale mode already on " << endl;
      }
      else if(transformToggled == true)
      {
        clearTransformToggles();
        cout << "Clearing transform toggles " << endl;
        transformToggled = true;
        scaleToggled = true;
        cout << "[Scale Enabled]" << endl;
      }
      else
      {
        cout << "[Scale Enabled]" << endl;
        transformToggled = true;
        scaleToggled = true;
      }
      break;
    }

		default:
			break;
	}

  //WASD HANDLER
  if((transformToggled == true) && (key == 'W' || key == 'A' ||key == 'S'||key == 'D'))
  {
    cout << "WASD USED toggled" << endl;

    if(translationToggled == true)
    {
      //do key
      translation(key);
    }

    if(rotationToggled == true){
      rotation(key);
    }

    if(scaleToggled == true){
      scale(key);
    }


  }
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


//IMPORTS AND READS OBJ
void readIn ()
{
  if (!inFile)
  {
    cerr << "Unable to open file";
    exit(1);   // call system to stop
  }

  //input.open(input);

  string firstElement;
  string trash;
  //double tempDouble;
  Vertex tempVertex;
  faceIndex tempFace;

  while(inFile >> firstElement)
  {
    if(firstElement == "v")
    {
      inFile >> tempVertex.xPoint >> tempVertex.yPoint >> tempVertex.zPoint;
      vertexVector.push_back(tempVertex);
    }
    else if(firstElement == "f")
    {
        int tempIndex;

      // while(inFile.peek() != 'f' || inFile.peek() != 's')
      // {
        inFile >> firstElement;
        if(firstElement.substr(1,1) == "/")
        {
          istringstream(firstElement.substr(0,1)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert1 = tempIndex;

          inFile >> firstElement;
          istringstream(firstElement.substr(0,1)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert2 = tempIndex;

          inFile >> firstElement;
          istringstream(firstElement.substr(0,1)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert3 = tempIndex;

          faceVector.push_back(tempFace);
        }
        //handles double digits
        else
        {
          istringstream(firstElement.substr(0,2)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert1 = tempIndex;

          inFile >> firstElement;
          istringstream(firstElement.substr(0,2)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert2 = tempIndex;

          inFile >> firstElement;
          istringstream(firstElement.substr(0,2)) >> tempIndex;
          //normalize index to index number in vertexVector
          tempIndex = tempIndex - 1;
          tempFace.vert3 = tempIndex;

          faceVector.push_back(tempFace);
        }

      //
      // //}
      // //store vertex location into face vector
      // int index1, index2, index3;
      //
      // inFile >> firstElement;
      // istringstream(firstElement.substr(0,1)) >> index1;
      //
      // inFile >> firstElement;
      // istringstream(firstElement.substr(0,1)) >> index2;
      //
      // inFile >> firstElement;
      // istringstream(firstElement.substr(0,1)) >> index3;
      //
      //
      // //Handles vertexes 4 or larger
      // if(inFile != "f" || inFile != "s")
      // {
      //
      // }
      //
      //
      //
      // = vertexVector[index1].xPoint;


    }
    else
    {
      getline(inFile, trash);
    }
  }
  inFile.close();
  //testing
  //
  // cout << "Output of begin and end: " << endl;
  // for(int i = 0; i < vertexVector.size(); i++)
  // {
  //   cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint << vertexVector[i].zPoint << " " << endl;
  // }

  }

/***************************************************************************/
//NORMALIZE THE POINTS TO ORGIN AND PUT INTO VIEW
void normalizePoints()
{
  double xAvg = 0, yAvg = 0, zAvg = 0;

  //average each coordinate
  for(int i = 0; i < vertexVector.size(); i++)
  {
    xAvg += vertexVector[i].xPoint;
    yAvg += vertexVector[i].yPoint;
    zAvg += vertexVector[i].zPoint;
  }
  xAvg = xAvg/vertexVector.size();
  yAvg = yAvg/vertexVector.size();
  zAvg = zAvg/vertexVector.size();

  // //TESTING
  // cout << xAvg << endl;
  // cout << yAvg << endl;
  // cout << zAvg << endl;
  // cout << "Output of begin and end: " << endl;
  // for(int i = 0; i < vertexVector.size(); i++)
  // {
  //   cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint << ", " <<  vertexVector[i].zPoint << " " << endl;
  // }

  //center verticies/object
  for(int i = 0; i < vertexVector.size(); i++)
  {
    vertexVector[i].xPoint -= xAvg;
    vertexVector[i].yPoint -= yAvg;
    vertexVector[i].zPoint -= zAvg;
  }

//  TESTING
  // cout << "Output of begin and end NORMALIZED: " << endl;
  // for(int i = 0; i < vertexVector.size(); i++)
  // {
  //   cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint <<  ", " <<  vertexVector[i].zPoint << " " << endl;
  // }


  //Putting object into view (add half of window size to verties)
  for(int i = 0; i < vertexVector.size(); i++)
  {
    vertexVector[i].xPoint += (WIDTH/2);
    vertexVector[i].yPoint += (HEIGHT/2);
    vertexVector[i].zPoint += (WIDTH/2);
  }

  // TESTING
  // cout << "Output of begin and end VIEWSPACED: " << endl;
  // for(int i = 0; i < vertexVector.size(); i++)
  // {
  //   cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint <<  ", " <<  vertexVector[i].zPoint << " " << endl;
  // }

  // TESTING FACE VECTOR
  // for(int i = 0; i < faceVector.size(); i++)
  // {
  //   cout << faceVector[i].vert1 << ", " << faceVector[i].vert2 << ", " << faceVector[i].vert3 << endl;
  //
  // }



}

/***************************************************************************/
//DRAW FACES
void drawFaces()
{
  //lineToggled = true;

  //cout << "TEST VALUE: " << vertexVector[(faceVector[0].vert1)].xPoint << endl;
  //lineSegment(199, 224,  280, 295);


  // for(int i = 0; i < faceVector.size(); i++)
  // {
  //
  //     //cout << "RUN: "  << i << endl;
  //
  //     //
  //     triangle(vertexVector[(faceVector[i].vert1)].xPoint, vertexVector[(faceVector[i].vert1)].yPoint,
  //               vertexVector[(faceVector[i].vert2)].xPoint, vertexVector[(faceVector[i].vert2)].yPoint,
  //               vertexVector[(faceVector[i].vert3)].xPoint, vertexVector[(faceVector[i].vert3)].yPoint);
  // }


  // cout << vertexVector[(faceVector[0].vert1)].xPoint << ", " << vertexVector[(faceVector[0].vert1)].yPoint << ", "
  //      << vertexVector[(faceVector[0].vert2)].xPoint << ", " << vertexVector[(faceVector[0].vert2)].yPoint << ", "
  //     <<  vertexVector[(faceVector[0].vert3)].xPoint << ", " << vertexVector[(faceVector[0].vert3)].yPoint << endl;
  //
  //

  for(int i = 0; i < faceVector.size(); i++)
  {
    lineSegment(vertexVector[(faceVector[i].vert1)].xPoint, vertexVector[(faceVector[i].vert1)].yPoint,
                vertexVector[(faceVector[i].vert2)].xPoint, vertexVector[(faceVector[i].vert2)].yPoint);


    lineSegment(vertexVector[(faceVector[i].vert2)].xPoint, vertexVector[(faceVector[i].vert2)].yPoint,
                vertexVector[(faceVector[i].vert3)].xPoint, vertexVector[(faceVector[i].vert3)].yPoint);

    lineSegment(vertexVector[(faceVector[i].vert3)].xPoint, vertexVector[(faceVector[i].vert3)].yPoint,
                vertexVector[(faceVector[i].vert1)].xPoint, vertexVector[(faceVector[i].vert1)].yPoint);


  }
    //
    // lineSegment(vertexVector[(faceVector[0].vert2)].xPoint, vertexVector[(faceVector[0].vert2)].yPoint,
    //             vertexVector[(faceVector[0].vert3)].xPoint, vertexVector[(faceVector[0].vert3)].yPoint);
    //
    //             lineSegment(353, 146, 146, 353);




            // triangle(vertexVector[(faceVector[0].vert1)].xPoint, vertexVector[(faceVector[0].vert1)].yPoint,
            //           vertexVector[(faceVector[0].vert2)].xPoint, vertexVector[(faceVector[0].vert2)].yPoint,
            //           vertexVector[(faceVector[0].vert3)].xPoint, vertexVector[(faceVector[0].vert3)].yPoint);

  // cout << "Output of begin and end VIEWSPACED: " << endl;
  // for(int i = 0; i < vertexVector.size(); i++)
  // {
  //   cout << vertexVector[i].xPoint << ", " << vertexVector[i].yPoint <<  ", " <<  vertexVector[i].zPoint << " " << endl;
  // }


//   triangle(146.467, 146.467,
// 353.533, 146.467,
// 146.467, 353.533);

}
/***************************************************************************//***************************************************************************/
/***************************************************************************/
/***************************************************************************/

//TRANSLATION
void translation(unsigned char key)
{
  switch(key)
  {
    //up
    case 'W':
    {
      //Add to Y points in verticies
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].yPoint += 1;
      }
      drawFaces();
      break;
    }

    //left
    case 'A':
    {
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].xPoint -= 1;
      }
      drawFaces();
      break;
    }
    //right
    case 'D':
    {
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].xPoint += 1;
      }
      drawFaces();
      break;
    }
    //down
    case 'S':
    {
      //Minus to Y points in verticies
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].yPoint = vertexVector[i].yPoint - 1;
      }
      drawFaces();
      break;
    }
    default:
    break;
  }
}
/***************************************************************************/
//ROTATION
void rotation(unsigned char key)
{
  switch(key)
  {
    //W & S Rotates horizontal axis
    //WS rotates about the horizontal axis
    case 'W':
    {
      matrixMul('x', 359);
      drawFaces();
      break;
    }
    case 'S':
    {
      matrixMul('x', 1);
      drawFaces();
      break;
    }

    //AD rotates the model about the vertical axis
    //A & D Rotates about vertical axis
    //counterclickwise i.e. adding degrees about x
    case 'A':
    {
      matrixMul('y', 359);
      drawFaces();
      break;
    }
    case 'D':
    {
      matrixMul('y', 1);
      drawFaces();
      break;
    }
    default:
    break;
  }
}
//, double x, double y, double z
//MATRIX MULTIPLICATION FOR 3x3 TO 3x1
void matrixMul(char rotateType, double theta)
{
  //Vertex tempVertex;
  if (theta < 0) theta = -1 * theta * (M_PI / 180) * 5;
  else theta = theta * (M_PI / 180) * 5;

  switch(rotateType)
  {
    case 'x':
    {
      double xRotation[3][3] = {{1,0,0},{0,cos(theta),-sin(theta)},{0,sin(theta), cos(theta)}};

      for(int i = 0; i < 3; i++)
      {
        for(int g = 0; g < vertexVector.size(); g++)
        {
          //calculate multiplcation
          //if first row multiplication, store back into x for vector
          if(i == 0 )
          {
            vertexVector[g].xPoint =
            (xRotation[i][0] * vertexVector[g].xPoint) +
            (xRotation[i][1] * vertexVector[g].yPoint) +
            (xRotation[i][2] * vertexVector[g].zPoint);
          }

          if(i == 1)
          {
            vertexVector[g].yPoint =
            (xRotation[i][0] * vertexVector[g].xPoint) +
            (xRotation[i][1] * vertexVector[g].yPoint) +
            (xRotation[i][2] * vertexVector[g].zPoint);
          }
          if(i == 2 )
          {
            vertexVector[g].zPoint =
            (xRotation[i][0] * vertexVector[g].xPoint) +
            (xRotation[i][1] * vertexVector[g].yPoint) +
            (xRotation[i][2] * vertexVector[g].zPoint);
          }
        }
      }
      break;
    }

    case 'y':
    {
      double yRotation[3][3] = {{cos(theta), 0, sin(theta)},
                                {0, 1, 0},
                                {-sin(theta), 0, cos(theta)}};

      for(int i = 0; i < 3; i++)
      {
        for(int g = 0; g < vertexVector.size(); g++)
        {
          //calculate multiplcation
          //if first row multiplication, store back into x for vector
          if(i == 0 )
          {
            vertexVector[g].xPoint =
            (yRotation[i][0] * vertexVector[g].xPoint) +
            (yRotation[i][1] * vertexVector[g].yPoint) +
            (yRotation[i][2] * vertexVector[g].zPoint);
          }

          if(i == 1)
          {
            vertexVector[g].yPoint =
            (yRotation[i][0] * vertexVector[g].xPoint) +
            (yRotation[i][1] * vertexVector[g].yPoint) +
            (yRotation[i][2] * vertexVector[g].zPoint);
          }
          if(i == 2 )
          {
            vertexVector[g].zPoint =
            (yRotation[i][0] * vertexVector[g].xPoint) +
            (yRotation[i][1] * vertexVector[g].yPoint) +
            (yRotation[i][2] * vertexVector[g].zPoint);
          }
        }
      }

      break;
    }
    case 'z':
    {
      double zRotation[3][3] = {{cos(theta),-sin(theta), 0},
                                {sin(theta), cos(theta), 0},
                                {0, 0, 1}};

      for(int i = 0; i < 3; i++)
      {
        for(int g = 0; g < vertexVector.size(); g++)
        {
          //calculate multiplcation
          //if first row multiplication, store back into x for vector
          if(i == 0 )
          {
            vertexVector[g].xPoint =
            (zRotation[i][0] * vertexVector[g].xPoint) +
            (zRotation[i][1] * vertexVector[g].yPoint) +
            (zRotation[i][2] * vertexVector[g].zPoint);
          }

          if(i == 1)
          {
            vertexVector[g].yPoint =
            (zRotation[i][0] * vertexVector[g].xPoint) +
            (zRotation[i][1] * vertexVector[g].yPoint) +
            (zRotation[i][2] * vertexVector[g].zPoint);
          }
          if(i == 2 )
          {
            vertexVector[g].zPoint =
            (zRotation[i][0] * vertexVector[g].xPoint) +
            (zRotation[i][1] * vertexVector[g].yPoint) +
            (zRotation[i][2] * vertexVector[g].zPoint);
          }
        }
      }
      break;
    }

    default:
      break;
    }
  // }
  // double xRotation[3][3] = {{1,0,0},
  //                           {0,cos(theta),-sin(theta)},
  //                           {0,sin(theta), cos(theta)}};
  // double yRotation[3][3] = {{cos(theta), 0, sin(theta)},
  //                           {0, 1, 0},
  //                           {-sin(theta), 0, cos(theta)}};
  //
  // double zRotation[3][3] = {{cos(theta),-sin(theta), 0},
  //                           {sin(theta), cos(theta), 0},
  //                           {0, 0, 1}};
  //
  //
}

/***************************************************************************/
//AD or WS is used to make the model bigger or smaller.
void scale(unsigned char key)
{
  switch(key)
  {
    //Scale vertically (stretch)
    //hold x constant
    case 'W':
    {
      double scalar = 1.1;
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].yPoint = vertexVector[i].yPoint * scalar;
      }
      drawFaces();
      break;
    }
    //Scale vertically (squish)
    //hold x constant
    case 'S':
    {
      double scalar = 0.9;
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].yPoint = vertexVector[i].yPoint * scalar;
      }
      drawFaces();
      break;
    }

    //Scale horizontal
    case 'A':
    {
      double scalar = 1.1;
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].xPoint = vertexVector[i].xPoint * scalar;
      }
      drawFaces();
      break;
    }
    //Scale horizontal
    case 'D':
    {
      double scalar = 0.9;
      for(int i = 0; i < vertexVector.size(); i++)
      {
        vertexVector[i].xPoint = vertexVector[i].xPoint * scalar;
      }
      drawFaces();
      break;
    }


    default:
    break;

  }

}

/***************************************************************************/

int main (int argc, char *argv[])
{
/* This main function sets up the main loop of the program and continues the
   loop until the end of the data is reached.  Then the window can be closed
   using the escape key.						  */

	glutInit            ( &argc, argv );
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize  ( 500,500 );
	glutCreateWindow    ( "Computer Graphics" );
	glutDisplayFunc     ( display );
	glutIdleFunc	      ( display );
	glutMouseFunc       ( mouse );
	glutKeyboardFunc    ( keyboard );

  //import and normalize
  inFile.open(argv[1]);
  readIn();
  normalizePoints();

  init_window();				                   //create_window

  somethingToggled = false;
  input = 0;

	glutMainLoop        ( );                 // Initialize The Main Loop
}

// p01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height;
unsigned char *pixmap;
unsigned char *pixmapRead, *pixmapWrite;
int imageWidth, imageHeight, maxColor;
int red = 0,green = 0,blue = 0;


// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setPixels()
{
	if(red == 1 || green == 1 || blue == 1)
	{
		for(int y = 0; y < height ; y++)
		{
			for(int x = 0; x < width; x++)
			{
				int i = (y * width + x) * 3;
				pixmap[i++] = red * 255;
				pixmap[i++] = green * 255; //Do you know what "0xFF" represents? Google it!
				pixmap[i] = blue * 255; //Learn to use the "0x" notation to your advantage
			}
		}
	}
   if(red == 2)
	{
		for(int y = 0; y < height ; y++)
		{
			for(int x = 0; x < width; x++)
			{
				int i = (y * width + x) * 3;
				if(x <= width/2 && y <= height/2)
				{
					pixmap[i++] = 0;
					pixmap[i++] = 0; 
					pixmap[i] = 255; 
				}
				else if(x < width/2 && y > height/2)
				{
					pixmap[i++] = 255;
					pixmap[i++] = 0; 
					pixmap[i] = 0; 
				}
				else if(x > width/2 && y < height/2)
				{
					pixmap[i++] = 255;
					pixmap[i++] = 255; 
					pixmap[i] = 0; 
				}
				else
				{
					pixmap[i++] = 0;
					pixmap[i++] = 255; 
					pixmap[i] = 0; 
				}
			}
		}
	}
   if(red == 3)
   {
	   int radius = 200;
	   int px, nx, py, ny, d;
	   for (int x = 0; x <= radius; x++)
        {
			d = (int)ceil(sqrt((float)(radius*radius - x*x)));
            for (int y = 0; y <= d; y++)
            {
                px = width/2 + x;
                nx = width/2 - x;
                py = height/2 + y;
                ny = height/2 - y;
				
				int i = (py * width + px) * 3;
				int j = (py * width + nx) * 3;
				int k = (ny * width + px) * 3;
				int l = (ny * width + nx) * 3;
				
				pixmap[i++] = 255;
				pixmap[i++] = 255; 
				pixmap[i] = 0; 

				pixmap[j++] = 255;
				pixmap[j++] = 255; 
				pixmap[j] = 0; 

				pixmap[k++] = 255;
				pixmap[k++] = 255; 
				pixmap[k] = 0; 

				pixmap[l++] = 255;
				pixmap[l++] = 255; 
				pixmap[l] = 0; 
 
            }
        }
	   for(int y = 0; y < height ; y++)
		{
			for(int x = 0; x < width; x++)
			{
				int i = (y * width + x) * 3;
				if(pixmap[i] == 255)
					continue;
				else
				{
					pixmap[i++] = 0;
					pixmap[i++] = 0; 
					pixmap[i] = 255;
					
				}
			}
		}

   }
}

void readP6PPM(char fileName[])
{
	FILE *fp;
	fp = fopen(fileName, "rb");
	if(fp == NULL)
	{
		cout << "File not found / Unable to open " << fileName << " " << endl;
		exit(0);
	}
	char magic[3];
	fscanf(fp, "%s", magic);
	if(magic[0] != 'P' && magic[1] != '6')
	{
		cout << "PPM is not in P6 format" <<endl;
		exit(0);
	}
	int c;
	c = fgetc(fp);
	do
	{
		if(c == '#');
		c = fgetc(fp);
	}while(c == '\n');
	ungetc(c, fp);
	c = fgetc(fp);
	while(c == '#')
	{
		while(fgetc(fp) != '\n');
		c = fgetc(fp);
	}
	ungetc(c, fp);
	fscanf(fp, "%d %d %d", &imageWidth, &imageHeight, &maxColor);
	fgetc(fp);
	pixmapRead = new unsigned char[imageWidth * imageHeight * 3];
	int x,y,i;
	unsigned char r,g,b;
	for(y = imageHeight - 1; y >= 0; y--)
	{
		for(x =0 ; x < imageWidth; x++)
		{
			fscanf(fp, "%c%c%c", &r, &g, &b);
			i = (y * imageWidth + x) * 3;
			pixmapRead[i++] = r;
			pixmapRead[i++] = g;
			pixmapRead[i] = b;
		}
	}
	width = imageWidth;
	height = imageHeight;
	fclose(fp);
}
void writeP6PPM(char *argv[])
{
	readP6PPM(argv[1]);
	FILE *fp;
	fp = fopen(argv[2], "wb");
	fprintf(fp, "%c%c%c", 'P', '6', '\n');
	fprintf(fp, "%d %d", imageWidth, imageHeight);
	fputc('\n',fp);
	fprintf(fp, "%d" , maxColor);
	fputc('\n',fp);
	for(int y = imageHeight - 1; y >= 0; y--)
	{
		for(int x =0 ; x < imageWidth; x++)
		{
			int i = (y * imageWidth + x) * 3;
			unsigned char r = pixmapRead[i++];
			unsigned char g = pixmapRead[i++];
			unsigned char b = pixmapRead[i];
			fprintf(fp, "%c%c%c", r, g, b);
		}
	}
	fclose(fp);
}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
static void windowResize(int w, int h)
{   
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(w/2),0,(h/2),0,1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity() ;
}
static void windowDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(0,0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  if(red == 4)
  {
	  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmapRead);
  }
  else
  {
	  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
  } 
  glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
  if(state == GLUT_UP)
  exit(0);               // Exit on mouse click.
}
static void init(int argc, char *argv[])
{
	glClearColor(1,1,1,1);
   // Set background color.
  if(argc == 1)
  {
	  return;
  }
  if(argc == 2)
  {
	  if(strcmp(argv[1], "red") == 0)
	  {
		  cout << "Red " << endl;
		  red = 1;
	  }
	  else if(strcmp(argv[1], "blue") == 0)
	  {
		  blue = 1;
	  }
	  else if(strcmp(argv[1], "green") == 0)
	  {
		  green = 1;
	  }
	  else if(strcmp(argv[1], "all") == 0)
	  {
		  red = green = blue = 2;
	  }
	  else if(strcmp(argv[1], "circle") == 0)
	  {
		  red = green = blue = 3;
	  }
	  else if(strstr(argv[1], ".ppm") != NULL)
	  {
		  red = 4;
		  cout << argv[1] << endl;
		  readP6PPM(argv[1]);
	  }
  }
  else if(argc == 3)
  {
	  red = 4;
	  writeP6PPM(argv);
  }
  setPixels();
}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

  //initialize the global variables
  width = 640;
  height = 480;
  pixmap = new unsigned char[width * height * 3];  //Do you know why "3" is used?
  // OpenGL Commands:
  // Once "glutMainLoop" is executed, the program loops indefinitely to all
  // glut functions.  
  glutInit(&argc, argv);
  init(argc, argv);
  glutInitWindowPosition(100, 100); // Where the window will display on-screen.
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Sathish Sekar - Homework Zero");
  glutReshapeFunc(windowResize);
  glutDisplayFunc(windowDisplay);
  glutMouseFunc(processMouse);
  glutMainLoop();

  return 0; //This line never gets reached. We use it because "main" is type int.
}



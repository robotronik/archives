#include <stdio.h>
#include <math.h>
#include "highgui.h"
#include "cv.h"

#define LAPTOP 0
#define CAMERA 1

#define NB_PICKS 10

#define max( a, b ) ((a) > (b) ? (a) : (b))

// Images are 640 x 480

IplImage *frame = NULL;

int normalize( unsigned char value )
{
  double v = value;
  
  v -= 255;
  v = v * (5.0/255.0);
  v = 100 * exp((-1.0/5.0) * v* (double) v );
  
  return v;
}

int computeCriteria( int b, int g, int r )
{
  double value = (double) (r - b) / (r + b + g);
  value = (int)round( 2 / (1 + exp(-3.0 * value)) - 1 );

  return value;
}

CvScalar chooseColor( int value )
{
  CvScalar red = cvScalar( 0, 0, 255, 0 );
  CvScalar blue = cvScalar( 255, 0, 0, 0 );
  CvScalar white = cvScalar( 255, 255, 255, 0 );
  CvScalar t[3] = {blue, white, red};

  return t[value + 1];
}

int main( int argc, char** argv )
{
  CvCapture *cam = cvCreateCameraCapture( LAPTOP );
  char key;
  int i;
  char *dat = NULL;

  int r_sum = 0, g_sum = 0, b_sum = 0;
  unsigned char r[NB_PICKS], g[NB_PICKS], b[NB_PICKS];

  CvPoint p1 = cvPoint( 313, 99 );
  CvPoint p2 = cvPoint( 324, 101 );
  CvScalar col = cvScalar( 0, 0, 255, 0 );

  IplImage *color = cvCreateImage(cvSize(200, 100), IPL_DEPTH_8U, 3);

  if( !cam )
  {
    printf( "The camera or webcam could not be loaded!\n" );
    exit( EXIT_FAILURE );
  }

  cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "Color", CV_WINDOW_AUTOSIZE );

  while( key != 27 ) // 27 = ESCAPE
  {
    frame = cvQueryFrame( cam );
    if( !frame ) break;

    // Draws the picking area
    cvRectangle( frame, p1, p2, col, 1, 8, 0 );

    dat = (*frame).imageData + (640*3) * 100;

    b_sum = 0;
    g_sum = 0;
    r_sum = 0;

    for( i = 0; i < NB_PICKS; i++ )
    {
      b[i] = *(dat + 314*3 + 3*i);
      g[i] = *(dat + 314*3 + 3*i + 1);
      r[i] = *(dat + 314*3 + 3*i + 2);

      //  printf( "Pixel %d: r=%d, g=%d, b=%d\n", i, r[i], g[i], b[i] );
    }

    for( i = 0; i < NB_PICKS; i++ )
    {
      b_sum += normalize( b[i] );
      g_sum += normalize( g[i] );
      r_sum += normalize( r[i] );
    }

    printf( "Red = %d, Green = %d, Blue = %d\n", r_sum, g_sum, b_sum );

//    printf( "Criteria red = %lf, Criteria blue = %lf\n", computeCriteria( r_sum, b_sum, g_sum, r_sum), computeCriteria( b_sum, b_sum, g_sum, r_sum ) );

    cvRectangle( color, cvPoint( 0, 0 ), cvPoint( 200, 100 ), chooseColor( computeCriteria( b_sum, g_sum, r_sum ) ), CV_FILLED, 8, 0 ) ;

    key = cvWaitKey( 33 );

    cvShowImage( "Camera", frame );
    cvShowImage( "Color", color );
  }

  cvReleaseCapture( &cam );
  cvDestroyAllWindows();
}

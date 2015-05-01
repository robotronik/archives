#include <stdio.h>
#include <math.h>
#include "highgui.h"
#include "cv.h"

#define LAPTOP 0
#define CAMERA 1

#define NB_PICKS 10

#define max( a, b ) ((a) > (b) ? (a) : (b))

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
  CvCapture *cam = cvCreateCameraCapture( CAMERA );
  char key;
  int i;
  char *dat1 = NULL;
  char *dat2 = NULL;

  int r_sum_1 = 0, g_sum_1 = 0, b_sum_1 = 0;
  int r_sum_2 = 0, g_sum_2 = 0, b_sum_2 = 0;
  unsigned char r1[NB_PICKS], g1[NB_PICKS], b1[NB_PICKS];
  unsigned char r2[NB_PICKS], g2[NB_PICKS], b2[NB_PICKS];

  IplImage *color1 = cvCreateImage(cvSize(200, 100), IPL_DEPTH_8U, 3);
  IplImage *color2 = cvCreateImage(cvSize(200, 100), IPL_DEPTH_8U, 3);

  FILE * h1 = NULL;
  FILE * h2 = NULL;
  char y1txt[10];
  char y2txt[10];
  int y1;
  int y2;

  // Check that the cam works
  if( !cam )
  {
    printf( "The camera or webcam could not be loaded!\n" );
    exit( EXIT_FAILURE );
  }

  // Get the y coordinates for the targets
  h1 = fopen( "y_top.txt", "r" );
  h2 = fopen( "y_bottom.txt", "r" );

  if( h1 == NULL || h2 == NULL )
  {
    printf( "Y coordinates files not read !\n" );
    exit( EXIT_FAILURE );
  }

  fgets( y1txt, 10, h1 );
  fgets( y2txt, 10, h2 );

  y1 = atoi( y1txt );
  y2 = atoi( y2txt );

  // The picking areas anchor points
  CvPoint p1 = cvPoint( 154, y1 );
  CvPoint p2 = cvPoint( 166, y1+2 );
  CvPoint p3 = cvPoint( 154, y2 );
  CvPoint p4 = cvPoint( 166, y2+2 );

  CvScalar col = cvScalar( 0, 0, 255, 0 );

  cvSetCaptureProperty( cam, CV_CAP_PROP_FRAME_WIDTH, 320 );
  cvSetCaptureProperty( cam, CV_CAP_PROP_FRAME_HEIGHT, 240 );

  cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "Color1", CV_WINDOW_AUTOSIZE );
  cvNamedWindow( "Color2", CV_WINDOW_AUTOSIZE );

  while( key != 27 ) // 27 = ESCAPE
  {
    frame = cvQueryFrame( cam );
    if( !frame ) break;

    // Draws the two picking areas
    cvRectangle( frame, p1, p2, col, 1, 8, 0 );
    cvRectangle( frame, p3, p4, col, 1, 8, 0 );

    dat1 = (*frame).imageData + (320*3) * (y1+1) + (154*3);
    dat2 = (*frame).imageData + (320*3) * (y2+1) + (154*3);

    b_sum_1 = 0;
    g_sum_1 = 0;
    r_sum_1 = 0;

    b_sum_2 = 0;
    g_sum_2 = 0;
    r_sum_2 = 0;

    for( i = 0; i < NB_PICKS; i++ )
    {
      b1[i] = *(dat1 + 3*i);
      g1[i] = *(dat1 + 3*i + 1);
      r1[i] = *(dat1 + 3*i + 2);

      b2[i] = *(dat2 + 3*i);
      g2[i] = *(dat2 + 3*i + 1);
      r2[i] = *(dat2 + 3*i + 2);
      
      //  printf( "Pixel %d: r=%d, g=%d, b=%d\n", i, r[i], g[i], b[i] );
    }

    for( i = 0; i < NB_PICKS; i++ )
    {
      b_sum_1 += normalize( b1[i] );
      g_sum_1 += normalize( g1[i] );
      r_sum_1 += normalize( r1[i] );

      b_sum_2 += normalize( b2[i] );
      g_sum_2 += normalize( g2[i] );
      r_sum_2 += normalize( r2[i] );
    }

//    printf( "Red = %d, Green = %d, Blue = %d\n", r_sum, g_sum, b_sum );

//    printf( "Criteria red = %lf, Criteria blue = %lf\n", computeCriteria( r_sum, b_sum, g_sum, r_sum), computeCriteria( b_sum, b_sum, g_sum, r_sum ) );

    cvRectangle( color1, cvPoint( 0, 0 ), cvPoint( 200, 100 ), chooseColor( computeCriteria( b_sum_1, g_sum_1, r_sum_1 ) ), CV_FILLED, 8, 0 ) ;

    cvRectangle( color2, cvPoint( 0, 0 ), cvPoint( 200, 100 ), chooseColor( computeCriteria( b_sum_2, g_sum_2, r_sum_2 ) ), CV_FILLED, 8, 0 ) ;

    key = cvWaitKey( 33 );

    cvShowImage( "Camera", frame );
    cvShowImage( "Color1", color1 );
    cvShowImage( "Color2", color2 );
  }

  cvReleaseCapture( &cam );
  cvDestroyAllWindows();
}

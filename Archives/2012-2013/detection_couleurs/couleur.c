#include "couleur.h"
#define BLEU 0
#define ROUGE 1
IplImage *frame = NULL;
extern char tirette_pulled;

int normalize( unsigned char value )
{
    double v = value;

    v -= 255;
    v = v * (5.0/255.0);
    v = 100 * exp((-1.0/5.0) * v* (double) v );

    return v;
}
/*
int computeCriteria( int b, int g, int r )
{
    double value = (double) (r - b) / (r + b + g);
    value = (int)round( 2 / (1 + exp(-3.0 * value)) - 1 );
    
    return value;
}
*/

int computeCriteria( int b, int g, int r )
{
    double value = (double) (r - b) / (r + b + g);
    value =  2 / (1 + exp(-3.0 * (value-0.1))) - 1 ;
    
    return value > 0 ? 1 : -1;
}

char convertUart(int a)
{
    if(a == -1)
	return BLEU;
    else 
	return ROUGE;
}
CvScalar chooseColor( int value )
{
    CvScalar red = cvScalar( 0, 0, 255, 0 );
    CvScalar blue = cvScalar( 255, 0, 0, 0 );
    CvScalar white = cvScalar( 255, 255, 255, 0 );
    CvScalar t[3] = {blue, white, red};

    return t[value + 1];
}

int vision(char* M1, char* M2, char *M3, char *M4, char *M5, char *M6, int couleurEquipe) 
{
    CvCapture *cam = cvCreateCameraCapture( LAPTOP );
    char key;
    int i;
    char *dat1 = NULL;
    char *dat2 = NULL;
    char *dat3 = NULL;
    char *dat4 = NULL;
    char *dat5 = NULL;
    char *dat6 = NULL;

    int r_sum_1 = 0, g_sum_1 = 0, b_sum_1 = 0;
    int r_sum_2 = 0, g_sum_2 = 0, b_sum_2 = 0;
    int r_sum_3 = 0, g_sum_3 = 0, b_sum_3 = 0;
    int r_sum_4 = 0, g_sum_4 = 0, b_sum_4 = 0;
    int r_sum_5 = 0, g_sum_5 = 0, b_sum_5 = 0;
    int r_sum_6 = 0, g_sum_6 = 0, b_sum_6 = 0;

    unsigned char r1[NB_PICKS], g1[NB_PICKS], b1[NB_PICKS];
    unsigned char r2[NB_PICKS], g2[NB_PICKS], b2[NB_PICKS];
    unsigned char r3[NB_PICKS], g3[NB_PICKS], b3[NB_PICKS];
    unsigned char r4[NB_PICKS], g4[NB_PICKS], b4[NB_PICKS];
    unsigned char r5[NB_PICKS], g5[NB_PICKS], b5[NB_PICKS];
    unsigned char r6[NB_PICKS], g6[NB_PICKS], b6[NB_PICKS];

    IplImage *color1 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);
    IplImage *color2 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);
    IplImage *color3 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);
    IplImage *color4 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);
    IplImage *color5 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);
    IplImage *color6 = cvCreateImage(cvSize(50, 50), IPL_DEPTH_8U, 3);

    FILE * positions = NULL;
    //char buff[10];
    int x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;

    // Get the y coordinates for the targets
    if( couleurEquipe == BLEU )
	positions = fopen( "/home/pi/Robotronik/robotronik/detection_couleurs/positions_mires_bleu.txt", "r" );
    else if( couleurEquipe == ROUGE )
	positions = fopen( "/home/pi/Robotronik/robotronik/detection_couleurs/positions_mires_rouge.txt", "r" );
    else
	exit( EXIT_FAILURE );

    if( positions == NULL )
    {
        printf( "Coordinates file not read !\n" );
        exit( EXIT_FAILURE );
    }

    // Get the positions of the cross
    fscanf( positions, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4, &x5, &y5, &x6, &y6);

    // The picking areas anchor points
    CvPoint p1 = cvPoint( x1-6, y1 );
    CvPoint p2 = cvPoint( x1+6, y1+2 );
    CvPoint p3 = cvPoint( x2-6, y2 );
    CvPoint p4 = cvPoint( x2+6, y2+2 );
    CvPoint p5 = cvPoint( x3-6, y3 );
    CvPoint p6 = cvPoint( x3+6, y3+2 );
    CvPoint p7 = cvPoint( x4-6, y4 );
    CvPoint p8 = cvPoint( x4+6, y4+2 );
    CvPoint p9 = cvPoint( x5-6, y5 );
    CvPoint p10 = cvPoint( x5+6, y5+2 );
    CvPoint p11 = cvPoint( x6-6, y6 );
    CvPoint p12 = cvPoint( x6+6, y6+2 );

    CvScalar col = cvScalar( 0, 255, 0, 0 );

    // Check that the cam works
    if( !cam )
    {
        printf( "The camera or webcam could not be loaded!\n" );
        exit( EXIT_FAILURE );
    }

    cvSetCaptureProperty( cam, CV_CAP_PROP_FRAME_WIDTH, 320 );
    cvSetCaptureProperty( cam, CV_CAP_PROP_FRAME_HEIGHT, 240 );

    cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );

    while( key != 27 && !tirette_pulled) // 27 = ESCAPE
    {
        frame = cvQueryFrame( cam );
        if( !frame ) break;

        // Draws the two picking areas
        cvRectangle( frame, p1, p2, col, 1, 8, 0 );
        cvRectangle( frame, p3, p4, col, 1, 8, 0 );
        cvRectangle( frame, p5, p6, col, 1, 8, 0 );
        cvRectangle( frame, p7, p8, col, 1, 8, 0 );
        cvRectangle( frame, p9, p10, col, 1, 8, 0 );
        cvRectangle( frame, p11, p12, col, 1, 8, 0 );

        dat1 = (*frame).imageData + (320*3) * (y1+1) + ((x1-6)*3);
        dat2 = (*frame).imageData + (320*3) * (y2+1) + ((x2-6)*3);
        dat3 = (*frame).imageData + (320*3) * (y3+1) + ((x3-6)*3);
        dat4 = (*frame).imageData + (320*3) * (y4+1) + ((x4-6)*3);
        dat5 = (*frame).imageData + (320*3) * (y5+1) + ((x5-6)*3);
        dat6 = (*frame).imageData + (320*3) * (y6+1) + ((x6-6)*3);

        b_sum_1 = 0;
        g_sum_1 = 0;
        r_sum_1 = 0;

        b_sum_2 = 0;
        g_sum_2 = 0;
        r_sum_2 = 0;

        b_sum_3 = 0;
        g_sum_3 = 0;
        r_sum_3 = 0;

        b_sum_4 = 0;
        g_sum_4 = 0;
        r_sum_4 = 0;

        b_sum_5 = 0;
        g_sum_5 = 0;
        r_sum_5 = 0;

        b_sum_6 = 0;
        g_sum_6 = 0;
        r_sum_6 = 0;

        for( i = 0; i < NB_PICKS; i++ )
        {
            b1[i] = *(dat1 + 3*i);
            g1[i] = *(dat1 + 3*i + 1);
            r1[i] = *(dat1 + 3*i + 2);

            b2[i] = *(dat2 + 3*i);
            g2[i] = *(dat2 + 3*i + 1);
            r2[i] = *(dat2 + 3*i + 2);

            b3[i] = *(dat3 + 3*i);
            g3[i] = *(dat3 + 3*i + 1);
            r3[i] = *(dat3 + 3*i + 2);

            b4[i] = *(dat4 + 3*i);
            g4[i] = *(dat4 + 3*i + 1);
            r4[i] = *(dat4 + 3*i + 2);

            b5[i] = *(dat5 + 3*i);
            g5[i] = *(dat5 + 3*i + 1);
            r5[i] = *(dat5 + 3*i + 2);

            b6[i] = *(dat6 + 3*i);
            g6[i] = *(dat6 + 3*i + 1);
            r6[i] = *(dat6 + 3*i + 2);
        }

        for( i = 0; i < NB_PICKS; i++ )
        {
            b_sum_1 += normalize( b1[i] );
            g_sum_1 += normalize( g1[i] );
            r_sum_1 += normalize( r1[i] );

            b_sum_2 += normalize( b2[i] );
            g_sum_2 += normalize( g2[i] );
            r_sum_2 += normalize( r2[i] );

            b_sum_3 += normalize( b3[i] );
            g_sum_3 += normalize( g3[i] );
            r_sum_3 += normalize( r3[i] );

            b_sum_4 += normalize( b4[i] );
            g_sum_4 += normalize( g4[i] );
            r_sum_4 += normalize( r4[i] );

            b_sum_5 += normalize( b5[i] );
            g_sum_5 += normalize( g5[i] );
            r_sum_5 += normalize( r5[i] );

            b_sum_6 += normalize( b6[i] );
            g_sum_6 += normalize( g6[i] );
            r_sum_6 += normalize( r6[i] );
        }

//	printf( "value = %lf\n", (double) (r_sum_1 - b_sum_1) / (r_sum_1 + b_sum_1 + g_sum_1)  );

        cvRectangle( color1, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_1, g_sum_1, r_sum_1 ) ), CV_FILLED, 8, 0 ) ;

        cvRectangle( color2, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_2, g_sum_2, r_sum_2 ) ), CV_FILLED, 8, 0 ) ;

        cvRectangle( color3, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_3, g_sum_3, r_sum_3 ) ), CV_FILLED, 8, 0 ) ;

        cvRectangle( color4, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_4, g_sum_4, r_sum_4 ) ), CV_FILLED, 8, 0 ) ;

        cvRectangle( color5, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_5, g_sum_5, r_sum_5 ) ), CV_FILLED, 8, 0 ) ;

        cvRectangle( color6, cvPoint( 0, 0 ), cvPoint( 50, 50 ), chooseColor( computeCriteria( b_sum_6, g_sum_6, r_sum_6 ) ), CV_FILLED, 8, 0 ) ;

        key = cvWaitKey( 33 );

        cvShowImage( "Camera", frame );
        cvShowImage( "Color1", color1 );
        cvShowImage( "Color2", color2 );
        cvShowImage( "Color3", color3 );
        cvShowImage( "Color4", color4 );
        cvShowImage( "Color5", color5 );
        cvShowImage( "Color6", color6 );


	cvMoveWindow("Color1", 50, 10);
	cvMoveWindow("Color2", 110, 10);
	cvMoveWindow("Color3", 180, 10);

	cvMoveWindow("Color4", 50, 300);
	cvMoveWindow("Color5", 110, 300);
	cvMoveWindow("Color6", 180, 300);


    }
    *M1 = convertUart(computeCriteria(b_sum_1, g_sum_1, r_sum_1));
    *M2 = convertUart(computeCriteria(b_sum_2, g_sum_2, r_sum_2));
    *M3 = convertUart(computeCriteria(b_sum_3, g_sum_3, r_sum_3));
    *M4 = convertUart(computeCriteria(b_sum_4, g_sum_4, r_sum_4));
    *M5 = convertUart(computeCriteria(b_sum_5, g_sum_5, r_sum_5));
    *M6 = convertUart(computeCriteria(b_sum_6, g_sum_6, r_sum_6));

    cvReleaseCapture( &cam );
    cvDestroyAllWindows();
    return 0;
}

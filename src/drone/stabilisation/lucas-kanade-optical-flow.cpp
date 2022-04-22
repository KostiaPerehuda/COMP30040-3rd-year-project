/*
 * OUTDATED FILE
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

#include "lucas-kanade-optical-flow.h"

LucasKanadeOpticalFlow::LucasKanadeOpticalFlow(std::string windowName, SpeedController* controller)
	: controller(controller), windowName_(windowName), shouldFindCorners_(true), visualizer("Displacement")
{
	filterX.setTau(1);
	filterY.setTau(1);
}

LucasKanadeOpticalFlow::~LucasKanadeOpticalFlow() {
}

void LucasKanadeOpticalFlow::onImageReceived(cv::Mat image) {

	// set new corners to track
	if (shouldFindCorners_ || previousPoints_.size() <= 10) {

		// Take first frame and find corners in it
		cv::cvtColor(image, previousGrayImage_, cv::COLOR_BGR2GRAY);
		// goodFeaturesToTrack(image, output vector,
		//	 maxCorners ( 0 = unlimited), quality level (rejects features vith values < level * max feature value found),
		//   min euclidean distance between features), mask (if not empty => subset of image where features are detected)
		//   derivative block size, to use Harris, free Harris param)
		//cv::goodFeaturesToTrack(previousGrayImage_, previousPoints_, 100, 0.3, 7, cv::Mat(), 7, false, 0.04);
		//cv::goodFeaturesToTrack(previousGrayImage_, previousPoints_, 0, 0.07, 7, cv::Mat(), 7, false, 0.04);
		cv::goodFeaturesToTrack(previousGrayImage_, previousPoints_, 0, 0.1, 7, cv::Mat(), 7, false, 0.04);
		//cv::goodFeaturesToTrack(previousGrayImage_, previousPoints_, 0, 0.1, 5, cv::Mat(), 7, false, 0.04);

		// Create a mask image for drawing lines of detected features
		mask_ = cv::Mat::zeros(image.size(), image.type());

		if (previousPoints_.size() == 0) return;
		shouldFindCorners_ = false;
	}

	cv::Mat grayImage;
	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

	std::vector<cv::Point2f> newPoints;

	// calculate optical flow
	std::vector<uchar> status;
	std::vector<float> err;
	cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);

	// calcOpticalFlowPyrLK(prevImage, nextImage, prevPoints, nextPoints, array status of each point(1 = found, 0 = not found),
	// 		errors of each point, size of min window detection, height of pyramid (scaling of detection window),
	//		criteria of iterative algorithms(type, iteration count, accuracy))
	//cv::calcOpticalFlowPyrLK(previousGrayImage_, grayImage, previousPoints_, newPoints, status, err, cv::Size(15,15), 2, criteria);
	cv::calcOpticalFlowPyrLK(previousGrayImage_, grayImage, previousPoints_, newPoints, status, err, cv::Size(11,11), 1, criteria);
	std::vector<cv::Point2f> goodNewPoints;


	cv::Mat circles = cv::Mat::zeros(image.size(), image.type());
	cv::Point2f displacement(0.0f, 0.0f);
	int count = 0;
	for(uint i = 0; i < previousPoints_.size(); i++)
	{
		// Select good points
		if(status[i] == 1) {
			goodNewPoints.push_back(newPoints[i]);
			// draw the tracks
			cv::line(mask_,newPoints[i], previousPoints_[i], cv::Scalar(0, 100, 255), 2);
			cv::circle(circles, newPoints[i], 5, cv::Scalar(0, 100, 255), -1);
			displacement += (newPoints[i] - previousPoints_[i]);
			count++;
		}
	}
	displacement /= (count ? count : 1);
	displacement = cv::Point2f(filterX.process(displacement.x, 0.04), filterY.process(displacement.y, 0.04));
	//std::cout << displacement << std::endl;

	controller->update({-displacement.y, 0, -displacement.x, 0});

	cv::Mat img, img2;
	cv::add(image, mask_, img2);
	cv::add(img2, circles, img);
	imshow(windowName_, img);

	/*cv::Point2f center(200.0f, 200.0f);
	displacement = displacement * 200 + center;

	cv::Mat displacement_image = cv::Mat::zeros(cv::Size(400, 400), image.type());
	cv::line(displacement_image, center, displacement, cv::Scalar(0, 0, 255), 2);
	cv::circle(displacement_image, displacement, 5, cv::Scalar(0, 0, 255), -1);
	imshow("Displacement", displacement_image);*/

	visualizer.draw(displacement.x, displacement.y);

	// Now update the previous frame and previous points
	previousGrayImage_ = grayImage.clone();
	previousPoints_ = goodNewPoints;

	char c = (char)cv::waitKey(10);
	switch( c )
	{
	case 'e':
		controller->reset();
		break;
	case 'c':
		mask_ = cv::Mat::zeros(image.size(), image.type());
		break;
	case 'q':
		shouldFindCorners_ = true;
		break;
	}
}

//
//using namespace cv;
//using namespace std;
//static void help()
//{
//    // print a welcome message, and the OpenCV version
//    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
//            "Using OpenCV version " << CV_VERSION << endl;
//    cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
//    cout << "\nHot keys: \n"
//            "\tESC - quit the program\n"
//            "\tr - auto-initialize tracking\n"
//            "\tc - delete all the points\n"
//            "\tn - switch the \"night\" mode on/off\n"
//            "To add/remove a feature point click it\n" << endl;
//}
//Point2f point;
//bool addRemovePt = false;
//static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
//{
//    if( event == EVENT_LBUTTONDOWN )
//    {
//        point = Point2f((float)x, (float)y);
//        addRemovePt = true;
//    }
//}
//int main( int argc, char** argv )
//{
//    VideoCapture cap;
//    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
//    Size subPixWinSize(10,10), winSize(31,31);
//    const int MAX_COUNT = 500;
//    bool needToInit = false;
//    bool nightMode = false;
//    help();
//    cv::CommandLineParser parser(argc, argv, "{@input|0|}");
//    string input = parser.get<string>("@input");
//    if( input.size() == 1 && isdigit(input[0]) )
//        cap.open(input[0] - '0');
//    else
//        cap.open(input);
//    if( !cap.isOpened() )
//    {
//        cout << "Could not initialize capturing...\n";
//        return 0;
//    }
//    namedWindow( "LK Demo", 1 );
//    setMouseCallback( "LK Demo", onMouse, 0 );
//    Mat gray, prevGray, image, frame;
//    vector<Point2f> points[2];
//    for(;;)
//    {
//        cap >> frame;
//        if( frame.empty() )
//            break;
//        frame.copyTo(image);
//        cvtColor(image, gray, COLOR_BGR2GRAY);
//        if( nightMode )
//            image = Scalar::all(0);
//        if( needToInit )
//        {
//            // automatic initialization
//            goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 3, 0, 0.04);
//            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
//            addRemovePt = false;
//        }
//        else if( !points[0].empty() )
//        {
//            vector<uchar> status;
//            vector<float> err;
//            if(prevGray.empty())
//                gray.copyTo(prevGray);
//            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
//                                 3, termcrit, 0, 0.001);
//            size_t i, k;
//            for( i = k = 0; i < points[1].size(); i++ )
//            {
//                if( addRemovePt )
//                {
//                    if( norm(point - points[1][i]) <= 5 )
//                    {
//                        addRemovePt = false;
//                        continue;
//                    }
//                }
//                if( !status[i] )
//                    continue;
//                points[1][k++] = points[1][i];
//                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
//            }
//            points[1].resize(k);
//        }
//        if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
//        {
//            vector<Point2f> tmp;
//            tmp.push_back(point);
//            cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
//            points[1].push_back(tmp[0]);
//            addRemovePt = false;
//        }
//        needToInit = false;
//        imshow("LK Demo", image);
//        char c = (char)waitKey(10);
//        if( c == 27 )
//            break;
//        switch( c )
//        {
//        case 'r':
//            needToInit = true;
//            break;
//        case 'c':
//            points[0].clear();
//            points[1].clear();
//            break;
//        case 'n':
//            nightMode = !nightMode;
//            break;
//        }
//        std::swap(points[1], points[0]);
//        cv::swap(prevGray, gray);
//    }
//    return 0;
//}

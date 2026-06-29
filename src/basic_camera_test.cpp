#include "opencv2/opencv.hpp"
#include <iostream>
#include "time.h"

using namespace std;
using namespace cv;

int main()
  {
    cv::VideoCapture cap(0);
    
    // Check if camera opened successfully
    if(!cap.isOpened()){
      cout << "Error opening video stream" << endl;
          return -1;
    }
    
    // Default resolutions of the frame are obtained.The default resolutions are system dependent.
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    VideoWriter video("outcpp.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, Size(frame_width,frame_height), false);
    
    double runtime_s = 10;
    
    Mat frame;
    Mat grey;
    Mat binary;

    time_t start = time(0);

    while (difftime( time(0), start) < runtime_s)
    {
        // Capture frame-by-frame
        cap >> frame;
    
        // If the frame is empty, break immediately
        if (frame.empty())
        {
            std::cout << "ERROR: Frame is empty\n" << std::flush;
            break;
        }

        cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

        // 3. Find the brightest spot
        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;

        cv::minMaxLoc(grey, &minVal, &maxVal, &minLoc, &maxLoc);

        std::cout << "Max brightness: " << maxVal << " at position: " << maxLoc << std::endl;

        std::cout << "did color conversion\n" << std::flush;

        // 2. Threshold the image to get bright pixels (e.g., intensity > 200)
        threshold(grey, binary, 240, 255, THRESH_BINARY);

        // 3. Find connected components to form pixel clusters
        Mat labels, stats, centroids;
        int numLabels = connectedComponentsWithStats(binary, labels, stats, centroids, 8, CV_32S);

        // 4. Print the centroids of the bright clusters
        cout << "Total bright clusters found: " << numLabels - 1 << endl; // Exclude background

        vector<Point2f> brightCenters;
        for(int i = 1; i < numLabels; i++) { // Start from 1 to skip the background
            Point2f center(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
            brightCenters.push_back(center);
            cout << "Cluster " << i << " Centroid: X = " << center.x << ", Y = " << center.y << endl;

            // 4. Draw a circle on the brightest spot
            // Arguments: image, center, radius, color (BGR), thickness (-1 fills the circle)
            cv::circle(frame, center, 3, cv::Scalar(0, 0, 255), -1);
        }

        // Write the frame into the file 'outcpp.avi'
        video.write(binary);
    }
 
  // When everything done, release the video capture and write object
  cap.release();
  video.release();
 
  // Closes all the frames
  destroyAllWindows();
  return 0;
}
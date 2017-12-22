// ----------------------------------------------------------------------------
// ZebrafishTrackerCalibration
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <signal.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/features2d.hpp>


int main(int argc, char * argv[])
{
  //////
  std::vector<cv::Point2f> image_pts;
  std::vector<cv::Point2f> stage_pts;

  image_pts.push_back(cv::Point2f(641,242));
  image_pts.push_back(cv::Point2f(1245,204));

  cv::FileStorage fs("../calibration.yml", cv::FileStorage::WRITE);
  fs << "image_pts" << image_pts;
  fs.release();

  // std::vector<cv::Point2f> image_pts2;
  // std::vector<cv::Point2f> stage_pts2;

  // cv::FileStorage fs2("calibration.yml", cv::FileStorage::READ);

  // fs2["image_pts"] >> image_pts2;
  // fs2.release();

  // std::cout << "image_pts = " << image_pts << std::endl;
  // std::cout << "image_pts.size() = " << image_pts.size() << std::endl;

  return 0;
}

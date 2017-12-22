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
#include <opencv2/calib3d/calib3d.hpp>


int main(int argc, char * argv[])
{
  //////
  std::vector<cv::Point2f> image_pts;
  std::vector<cv::Point2f> stage_pts;

  image_pts.push_back(cv::Point2f(641,242));
  stage_pts.push_back(cv::Point2f(89000,21000));

  image_pts.push_back(cv::Point2f(1245,204));
  stage_pts.push_back(cv::Point2f(88500,85000));

  image_pts.push_back(cv::Point2f(645,508));
  stage_pts.push_back(cv::Point2f(63000,21000));

  image_pts.push_back(cv::Point2f(878,502));
  stage_pts.push_back(cv::Point2f(63000,47000));

  image_pts.push_back(cv::Point2f(1122,498));
  stage_pts.push_back(cv::Point2f(63000,73000));

  image_pts.push_back(cv::Point2f(881,641));
  stage_pts.push_back(cv::Point2f(50000,47000));

  image_pts.push_back(cv::Point2f(645,777));
  stage_pts.push_back(cv::Point2f(37000,21000));

  image_pts.push_back(cv::Point2f(1131,783));
  stage_pts.push_back(cv::Point2f(37000,72000));

  image_pts.push_back(cv::Point2f(649,916));
  stage_pts.push_back(cv::Point2f(24000,21000));

  image_pts.push_back(cv::Point2f(1264,933));
  stage_pts.push_back(cv::Point2f(24000,85000));

  cv::FileStorage fs("calibration.yml", cv::FileStorage::WRITE);
  fs << "image_pts" << image_pts;
  fs << "stage_pts" << stage_pts;

  cv::Mat homography_image_to_stage = cv::findHomography(image_pts,stage_pts,CV_LMEDS);
  fs << "homography_image_to_stage" << homography_image_to_stage;

  cv::Mat homography_stage_to_image = cv::findHomography(stage_pts,image_pts,CV_LMEDS);
  fs << "homography_stage_to_image" << homography_stage_to_image;

  std::vector<cv::Point2f> image_pts_in;
  std::vector<cv::Point2f> stage_pts_out;

  image_pts_in.push_back(cv::Point2f(700,250));
  image_pts_in.push_back(cv::Point2f(1250,250));
  image_pts_in.push_back(cv::Point2f(700,950));
  image_pts_in.push_back(cv::Point2f(1250,950));

  cv::perspectiveTransform(image_pts_in,stage_pts_out,homography_image_to_stage);
  fs << "image_pts_in" << image_pts_in;
  fs << "stage_pts_out" << stage_pts_out;

  cv::Mat background = cv::imread("background.png",CV_LOAD_IMAGE_GRAYSCALE );
  if(!background.data )
  {
    std::cout <<  "Could not open or find the background image!" << std::endl ;
    return -1;
  }
  cv::Mat calibrated;
  cv::cvtColor(background,calibrated,CV_GRAY2BGR);

  cv::line(calibrated,
           image_pts_in[0],
           image_pts_in[1],
           cv::Scalar(255,0,0),
           4);
  cv::line(calibrated,
           image_pts_in[0],
           image_pts_in[2],
           cv::Scalar(0,255,0),
           4);
  cv::line(calibrated,
           image_pts_in[1],
           image_pts_in[3],
           cv::Scalar(255,255,0),
           4);
  cv::line(calibrated,
           image_pts_in[2],
           image_pts_in[3],
           cv::Scalar(255,255,0),
           4);

  std::stringstream image_in_ss_0;
  image_in_ss_0 << "i" << image_pts_in[0] << " -> " << "s" << stage_pts_out[0];
  cv::Point2f image_in_0(image_pts_in[0].x,image_pts_in[0].y - 20);
  cv::putText(calibrated,
              image_in_ss_0.str(),
              image_in_0,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_1;
  image_in_ss_1 << "i" << image_pts_in[1] << " -> " << "s" << stage_pts_out[1];
  cv::Point2f image_in_1(image_pts_in[1].x,image_pts_in[1].y - 20);
  cv::putText(calibrated,
              image_in_ss_1.str(),
              image_in_1,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_2;
  image_in_ss_2 << "i" << image_pts_in[2] << " -> " << "s" << stage_pts_out[2];
  cv::Point2f image_in_2(image_pts_in[2].x,image_pts_in[2].y + 30);
  cv::putText(calibrated,
              image_in_ss_2.str(),
              image_in_2,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_3;
  image_in_ss_3 << "i" << image_pts_in[3] << " -> " << "s" << stage_pts_out[3];
  cv::Point2f image_in_3(image_pts_in[3].x,image_pts_in[3].y + 30);
  cv::putText(calibrated,
              image_in_ss_3.str(),
              image_in_3,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::vector<cv::Point2f> stage_pts_in;
  std::vector<cv::Point2f> image_pts_out;

  stage_pts_in.push_back(cv::Point2f(0,0));
  stage_pts_in.push_back(cv::Point2f(100000,0));
  stage_pts_in.push_back(cv::Point2f(0,100000));
  stage_pts_in.push_back(cv::Point2f(100000,100000));

  cv::perspectiveTransform(stage_pts_in,image_pts_out,homography_stage_to_image);
  fs << "stage_pts_in" << stage_pts_in;
  fs << "image_pts_out" << image_pts_out;

  fs.release();

  cv::line(calibrated,
           image_pts_out[0],
           image_pts_out[1],
           cv::Scalar(255,0,0),
           4);
  cv::line(calibrated,
           image_pts_out[0],
           image_pts_out[2],
           cv::Scalar(0,255,0),
           4);
  cv::line(calibrated,
           image_pts_out[1],
           image_pts_out[3],
           cv::Scalar(255,255,0),
           4);
  cv::line(calibrated,
           image_pts_out[2],
           image_pts_out[3],
           cv::Scalar(255,255,0),
           4);
  std::stringstream image_out_ss_0;
  // image_out_ss_0 << stage_pts_in[0] << "->" << image_pts_out[0];
  image_out_ss_0 << "s" << stage_pts_in[0];
  cv::Point2f image_out_0(image_pts_out[0].x + 20,image_pts_out[0].y - 20);
  cv::putText(calibrated,
              image_out_ss_0.str(),
              image_out_0,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_out_ss_1;
  // image_out_ss_1 << stage_pts_in[1] << "->" << image_pts_out[1];
  image_out_ss_1 << "s" << stage_pts_in[1];
  cv::Point2f image_out_1(image_pts_out[1].x + 20,image_pts_out[1].y + 30);
  cv::putText(calibrated,
              image_out_ss_1.str(),
              image_out_1,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_out_ss_3;
  // image_out_ss_3 << stage_pts_in[3] << "->" << image_pts_out[3];
  image_out_ss_3 << "s" << stage_pts_in[3];
  cv::Point2f image_out_3(image_pts_out[3].x,image_pts_out[3].y - 20);
  cv::putText(calibrated,
              image_out_ss_3.str(),
              image_out_3,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  cv::imwrite("calibrated.png",calibrated);

  // std::vector<cv::Point2f> image_pts2;
  // std::vector<cv::Point2f> stage_pts2;

  // cv::FileStorage fs2("calibration.yml", cv::FileStorage::READ);

  // fs2["image_pts"] >> image_pts2;
  // fs2.release();

  // std::cout << "image_pts = " << image_pts << std::endl;
  // std::cout << "image_pts.size() = " << image_pts.size() << std::endl;

  return 0;
}

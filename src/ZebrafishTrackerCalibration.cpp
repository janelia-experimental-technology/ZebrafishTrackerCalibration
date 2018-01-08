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
  cv::Mat checkerboard = cv::imread("images/checkerboard.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!checkerboard.data)
  {
    std::cout <<  "Could not open or find the checkerboard image!" << std::endl;
    return -1;
  }

  size_t pattern_count_col = 6;
  size_t pattern_count_row = 8;
  cv::Size patternsize(pattern_count_col,pattern_count_row);
  std::vector<cv::Point2f> corners;

  bool patternfound = cv::findChessboardCorners(checkerboard,
                                                patternsize,
                                                corners,
                                                cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

  if(!patternfound)
  {
    std::cout <<  "Could not find the checkerboard corners!" << std::endl;
    return -1;
  }
  if(patternfound)
  {
    cv::cornerSubPix(checkerboard,
                     corners,
                     cv::Size(11, 11),
                     cv::Size(-1, -1),
                     cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
  }

  std::vector<cv::Point2f> image_pts;
  for (size_t row=0; row<pattern_count_row; ++row)
  {
    for (size_t col=0; col<pattern_count_col; ++col)
    {
      if (((col % (pattern_count_col - 1)) == 0) && ((row % 2) == 0))
      {
        image_pts.push_back(corners[row*pattern_count_col + col]);
      }
    }
  }

  cv::Mat checkerboard_image_points;
  cv::cvtColor(checkerboard,checkerboard_image_points,CV_GRAY2BGR);
  cv::drawChessboardCorners(checkerboard_image_points,patternsize,cv::Mat(corners),patternfound);
  for (std::vector<cv::Point2f>::iterator it = image_pts.begin(); it != image_pts.end(); ++it)
  {
    cv::Point2i image_pt = *it;
    std::stringstream image_pt_ss;
    image_pt_ss << "i" << image_pt;
    cv::Point2i text_location(image_pt.x,image_pt.y - 10);
    cv::putText(checkerboard_image_points,
                image_pt_ss.str(),
                text_location,
                cv::FONT_HERSHEY_SIMPLEX,
                0.4,
                cv::Scalar(0,255,255),
                1);
  }
  cv::imwrite("images/checkerboard_image_points.png",checkerboard_image_points);

  std::vector<cv::Point2f> stage_pts;

  // found experimentally on rig
  stage_pts.push_back(cv::Point2f(80100,34700)); stage_pts.push_back(cv::Point2f(80500,69800));

  stage_pts.push_back(cv::Point2f(66000,34900)); stage_pts.push_back(cv::Point2f(66500,70000));

  stage_pts.push_back(cv::Point2f(51900,35000)); stage_pts.push_back(cv::Point2f(52300,70200));

  stage_pts.push_back(cv::Point2f(37800,35200)); stage_pts.push_back(cv::Point2f(38200,70300));

  cv::FileStorage fs("calibration/calibration.yml", cv::FileStorage::WRITE);
  std::vector<cv::Point2i> image_pts_i;
  cv::Mat(image_pts).copyTo(image_pts_i);
  std::vector<cv::Point2i> stage_pts_i;
  cv::Mat(stage_pts).copyTo(stage_pts_i);
  fs << "image_pts" << image_pts_i;
  fs << "stage_pts" << stage_pts_i;

  cv::Mat homography_image_to_stage = cv::findHomography(image_pts,stage_pts,CV_LMEDS);
  fs << "homography_image_to_stage" << homography_image_to_stage;

  cv::Mat homography_stage_to_image = cv::findHomography(stage_pts,image_pts,CV_LMEDS);
  fs << "homography_stage_to_image" << homography_stage_to_image;

  fs.release();

  cv::Mat background = cv::imread("images/background.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!background.data)
  {
    std::cout <<  "Could not open or find the background image!" << std::endl;
    return -1;
  }
  cv::Mat calibrated;
  cv::cvtColor(background,calibrated,CV_GRAY2BGR);

  std::vector<cv::Point2f> stage_pts_in;
  std::vector<cv::Point2f> image_pts_out;

  stage_pts_in.push_back(cv::Point2f(0,0));
  stage_pts_in.push_back(cv::Point2f(100000,0));
  stage_pts_in.push_back(cv::Point2f(0,100000));
  stage_pts_in.push_back(cv::Point2f(100000,100000));

  cv::perspectiveTransform(stage_pts_in,image_pts_out,homography_stage_to_image);
  std::vector<cv::Point2i> stage_pts_in_i;
  cv::Mat(stage_pts_in).copyTo(stage_pts_in_i);
  std::vector<cv::Point2i> image_pts_out_i;
  cv::Mat(image_pts_out).copyTo(image_pts_out_i);

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
  image_out_ss_0 << "s" << stage_pts_in_i[0];
  cv::Point2f image_out_0(image_pts_out[0].x + 20,image_pts_out[0].y - 20);
  cv::putText(calibrated,
              image_out_ss_0.str(),
              image_out_0,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_out_ss_1;
  image_out_ss_1 << "s" << stage_pts_in_i[1];
  cv::Point2f image_out_1(image_pts_out[1].x + 20,image_pts_out[1].y + 30);
  cv::putText(calibrated,
              image_out_ss_1.str(),
              image_out_1,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_out_ss_3;
  image_out_ss_3 << "s" << stage_pts_in_i[3];
  cv::Point2f image_out_3(image_pts_out[3].x,image_pts_out[3].y - 20);
  cv::putText(calibrated,
              image_out_ss_3.str(),
              image_out_3,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::vector<cv::Point2f> image_pts_in;
  std::vector<cv::Point2f> stage_pts_out;

  image_pts_in.push_back(cv::Point2f(700,250));
  image_pts_in.push_back(cv::Point2f(1250,250));
  image_pts_in.push_back(cv::Point2f(700,950));
  image_pts_in.push_back(cv::Point2f(1250,950));

  cv::perspectiveTransform(image_pts_in,stage_pts_out,homography_image_to_stage);
  std::vector<cv::Point2i> image_pts_in_i;
  cv::Mat(image_pts_in).copyTo(image_pts_in_i);
  std::vector<cv::Point2i> stage_pts_out_i;
  cv::Mat(stage_pts_out).copyTo(stage_pts_out_i);

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
  image_in_ss_0 << "i" << image_pts_in_i[0] << " -> " << "s" << stage_pts_out_i[0];
  cv::Point2f image_in_0(image_pts_in_i[0].x,image_pts_in_i[0].y - 20);
  cv::putText(calibrated,
              image_in_ss_0.str(),
              image_in_0,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_1;
  image_in_ss_1 << "i" << image_pts_in_i[1] << " -> " << "s" << stage_pts_out_i[1];
  cv::Point2f image_in_1(image_pts_in_i[1].x,image_pts_in_i[1].y - 20);
  cv::putText(calibrated,
              image_in_ss_1.str(),
              image_in_1,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_2;
  image_in_ss_2 << "i" << image_pts_in_i[2] << " -> " << "s" << stage_pts_out_i[2];
  cv::Point2f image_in_2(image_pts_in_i[2].x,image_pts_in_i[2].y + 30);
  cv::putText(calibrated,
              image_in_ss_2.str(),
              image_in_2,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  std::stringstream image_in_ss_3;
  image_in_ss_3 << "i" << image_pts_in_i[3] << " -> " << "s" << stage_pts_out_i[3];
  cv::Point2f image_in_3(image_pts_in_i[3].x,image_pts_in_i[3].y + 30);
  cv::putText(calibrated,
              image_in_ss_3.str(),
              image_in_3,
              cv::FONT_HERSHEY_SIMPLEX,
              0.75,
              cv::Scalar(0,0,255),
              2);

  cv::imwrite("images/calibrated.png",calibrated);

  return 0;
}

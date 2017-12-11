#include "ImageProcessor.h"
#include "VideoProcessor.h"
#include <algorithm>
#include <opencv2\imgproc.hpp>
#include <vector>
#include "BaseEffects.h"



int main()
{
  ImageProcessor processor("data\\text.jpg");
  //VideoProcessor processor("data\\video_registrator_1.mp4");

  auto show_hist = [](cv::Mat &src)
  {
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 };
    const float* histRange = { range };

    cv::Mat hist;

    /// Compute the histograms:
    calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    /// Normalize the result to [ 0, histImage.rows ]
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    /// Draw for each channel
    for (int i = 1; i < histSize; i++)
    {
      cv::line(histImage,
        cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
        cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
        cv::Scalar(255, 0, 0), 2, 8, 0);
    }

    static int i = 0;
    ++i;
    cv::imshow((std::string("hist") + std::to_string(i)).c_str(), histImage);
    return true;
  };

  processor.AddProcedure(std::make_unique<BaseEffects>(), "out", [&processor]()
  {
    processor.Load("data\\text.jpg");
    processor.Run();
  });

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::cvtColor(img, img, CV_BGR2GRAY);
//     return true;
//   }, "gray1");

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat out;
//     int elem = 0;
//     int size = 1;
//     int type;
//     if (elem == 0) { type = cv::MORPH_RECT; }
//     else if (elem == 1) { type = cv::MORPH_CROSS; }
//     else if (elem == 2) { type = cv::MORPH_ELLIPSE; }
// 
//     cv::Mat element = cv::getStructuringElement(type,
//       cv::Size(2 * size + 1, 2 * size + 1),
//       cv::Point(size, size));
// 
//     cv::erode(img, out, element);
//     img = out;
//     return true;
//   }, "gray2");

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat out;
//     int elem = 0;
//     int size = 1;
//     int type;
//     if (elem == 0) { type = cv::MORPH_RECT; }
//     else if (elem == 1) { type = cv::MORPH_CROSS; }
//     else if (elem == 2) { type = cv::MORPH_ELLIPSE; }
// 
//     cv::Mat element = cv::getStructuringElement(type,
//       cv::Size(2 * size + 1, 2 * size + 1),
//       cv::Point(size, size));
// 
//     cv::dilate(img, out, element);
//     img = out;
//     return true;
//   }, "gray3");

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat hist;
//     cv::threshold(img, hist, 127, 255, cv::THRESH_BINARY);
//     img = hist;
//     return true;
//   }, "gray2");

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat hist;
//     cv::adaptiveThreshold(img, hist, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
//     img = hist;
//     return true;
//   }, "gray2");

//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat hist;
//     cv::adaptiveThreshold(img, hist, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
//     img = hist;
//     return true;
//   }, "gray2");

//   processor.AddProcedure(show_hist);
// 
//   processor.AddProcedure([](cv::Mat &img)
//   {
//     cv::Mat hist;
//     cv::equalizeHist(img, hist);
//     img = hist;
//     return true;
//   }, "gray4");
// 
//   processor.AddProcedure(show_hist);

  processor.Run();


  cv::waitKey(0);
  return 0;
}


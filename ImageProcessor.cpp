#include "ImageProcessor.h"
#include <opencv2\imgcodecs.hpp>



ImageProcessor::ImageProcessor(const std::string &filename)
{
  Load(filename);
}

void ImageProcessor::Load(const std::string &filename)
{
  mImage = cv::imread(filename.c_str(), cv::IMREAD_COLOR);
}

bool ImageProcessor::Run()
{
  IProcessor::Run();
  if (!mImage.data)
    return false;

  FrameImage(mImage);
  return true;
}

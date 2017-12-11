#pragma once

#include "Processor.h"
#include <opencv2/videoio.hpp>

class VideoProcessor : public IProcessor
{
public:
  VideoProcessor(const std::string &filename);

  virtual bool Run() override;

protected:
  cv::VideoCapture mCapture;
  cv::Mat mImage;
};


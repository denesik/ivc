#pragma once

#include <opencv2/core/core.hpp>


class IProcessorFunction
{
public:
  virtual ~IProcessorFunction() = default;

  virtual bool Exec(cv::Mat &image) = 0;

  virtual bool ShowGui(std::function<void()> &callback) = 0;
};


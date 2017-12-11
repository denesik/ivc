#pragma once

#include "Processor.h"
#include <string>

class ImageProcessor : public IProcessor
{
public:
  ImageProcessor(const std::string &filename);

  void Load(const std::string &filename);

  virtual bool Run() override;

protected:
  cv::Mat mImage;

};


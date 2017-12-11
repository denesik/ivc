#include "VideoProcessor.h"



VideoProcessor::VideoProcessor(const std::string &filename)
{
  mCapture.open(filename.c_str());
}

bool VideoProcessor::Run()
{
  IProcessor::Run();
  if (!mCapture.isOpened()) 
    return false;

  for (;;)
  {
    mCapture >> mImage;
    if (!FrameImage(mImage))
    {
      cv::waitKey(1);
      return false;
    }
    cv::waitKey(1);
  }

  return true;
}

#pragma once

#include "ProcessorFunction.h"


class BaseEffects : public IProcessorFunction
{
public:
  virtual bool Exec(cv::Mat &img) override;

  virtual bool ShowGui(std::function<void()> &callback) override;
  
private:
  int mValueOffset = 255;
  int mValueMax = 255 + mValueOffset;

  int mSaturation = 255;
  int mExposition = 255;
  int mHue = 255;
  int mTemperature = 255;
  int mLight = 255;
  int mShadow = 255;
  int mContrast = 255;
  int mClarity = 255;
private:
  int Value(int val);


  cv::Mat GetGammaExpo(int step);
  cv::Mat GetGammaLightShadow(int step, bool reverse);
  uchar AddDoubleToByte(uchar bt, double d);

};


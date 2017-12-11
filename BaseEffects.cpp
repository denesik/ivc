#include "BaseEffects.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


//https://habrahabr.ru/post/268115/


bool BaseEffects::Exec(cv::Mat &img)
{
  cv::Mat hsv;
  cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV_FULL);

  std::vector<cv::Mat> comp;
  cv::split(hsv, comp);
  
  comp[1] += Value(mSaturation);
  {
    cv::Mat lut = GetGammaExpo(Value(mExposition));
    cv::LUT(comp[2], lut, comp[2]);
  }

  {
    cv::Mat lut = GetGammaLightShadow(Value(mLight), true);
    cv::LUT(comp[2], lut, comp[2]);
  }

  {
    cv::Mat lut = GetGammaLightShadow(Value(mShadow), false);
    cv::LUT(comp[2], lut, comp[2]);
  }

  cv::merge(comp, hsv);
  cv::cvtColor(hsv, img, cv::COLOR_HSV2RGB_FULL);

  {
    std::vector<cv::Mat> rgb;
    cv::Mat lut0 = GetGammaExpo(Value(mHue));
    cv::Mat lut1 = GetGammaExpo(-Value(mHue));
    cv::Mat lut2 = GetGammaExpo(Value(mHue));
    cv::split(img, rgb);
    cv::LUT(rgb[0], lut0, rgb[0]);
    cv::LUT(rgb[1], lut1, rgb[1]);
    cv::LUT(rgb[2], lut2, rgb[2]);
    cv::merge(rgb, img);
  }

  {
    std::vector< cv::Mat> rgb;
    cv::Mat lut0 = GetGammaExpo(-Value(mTemperature) * 2);
    cv::Mat lut1 = GetGammaExpo(Value(mTemperature));
    cv::Mat lut2 = GetGammaExpo(Value(mTemperature));
    cv::split(img, rgb);
    cv::LUT(rgb[0], lut0, rgb[0]);
    cv::LUT(rgb[1], lut1, rgb[1]);
    cv::LUT(rgb[2], lut2, rgb[2]);
    cv::merge(rgb, img);
  }

  {
    std::vector<cv::Mat> rgb;
    cv::split(img, rgb);
    cv::Mat lut(1, 256, CV_8UC1);
    double contrastLevel = double(100 + Value(mContrast)) / 100;
    uchar* p = lut.data;
    double d;
    for (int i = 0; i < 256; i++)
    {
      d = ((double(i) / 255 - 0.5)*contrastLevel + 0.5) * 255;
      if (d > 255)
        d = 255;
      if (d < 0)
        d = 0;
      p[i] = d;
    }
    cv::LUT(rgb[0], lut, rgb[0]);
    cv::LUT(rgb[1], lut, rgb[1]);
    cv::LUT(rgb[2], lut, rgb[2]);
    cv::merge(rgb, img);
  }

  {
    if (mClarity < 0)
    {
      cv::blur(img, img, cv::Size(-Value(mClarity) * 2 + 1, -Value(mClarity) * 2 + 1));
    }
    else 
    {
      cv::Mat dst = img.clone();
      float matr[9]{
        -0.0375 - 0.05*Value(mClarity), -0.0375 - 0.05*Value(mClarity), -0.0375 - 0.05*Value(mClarity),
        -0.0375 - 0.05*Value(mClarity), 1.3 + 0.4*Value(mClarity), -0.0375 - 0.05*Value(mClarity),
        -0.0375 - 0.05*Value(mClarity), -0.0375 - 0.05*Value(mClarity), -0.0375 - 0.05*Value(mClarity)
      };
      cv::Mat kernel_matrix = cv::Mat(3, 3, CV_32FC1, &matr);
      cv::filter2D(img, dst, 32, kernel_matrix);
      img = dst;
    }
  }

  return true;
}

bool BaseEffects::ShowGui(std::function<void()> &callback)
{
  cv::namedWindow("menu", CV_WINDOW_FREERATIO);
  auto lambda = [](int, void *func)
  {
    auto &callback = (*reinterpret_cast<std::function<void()> *>(func));
    if (callback) callback();
  };

  cv::createTrackbar("Saturation", "menu", &mSaturation, mValueMax, lambda, &callback);
  cv::createTrackbar("Exposition", "menu", &mExposition, mValueMax, lambda, &callback);
  cv::createTrackbar("Hue", "menu", &mHue, mValueMax, lambda, &callback);
  cv::createTrackbar("Temperature", "menu", &mTemperature, mValueMax, lambda, &callback);
  cv::createTrackbar("Light", "menu", &mLight, mValueMax, lambda, &callback);
  cv::createTrackbar("Shadow", "menu", &mShadow, mValueMax, lambda, &callback);
  cv::createTrackbar("Contrast", "menu", &mContrast, mValueMax, lambda, &callback);
  cv::createTrackbar("Clarity", "menu", &mClarity, mValueMax, lambda, &callback);
  return true;
}

int BaseEffects::Value(int val)
{
  return val - mValueOffset;
}

cv::Mat BaseEffects::GetGammaExpo(int step)
{
  cv::Mat result(1, 256, CV_8UC1);

  uchar* p = result.data;
  for (int i = 0; i < 256; i++)
  {
    p[i] = AddDoubleToByte(i, std::sin(i * 0.01255) * step * 10);
  }

  return result;
}

cv::Mat BaseEffects::GetGammaLightShadow(int step, bool reverse)
{
  cv::Mat result(1, 256, CV_8UC1);
  for (int i = 0; i < 256; i++)
  {
    *(result.data + i) = AddDoubleToByte(i, std::pow(0.36811145*M_E,-std::pow(abs((reverse ? 256 : 0) - i), 1.7))*0.2*step*abs((reverse ? 256 : 0) - i));
  }
  return result;
}

uchar BaseEffects::AddDoubleToByte(uchar bt, double d)
{
  uchar result = bt;
  if (double(result) + d > 255)
    result = 255;
  else if (double(result) + d < 0)
    result = 0;
  else
  {
    result += uchar(d);
  }
  return result;
}

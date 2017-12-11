#pragma once

#include <functional>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <memory>
#include "ProcessorFunction.h"

class IProcessor
{
public:
  virtual ~IProcessor() = default;

  using ProcessFunc = std::unique_ptr<IProcessorFunction>;
  
  void AddProcedure(ProcessFunc func, const std::string &win_name = "", std::function<void()> gui_callback = nullptr)
  {
    mFuncs.emplace_back(decltype(mFuncs)::value_type{ std::move(func), win_name, gui_callback });
  }

  virtual bool Run()
  {
    for (auto &data : mFuncs)
    {
      data.func->ShowGui(data.gui_callback);
    }
    return true;
  }

  virtual bool FrameImage(cv::Mat &image)
  {
    if (image.empty())
      return false;
    cv::imshow("original", image);

    for (auto &data : mFuncs)
    {
      if (!data.func->Exec(image) || image.empty())
        return false;
      if (!data.window_name.empty())
        cv::imshow(data.window_name.c_str(), image);
    }
    return true;
  }

protected:
  struct
  {
    ProcessFunc func;
    std::string window_name;
    std::function<void()> gui_callback;
  } mData;
  std::vector<decltype(mData)> mFuncs;
};

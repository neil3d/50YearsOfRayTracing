#pragma once

#include <exception>
#include <string>

class MyException : public std::exception {
 public:
  MyException(const std::string& szMessage) : mMessage(szMessage) {}

  virtual const char* what() const noexcept override {
    return mMessage.c_str();
  }

 private:
  std::string mMessage;
};
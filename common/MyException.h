#pragma once

#include <exception>
#include <string>

class MyException : public std::exception {
 public:
  MyException(const std::string& szMessage) : mMessage(szMessage) {}
  MyException(const std::string& szMessage, const char* SDLError) {
    mMessage = szMessage;
    mMessage.append(", SDL Error: ");
    mMessage.append(SDLError);
  }

  virtual const char* what() const noexcept override {
    return mMessage.c_str();
  }

 private:
  std::string mMessage;
};
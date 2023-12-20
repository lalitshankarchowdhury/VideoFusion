#include "mmobject.h"

MMObject::MMObject(std::string filePath)
    : context(nullptr)
{
    if (avformat_open_input(&context, filePath.c_str(), nullptr, nullptr) < 0) {
        context = nullptr;
        throw Exception("Failed to read file header");
    }
}

MMObject::~MMObject()
{
    if (context) {
        avformat_close_input(&context);
    }
}

MMObject::Exception::Exception(std::string what)
{
    whatBuffer = what;
}

const char* MMObject::Exception::what() const noexcept
{
    return whatBuffer.c_str();
}

#ifndef MMOBJECT_H
#define MMOBJECT_H

#include <exception>
extern "C" {
#include <libavformat/avformat.h>
}
#include <string>

class MMObject {
public:
    class Exception : std::exception {
    public:
        Exception(std::string what);
        const char* what() const noexcept override;

    private:
        std::string whatBuffer;
    };

public:
    MMObject(std::string filePath);
    ~MMObject();

private:
    AVFormatContext* context;
};

#endif // MMOBJECT_H

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
    const char* getVideoCodec() const noexcept;
    double getFrameRate() const noexcept;
    const char* getAudioCodec() const noexcept;
    int getSampleRate() const noexcept;

private:
    AVFormatContext* context;
    const char* videoCodec;
    double frameRate;
    const char* audioCodec;
    int sampleRate;
};

#endif // MMOBJECT_H

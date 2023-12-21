#include "mmobject.h"

MMObject::Exception::Exception(std::string what)
{
    whatBuffer = what;
}

const char* MMObject::Exception::what() const noexcept
{
    return whatBuffer.c_str();
}

MMObject::MMObject(std::string filePath)
    : context(nullptr)
    , videoCodec("No Video Stream")
    , frameRate(0.0)
    , audioCodec("No Audio Stream")
    , sampleRate(0)
{
    if (avformat_open_input(&context, filePath.c_str(), nullptr, nullptr) < 0) {
        context = nullptr;
        throw Exception("Failed to read header");
    }

    int video_stream_index = av_find_best_stream(context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream_index == AVERROR_STREAM_NOT_FOUND) {
        throw Exception("No video stream found");
    }
    videoCodec = avcodec_get_name(context->streams[video_stream_index]->codecpar->codec_id);
    frameRate = av_q2d(context->streams[video_stream_index]->r_frame_rate);

    int audio_stream_index = av_find_best_stream(context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream_index != AVERROR_STREAM_NOT_FOUND) {
        audioCodec = avcodec_get_name(context->streams[audio_stream_index]->codecpar->codec_id);
        sampleRate = context->streams[audio_stream_index]->codecpar->sample_rate;
    }
}

MMObject::~MMObject()
{
    if (context) {
        avformat_close_input(&context);
    }
}

const char* MMObject::getVideoCodec() const noexcept
{
    return videoCodec;
}

double MMObject::getFrameRate() const noexcept
{
    return frameRate;
}

const char* MMObject::getAudioCodec() const noexcept
{
    return audioCodec;
}

int MMObject::getSampleRate() const noexcept
{
    return sampleRate;
}
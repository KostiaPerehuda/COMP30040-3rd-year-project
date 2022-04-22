#include "video-recorder.h"

#include <ctime>

VideoRecorder::VideoRecorder()
{
    // Video name
    std::time_t t = std::time(NULL);
    std::tm* local = std::localtime(&t);
    std::ostringstream stream;
    stream << "videos/" << 1900 + local->tm_year << "-" << 1 + local->tm_mon << "-" << local->tm_mday << "-" << local->tm_hour << "-" << local->tm_min << "-" << local->tm_sec << ".avi";

    // Video writer
    writer = cv::VideoWriter(stream.str(), cv::VideoWriter::fourcc('D', 'I', 'B', ' '), 15, cv::Size(640, 360));
}

VideoRecorder::~VideoRecorder()
{
    writer.release();
}

void VideoRecorder::onImageReceived(cv::Mat image)
{
    writer << image;
}
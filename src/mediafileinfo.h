#ifndef MEDIAFILEINFO_H
#define MEDIAFILEINFO_H

#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

/**
 * @brief The MediaFileInfo class
 * Class to get the meta data of a media file
 * Actually only use to get media file duration
 */
class MediaFileInfo
{
public:
    MediaFileInfo();
    ~MediaFileInfo();
    void find_meta_data(const char* filename);
    int64_t getMS();
    int getUSecond();
    int getSecond();
    int getMinute();
    int getHour();

private:
    std::string filename;
    AVFormatContext *pFormatCtx;
    int64_t duration;

};

#endif // MEDIAFILEINFO_H
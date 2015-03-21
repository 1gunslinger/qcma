/*
 *  QCMA: Cross-platform content manager assistant for the PS Vita
 *
 *  Copyright (C) 2013  Codestation
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AVDECODER_H
#define AVDECODER_H

#include <QString>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/mathematics.h>
}

#include <vitamtp.h>

class AVDecoder
{
public:
    AVDecoder();
    ~AVDecoder();

    enum codec_type {CODEC_VIDEO = AVMEDIA_TYPE_VIDEO, CODEC_AUDIO = AVMEDIA_TYPE_AUDIO};

    bool open(const QString filename);
    void close();
    bool loadCodec(codec_type codec);

    QByteArray getAudioThumbnail(int width, int height);
    QByteArray getVideoThumbnail(int width, int height);
    void getAudioMetadata(metadata_t &metadata);
    void getVideoMetadata(metadata_t &metadata);
    const char *getMetadataEntry(const char *key, const char *default_value = NULL);

    inline int getWidth() {
        return pCodecCtx->width;
    }

    inline int getHeight() {
        return pCodecCtx->height;
    }

    inline int getDuration() {
        return pFormatCtx->duration / 1000;
    }

    inline int getBitrate() {
        return pCodecCtx->bit_rate;
    }

    // simulate a static constructor to initialize libav only once
    class AvInit
    {
    public:
        AvInit() {
            av_register_all();
            // hide warning logs
            av_log_set_level(AV_LOG_ERROR);
        }
    };

    static AvInit init;

private:
    AVFrame *getDecodedFrame(AVCodecContext *pCodecCtx, int stream_index);

    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVStream *av_stream;
    AVCodec *av_codec;
    int stream_index;
};

#endif // AVDECODER_H

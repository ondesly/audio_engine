//
//  a_audio_test.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <aaudio/AAudio.h>

#include "a_audio_test.h"

maw::a_audio_test::a_audio_test() {
    AAudioStreamBuilder *builder;

    auto result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK) {
        return;
    }

    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);

    AAudioStreamBuilder_setSampleRate(builder, 44100);
    AAudioStreamBuilder_setChannelCount(builder, 2);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_FLOAT);

    AAudioStreamBuilder_setBufferCapacityInFrames(builder, 1323);
    AAudioStreamBuilder_setFramesPerDataCallback(builder, 1323 / 441);

    AAudioStreamBuilder_setDataCallback(builder,
                                        [](AAudioStream *stream, void *userData, void *audioData,
                                           int32_t numFrames) -> aaudio_data_callback_result_t {
                                            std::cout << "callback" << std::endl;
                                            return 0;
                                        }, nullptr);

    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

    AAudioStreamBuilder_setErrorCallback(builder, [](AAudioStream *stream, void *userData,
                                                     aaudio_result_t error) {
        std::cout << "error" << std::endl;
    }, nullptr);

    //

    result = AAudioStreamBuilder_openStream(builder, &m_stream);
    AAudioStreamBuilder_delete(builder);
    if (result != AAUDIO_OK) {
        return;
    }
}

maw::a_audio_test::~a_audio_test() {
    auto result = AAudioStream_close(m_stream);
    if (result != AAUDIO_OK) {
        return;
    }
}

void maw::a_audio_test::start() {
    auto result = AAudioStream_requestStart(m_stream);
    if (result != AAUDIO_OK) {
        AAudioStream_close(m_stream);
        return;
    }

    auto state = AAudioStream_getState(m_stream);
    if (state != AAUDIO_STREAM_STATE_STARTED) {
        if (state != AAUDIO_STREAM_STATE_STARTING) {
            AAudioStream_close(m_stream);
            return;
        }

        aaudio_stream_state_t new_state = AAUDIO_STREAM_STATE_STARTED;
        result = AAudioStream_waitForStateChange(m_stream, state, &new_state,
                                                 5000000000); /* 5 second timeout. */

        if (result != AAUDIO_OK) {
            AAudioStream_close(m_stream);
            return;
        }

        if (new_state != AAUDIO_STREAM_STATE_STARTED) {
            AAudioStream_close(m_stream);
            return;
        }

        state = new_state;
    }
}

void maw::a_audio_test::stop() {
    auto result = AAudioStream_requestStop(m_stream);
    if (result != AAUDIO_OK) {
        AAudioStream_close(m_stream);
        return;
    }

    auto state = AAudioStream_getState(m_stream);
    if (state != AAUDIO_STREAM_STATE_STOPPED) {
        if (state != AAUDIO_STREAM_STATE_STOPPING) {
            AAudioStream_close(m_stream);
            return;
        }

        aaudio_stream_state_t new_state = AAUDIO_STREAM_STATE_STOPPED;
        result = AAudioStream_waitForStateChange(m_stream, state, &new_state,
                                                 5000000000); /* 5 second timeout. */

        if (result != AAUDIO_OK) {
            AAudioStream_close(m_stream);
            return;
        }

        if (new_state != AAUDIO_STREAM_STATE_STOPPED) {
            AAudioStream_close(m_stream);
            return;
        }

        state = new_state;
    }
}

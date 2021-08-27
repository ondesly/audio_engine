//
//  decoder_android.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 27.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <android/asset_manager.h>

#include "maw/a_asset_manager_holder.h"

#include "maw/decoder.h"

namespace {

    AAssetManager *s_asset_manager = nullptr;

    ma_result on_open(ma_vfs *vfs, const char *path, ma_uint32 mode, ma_vfs_file *file) {
        if (!s_asset_manager) {
            return MA_ERROR;
        }

        auto asset = AAssetManager_open(s_asset_manager, path, AASSET_MODE_STREAMING);

        if (!asset) {
            return MA_DOES_NOT_EXIST;
        }

        *file = static_cast<ma_vfs_file>(asset);

        return MA_SUCCESS;
    }

    ma_result on_close(ma_vfs *vfs, ma_vfs_file file) {
        auto asset = static_cast<AAsset *>(file);

        AAsset_close(asset);

        return MA_SUCCESS;
    }

    ma_result on_read(ma_vfs *vfs, ma_vfs_file file, void *dst, size_t size, size_t *read) {
        auto asset = static_cast<AAsset *>(file);
        const auto result = AAsset_read(asset, dst, size);

        if (read) {
            *read = result;
        }

        if (result == 0) {
            return MA_AT_END;
        } else if (result < 0) {
            return MA_ERROR;
        } else {
            return MA_SUCCESS;
        }
    }

    ma_result on_seek(ma_vfs *vfs, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin) {
        int whence;
        if (origin == ma_seek_origin_start) {
            whence = SEEK_SET;
        } else if (origin == ma_seek_origin_end) {
            whence = SEEK_END;
        } else {
            whence = SEEK_CUR;
        }

        auto asset = static_cast<AAsset *>(file);
        const auto result = AAsset_seek(asset, offset, whence);

        if (result == -1) {
            return MA_ERROR;
        }

        return MA_SUCCESS;
    }

    ma_vfs_callbacks s_vfs = {
            on_open,
            nullptr,
            on_close,
            on_read,
            nullptr,
            on_seek,
            nullptr,
            nullptr
    };

}

oo::decoder::decoder() {
    a_asset_manager_holder holder{};
    s_asset_manager = holder.get();
}

oo::decoder::~decoder() {
    ma_decoder_uninit(&m_decoder);
}

bool oo::decoder::init(const std::string &path) {
    return ma_decoder_init_vfs(&s_vfs, path.c_str(), nullptr, &m_decoder) == MA_SUCCESS;
}

ma_uint64 oo::decoder::read(float *out, ma_uint64 count) {
    return ma_decoder_read_pcm_frames(&m_decoder, out, count);
}

bool oo::decoder::seek(ma_uint64 frame) {
    return ma_decoder_seek_to_pcm_frame(&m_decoder, frame) == MA_SUCCESS;
}

ma_format oo::decoder::get_output_format() const {
    return m_decoder.outputFormat;
}

ma_uint32 oo::decoder::get_output_channels() const {
    return m_decoder.outputChannels;
}

ma_uint32 oo::decoder::get_output_sample_rate() const {
    return m_decoder.outputSampleRate;
}
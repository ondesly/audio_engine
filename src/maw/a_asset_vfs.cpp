//
//  a_asset_vfs.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <cstdio>

#include <android/asset_manager.h>
#include <miniaudio.h>

#include "maw/a_asset_vfs.h"

namespace {

    AAssetManager *s_asset_manager = nullptr;

}

oo::a_asset_vfs::a_asset_vfs(AAssetManager *asset_manager) {
    s_asset_manager = asset_manager;
}

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

    if (result != size) {
        if (result == 0) {
            return MA_AT_END;
        } else {
            return MA_ERROR;
        }
    }

    return MA_SUCCESS;
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

ma_vfs *
oo::a_asset_vfs::create_vfs() const { // NOLINT(readability-convert-member-functions-to-static)
    return new ma_vfs_callbacks{
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

void oo::a_asset_vfs::delete_vfs(ma_vfs *vfs) const { // NOLINT(readability-convert-member-functions-to-static)
    auto callbacks = static_cast<ma_vfs_callbacks *>(vfs);
    delete callbacks;
}
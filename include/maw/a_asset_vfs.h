//
//  a_asset_vfs.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

typedef struct AAssetManager AAssetManager;
typedef void ma_vfs;

namespace oo {

    class a_asset_vfs {
    public:

        a_asset_vfs() = default;

        explicit a_asset_vfs(AAssetManager *asset_manager);

    public:

        [[nodiscard]] ma_vfs *create_vfs() const;

        void delete_vfs(ma_vfs *) const;

    };

}
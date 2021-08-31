//
//  a_asset_manager_holder.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 27.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

typedef struct AAssetManager AAssetManager;

namespace maw {

    class a_asset_manager_holder {
    public:

        void set(AAssetManager *asset_manager) {
            s_asset_manager = asset_manager;
        }

        AAssetManager *get() {
            return s_asset_manager;
        }

    private:

        inline static AAssetManager *s_asset_manager = nullptr;

    };

}
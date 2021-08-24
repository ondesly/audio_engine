//
//  MainActivity.java
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

package com.github.ondesly.maw.test;

import android.content.res.AssetManager;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.github.ondesly.maw.test.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("test_android");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        setAssetManager(getAssets());
        play("sample.mp3");
    }

    private native void play(String path);

    private native void setAssetManager(AssetManager assetManager);

}
//
//  MainActivity.java
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

package com.github.ondesly.audio_engine.test;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.github.ondesly.audio_engine.test.databinding.ActivityMainBinding;

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

        findViewById(R.id.button_play).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                play("sample4.mp3");
            }

        });

        findViewById(R.id.button_stop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                stop("sample4.mp3");
            }

        });

        findViewById(R.id.button_stop_all).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                stopAll();
            }

        });

        findViewById(R.id.button_term).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                term();
            }

        });

        // AAudio

        findViewById(R.id.button_aa_open).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                aaOpen();
            }

        });

        findViewById(R.id.button_aa_start).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                aaStart();
            }

        });

        findViewById(R.id.button_aa_stop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                aaStop();
            }

        });

        findViewById(R.id.button_aa_close).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                aaClose();
            }

        });

        // Miniaudio

        findViewById(R.id.button_mini_init).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                miniInit();
            }

        });

        findViewById(R.id.button_mini_start).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                miniStart();
            }

        });

        findViewById(R.id.button_mini_stop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                miniStop();
            }

        });

        findViewById(R.id.button_mini_uninit).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                miniUninit();
            }

        });

        // Miniaudio Async

        findViewById(R.id.button_async_init).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                asyncInit();
            }

        });

        findViewById(R.id.button_async_start).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                asyncStart();
            }

        });

        findViewById(R.id.button_async_stop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                asyncStop();
            }

        });

        findViewById(R.id.button_async_uninit).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                asyncUninit();
            }

        });

    }

    private native void setAssetManager(AssetManager assetManager);

    // Audio engine

    private native void play(String path);

    private native void stop(String path);

    private native void stopAll();

    private native void term();

    // AAudio

    private native void aaOpen();

    private native void aaStart();

    private native void aaStop();

    private native void aaClose();

    // Miniaudio

    private native void miniInit();

    private native void miniStart();

    private native void miniStop();

    private native void miniUninit();

    // Miniaudio Async

    private native void asyncInit();

    private native void asyncStart();

    private native void asyncStop();

    private native void asyncUninit();

}
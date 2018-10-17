package com.example.a83849.player;

/**
 * Created by 83849 on 2017/4/4.
 */

import android.content.Intent;
import android.media.MediaPlayer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.VideoView;

public class PlayerActivity extends AppCompatActivity {

    private VideoView video;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        Intent intent=getIntent();
        String path=intent.getStringExtra("path");

        video=(VideoView)findViewById(R.id.videoPlayer);
        video.setMediaController(new android.widget.MediaController(this));
        video.setVideoPath(path);
        video.requestFocus();
        try{
            video.start();
        }catch(Exception e){
            e.printStackTrace();
        }
        video.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                finish();
            }
        });
    }
}

//import android.app.Activity;
//import android.content.Intent;
//import android.content.pm.ActivityInfo;
//import android.os.Bundle;
//import android.util.Log;
//import android.view.SurfaceHolder;
//import android.view.SurfaceView;
//import android.view.Window;
//import android.view.View;
//import android.widget.ImageButton;
//import android.widget.SeekBar;
//import android.widget.TextView;
//
//public class PlayerActivity extends Activity {
//    private SurfaceView surfaceView;
//    private Player player;
//    private SeekBar seekBar;
//    private SurfaceHolder mSurfaceHolder01;
//
//
//    private ImageButton mPlay;
//    private ImageButton mPause;
//    private ImageButton mStop;
//
//
//    private boolean bIsPaused = false;
//    private boolean bIsReleased = false;
//    boolean start=true;
//
//    private TextView mTextView01;
//    private static final String TAG = "HIPPO_MediaPlayer"; //打印日志的标志
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        requestWindowFeature(Window.FEATURE_NO_TITLE);
//
//        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
//        setContentView(R.layout.activity_player);
//
//        mTextView01 = (TextView) findViewById(R.id.myTextView1);
//        mPlay = (ImageButton) findViewById(R.id.play);
//        mPause = (ImageButton) findViewById(R.id.pause);
////        mReset = (ImageButton) findViewById(R.id.reset);
//        mStop = (ImageButton) findViewById(R.id.stop);
//
//        Intent intent = getIntent();
//        String path = intent.getStringExtra("path");
//        Log.d("PlayerActivity", " url:" + path);
//
//        surfaceView = (SurfaceView) this.findViewById(R.id.surfaceView1);
//        mSurfaceHolder01 = surfaceView.getHolder();
//        mSurfaceHolder01.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
//
//        seekBar = (SeekBar) this.findViewById(R.id.seekBar);
//        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
//            int progress;
//            @Override
//            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
//                this.progress = progress * player.mediaPlayer.getDuration()
//                        / seekBar.getMax();
//            }
//
//            @Override
//            public void onStartTrackingTouch(SeekBar seekBar) {
//
//            }
//
//            @Override
//            public void onStopTrackingTouch(SeekBar seekBar) {
//                player.mediaPlayer.seekTo(progress);
//            }
//        });
//
//        player = new Player(surfaceView, seekBar, path);
//
//        mPlay.setOnClickListener(new ImageButton.OnClickListener() {
//
//            public void onClick(View view) {
//                if(bIsPaused||start) {
//                    player.play();
//                    mTextView01.setText(R.string.str_play);
//                    bIsPaused=false;
//                }
//                start=false;
//            }
//        });
//
//
//        mPause.setOnClickListener(new ImageButton.OnClickListener() {
//            public void onClick(View view) {
//                if (bIsReleased == false) {
//                    if (bIsPaused == false) {
//                        player.pause();
//                        bIsPaused = true;
//                        mTextView01.setText(R.string.str_pause);
//                    } else if (bIsPaused == true) {
//                        player.play();
//                        bIsPaused = false;
//                        mTextView01.setText(R.string.str_play);
//                    }
//                }
//            }
//        });
//
////        mReset.setOnClickListener(new ImageButton.OnClickListener()
////        {
////            public void onClick(View view)
////            {
////                if(checkSDCard())
////                {
////                    if(bIsReleased == false)
////                    {
////                        if (mMediaPlayer01 != null)
////                        {
////                            mMediaPlayer01.seekTo(0);
////                        }
////                    }
////                }
////            }
////        });
//
//
//        mStop.setOnClickListener(new ImageButton.OnClickListener() {
//            public void onClick(View view) {
//                if (bIsReleased == false) {
//                    player.stop();
//                    bIsReleased = true;
//                    mTextView01.setText(R.string.str_stop);
//                    start=true;
//                }
//            }
//
//        });
//    }
//
//
//    @Override
//    protected void onResume() {
//        if (getRequestedOrientation() != ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) {
//            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
//        }
//        super.onResume();
//    }
//    }
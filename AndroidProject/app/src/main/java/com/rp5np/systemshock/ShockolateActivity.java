package com.rp5np.systemshock;

import android.app.KeyguardManager;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;

import org.libsdl.app.SDLActivity;

public class ShockolateActivity extends SDLActivity {
    private static final String TAG = "RP5NP";

    private void lockOrientation(String phase) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        Log.i(TAG, "JAVA_ORIENTATION_LOCK=LANDSCAPE phase=" + phase);
    }

    @Override public void setOrientationBis(int w, int h, boolean resizable, String hint) {
        lockOrientation("sdl");
        Log.i(TAG, "JAVA_SDL_ORIENTATION_OVERRIDE width=" + w + " height=" + h +
                " resizable=" + resizable + " hint=" + hint);
    }

    private void applyWindowPolicy() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON | WindowManager.LayoutParams.FLAG_FULLSCREEN);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            getWindow().setDecorFitsSystemWindows(false);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowInsetsController c = getWindow().getInsetsController();
            if (c != null) {
                c.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
                c.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
            }
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            );
        }
    }

    private void applyKeyguardPolicy() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
            setShowWhenLocked(true);
            setTurnScreenOn(true);
        }
        KeyguardManager km = (KeyguardManager)getSystemService(Context.KEYGUARD_SERVICE);
        if (km == null) return;
        Log.i(TAG, "JAVA_KEYGUARD_LOCKED=" + km.isKeyguardLocked());
        if (km.isKeyguardLocked() && Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            km.requestDismissKeyguard(this, null);
        }
    }

    @Override protected void onCreate(Bundle state) {
        Log.i(TAG, "JAVA_ONCREATE_BEGIN");
        lockOrientation("onCreate-pre-super");
        super.onCreate(state);
        lockOrientation("onCreate-post-super");
        applyWindowPolicy();
        applyKeyguardPolicy();
        Log.i(TAG, "JAVA_ONCREATE_END");
    }

    @Override protected void onResume() {
        super.onResume();
        lockOrientation("onResume");
        applyWindowPolicy();
        applyKeyguardPolicy();
        Log.i(TAG, "JAVA_ONRESUME");
    }

    @Override public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Log.i(TAG, "JAVA_WINDOW_FOCUS=" + hasFocus);
        if (hasFocus) applyWindowPolicy();
    }

    @Override protected String[] getArguments() {
        return new String[] { "-nosplash" };
    }
}
package com.zego.flutter.example;

import com.tencent.bugly.crashreport.CrashReport;

import io.flutter.app.FlutterApplication;

public class MyApplication extends FlutterApplication {

    final static private String BUGLY_APP_KEY = "156048a981";

    @Override
    public void onCreate() {
        super.onCreate();

        setupBugly();
    }

    private void setupBugly() {
        CrashReport.initCrashReport(this, BUGLY_APP_KEY, true);
    }
}
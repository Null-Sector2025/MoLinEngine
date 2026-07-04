#!/bin/bash
echo "========================================"
echo " 墨麟引擎 Android APK 构建脚本"
echo "========================================"

ENGINE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
LIBS_DIR="$ENGINE_DIR/libs/android"
BUILD_DIR="$ENGINE_DIR/build-android-apk"

# 创建 Android 项目结构
mkdir -p "$BUILD_DIR/app/src/main/java/com/molin/engine"
mkdir -p "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a"
mkdir -p "$BUILD_DIR/app/src/main/assets"
mkdir -p "$BUILD_DIR/app/src/main/res/values"
mkdir -p "$BUILD_DIR/gradle/wrapper"

# 复制预编译的 .so 文件
cp "$LIBS_DIR/arm64-v8a/libMoLinEngine.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"
cp "$LIBS_DIR/arm64-v8a/libSDL2.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"
cp "$LIBS_DIR/arm64-v8a/libSDL2_image.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"
cp "$LIBS_DIR/arm64-v8a/libSDL2_mixer.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"
cp "$LIBS_DIR/arm64-v8a/libSDL2_ttf.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"
cp "$LIBS_DIR/arm64-v8a/liblua.so" "$BUILD_DIR/app/src/main/jniLibs/arm64-v8a/"

# 创建 build.gradle
cat > "$BUILD_DIR/build.gradle" << 'GRADLE'
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:8.2.0'
    }
}
GRADLE

# 创建 settings.gradle
cat > "$BUILD_DIR/settings.gradle" << 'GRADLE'
rootProject.name = "MoLinEngine"
include ':app'
GRADLE

# 创建 app/build.gradle
cat > "$BUILD_DIR/app/build.gradle" << 'GRADLE'
plugins {
    id 'com.android.application'
}

android {
    namespace 'com.molin.engine'
    compileSdk 34

    defaultConfig {
        applicationId "com.molin.engine"
        minSdk 21
        targetSdk 34
        versionCode 1
        versionName "1.0"
        ndk { abiFilters 'arm64-v8a' }
    }

    buildTypes {
        release {
            minifyEnabled false
        }
    }

    sourceSets {
        main {
            jniLibs.srcDirs = ['src/main/jniLibs']
            assets.srcDirs = ['src/main/assets']
        }
    }
}

dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
}
GRADLE

# 创建 AndroidManifest.xml
cat > "$BUILD_DIR/app/src/main/AndroidManifest.xml" << 'XML'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <uses-feature android:glEsVersion="0x00020000" />
    <uses-permission android:name="android.permission.INTERNET" />

    <application
        android:allowBackup="true"
        android:label="墨麟引擎"
        android:theme="@android:style/Theme.NoTitleBar.Fullscreen">
        <activity
            android:name="com.molin.engine.MainActivity"
            android:configChanges="orientation|screenSize|keyboardHidden"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
XML

# 创建 MainActivity.java
cat > "$BUILD_DIR/app/src/main/java/com/molin/engine/MainActivity.java" << 'JAVA'
package com.molin.engine;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class MainActivity extends Activity {
    static { System.loadLibrary("MoLinEngine"); }

    private native void nativeInit(String assetsPath);
    private native void nativeRun();
    private native void nativeDestroy();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        nativeInit(getFilesDir().getAbsolutePath());
        nativeRun();
    }

    @Override
    protected void onDestroy() {
        nativeDestroy();
        super.onDestroy();
    }
}
JAVA

# 创建 strings.xml
cat > "$BUILD_DIR/app/src/main/res/values/strings.xml" << 'XML'
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">墨麟引擎</string>
</resources>
XML

# 创建 gradle.properties
cat > "$BUILD_DIR/gradle.properties" << 'PROPS'
android.useAndroidX=true
org.gradle.jvmargs=-Xmx2048m
PROPS

echo ""
echo "Android 项目已生成: $BUILD_DIR"
echo "在 Android Studio 中打开此目录，或使用命令行构建:"
echo "  cd $BUILD_DIR && ./gradlew assembleRelease"
echo ""
echo "APK 输出路径: $BUILD_DIR/app/build/outputs/apk/release/app-release.apk"

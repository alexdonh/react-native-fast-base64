package com.fastbase64;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

@ReactModule(name = FastBase64Module.NAME)
public class FastBase64Module extends ReactContextBaseJavaModule {

  public FastBase64Module(ReactApplicationContext reactContext) {
    super(reactContext);
  }
  public static final String NAME = "FastBase64";

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    try {
      // Used to load the 'native-lib' library on application startup.
      System.loadLibrary("fastbase64");
    } catch (Exception ignored) {
    }
  }

  private native void nativeInstall(long jsiPtr);

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void install() {
    JavaScriptContextHolder reactContext = getReactApplicationContext().getJavaScriptContextHolder();
    //if (reactContext.get() != 0) {
    try {
      this.nativeInstall(reactContext.get());
    //} else {
    } catch (Exception exception) {
      Log.e("FastBase64Module", "JSI Runtime is not available in debug mode");
    }
  }
}

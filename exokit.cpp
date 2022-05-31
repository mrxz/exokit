#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <thread>
#include <functional>

#include <v8.h>
#include <bindings.h>

#ifdef OPENVR
#include <openvr-bindings.h>
#endif
#ifdef ANDROID
#include <oculus-mobile.h>
#endif

#ifdef OCULUSVR
#include <oculus-bindings.h>
#endif

using namespace v8;

namespace exokit {

void InitExports(Local<Object> exports, Local<Context> context) {
  std::pair<Local<Value>, Local<FunctionTemplate>> glResult = makeGl();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeGl").ToLocalChecked(), glResult.first);

  std::pair<Local<Value>, Local<FunctionTemplate>> gl2Result = makeGl2(glResult.second);
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeGl2").ToLocalChecked(), gl2Result.first);

  Local<Value> image = makeImage();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeImage").ToLocalChecked(), image);

  Local<Value> imageData = makeImageData();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeImageData").ToLocalChecked(), imageData);

  Local<Value> imageBitmap = makeImageBitmap();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeImageBitmap").ToLocalChecked(), imageBitmap);

  Local<Value> path2d = makePath2D();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativePath2D").ToLocalChecked(), path2d);

  Local<Value> canvasGradient = makeCanvasGradient();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeCanvasGradient").ToLocalChecked(), canvasGradient);

  Local<Value> canvasPattern = makeCanvasPattern();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeCanvasPattern").ToLocalChecked(), canvasPattern);

  Local<Value> canvas = makeCanvasRenderingContext2D(imageData, canvasGradient, canvasPattern);
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeCanvasRenderingContext2D").ToLocalChecked(), canvas);

  Local<Value> audio = makeAudio();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeAudio").ToLocalChecked(), audio);

  Local<Value> video = makeVideo(imageData);
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeVideo").ToLocalChecked(), video);

#if defined(ANDROID) || defined(LUMIN)
  Local<Value> browser = makeBrowser();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeBrowser").ToLocalChecked(), browser);
#endif

  Local<Value> rtc = makeRtc();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeRtc").ToLocalChecked(), rtc);

  /* Local<Value> glfw = makeGlfw();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeGlfw").ToLocalChecked(), glfw); */

  Local<Value> window = makeWindow();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeWindow").ToLocalChecked(), window);

#ifdef OPENVR
  Local<Value> vr = makeOpenVR();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeOpenVR").ToLocalChecked(), vr);
#endif

#ifdef OCULUSVR
  Local<Value> oculusVR = makeOculusVR();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeOculusVR").ToLocalChecked(), oculusVR);
#endif

#if LEAPMOTION
  Local<Value> lm = makeLm();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeLm").ToLocalChecked(), lm);
#endif

#ifdef ANDROID
  Local<Value> oculusMobileVr = makeOculusMobileVr();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeOculusMobileVr").ToLocalChecked(), oculusMobileVr);
#endif

#if defined(LUMIN)
  Local<Value> ml = makeMl();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeMl").ToLocalChecked(), ml);
#endif

#if defined(ANDROID)
#define NATIVE_PLATFORM "android"
#elif defined(LUMIN)
#define NATIVE_PLATFORM "lumin"
#else
#define NATIVE_PLATFORM ""
#endif
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativePlatform").ToLocalChecked(), JS_STR(NATIVE_PLATFORM));

  Local<Value> console = makeConsole();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeConsole").ToLocalChecked(), console);

  Local<Value> cache = makeCache();
  exports->Set(context, v8::String::NewFromUtf8(Isolate::GetCurrent(), "nativeCache").ToLocalChecked(), cache);

  /* uintptr_t initFunctionAddress = (uintptr_t)InitExports;
  Local<Array> initFunctionAddressArray = Nan::New<Array>(2);
  initFunctionAddressArray->Set(0, Nan::New<Integer>((uint32_t)(initFunctionAddress >> 32)));
  initFunctionAddressArray->Set(1, Nan::New<Integer>((uint32_t)(initFunctionAddress & 0xFFFFFFFF)));
  exports->Set(JS_STR("initFunctionAddress"), initFunctionAddressArray); */
}

void Init(Local<Object> exports, Local<Context> context) {
  InitExports(exports, context);
}

}

#if !defined(ANDROID) && !defined(LUMIN)
NODE_MODULE_INIT(/* exports, module, context */) {
  exokit::Init(exports, context);
}
#else
extern "C" {
  void node_register_module_exokit(Local<Object> exports, Local<Value> module, Local<Context> context) {
    exokit::Init(exports, context);
  }
}
#endif

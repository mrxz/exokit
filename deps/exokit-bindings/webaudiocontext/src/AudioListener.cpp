#include <AudioListener.h>

namespace webaudio {

AudioListener::AudioListener() {}

AudioListener::~AudioListener() {}

Local<Object> AudioListener::Initialize(Isolate *isolate, Local<Value> fakeAudioParamCons) {
  Nan::EscapableHandleScope scope;

  // constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(JS_STR("AudioListener"));

  // prototype
  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  Nan::SetMethod(proto, "setPosition", SetPosition);
  Nan::SetMethod(proto, "setOrientation", SetOrientation);

  Local<Function> ctorFn = Nan::GetFunction(ctor).ToLocalChecked();
  
  ctorFn->Set(Nan::GetCurrentContext(), JS_STR("FakeAudioParam"), fakeAudioParamCons);

  return scope.Escape(ctorFn);
}

NAN_METHOD(AudioListener::New) {
  Nan::HandleScope scope;

  if (info[0]->IsObject() && JS_OBJ(JS_OBJ(info[0])->Get(Nan::GetCurrentContext(), JS_STR("constructor")).ToLocalChecked())->Get(Nan::GetCurrentContext(), JS_STR("name")).ToLocalChecked()->StrictEquals(JS_STR("AudioContext"))) {
    Local<Object> audioContextObj = Local<Object>::Cast(info[0]);
    AudioContext *audioContext = ObjectWrap::Unwrap<AudioContext>(audioContextObj);

    AudioListener *audioListener = new AudioListener();
    Local<Object> audioListenerObj = info.This();
    audioListener->Wrap(audioListenerObj);
    
    lab::AudioListener *labAudioListener = &audioContext->audioContext->listener();
    audioListener->audioListener = labAudioListener;
    
    Local<Function> fakeAudioParamConstructor = Local<Function>::Cast(JS_OBJ(audioListenerObj->Get(Nan::GetCurrentContext(), JS_STR("constructor")).ToLocalChecked())->Get(Nan::GetCurrentContext(), JS_STR("FakeAudioParam")).ToLocalChecked());

    Local<Object> positionXAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *positionXAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionXAudioParamObj);
    positionXAudioParam->getter = [=]() { return labAudioListener->position().x; };
    positionXAudioParam->setter = [=](float x) { lab::FloatPoint3D position = labAudioListener->position(); position.x = x; labAudioListener->setPosition(position); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("positionX"), positionXAudioParamObj);

    Local<Object> positionYAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *positionYAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionYAudioParamObj);
    positionYAudioParam->getter = [=]() { return labAudioListener->position().y; };
    positionYAudioParam->setter = [=](float y) { lab::FloatPoint3D position = labAudioListener->position(); position.y = y; labAudioListener->setPosition(position); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("positionY"), positionYAudioParamObj);

    Local<Object> positionZAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *positionZAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(positionZAudioParamObj);
    positionZAudioParam->getter = [=]() { return labAudioListener->position().z; };
    positionZAudioParam->setter = [=](float z) { lab::FloatPoint3D position = labAudioListener->position(); position.z = z; labAudioListener->setPosition(position); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("positionZ"), positionZAudioParamObj);

    Local<Object> forwardXAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *forwardXAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(forwardXAudioParamObj);
    forwardXAudioParam->getter = [=]() { return labAudioListener->orientation().x; };
    forwardXAudioParam->setter = [=](float x) { lab::FloatPoint3D orientation = labAudioListener->orientation(); orientation.x = x; labAudioListener->setOrientation(orientation); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("forwardX"), forwardXAudioParamObj);

    Local<Object> forwardYAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *forwardYAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(forwardYAudioParamObj);
    forwardYAudioParam->getter = [=]() { return labAudioListener->orientation().y; };
    forwardYAudioParam->setter = [=](float y) { lab::FloatPoint3D orientation = labAudioListener->orientation(); orientation.y = y; labAudioListener->setOrientation(orientation); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("forwardY"), forwardYAudioParamObj);

    Local<Object> forwardZAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *forwardZAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(forwardZAudioParamObj);
    forwardZAudioParam->getter = [=]() { return labAudioListener->orientation().z; };
    forwardZAudioParam->setter = [=](float z) { lab::FloatPoint3D orientation = labAudioListener->orientation(); orientation.z = z; labAudioListener->setOrientation(orientation); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("forwardZ"), forwardZAudioParamObj);

    Local<Object> upXAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *upXAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(upXAudioParamObj);
    upXAudioParam->getter = [=]() { return labAudioListener->upVector().x; };
    upXAudioParam->setter = [=](float x) { lab::FloatPoint3D up = labAudioListener->upVector(); up.x = x; labAudioListener->setUpVector(up); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("upX"), upXAudioParamObj);

    Local<Object> upYAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *upYAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(upYAudioParamObj);
    upYAudioParam->getter = [=]() { return labAudioListener->upVector().y; };
    upYAudioParam->setter = [=](float y) { lab::FloatPoint3D up = labAudioListener->upVector(); up.y = y; labAudioListener->setUpVector(up); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("upY"), upYAudioParamObj);

    Local<Object> upZAudioParamObj = fakeAudioParamConstructor->NewInstance(Isolate::GetCurrent()->GetCurrentContext(), 0, nullptr).ToLocalChecked();
    FakeAudioParam *upZAudioParam = ObjectWrap::Unwrap<FakeAudioParam>(upZAudioParamObj);
    upZAudioParam->getter = [=]() { return labAudioListener->upVector().z; };
    upZAudioParam->setter = [=](float z) { lab::FloatPoint3D up = labAudioListener->upVector(); up.z = z; labAudioListener->setUpVector(up); };
    audioListenerObj->Set(Nan::GetCurrentContext(), JS_STR("upZ"), upZAudioParamObj);

    info.GetReturnValue().Set(audioListenerObj);
  } else {
    Nan::ThrowError("invalid arguments");
  }
}

NAN_METHOD(AudioListener::SetPosition) {
  Nan::HandleScope scope;

  if (info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsNumber()) {
    AudioListener *audioListener = ObjectWrap::Unwrap<AudioListener>(info.This());

    float x = TO_FLOAT(info[0]);
    float y = TO_FLOAT(info[1]);
    float z = TO_FLOAT(info[2]);

    audioListener->audioListener->setPosition(lab::FloatPoint3D{x, y, z});
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatFrequencyData: invalid arguments");
  }
}

NAN_METHOD(AudioListener::SetOrientation) {
  Nan::HandleScope scope;

  if (info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsNumber()) {
    AudioListener *audioListener = ObjectWrap::Unwrap<AudioListener>(info.This());

    float x = TO_FLOAT(info[0]);
    float y = TO_FLOAT(info[1]);
    float z = TO_FLOAT(info[2]);

    audioListener->audioListener->setOrientation(lab::FloatPoint3D{x, y, z});
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatFrequencyData: invalid arguments");
  }
}

}

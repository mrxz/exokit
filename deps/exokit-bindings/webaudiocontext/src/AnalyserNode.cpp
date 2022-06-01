#include <AnalyserNode.h>
#include <AudioContext.h>

namespace webaudio {

AnalyserNode::AnalyserNode() {}

AnalyserNode::~AnalyserNode() {}

Local<Object> AnalyserNode::Initialize(Isolate *isolate) {
  Nan::EscapableHandleScope scope;

  // constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(JS_STR("AnalyserNode"));

  // prototype
  Local<ObjectTemplate> proto = ctor->PrototypeTemplate();
  AudioNode::InitializePrototype(proto);
  AnalyserNode::InitializePrototype(proto);

  Local<Function> ctorFn = Nan::GetFunction(ctor).ToLocalChecked();

  return scope.Escape(ctorFn);
}

void AnalyserNode::InitializePrototype(Local<ObjectTemplate> proto) {
  Nan::SetAccessor(proto, JS_STR("fftSize"), FftSizeGetter, FftSizeSetter);
  Nan::SetAccessor(proto, JS_STR("frequencyBinCount"), FrequencyBinCountGetter);
  Nan::SetAccessor(proto, JS_STR("minDecibels"), MinDecibelsGetter);
  Nan::SetAccessor(proto, JS_STR("maxDecibels"), MaxDecibelsGetter);
  Nan::SetAccessor(proto, JS_STR("smoothingTimeConstant"), SmoothingTimeConstantGetter);
  Nan::SetMethod(proto, "getFloatFrequencyData", GetFloatFrequencyData);
  Nan::SetMethod(proto, "getByteFrequencyData", GetByteFrequencyData);
  Nan::SetMethod(proto, "getFloatTimeDomainData", GetFloatTimeDomainData);
  Nan::SetMethod(proto, "getByteTimeDomainData", GetByteTimeDomainData);
}

NAN_METHOD(AnalyserNode::New) {
  Nan::HandleScope scope;

  Local<Context> context = Nan::GetCurrentContext();
  if (info[0]->IsObject() && JS_OBJ(JS_OBJ(info[0])->Get(context, JS_STR("constructor")).ToLocalChecked())->Get(context, JS_STR("name")).ToLocalChecked()->StrictEquals(JS_STR("AudioContext"))) {
    Local<Object> audioContextObj = Local<Object>::Cast(info[0]);

    AnalyserNode *analyserNode = new AnalyserNode();
    Local<Object> analyserNodeObj = info.This();
    analyserNode->Wrap(analyserNodeObj);

    analyserNode->context.Reset(audioContextObj);
    analyserNode->audioNode = make_shared<lab::AnalyserNode>(2048);

    info.GetReturnValue().Set(analyserNodeObj);
  } else {
    Nan::ThrowError("invalid arguments");
  }
}

NAN_GETTER(AnalyserNode::FftSizeGetter) {
  Nan::HandleScope scope;

  AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
  shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);

  unsigned int fftSize = labAnalyserNode->fftSize();

  info.GetReturnValue().Set(JS_INT(fftSize));
}

NAN_SETTER(AnalyserNode::FftSizeSetter) {
  Nan::HandleScope scope;

  if (value->IsNumber()) {
    AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
    shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);
    Local<Object> audioContextObj = Nan::New(analyserNode->context);
    AudioContext *audioContext = ObjectWrap::Unwrap<AudioContext>(audioContextObj);
    {
      lab::ContextRenderLock lock(audioContext->audioContext.get(), "AnalyserNode::FftSizeSetter");
      labAnalyserNode->setFFTSize(lock, TO_UINT32(value));
    }
  } else {
    Nan::ThrowError("value: invalid arguments");
  }
}

NAN_GETTER(AnalyserNode::FrequencyBinCountGetter) {
  Nan::HandleScope scope;

  AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
  shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);

  unsigned int frequencyBinCount = labAnalyserNode->frequencyBinCount();

  info.GetReturnValue().Set(JS_INT(frequencyBinCount));
}

NAN_GETTER(AnalyserNode::MinDecibelsGetter) {
  Nan::HandleScope scope;

  AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
  shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);

  double minDecibels = labAnalyserNode->minDecibels();

  info.GetReturnValue().Set(JS_NUM(minDecibels));
}

NAN_GETTER(AnalyserNode::MaxDecibelsGetter) {
  Nan::HandleScope scope;

  AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
  shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);

  double maxDecibels = labAnalyserNode->maxDecibels();

  info.GetReturnValue().Set(JS_NUM(maxDecibels));
}

NAN_GETTER(AnalyserNode::SmoothingTimeConstantGetter) {
  Nan::HandleScope scope;

  AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
  shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);

  double smoothingTimeConstant = labAnalyserNode->smoothingTimeConstant();

  info.GetReturnValue().Set(JS_NUM(smoothingTimeConstant));
}

NAN_METHOD(AnalyserNode::GetFloatFrequencyData) {
  Nan::HandleScope scope;

  if (info[0]->IsFloat32Array()) {
    AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
    shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);
    
    Local<Float32Array> arg = Local<Float32Array>::Cast(info[0]);
    Local<ArrayBuffer> arrayBuffer = arg->Buffer();

    vector<float> buffer(arg->Length());
    labAnalyserNode->getFloatFrequencyData(buffer);

    memcpy((unsigned char *)arrayBuffer->GetContents().Data() + arg->ByteOffset(), buffer.data(), arg->Length());
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatFrequencyData: invalid arguments");
  }
}

NAN_METHOD(AnalyserNode::GetByteFrequencyData) {
  Nan::HandleScope scope;

  if (info[0]->IsUint8Array()) {
    AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
    shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);
    
    Local<Uint8Array> arg = Local<Uint8Array>::Cast(info[0]);
    Local<ArrayBuffer> arrayBuffer = arg->Buffer();

    vector<uint8_t> buffer(arg->Length());
    labAnalyserNode->getByteFrequencyData(buffer);

    memcpy((unsigned char *)arrayBuffer->GetContents().Data() + arg->ByteOffset(), buffer.data(), arg->Length());
  } else {
    Nan::ThrowError("AnalyserNode::GetByteFrequencyData: invalid arguments");
  }
}

NAN_METHOD(AnalyserNode::GetFloatTimeDomainData) {
  Nan::HandleScope scope;

  if (info[0]->IsFloat32Array()) {
    AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
    shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);
    
    Local<Float32Array> arg = Local<Float32Array>::Cast(info[0]);
    Local<ArrayBuffer> arrayBuffer = arg->Buffer();

    vector<float> buffer(arg->Length());
    labAnalyserNode->getFloatTimeDomainData(buffer);

    memcpy((unsigned char *)arrayBuffer->GetContents().Data() + arg->ByteOffset(), buffer.data(), arg->Length());
  } else {
    Nan::ThrowError("AnalyserNode::GetFloatTimeDomainData: invalid arguments");
  }
}

NAN_METHOD(AnalyserNode::GetByteTimeDomainData) {
  Nan::HandleScope scope;

  if (info[0]->IsUint8Array()) {
    AnalyserNode *analyserNode = ObjectWrap::Unwrap<AnalyserNode>(info.This());
    shared_ptr<lab::AnalyserNode> labAnalyserNode = *(shared_ptr<lab::AnalyserNode> *)(&analyserNode->audioNode);
    
    Local<Uint8Array> arg = Local<Uint8Array>::Cast(info[0]);
    Local<ArrayBuffer> arrayBuffer = arg->Buffer();

    vector<uint8_t> buffer(arg->Length());
    labAnalyserNode->getByteTimeDomainData(buffer);

    memcpy((unsigned char *)arrayBuffer->GetContents().Data() + arg->ByteOffset(), buffer.data(), arg->Length());
  } else {
    Nan::ThrowError("AnalyserNode::GetByteTimeDomainData: invalid arguments");
  }
}

}

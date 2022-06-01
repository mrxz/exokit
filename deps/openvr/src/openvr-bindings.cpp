#include <ivrsystem.h>
#include <ivrcompositor.h>
#include <openvr-bindings.h>

#include <nan.h>

#include <node.h>
#include <openvr.h>
#include <v8.h>

using namespace v8;

vr::IVRSystem *vrSystem = nullptr;

//=============================================================================
// inline IVRSystem *VR_Init( EVRInitError *peError, EVRApplicationType eApplicationType );
NAN_METHOD(VR_Init)
{
  if (info.Length() != 2)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Argument[0] must be a number (EVRApplicationType).");
    return;
  }

  if (!info[1]->IsString())
  {
    Nan::ThrowTypeError("Argument[1] must be a string.");
    return;
  }

  uint32_t applicationType = TO_UINT32(info[0]);
  // TODO: is there a better way to do this?
  constexpr uint32_t applicationTypeMax = vr::VRApplication_Max;
  if (applicationType >= applicationTypeMax)
  {
    Nan::ThrowTypeError("Argument[0] was out of enum range (EVRApplicationType).");
    return;
  }

  Local<String> actionManifestPath = Local<String>::Cast(info[1]);
  Nan::Utf8String actionManifestPathUtf8String(actionManifestPath);
  const char *actionManifestPathString = *actionManifestPathUtf8String;

  // Perform the actual wrapped call.
  vr::EVRInitError error;
  vrSystem = vr::VR_Init(
    &error,
    static_cast<vr::EVRApplicationType>(applicationType)
  );

  // If the VR system failed to initialize, immediately raise a node exception.
  if (vrSystem == nullptr)
  {
    Local<Value> err = Exception::Error(String::NewFromUtf8(Isolate::GetCurrent(), vr::VR_GetVRInitErrorAsEnglishDescription(error)).ToLocalChecked());
    Local<Object>::Cast(err)->Set(Nan::GetCurrentContext(), String::NewFromUtf8(Isolate::GetCurrent(), "code").ToLocalChecked(), Number::New(Isolate::GetCurrent(), error));
    Nan::ThrowError(err);
    return;
  }

  {
    vr::EVRInputError result = vr::VRInput()->SetActionManifestPath(actionManifestPathString);
    if (result != vr::EVRInputError::VRInputError_None) {
      Local<Value> err = Exception::Error(String::NewFromUtf8(Isolate::GetCurrent(), "Failed to initialize action manifest path").ToLocalChecked());
      Nan::ThrowError(err);
      return;
    }
  }

  // Wrap the resulting system in the correct wrapper and return it.
  auto result = IVRSystem::NewInstance(vrSystem);
  info.GetReturnValue().Set(result);
}

//=============================================================================
// inline IVRSystem *VR_Init( EVRInitError *peError, EVRApplicationType eApplicationType );
NAN_METHOD(VR_Shutdown)
{
  if (info.Length() != 0)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  vr::VR_Shutdown();
}

//=============================================================================
/// VR_INTERFACE bool VR_CALLTYPE VR_IsHmdPresent();
NAN_METHOD(VR_IsHmdPresent)
{
  if (info.Length() != 0)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  const auto result = vr::VR_IsHmdPresent();
  info.GetReturnValue().Set(Nan::New<Boolean>(result));
}

//=============================================================================
/// VR_INTERFACE bool VR_CALLTYPE VR_IsRuntimeInstalled();
NAN_METHOD(VR_IsRuntimeInstalled)
{
  if (info.Length() != 0)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  const auto result = vr::VR_IsRuntimeInstalled();
  info.GetReturnValue().Set(Nan::New<Boolean>(result));
}

/* //=============================================================================
/// VR_INTERFACE const char *VR_CALLTYPE VR_RuntimePath();
NAN_METHOD(VR_RuntimePath)
{
  if (info.Length() != 0)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  const char *result = vr::VR_RuntimePath();
  info.GetReturnValue().Set(Nan::New<String>(result).ToLocalChecked());
} */

//=============================================================================
/// VR_INTERFACE const char *VR_CALLTYPE VR_GetVRInitErrorAsSymbol( EVRInitError error );
NAN_METHOD(VR_GetVRInitErrorAsSymbol)
{
  if (info.Length() != 1)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Argument[0] must be a number (EVRInitError).");
    return;
  }

  uint32_t nError = TO_UINT32(info[0]);
  vr::EVRInitError eError = static_cast<vr::EVRInitError>(nError);
  const char *result = vr::VR_GetVRInitErrorAsSymbol(eError);
  info.GetReturnValue().Set(Nan::New<String>(result).ToLocalChecked());
}

//=============================================================================
/// VR_INTERFACE const char *VR_CALLTYPE VR_GetVRInitErrorAsEnglishDescription( EVRInitError error );
NAN_METHOD(VR_GetVRInitErrorAsEnglishDescription)
{
  if (info.Length() != 1)
  {
    Nan::ThrowError("Wrong number of arguments.");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Argument[0] must be a number (EVRInitError).");
    return;
  }

  uint32_t nError = TO_UINT32(info[0]);
  vr::EVRInitError eError = static_cast<vr::EVRInitError>(nError);
  const auto result = vr::VR_GetVRInitErrorAsEnglishDescription(eError);
  info.GetReturnValue().Set(Nan::New<String>(result).ToLocalChecked());
}

//=============================================================================
/// VR_INTERFACE uint32_t VR_CALLTYPE VR_GetInitToken();
NAN_METHOD(VR_GetInitToken)
{
  if (info.Length() != 0)
  {
    Nan::ThrowTypeError("Wrong number of arguments.");
    return;
  }

  const auto result = vr::VR_GetInitToken();
  info.GetReturnValue().Set(Nan::New<Number>(result));
}

NAN_METHOD(GetGlobalSystem) {
  if (vrSystem != nullptr) {
    auto result = IVRSystem::NewInstance(vrSystem);
    info.GetReturnValue().Set(result);
  } else {
    info.GetReturnValue().Set(Nan::Null());
  }
}

Local<Object> makeOpenVR() {
  v8::EscapableHandleScope scope(Isolate::GetCurrent());

  Local<Object> exports = Object::New(Isolate::GetCurrent());

  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_Init").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_Init)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_Shutdown").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_Shutdown)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_IsHmdPresent").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_IsHmdPresent)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_IsRuntimeInstalled").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_IsRuntimeInstalled)).ToLocalChecked());
  // exports->Set(Nan::GetCurrentContext(), Nan::New("VR_RuntimePath").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_RuntimePath)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_GetVRInitErrorAsSymbol").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsSymbol)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_GetVRInitErrorAsEnglishDescription").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsEnglishDescription)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("VR_GetInitToken").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(VR_GetInitToken)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("NewCompositor").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(NewCompositor)).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New("GetGlobalSystem").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(GetGlobalSystem)).ToLocalChecked());

  return scope.Escape(exports);
}

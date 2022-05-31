#include <defines.h>

Local<Array> pointerToArray(void *ptr) {
  uintptr_t n = (uintptr_t)ptr;
  Local<Array> result = Nan::New<Array>(2);
  Local<Context> context = Nan::GetCurrentContext();
  result->Set(context, 0, JS_NUM((uint32_t)(n >> 32)));
  result->Set(context, 1, JS_NUM((uint32_t)(n & 0xFFFFFFFF)));
  return result;
}

uintptr_t arrayToPointer(Local<Array> array) {
  Local<Context> context = Nan::GetCurrentContext();
  return ((uintptr_t)TO_UINT32(array->Get(context, 0).ToLocalChecked()) << 32) | (uintptr_t)TO_UINT32(array->Get(context, 1).ToLocalChecked());
}

uintptr_t arrayToPointer(Local<Uint32Array> array) {
  Local<Context> context = Nan::GetCurrentContext();
  return ((uintptr_t)TO_UINT32(array->Get(context, 0).ToLocalChecked()) << 32) | (uintptr_t)TO_UINT32(array->Get(context, 1).ToLocalChecked());
}

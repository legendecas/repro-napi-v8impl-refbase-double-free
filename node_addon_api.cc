#include <napi.h>

namespace {
class MyObject : public Napi::ObjectWrap<MyObject> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  MyObject(const Napi::CallbackInfo& info);

  // Force trigger gc on weak references on finalization.
  //
  // Explanation:
  // Napi::Reference (base of Napi::ObjectWrap) calls napi_delete_reference on
  // finalization, which would force v8impl::RefBase::Delete to delete the
  // v8impl::Reference before weak callbacks.
  // Conditions like nyc would load plenty of codes/coverage informations in
  // the instrumenting process, and offloads those data on exit, which
  // may eventually trigger a GC (I didn't figure out what the exact reason the
  // GC was triggered -- it was been triggered anyway, we force the gc here to
  // produce a stable crash, as it's valid to call any javascript here) after
  // addon tear down, may apply on node processes used node-addon-api.
  void Finalize(Napi::Env env) override;
};

void Cleanup(Napi::Env env, void*) {
  env.Global().Get("cleanup").As<Napi::Function>().Call({});
}

Napi::Object MyObject::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env,
                  "MyObject",
                  {});

  exports.Set("MyObject", func);
  return exports;
}

MyObject::MyObject(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<MyObject>(info) {}

void MyObject::Finalize(Napi::Env env) {
  env.Global().Get("cleanup").As<Napi::Function>().Call({});
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return MyObject::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
}

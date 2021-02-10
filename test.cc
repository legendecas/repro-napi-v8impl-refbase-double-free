#include <node_api.h>
#include <assert.h>
#include <stdio.h>

class MyObject {
 public:
  static napi_value Init(napi_env env, napi_value exports){
    napi_status status;

    napi_value cons;
    status = napi_define_class(
        env, "MyObject", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    assert(status == napi_ok);

    status = napi_set_named_property(env, exports, "MyObject", cons);
    assert(status == napi_ok);
    return exports;
  }

  static void Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    delete reinterpret_cast<MyObject*>(nativeObject);
  }

 private:
  explicit MyObject(): env_(nullptr), wrapper_(nullptr) {};
  ~MyObject() {
    napi_delete_reference(env_, wrapper_);
  };

  static napi_value New(napi_env env, napi_callback_info info){
    napi_status status;
    napi_value target;
    size_t argc = 0;
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, nullptr, &jsthis, nullptr);
    assert(status == napi_ok);

    // Invoked as constructor: `new MyObject(...)`
    MyObject* obj = new MyObject();
    obj->env_ = env;
    status = napi_wrap(env,
                      jsthis,
                      reinterpret_cast<void*>(obj),
                      MyObject::Destructor,
                      nullptr,  // finalize_hint
                      &obj->wrapper_);
    assert(status == napi_ok);
    status = napi_reference_ref(env, obj->wrapper_, nullptr);
    assert(status == napi_ok);

    status = napi_set_instance_data(
        env,
        obj,
        [](napi_env env, void* data, void* hint) {},
        nullptr);
    assert(status == napi_ok);

    return jsthis;
  }

  napi_env env_;
  napi_ref wrapper_;
};

napi_value Init(napi_env env, napi_value exports) {
  return MyObject::Init(env, exports);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

#include "react-native-fast-base64.h"
#include "Base64.h"
//#include "TypedArray.h"
//#include <iostream>
#include <jsi/jsi.h>

using namespace facebook;

namespace fastbase64 {
	void install(jsi::Runtime& jsiRuntime) {
    // std::cout << "Initializing react-native-fast-base64" << "\n";

    auto encode = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "encode"),
      2,
      [](jsi::Runtime & runtime, const jsi::Value & thisValue, const jsi::Value * arguments, size_t count) -> jsi::Value {
        std::string ret = Base64::encode(arguments[0].getString(runtime).utf8(runtime));
        return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, ret));
      }
    );

    auto decode = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "decode"),
      1,
      [](jsi::Runtime & runtime, const jsi::Value & thisValue, const jsi::Value * arguments, size_t count) -> jsi::Value {
        std::string ret = Base64::decode(arguments[0].getString(runtime).utf8(runtime));
        return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, ret));
      }
    );

    auto fromArrayBuffer = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "fromArrayBuffer"),
      1,
      [](jsi::Runtime & runtime, const jsi::Value & thisValue, const jsi::Value * arguments, size_t count) -> jsi::Value {
        if (!arguments[0].isObject()) {
          throw jsi::JSError(runtime, "Input must be a typeof ArrayBufferLike");
        }
        auto obj = arguments[0].asObject(runtime);
        if (!obj.isArrayBuffer(runtime)) {
          throw jsi::JSError(runtime, "Input must be a typeof ArrayBufferLike");
        }
        auto buf = obj.getArrayBuffer(runtime);
        std::string str((char*)buf.data(runtime), buf.size(runtime));
        std::string ret = Base64::encode(str);
        return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, ret));
      }
    );

    auto toArrayBuffer = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "toArrayBuffer"),
      1,
      [](jsi::Runtime & runtime, const jsi::Value & thisValue, const jsi::Value * arguments, size_t count) -> jsi::Value {
        std::string ret = Base64::decode(arguments[0].getString(runtime).utf8(runtime));
        jsi::Function arrayBufferCtor = runtime.global().getPropertyAsFunction(runtime, "ArrayBuffer");
        jsi::Object o = arrayBufferCtor.callAsConstructor(runtime, (int)ret.length()).getObject(runtime);
        jsi::ArrayBuffer buf = o.getArrayBuffer(runtime);
        memcpy(buf.data(runtime), ret.c_str(), ret.size());
        return o;
      }
    );

    auto FastBase64 = jsi::Object(jsiRuntime);
    FastBase64.setProperty(jsiRuntime, "encode", std::move(encode));
    FastBase64.setProperty(jsiRuntime, "decode", std::move(decode));
    FastBase64.setProperty(jsiRuntime, "fromArrayBuffer", std::move(fromArrayBuffer));
    FastBase64.setProperty(jsiRuntime, "toArrayBuffer", std::move(toArrayBuffer));

    jsiRuntime.global().setProperty(jsiRuntime, "FastBase64", FastBase64);
  }
}

#include "react-native-fast-base64.h"
#include "Base64.h"
#include <jsi/jsi.h>

using namespace facebook;

namespace fastbase64 {

void install(jsi::Runtime& jsiRuntime) {

  // encode(str, urlSafe=false)
  auto encode = jsi::Function::createFromHostFunction(
    jsiRuntime,
    jsi::PropNameID::forAscii(jsiRuntime, "encode"),
    2,
    [](jsi::Runtime & runtime, const jsi::Value &, const jsi::Value * arguments, size_t count) -> jsi::Value {
      if (count < 1 || !arguments[0].isString()) {
        throw jsi::JSError(runtime, "FastBase64.encode: First argument must be a string");
      }
      std::string input = arguments[0].asString(runtime).utf8(runtime);
      bool urlSafe = (count > 1 && arguments[1].isBool()) ? arguments[1].getBool() : false;
      std::string encoded = Base64::encode(input, urlSafe);
      return jsi::String::createFromUtf8(runtime, encoded);
    }
  );

  // fromArrayBuffer(arrayBuffer, urlSafe=false)
  auto fromArrayBuffer = jsi::Function::createFromHostFunction(
    jsiRuntime,
    jsi::PropNameID::forAscii(jsiRuntime, "fromArrayBuffer"),
    2,
    [](jsi::Runtime & runtime, const jsi::Value &, const jsi::Value * arguments, size_t count) -> jsi::Value {
      if (count < 1 || !arguments[0].isObject()) {
        throw jsi::JSError(runtime, "FastBase64.fromArrayBuffer: First argument must be an ArrayBuffer");
      }
      auto obj = arguments[0].asObject(runtime);
      if (!obj.isArrayBuffer(runtime)) {
        throw jsi::JSError(runtime, "FastBase64.fromArrayBuffer: First argument must be an ArrayBuffer");
      }
      auto buf = obj.getArrayBuffer(runtime);
      bool urlSafe = (count > 1 && arguments[1].isBool()) ? arguments[1].getBool() : false;
      std::string encoded = Base64::encode(
        reinterpret_cast<const uint8_t*>(buf.data(runtime)),
        buf.size(runtime),
        urlSafe
      );
      return jsi::String::createFromUtf8(runtime, encoded);
    }
  );

  // decode(str)
  auto decode = jsi::Function::createFromHostFunction(
    jsiRuntime,
    jsi::PropNameID::forAscii(jsiRuntime, "decode"),
    1,
    [](jsi::Runtime & runtime, const jsi::Value &, const jsi::Value * arguments, size_t count) -> jsi::Value {
      if (count < 1 || !arguments[0].isString()) {
        throw jsi::JSError(runtime, "FastBase64.decode: First argument must be a string");
      }
      std::string input = arguments[0].asString(runtime).utf8(runtime);
      std::string decoded = Base64::decode(input);
      return jsi::String::createFromUtf8(runtime, decoded);
    }
  );

  // toArrayBuffer(str)
  auto toArrayBuffer = jsi::Function::createFromHostFunction(
    jsiRuntime,
    jsi::PropNameID::forAscii(jsiRuntime, "toArrayBuffer"),
    1,
    [](jsi::Runtime & runtime, const jsi::Value &, const jsi::Value * arguments, size_t count) -> jsi::Value {
      if (count < 1 || !arguments[0].isString()) {
        throw jsi::JSError(runtime, "FastBase64.toArrayBuffer: First argument must be a string");
      }
      std::string input = arguments[0].asString(runtime).utf8(runtime);
      std::string decoded = Base64::decode(input);

      // Always return ArrayBuffer (empty if invalid)
      jsi::Function arrayBufferCtor = runtime.global().getPropertyAsFunction(runtime, "ArrayBuffer");
      jsi::Object bufObj = arrayBufferCtor.callAsConstructor(runtime, (int)decoded.size()).getObject(runtime);
      jsi::ArrayBuffer buf = bufObj.getArrayBuffer(runtime);
      if (!decoded.empty()) {
        memcpy(buf.data(runtime), decoded.data(), decoded.size());
      }
      return bufObj;
    }
  );

  // Export to JS global
  auto FastBase64 = jsi::Object(jsiRuntime);
  FastBase64.setProperty(jsiRuntime, "encode", std::move(encode));
  FastBase64.setProperty(jsiRuntime, "fromArrayBuffer", std::move(fromArrayBuffer));
  FastBase64.setProperty(jsiRuntime, "decode", std::move(decode));
  FastBase64.setProperty(jsiRuntime, "toArrayBuffer", std::move(toArrayBuffer));

  jsiRuntime.global().setProperty(jsiRuntime, "FastBase64", FastBase64);
}

} // namespace fastbase64

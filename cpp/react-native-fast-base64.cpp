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
    jsiRuntime.global().setProperty(jsiRuntime, "FastBase64Encode", std::move(encode));

    auto decode = jsi::Function::createFromHostFunction(
      jsiRuntime,
      jsi::PropNameID::forAscii(jsiRuntime, "decode"),
      1,
      [](jsi::Runtime & runtime, const jsi::Value & thisValue, const jsi::Value * arguments, size_t count) -> jsi::Value {
        std::string ret = Base64::decode(arguments[0].getString(runtime).utf8(runtime));
        return jsi::Value(runtime, jsi::String::createFromUtf8(runtime, ret));
      }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "FastBase64Decode", std::move(decode));
  }
}

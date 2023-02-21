#ifndef FASTBASE64_H
#define FASTBASE64_H

namespace facebook {
  namespace jsi {
    class Runtime;
  }
}

namespace fastbase64 {
  void install(facebook::jsi::Runtime& jsiRuntime);
}

#endif /* FASTBASE64_H */

#include "myobject.h"

namespace abc {

napi_value Init(napi_env env, napi_value exports) {
  return MyObject::Init(env, exports);
}

NAPI_MODULE(abc, Init)

}

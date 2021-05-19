#include <napi.h>

#define NAPI_METHOD(name) \
  napi_value name(napi_env env, napi_callback_info info)

#define DECLARE_NAPI_METHOD(name, func) \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

namespace mongoAddon {
napi_value MongoEmbeddedV1CallInvoke(napi_env env, napi_callback_info info);

napi_value Init(napi_env env, napi_value exports);
}

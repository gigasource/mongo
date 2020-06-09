#include "mongo-lib.h"

namespace mongoAddon {

napi_value MongoEmbeddedV1LibInit(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    napi_value instance;
    MongoLib::NewInstance(env, args[0], args[1], &instance);
    return instance;
}

napi_value CreateLibHandler(napi_env env) {
    MongoLib::Init(env);
    napi_value mongo_embedded_v1_lib_init;
    napi_value instance;

    napi_create_object(env, &instance);
    napi_create_function(env, "mongo_embedded_v1_lib_init", NAPI_AUTO_LENGTH, MongoEmbeddedV1LibInit, NULL, &mongo_embedded_v1_lib_init);

    napi_set_named_property(env, instance, "mongo_embedded_v1_lib_init", mongo_embedded_v1_lib_init);

    return instance;
}

}

#include "mongo-instance.h"

namespace mongoAddon {

napi_value MongoEmbeddedV1InstanceCreate(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value args[3];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    napi_value instance;
    MongoInstance::NewInstance(env, args[0], args[1], args[2], &instance);
    return instance;
}

napi_value CreateInstanceHandler(napi_env env) {
    MongoInstance::Init(env);
    napi_value mongo_embedded_v1_instance_create;
    napi_value instance;

    napi_create_object(env, &instance);
    napi_create_function(env, "mongo_embedded_v1_instance_create", NAPI_AUTO_LENGTH, MongoEmbeddedV1InstanceCreate, NULL, &mongo_embedded_v1_instance_create);

    napi_set_named_property(env, instance, "mongo_embedded_v1_instance_create", mongo_embedded_v1_instance_create);

    return instance;
}

}

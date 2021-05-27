#include "mongo-params.h"

namespace mongoAddon {

napi_value MongoEmbeddedV1ParamsCreate(napi_env env, napi_callback_info info) {
    size_t argc = 0;
    napi_get_cb_info(env, info, &argc, nullptr, nullptr, nullptr);
    napi_value instance;
    MongoParams::NewInstance(env, &instance);
    return instance;
}

napi_value SetParamsYamlConfig(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value args[3];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    MongoParams* params;
    napi_unwrap(env, args[0], reinterpret_cast<void**>(&params));

    int _size;
    napi_get_value_int32(env, args[1], &_size);
    _size += 1;
    char *buf;
    buf = (char*)calloc(_size, sizeof(char));
    napi_get_value_string_utf8(env, args[2], buf, _size, nullptr);
    params->SetYamlConfig(buf);
//    free(buf);
    return nullptr;
}

napi_value CreateParamsHandler(napi_env env) {
    MongoParams::Init(env);
    napi_value mongo_embedded_v1_init_params;
    napi_value set_yaml_config;
    napi_value instance;
    napi_create_object(env, &instance);
    napi_create_function(env, "mongo_embedded_v1_init_params", NAPI_AUTO_LENGTH, MongoEmbeddedV1ParamsCreate, NULL, &mongo_embedded_v1_init_params);
    napi_create_function(env, "set_yaml_config", NAPI_AUTO_LENGTH, SetParamsYamlConfig, NULL, &set_yaml_config);

    napi_set_named_property(env, instance, "mongo_embedded_v1_init_params", mongo_embedded_v1_init_params);
    napi_set_named_property(env, instance, "set_yaml_config", set_yaml_config);
    return instance;
}

} // namespace mongoAddon

#include "mongo-client.h"

namespace mongoAddon {
    napi_value MongoEmbeddedV1ClientCreate(napi_env env, napi_callback_info info) {
        size_t argc = 2;
        napi_value args[2];
        napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
        napi_value instance;
        MongoClient::NewInstance(env, args[0], args[1], &instance);
        return instance;
    }

    napi_value CreateClientHandler(napi_env env) {
        MongoClient::Init(env);
        napi_value mongo_embedded_v1_client_create;
        napi_value instance;

        napi_create_object(env, &instance);
        napi_create_function(env, "mongo_embedded_v1_client_create", NAPI_AUTO_LENGTH, MongoEmbeddedV1ClientCreate, NULL, &mongo_embedded_v1_client_create);

        napi_set_named_property(env, instance, "mongo_embedded_v1_client_create", mongo_embedded_v1_client_create);

        return instance;
    }
}

#include <napi.h>

#include "mongo-addon.h"
#include "wrapper.h"

namespace mongoAddon {
    napi_value MongoEmbeddedV1StatusCreate(napi_env env, napi_callback_info info) {
        size_t argc = 0;
        napi_get_cb_info(env, info, &argc, nullptr, nullptr, nullptr);
        napi_value instance;
        MongoStatus::NewInstance(env, &instance);
        return instance;
    }

    napi_value CreateStatusHandler(napi_env env) {
        MongoStatus::Init(env);
        napi_value mongo_embedded_v1_status_create;
        napi_value instance;

        napi_create_object(env, &instance);
        napi_create_function(env, "mongo_embedded_v1_status_create", NAPI_AUTO_LENGTH, MongoEmbeddedV1StatusCreate, NULL, &mongo_embedded_v1_status_create);

        napi_set_named_property(env, instance, "mongo_embedded_v1_status_create", mongo_embedded_v1_status_create);

        return instance;
    }
}

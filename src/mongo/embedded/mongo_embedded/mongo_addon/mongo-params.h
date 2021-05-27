#include <mongo_embedded/mongo_embedded.h>
#include <napi.h>

#include "wrapper.h"
#include "mongo-addon.h"

#ifndef MONGO_PARAMS_H
#define MONGO_PARAMS_H

namespace mongoAddon {

napi_value MongoEmbeddedV1ParamsCreate(napi_env env, napi_callback_info info);
napi_value SetParamsYamlConfig(napi_env env, napi_callback_info info);

napi_value CreateParamsHandler(napi_env env);

}

#endif

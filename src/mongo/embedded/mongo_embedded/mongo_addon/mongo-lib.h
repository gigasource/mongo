#include <mongo_embedded/mongo_embedded.h>
#include <napi.h>

#include "wrapper.h"
#include "mongo-addon.h"

#ifndef MONGO_LIB_H
#define MONGO_LIB_H

namespace mongoAddon {

NAPI_METHOD(MongoEmbeddedV1LibInit); //mongo_embedded_v1_lib_init

napi_value CreateLibHandler(napi_env env);

}

#endif

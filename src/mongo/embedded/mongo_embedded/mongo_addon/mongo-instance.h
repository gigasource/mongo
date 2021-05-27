#include <mongo_embedded/mongo_embedded.h>
#include <napi.h>

#include "wrapper.h"
#include "mongo-addon.h"

#ifndef MONGO_INSTANCE_H
#define MONGO_INSTANCE_H

namespace mongoAddon {

NAPI_METHOD(MongoEmbeddedV1InstanceCreate); //mongo_embedded_v1_instance_create

napi_value CreateInstanceHandler(napi_env env);

}

#endif

#include <mongo_embedded/mongo_embedded.h>
#include <napi.h>

#include "mongo-addon.h"

#ifndef MONGO_STATUS_H
#define MONGO_STATUS_H

namespace mongoAddon {

NAPI_METHOD(MongoEmbeddedV1StatusCreate);

napi_value CreateStatusHandler(napi_env env);

}

#endif // MONGO_STATUS_H

#include <mongo_embedded/mongo_embedded.h>

#include "wrapper.h"
#include "mongo-addon.h"

#ifndef MONGO_CLIENT_H
#define MONGO_CLIENT_H

namespace mongoAddon {

NAPI_METHOD(MongoEmbeddedV1ClientCreate);

napi_value CreateClientHandler(napi_env env);
} // namespace mongoAddon

#endif

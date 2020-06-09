#include <node_api.h>
#include <mongo_embedded/mongo_embedded.h>

#ifndef WRAPPER_H
#define WRAPPER_H

class MongoStatus {
public:
    static napi_status Init(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_status NewInstance(napi_env env,
                                     napi_value* instance);
    mongo_embedded_v1_status *status;
private:
    MongoStatus();
    ~MongoStatus();
    static napi_ref constructor;
    static napi_value New(napi_env, napi_callback_info info);
};

class MongoParams {
public:
    static napi_status Init(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_status NewInstance(napi_env env,
                                         napi_value* instance);
    void SetYamlConfig(char* yaml_config);
    mongo_embedded_v1_init_params params;
private:
    MongoParams();
    ~MongoParams();
    static napi_ref constructor;
    static napi_value New(napi_env, napi_callback_info info);
};

class MongoLib {
public:
    static napi_status Init(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_status NewInstance(napi_env env, napi_value params, napi_value status,
                                         napi_value* instance);
    mongo_embedded_v1_lib *lib;
    mongo_embedded_v1_status *status;
private:
    MongoLib();
    MongoLib(mongo_embedded_v1_init_params& params, mongo_embedded_v1_status* status);
    ~MongoLib();
    static napi_ref constructor;
    static napi_value New(napi_env, napi_callback_info info);
};

class MongoInstance {
public:
    static napi_status Init(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_status NewInstance(napi_env env, napi_value lib, napi_value params, napi_value status,
                                         napi_value* instance);
    mongo_embedded_v1_instance* db;
    mongo_embedded_v1_status *status;
private:
    MongoInstance();
    MongoInstance(mongo_embedded_v1_lib* lib, mongo_embedded_v1_init_params& params, mongo_embedded_v1_status* status);
    ~MongoInstance();
    static napi_ref constructor;
    static napi_value New(napi_env, napi_callback_info info);
};

class MongoClient {
public:
    static napi_status Init(napi_env env);
    static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_status NewInstance(napi_env env, napi_value db, napi_value status,
                                         napi_value* instance);
    mongo_embedded_v1_client* client;
    mongo_embedded_v1_status *status;
private:
    MongoClient();
    MongoClient(mongo_embedded_v1_instance* db, mongo_embedded_v1_status* status);
    ~MongoClient();
    static napi_ref constructor;
    static napi_value New(napi_env, napi_callback_info info);
};

#endif // WRAPPER_H

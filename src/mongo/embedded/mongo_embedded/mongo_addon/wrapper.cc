#include "wrapper.h"
#include "mongo-addon.h"

// mongo_embedded_v1_status

MongoStatus::MongoStatus() {
    status = mongo_embedded_v1_status_create();
}

MongoStatus::~MongoStatus() {
    puts("Destroy Status");
    mongo_embedded_v1_status_destroy(status);
}

void MongoStatus::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  reinterpret_cast<MongoStatus*>(nativeObject)->~MongoStatus();
}

napi_ref MongoStatus::constructor;

napi_status MongoStatus::Init(napi_env env) {
    napi_value cons;
    // bind constructor New to cons
    napi_define_class(
        env, "MongoStatus", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    napi_create_reference(env, cons, 1, &constructor);

    return napi_ok;
}

napi_value MongoStatus::New(napi_env env, napi_callback_info info) {
    napi_value jsThis;

    napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
    MongoStatus* obj = new MongoStatus();
    napi_wrap(
        env,
        jsThis,
        reinterpret_cast<void*>(obj),
        MongoStatus::Destructor,
        nullptr,
        nullptr);

    return jsThis;
}

napi_status MongoStatus::NewInstance(napi_env env, napi_value* instance) {
    napi_value cons;
    const int argc = 0;
    napi_get_reference_value(env, constructor, &cons);
    napi_new_instance(env, cons, argc, nullptr, instance);

    return napi_ok;
}

// end mongo_embedded_v1_status

// mongo_embedded_v1_params

MongoParams::MongoParams() {
    params.yaml_config = nullptr;
    params.log_flags = 0;
    params.log_callback = NULL;
    params.log_user_data = NULL;
}

MongoParams::~MongoParams() {
//    free(reinterpret_cast<void*>(params.yaml_config));
}

void MongoParams::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  puts("Destroy Params");
  reinterpret_cast<MongoParams*>(nativeObject)->~MongoParams();
}

napi_ref MongoParams::constructor;

napi_status MongoParams::Init(napi_env env) {
    napi_value cons;
    // bind constructor New to cons
    napi_define_class(
        env, "MongoParams", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    napi_create_reference(env, cons, 1, &constructor);

    return napi_ok;
}

napi_value MongoParams::New(napi_env env, napi_callback_info info) {
    napi_value jsThis;

    napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
    MongoParams* obj = new MongoParams;
    napi_wrap(
        env,
        jsThis,
        reinterpret_cast<void*>(obj),
        MongoParams::Destructor,
        nullptr,
        nullptr);

    return jsThis;
}

napi_status MongoParams::NewInstance(napi_env env, napi_value* instance) {
    napi_value cons;
    const int argc = 0;
    napi_get_reference_value(env, constructor, &cons);
    napi_new_instance(env, cons, argc, nullptr, instance);

    return napi_ok;
}

void MongoParams::SetYamlConfig(char* yaml_config) {
    params.yaml_config = yaml_config;
}


// end mongo_embedded_v1_params

// mongo_embedded_v1_lib

MongoLib::MongoLib() {
    lib = nullptr;
    status = nullptr;
}

MongoLib::MongoLib(mongo_embedded_v1_init_params& params, mongo_embedded_v1_status* _status) {
    lib = mongo_embedded_v1_lib_init(&params, _status);
    status = _status;
}

MongoLib::~MongoLib() {
    mongo_embedded_v1_lib_fini (lib, status);
}

void MongoLib::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  puts("Destroy Lib");
  reinterpret_cast<MongoLib*>(nativeObject)->~MongoLib();
}

napi_ref MongoLib::constructor;

napi_status MongoLib::Init(napi_env env) {
    napi_value cons;
    // bind constructor New to cons
    napi_define_class(
        env, "MongoLib", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    napi_create_reference(env, cons, 1, &constructor);

    return napi_ok;
}

napi_value MongoLib::New(napi_env env, napi_callback_info info) {
    napi_value jsThis;

    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, &jsThis, nullptr);

    MongoParams* params;
    napi_unwrap(env, args[0], reinterpret_cast<void**>(&params));

    MongoStatus* status;
    napi_unwrap(env, args[1], reinterpret_cast<void**>(&status));
    MongoLib* obj = new MongoLib(params->params, status->status);
    napi_wrap(
        env,
        jsThis,
        reinterpret_cast<void*>(obj),
        MongoLib::Destructor,
        nullptr,
        nullptr);

    return jsThis;
}

napi_status MongoLib::NewInstance(napi_env env, napi_value params, napi_value status, napi_value* instance) {
    napi_value cons;
    const int argc = 2;
    napi_value argv[argc] = { params, status };
    napi_get_reference_value(env, constructor, &cons);
    napi_new_instance(env, cons, argc, argv, instance);

    return napi_ok;
}

// end mongo_embedded_v1_lib

// mongo_embedded_v1_instance

MongoInstance::MongoInstance() {
    db = nullptr;
    status = nullptr;
}

MongoInstance::MongoInstance(mongo_embedded_v1_lib* lib,
                             mongo_embedded_v1_init_params& params,
                             mongo_embedded_v1_status* _status) {

    db = mongo_embedded_v1_instance_create(lib, params.yaml_config, _status);

    status = _status;
}

MongoInstance::~MongoInstance() {
    mongo_embedded_v1_instance_destroy (db, status);
}

void MongoInstance::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  puts("Destroy Instance");
  reinterpret_cast<MongoInstance*>(nativeObject)->~MongoInstance();
}

napi_ref MongoInstance::constructor;

napi_status MongoInstance::Init(napi_env env) {
    napi_value cons;
    // bind constructor New to cons
    napi_define_class(
        env, "MongoInstance", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    napi_create_reference(env, cons, 1, &constructor);

    return napi_ok;
}

napi_value MongoInstance::New(napi_env env, napi_callback_info info) {
    napi_value jsThis;

    size_t argc = 3;
    napi_value args[3];
    napi_get_cb_info(env, info, &argc, args, &jsThis, nullptr);

    MongoLib* lib;
    napi_unwrap(env, args[0], reinterpret_cast<void**>(&lib));
    MongoParams* params;
    napi_unwrap(env, args[1], reinterpret_cast<void**>(&params));
    MongoStatus* status;
    napi_unwrap(env, args[2], reinterpret_cast<void**>(&status));
    MongoInstance* obj = new MongoInstance(lib->lib, params->params, status->status);
    napi_wrap(
        env,
        jsThis,
        reinterpret_cast<void*>(obj),
        MongoInstance::Destructor,
        nullptr,
        nullptr);
    return jsThis;
}

napi_status MongoInstance::NewInstance(napi_env env,
                                       napi_value lib,
                                       napi_value params,
                                       napi_value status,
                                       napi_value* instance) {
    napi_value cons;
    const int argc = 3;
    napi_value argv[argc] = { lib, params, status };
    napi_get_reference_value(env, constructor, &cons);
    napi_new_instance(env, cons, argc, argv, instance);

    return napi_ok;
}

// end mongo_embedded_v1_instance

// mongo_embedded_v1_client

MongoClient::MongoClient() {
    client = nullptr;
    status = nullptr;
}

MongoClient::MongoClient(mongo_embedded_v1_instance* db,
                         mongo_embedded_v1_status* _status) {
    client = mongo_embedded_v1_client_create(db, _status);
    status = _status;
}

MongoClient::~MongoClient() {
    // todo: write destructor
}

void MongoClient::Destructor(napi_env env,
                             void* nativeObject,
                             void* /*finalize_hint*/) {
  puts("Destroy Client");
  reinterpret_cast<MongoClient*>(nativeObject)->~MongoClient();
}

napi_ref MongoClient::constructor;

napi_status MongoClient::Init(napi_env env) {
    napi_value cons;
    // bind constructor New to cons
    napi_define_class(
        env, "MongoInstance", NAPI_AUTO_LENGTH, New, nullptr, 0, nullptr, &cons);
    napi_create_reference(env, cons, 1, &constructor);

    return napi_ok;
}

napi_value MongoClient::New(napi_env env, napi_callback_info info) {
    napi_value jsThis;

    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, &jsThis, nullptr);

    MongoInstance* db;
    napi_unwrap(env, args[0], reinterpret_cast<void**>(&db));
    MongoStatus* status;
    napi_unwrap(env, args[1], reinterpret_cast<void**>(&status));
    MongoClient* obj = new MongoClient(db->db, status->status);
    napi_wrap(
        env,
        jsThis,
        reinterpret_cast<void*>(obj),
        MongoClient::Destructor,
        nullptr,
        nullptr);
    return jsThis;
}

napi_status MongoClient::NewInstance(napi_env env,
                                       napi_value db,
                                       napi_value status,
                                       napi_value* instance) {
    napi_value cons;
    const int argc = 2;
    napi_value argv[argc] = { db, status };
    napi_get_reference_value(env, constructor, &cons);
    napi_new_instance(env, cons, argc, argv, instance);

    return napi_ok;
}

#include <mongo_embedded/mongo_embedded.h>
#include <string>

#include "mongo-addon.h"
#include "mongo-params.h"
#include "mongo-status.h"
#include "mongo-lib.h"
#include "mongo-instance.h"
#include "mongo-client.h"
#include "wrapper.h"
#include "mongo/bson/bsonobjbuilder.h"

namespace mongoAddon {

class InvokerAsyncWorker : public Napi::AsyncWorker {
public:
    InvokerAsyncWorker (
        const Napi::Function& callback,
        const napi_value query,
        const napi_value client_wrapper,
        const napi_value status_wrapper,
        const napi_env env
    ) : Napi::AsyncWorker(callback)
    {
        void* tmp;
        napi_get_buffer_info(env, query, &tmp, &_size_json);
        json = (char*) tmp;
        napi_unwrap(env, client_wrapper, reinterpret_cast<void**>(&client));
        napi_unwrap(env, status_wrapper, reinterpret_cast<void**>(&status));
    }
protected:
    void Execute() override {
        mongo_embedded_v1_call_invoke(
            json, _size_json, client->client, status->status, result, result_size);
    }

    void OnOK() override {
        Napi::Env env = Env();
        Napi::Buffer<char> tmp = Napi::Buffer<char>::New(env, result, result_size);
        Callback().MakeCallback(
            Receiver().Value(),
            {
                env.Null(),
                tmp
            }
        );
        if (result != NULL) {
            delete[] result;
        }
    }

    void OnError(const Napi::Error& e) override {
        Napi::Env env = Env();

        Callback().MakeCallback(
            Receiver().Value(),
            {
                e.Value(),
                env.Undefined()
            }
        );
        if (result != NULL) {
            delete[] result;
        }
    }
private:
    size_t _size_json;
    char* json;
    MongoClient* client;
    MongoStatus* status;
    char* result;
    int result_size;
};

// my own implementation
napi_value MongoEmbeddedV1CallInvoke(napi_env env, napi_callback_info info) {
    size_t argc = 4;
    napi_value args[4];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    Napi::Value func_val = Napi::Value::From(env, args[3]);
    Napi::Function func = func_val.As<Napi::Function>();

    (new InvokerAsyncWorker(func, args[0], args[1], args[2], env))->Queue();
    return nullptr;
}

napi_value Init(napi_env env, napi_value exports) {
    MongoStatus::Init(env);
    MongoParams::Init(env);
    napi_value mongo_addon;
    napi_value params_handler = CreateParamsHandler(env);
    napi_value status_handler = CreateStatusHandler(env);
    napi_value lib_handler = CreateLibHandler(env);
    napi_value instance_handler = CreateInstanceHandler(env);
    napi_value client_handler = CreateClientHandler(env);
    napi_value invoker;

    napi_create_object(env, &mongo_addon);
    napi_create_function(env, "invoker", NAPI_AUTO_LENGTH, MongoEmbeddedV1CallInvoke, NULL, &invoker);

    napi_set_named_property(env, mongo_addon, "paramsHandler", params_handler);
    napi_set_named_property(env, mongo_addon, "statusHandler", status_handler);
    napi_set_named_property(env, mongo_addon, "libHandler", lib_handler);
    napi_set_named_property(env, mongo_addon, "instanceHandler", instance_handler);
    napi_set_named_property(env, mongo_addon, "clientHandler", client_handler);
    napi_set_named_property(env, mongo_addon, "invoker", invoker);
    napi_set_named_property(env, exports, "mongoAddon", mongo_addon);

    return exports;
}

NAPI_MODULE(mongoAddon, Init);

} // namespace mongoAddon

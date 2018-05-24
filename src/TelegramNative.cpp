#include <td/telegram/td_json_client.h>
#include <td/telegram/td_log.h>
#include "TelegramNative.h"
#include "unicode/unistr.h"

bool TelegramNative::Init(void *connection_) {
    connection = static_cast<IAddInDefBase *>(connection_);
    return connection != nullptr;
}

bool TelegramNative::setMemManager(void *memory_manager_) {
    memory_manager = static_cast<IMemoryManager *>(memory_manager_);
    return memory_manager != nullptr;
}

bool TelegramNative::RegisterExtensionAs(WCHAR_T **ext_name) {
    char16_t name[] = u"TelegramNative";

    if (!memory_manager || !memory_manager->AllocMemory(reinterpret_cast<void **>(ext_name), sizeof(name))) {
        return false;
    };

    memcpy(*ext_name, name, sizeof(name));

    return true;
}

long TelegramNative::FindMethod(const WCHAR_T *method_name) {
    icu::UnicodeString lookup_name{method_name};

    for (auto i = 0u; i < methods.size(); ++i) {
        if (methods[i].toLower() == lookup_name.toLower()) {
            return static_cast<long>(i);
        }
    }

    for (auto i = 0u; i < methods_ru.size(); ++i) {
        if (methods_ru[i].toLower() == lookup_name.toLower()) {
            return static_cast<long>(i);
        }
    }

    return -1;
}

const WCHAR_T *TelegramNative::GetMethodName(const long num, const long lang_alias) {
    if (num > LastMethod) {
        return nullptr;
    }

    icu::UnicodeString method_name;

    switch (lang_alias) {
        case 0:
            method_name = methods[num];
            break;
        case 1:
            method_name = methods_ru[num];
            break;
        default:
            return nullptr;
    }

    if (method_name.isEmpty()) {
        return nullptr;
    }

    WCHAR_T *result = nullptr;

    size_t bytes = (method_name.length() + 1) * sizeof(char16_t);

    if (!memory_manager || !memory_manager->AllocMemory(reinterpret_cast<void **>(&result), bytes)) {
        return nullptr;
    };

    memcpy(result, method_name.getTerminatedBuffer(), bytes);

    return result;
}

long TelegramNative::GetNParams(const long method_num) {
    switch (method_num) {
        case TdExecute:
        case TdSend:
        case SetAsyncMode:
        case TdSetLogFilePath:
        case TdSetLogMaxFileSize:
        case TdSetLogVerbosityLevel:
            return 1;
        case TdReceive:
        default:
            return 0;
    }
}

bool TelegramNative::HasRetVal(const long method_num) {
    switch (method_num) {
        case TdExecute:
        case TdReceive:
        case TdSetLogFilePath:
            return true;
        case TdSend:
        case SetAsyncMode:
        case TdSetLogMaxFileSize:
        case TdSetLogVerbosityLevel:
        default:
            return false;
    }
}

bool TelegramNative::CallAsProc(const long method_num, tVariant *params, const long array_size) {
    switch (method_num) {
        case TdSend: {

            std::string command;
            if (!parse_param(params[0], command)) {
                return false;
            };

            td_json_client_send(telegram_client, command.c_str());

            return true;
        }

        case SetAsyncMode: {

            if (params[0].vt != VTYPE_BOOL) {
                return false;
            }

            if (async_mode == params[0].bVal) {
                return false;
            }

            async_mode = params[0].bVal;

            if (connection && connection->GetEventBufferDepth() < buffer_depth) {
                connection->SetEventBufferDepth(buffer_depth);
            }

            if (async_mode) {
                rcv_thread = std::thread(&TelegramNative::rcv_loop, this);
            } else {
                if (rcv_thread.joinable()) {
                    rcv_thread.join();
                }
            }

            return true;
        }

        case TdSetLogMaxFileSize: {

            auto max_file_size = TV_I4(&params[0]);
            td_set_log_max_file_size(max_file_size);

            return true;
        }

        case TdSetLogVerbosityLevel: {

            auto log_level = TV_I4(&params[0]);
            td_set_log_verbosity_level(log_level);

            return true;
        }

        default:
            return false;
    }

}

bool
TelegramNative::CallAsFunc(const long method_num, tVariant *ret_value, tVariant *params, const long array_size) {

    switch (method_num) {
        case TdExecute: {

            std::string command;
            if (!parse_param(params[0], command)) {
                return false;
            };

            icu::UnicodeString result = td_json_client_execute(telegram_client, command.c_str());
            auto len = static_cast<uint32_t>(result.length());

            TV_VT(ret_value) = VTYPE_PWSTR;
            size_t bytes = (len + 1) * sizeof(WCHAR_T);

            if (!memory_manager ||
                !memory_manager->AllocMemory(reinterpret_cast<void **>(&ret_value->pstrVal), bytes)) {
                return false;
            };

            memcpy(ret_value->pwstrVal, result.getTerminatedBuffer(), bytes);
            ret_value->wstrLen = len;

            return true;

        }

        case TdReceive: {

            icu::UnicodeString result = td_json_client_receive(telegram_client, rcv_timeout);

            auto len = static_cast<uint32_t>(result.length());

            TV_VT(ret_value) = VTYPE_PWSTR;
            size_t bytes = (len + 1) * sizeof(WCHAR_T);

            if (!memory_manager ||
                !memory_manager->AllocMemory(reinterpret_cast<void **>(&ret_value->pstrVal), bytes)) {
                return false;
            };

            memcpy(ret_value->pwstrVal, result.getTerminatedBuffer(), bytes);
            ret_value->wstrLen = len;

            return true;
        }

        case TdSetLogFilePath: {

            std::string command;
            if (!parse_param(params[0], command)) {
                return false;
            };

            auto success = td_set_log_file_path(command.c_str());

            TV_VT(ret_value) = VTYPE_BOOL;
            ret_value->bVal = static_cast<bool>(success);

            return true;
        }

        default:
            return false;
    }

}

TelegramNative::TelegramNative() {

    methods = {
            u"Send", u"Receive", u"Execute", u"SetAsyncMode",
            u"SetLogFilePath", u"SetLogMaxFileSize", u"SetLogVerbosityLevel"
    };
    methods_ru = {
            u"Отправить", u"Получить", u"Выполнить", u"УстановитьАсинхронныйРежим",
            u"УстановитьФайлЖурнала", u"УстановитьМаксимальныйРазмерФайлаЖурнала", u"УстановитьУровеньДетализацииЖурнала"
    };

    telegram_client = td_json_client_create();
}

TelegramNative::~TelegramNative() {

    async_mode = false;

    if (rcv_thread.joinable()) {
        rcv_thread.join();
    }

    td_json_client_destroy(telegram_client);
}

bool TelegramNative::parse_param(const tVariant param, std::string &out) {
    switch (param.vt) {
        case VTYPE_PSTR: {
            out.assign(param.pstrVal, param.strLen);
            break;
        }
        case VTYPE_PWSTR: {
            icu::UnicodeString tmp{param.pwstrVal, static_cast<int32_t>(param.wstrLen)};
            tmp.toUTF8String(out);
            break;
        }
        default:
            return false;
    }

    return true;
}

void TelegramNative::rcv_loop() {

    while (async_mode) {

        char16_t source[] = u"TelegramNative";
        char16_t message[] = u"Response";

        icu::UnicodeString response = td_json_client_receive(telegram_client, rcv_timeout);

        if (response.isEmpty()) {
            continue;
        }

        auto len = static_cast<size_t>(response.length() + 1);
        auto data = new char16_t[len];
        memcpy(data, response.getTerminatedBuffer(), len * sizeof(char16_t));

        connection->ExternalEvent(reinterpret_cast<WCHAR_T *>(source),
                                  reinterpret_cast<WCHAR_T *>(message),
                                  reinterpret_cast<WCHAR_T *>(data));

        delete[] data;
    }

}

#ifndef __ADDIN_NATIVE_H__
#define __ADDIN_NATIVE_H__

#include <ComponentBase.h>
#include <AddInDefBase.h>
#include <IMemoryManager.h>
#include <types.h>
#include <thread>
#include <vector>
#include <unicode/unistr.h>

class TelegramNative : public IComponentBase {
public:
    enum Props {
        LastProp = 0
    };

    enum Methods {
        TdSend = 0,
        TdReceive,
        TdExecute,
        SetAsyncMode,
        TdSetLogFilePath,
        TdSetLogMaxFileSize,
        TdSetLogVerbosityLevel,
        LastMethod
    };

    TelegramNative();

    ~TelegramNative() override;

    bool ADDIN_API Init(void *connection_) override;

    bool ADDIN_API setMemManager(void *memory_manager_) override;

    long ADDIN_API GetInfo() override { return 2000; };

    void ADDIN_API Done() override {};

    bool ADDIN_API RegisterExtensionAs(WCHAR_T **ext_name) override;

    long ADDIN_API GetNProps() override { return LastProp; };

    long ADDIN_API FindProp(const WCHAR_T *prop_name) override { return -1; };

    const WCHAR_T *ADDIN_API GetPropName(long num, long lang_alias) override { return nullptr; };

    bool ADDIN_API GetPropVal(const long num, tVariant *value) override { return false; };

    bool ADDIN_API SetPropVal(const long num, tVariant *value) override { return false; };

    bool ADDIN_API IsPropReadable(const long lPropNum) override { return false; };

    bool ADDIN_API IsPropWritable(const long lPropNum) override { return false; };

    long ADDIN_API GetNMethods() override { return LastMethod; };

    long ADDIN_API FindMethod(const WCHAR_T *method_name) override;

    const WCHAR_T *ADDIN_API GetMethodName(const long num, const long lang_alias) override;

    long ADDIN_API GetNParams(const long method_num) override;

    bool ADDIN_API GetParamDefValue(const long method_num, const long param_num,
                                    tVariant *def_value) override { return false; };

    bool ADDIN_API HasRetVal(const long method_num) override;

    bool ADDIN_API CallAsProc(const long method_num, tVariant *params, const long array_size) override;

    bool ADDIN_API CallAsFunc(const long method_num, tVariant *ret_value, tVariant *params,
                              const long array_size) override;

    void ADDIN_API SetLocale(const WCHAR_T *locale) override {};

private:
    static bool parse_param(tVariant param, std::string &out);

    IAddInDefBase *connection{nullptr};
    IMemoryManager *memory_manager{nullptr};

    std::vector<icu::UnicodeString> methods;
    std::vector<icu::UnicodeString> methods_ru;

    long buffer_depth{100};

    std::thread rcv_thread;
    double rcv_timeout{1.0};

    void rcv_loop();

    bool async_mode{false};
    void *telegram_client{nullptr};
};

#endif //__ADDIN_NATIVE_H__

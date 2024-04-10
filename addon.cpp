#include <node-addon-api/napi.h>
#include <windows.h>

Napi::String CheckUserPrivileges(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        return Napi::String::New(env, "Ошибка при открытии токена процесса");
    }

    DWORD dwLength = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLength);
    PTOKEN_USER pTokenUser = (PTOKEN_USER)LocalAlloc(LPTR, dwLength);

    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwLength, &dwLength)) {
        LocalFree(pTokenUser);
        CloseHandle(hToken);
        return Napi::String::New(env, "Ошибка при получении информации о токене");
    }

    DWORD dwSize = 0;
    LookupAccountSid(NULL, pTokenUser->User.Sid, NULL, &dwSize, NULL, NULL, NULL);
    char* szAccount = new char[dwSize];
    char* szDomain = new char[dwSize];
    SID_NAME_USE eUse;
    LookupAccountSid(NULL, pTokenUser->User.Sid, szAccount, &dwSize, szDomain, &dwSize, &eUse);

    LocalFree(pTokenUser);
    CloseHandle(hToken);

    std::string username = std::string(szDomain) + "\\" + std::string(szAccount);

    if (IsUserAnAdmin()) {
        return Napi::String::New(env, username + " имеет административные привилегии.");
    } else {
        return Napi::String::New(env, username + " не имеет административных привилегий.");
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkUserPrivileges"), Napi::Function::New(env, CheckUserPrivileges));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
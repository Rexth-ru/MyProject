#ifndef ADDON_H
#define ADDON_H

#include <napi.h>

Napi::String CheckUserPrivileges(const Napi::CallbackInfo& info);
Napi::Object Init(Napi::Env env, Napi::Object exports);

#endif 

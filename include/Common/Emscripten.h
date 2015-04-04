#pragma once

#ifndef __EMSCRIPTEN__
#define EM_ASM(code) (Common::Emscripten::executeJs(Common::Emscripten::formatEmAsm<int>(#code).c_str()))
#define EM_ASM_ARGS(code, ...)
#define EM_ASM_INT(code, ...) (Common::Emscripten::executeJsInt(Common::Emscripten::formatEmAsm<int>(#code, __VA_ARGS__).c_str()))
#define EM_ASM_DOUBLE(code, ...) (Common::Emscripten::executeJsDouble(Common::Emscripten::formatEmAsm<double>(#code, __VA_ARGS__).c_str()))
#define EM_ASM_INT_V(code) (Common::Emscripten::executeJsInt(Common::Emscripten::formatEmAsm<int>(#code).c_str()))
#define EM_ASM_DOUBLE_V(code) (Common::Emscripten::executeJsDouble(Common::Emscripten::formatEmAsm<int>(#code).c_str()))

#include <Awesomium/Platform.h>
#include <cstdarg>
#include <sstream>
#include <vector>

namespace Common {
namespace Emscripten {

void startSimulation(Awesomium::NativeWindow hwnd, const char* page, int w, int h);
bool simulationStep();

void executeJs(const char* src);
int executeJsInt(const char* src);
double executeJsDouble(const char* src);

template <typename T>
std::string formatEmAsm(const char* txt, ...) {
    struct {
        va_list list;
        std::vector<T> args;

        T get(int i) {
            if (args.size() > i) {
                return args[i];
            }

            int argsToGet = i - args.size() + 1;
            for (int j = 0; j < argsToGet; j++) {
                args.push_back(va_arg(list, T));
            }
            return args[i];
        }
    } parameterGetter;

    va_start(parameterGetter.list, txt);

    std::stringstream str;
    int param_id = -1;
    int param_digits = 0;

    for(int i = 0;; i++) {
        if (i == 0 && txt[i] == '{') {
            str << "var execJs = function() {";
            continue;
        }

        if (param_id != -1) {
            if (txt[i] >= '0' && txt[i] <= '9') {
                if (param_digits > 0)
                    param_id += (txt[i] - '0') * (param_digits * 10);
                else
                    param_id = txt[i] - '0';

                param_digits++;
                continue;
            }
            else {
                str << parameterGetter.get(param_id);
                param_id = -1;
                param_digits = 0;
            }
        }

        if (txt[i] == '$') {
            param_id = 0;
            param_digits = 0;
            continue;
        }

        if (txt[i] == '\0') {
            if (txt[i - 1] == '}') {
                str << "; execJs();";
            }
            str << '\0';
            break;
        }

        str << txt[i];
    }

    va_end(parameterGetter.list);

    return str.str();
};

}
}

#else
#include <emscripten/emscripten.h>
#endif
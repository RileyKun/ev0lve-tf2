//
// Created by ruppet on 4/13/2020.
//

#include <ev0lve.h>
#include <misc/logger.h>

void logger::create_console() {
#ifdef _DEBUG
    AllocConsole();
    SetConsoleTitleA("ev0lve.xyz for " EVOLVE_GAME);

    freopen_s(&out_buffer, "CONOUT$", "w", stdout);
#endif
}

void logger::log(const char *txt, ...) {
#ifdef _DEBUG
    char buffer[8912]{0};

    va_list va;
    va_start(va, txt);
    vsprintf_s(buffer, 8912, txt, va);
    va_end(va);

    printf_s("[ INFO ] %s\n", buffer);
#endif
}

void logger::warn(const char *txt, ...) {
#ifdef _DEBUG
    char buffer[8912]{0};

    va_list va;
            va_start(va, txt);
    vsprintf_s(buffer, 8912, txt, va);
            va_end(va);

    printf_s("[ ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                             FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf_s("WARN");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                             FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    printf_s(" ] %s\n", buffer);
#endif
}

void logger::ok(const char *txt, ...) {
#ifdef _DEBUG
    char buffer[8912]{0};

    va_list va;
            va_start(va, txt);
    vsprintf_s(buffer, 8912, txt, va);
            va_end(va);

    printf_s("[ ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf_s(" OK ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    printf_s(" ] %s\n", buffer);
#endif
}

void logger::fail(const char *txt, ...) {
#ifdef _DEBUG
    char buffer[8912]{0};

    va_list va;
            va_start(va, txt);
    vsprintf_s(buffer, 8912, txt, va);
            va_end(va);

    printf_s("[ ");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf_s("FAIL");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    printf_s(" ] %s\n", buffer);
#endif
}

void logger::free_console() {
#ifdef _DEBUG
    fclose(out_buffer);
    FreeConsole();
#endif
}

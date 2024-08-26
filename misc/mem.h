//
// Created by ruppet on 4/14/2020.
//

#ifndef EV0LVE_MEM_H
#define EV0LVE_MEM_H

#include <ev0lve.h>

namespace mem {
    class handle {
    public:
        handle(void* p = nullptr):
            ptr(p) {}

        handle(uintptr_t p):
            ptr((void*)p) {}

        template <typename T>
        constexpr std::enable_if_t<std::is_pointer_v<T>, T> as() {
            return static_cast<T>(ptr);
        }

        template <typename T>
        constexpr std::enable_if_t<std::is_lvalue_reference_v<T>, T> as() {
            return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr);
        }

        template <typename T>
        constexpr std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as() {
            return reinterpret_cast<T>(ptr);
        }

        template <typename T>
        handle add(T offset) {
            return handle(as<uintptr_t>() + offset);
        }

        template <typename T>
        handle sub(T offset) {
            return handle(as<uintptr_t>() - offset);
        }

        handle rip() {
            if (!ptr)
                return nullptr;
            return add(4U).add(as<std::int32_t&>());
        }

        handle unref() {
            if (!ptr)
                return nullptr;
            return handle(*as<uint32_t*>());
        }

        handle translate(handle from, handle to)
        {
            return to.add(as<uintptr_t>() - from.as<uintptr_t>());
        }

        operator bool() noexcept {
            return ptr != nullptr;
        }

    protected:
        void* ptr = nullptr;
    };

    class region {
    public:
        region(handle _base, size_t _size):
            r_base(_base), r_size(_size) {}

        handle base() {
            return r_base;
        }

        handle end() {
            return r_base.add(r_size);
        }

        [[nodiscard]] size_t size() const {
            return r_size;
        }

        bool contains(handle p) {
            if (p.as<uintptr_t>() < r_base.as<uintptr_t>())
                return false;
            return p.as<uintptr_t>() <= r_base.add(r_size).as<uintptr_t>();
        }

        region patch(const char* n, char* o) {
            memcpy(o, r_base.as<void*>(), r_size);
            memcpy(r_base.as<void*>(), n, r_size);
            return *this;
        }

        region patch(const char* n) {
            memcpy(r_base.as<void*>(), n, r_size);
            return *this;
        }

        region protect_start(uint32_t p) {
            VirtualProtect(r_base.as<void*>(), r_size, p, &r_old_protect);
            return *this;
        }

        region protect_end() {
            DWORD tmp{};
            VirtualProtect(r_base.as<void*>(), r_size, r_old_protect, &tmp);
            return *this;
        }

    protected:
        handle r_base;
        size_t r_size;
        DWORD r_old_protect{};
    };

    class module : public region {
    public:
        module(HMODULE mod):
            region(mod, 0) {
            auto dosHeader = r_base.as<IMAGE_DOS_HEADER*>();
            auto ntHeader = r_base.add(dosHeader->e_lfanew).as<IMAGE_NT_HEADERS64*>();

            r_size = ntHeader->OptionalHeader.SizeOfImage;
        }

        module(const char* n):
            module(GetModuleHandleA(n)) {}

        module():
            module((const char*)nullptr) {}

        IMAGE_DOS_HEADER* get_dos_headers()
        {
            return r_base.as<IMAGE_DOS_HEADER*>();
        }

        IMAGE_NT_HEADERS64* get_nt_headers()
        {
            return r_base.add(r_base.as<IMAGE_DOS_HEADER*>()->e_lfanew).as<IMAGE_NT_HEADERS64*>();
        }

        handle get_export(const char* symbol) {
            return GetProcAddress(r_base.as<HMODULE>(), symbol);
        }

    private:
        template <typename R, typename T>
        R get_rva(T rva)
        {
            return r_base.add(rva).as<R>();
        }
    };

    class sig {
    public:
        struct element {
            uint8_t data{};
            bool wildcard{};
        };

        sig(const char* pattern) {
            static auto to_upper = [](char c) -> char
            {
                return c >= 'a' && c <= 'z' ? (char)(c + ('A' - 'a')) : (char)c;
            };

            static auto is_hex = [&](char c) -> bool
            {
                switch (to_upper(c))
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                        return true;
                    default:
                        return false;
                }
            };

            do
            {
                if (*pattern == ' ')
                    continue;
                if (*pattern == '?')
                {
                    elements.push_back(element{ {}, true });
                    continue;
                }

                if (*(pattern + 1) && is_hex(*pattern) && is_hex(*(pattern + 1)))
                {
                    char str[3] = { *pattern, *(pattern + 1), '\0' };
                    auto data = std::strtol(str, nullptr, 16);

                    elements.push_back(element{ (uint8_t)data, false });
                }
            } while (*(pattern++));
        }

        handle scan(region mod = module()) {
            static auto cmp_memory = [](const uint8_t* data, element* elem, std::size_t num) -> bool
            {
                for (size_t i = 0; i < num; ++i)
                {
                    if (!elem[i].wildcard)
                        if (data[i] != elem[i].data)
                            return false;
                }

                return true;
            };

            for (std::uintptr_t i = mod.base().as<uintptr_t>(), end = mod.end().as<uintptr_t>(); i != end; ++i)
            {
                if (cmp_memory((uint8_t*)i, elements.data(), elements.size()))
                    return handle(i);
            }

            return {};
        }
    private:
        std::vector<element> elements;
    };
}

#endif //EV0LVE_MEM_H

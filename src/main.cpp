#include <Windows.h>
#include <wininet.h>

#include <cassert>
#include <cwctype>
#include <ranges>
#include <functional>
#include <iostream>


namespace rngs = std::ranges;
namespace views = std::views;


#ifndef __clang__
constexpr
#endif
auto is_not_wspace = std::not_fn(std::iswspace);


constexpr auto trim_left(std::wstring& s) -> void {
    s.erase(s.cbegin(), rngs::find_if(s, is_not_wspace));
}


constexpr auto trim_right(std::wstring& s) -> void {
    s.erase(rngs::find_if(views::reverse(s), is_not_wspace).base(), s.cend());
}


constexpr auto trim(std::wstring& s) -> void {
    trim_left(s);
    trim_right(s);
}


auto show_proxy() -> void {
#ifndef _MSC_VER
    auto&& options = std::type_identity_t<INTERNET_PER_CONN_OPTIONW[2]>{};
#else
    INTERNET_PER_CONN_OPTIONW options[2]{};
#endif
    options[0].dwOption = INTERNET_PER_CONN_FLAGS_UI;
    options[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

    static_assert(sizeof(options) == sizeof(INTERNET_PER_CONN_OPTIONW[2]));

    auto option_list = INTERNET_PER_CONN_OPTION_LISTW{};
    option_list.dwSize = sizeof(option_list);
    option_list.dwOptionCount = 2;
    option_list.pOptions = options;

    auto buffer_size = DWORD{sizeof(option_list)};
    if (!InternetQueryOptionW(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION,
                              &option_list, &buffer_size)) {
        auto const ec = GetLastError();
        std::cout << "InternetQueryOptionW failed: " << ec << std::endl;
        return;
    }

    if (options[0].Value.dwValue & PROXY_TYPE_PROXY) {
        std::cout << "PROXY_TYPE_PROXY: ";
        if (options[1].Value.pszValue) {
            std::wcout << options[1].Value.pszValue;
        } else {
            std::cout << "EMPTY";
        }
        std::cout << std::endl;

    } else {
        std::cout << "NO PROXY_TYPE_PROXY" << std::endl;
    }

    if (options[1].Value.pszValue) {
        GlobalFree(options[1].Value.pszValue);
    }
}


auto set_proxy(std::wstring proxy) -> void {
#ifndef _MSC_VER
    auto&& options = std::type_identity_t<INTERNET_PER_CONN_OPTIONW[3]>{};
#else
    INTERNET_PER_CONN_OPTIONW options[3]{};
#endif

    options[0].dwOption = INTERNET_PER_CONN_FLAGS_UI;
    options[0].Value.dwValue =
        proxy.empty() ? PROXY_TYPE_DIRECT : PROXY_TYPE_PROXY;

    options[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
    options[1].Value.pszValue = proxy.empty() ? nullptr : proxy.data();

    options[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    options[2].Value.pszValue = nullptr;

    auto option_list = INTERNET_PER_CONN_OPTION_LISTW{};
    option_list.dwSize = sizeof(option_list);
    option_list.dwOptionCount = 3;
    option_list.pOptions = options;

    auto buffer_size = DWORD{sizeof(option_list)};
    if (!InternetSetOptionW(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION,
                            &option_list, buffer_size)) {
        auto const ec = GetLastError();
        std::cout << "InternetQueryOptionW failed: " << ec << std::endl;
        return;
    }

    show_proxy();
}


auto show_help() -> void {
    std::cout << R"(
Usage: setproxy <option> [...]

options:
  --show
  --clear
  --set <ipaddress>:<port>

  --help
    )";
}


auto wmain(int argc, wchar_t* argv[]) -> int {
    if (argc == 1 || std::wstring{argv[1]} == L"--help") {
        show_help();

    } else if (std::wstring{argv[1]} == L"--show") {
        show_proxy();

    } else if (std::wstring{argv[1]} == L"--clear") {
        set_proxy({});

    } else if (std::wstring{argv[1]} == L"--set") {
        if (argc < 3) {
            show_help();
            return -1;
        }

        auto address = std::wstring{argv[2]};
        trim(address);
        if (address.empty()) {
            show_help();
            return -1;
        }

        set_proxy(address);

    } else {
        show_help();
    }

    return 0;
}

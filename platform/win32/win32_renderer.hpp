#pragma once

#include <windows.h>
#include "sunay.hpp"

namespace Sunay {

    class Win32Renderer {
    public:
        Win32Renderer(HWND hwnd);
        void draw(const BrowserUIState& state);

    private:
        HWND hwnd_;
    };

}

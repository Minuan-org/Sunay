#include "../platform/win32/win32_renderer.hpp"

namespace Sunay {

    Win32Renderer::Win32Renderer(HWND hwnd)
        : hwnd_(hwnd) {}

    void Win32Renderer::draw(const BrowserUIState& state) {
        HDC hdc = GetDC(hwnd_);
        if (!hdc) return;

        RECT client;
        GetClientRect(hwnd_, &client);

        // Background
        HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &client, bg);
        DeleteObject(bg);

        // Tab bar
        RECT tabBar = {0, 0, client.right, 32};
        HBRUSH tabBg = CreateSolidBrush(RGB(225, 225, 225));
        FillRect(hdc, &tabBar, tabBg);
        DeleteObject(tabBg);

        int x = 8;
        for (auto& tab : state.tabs) {
            RECT r = {x, 4, x + 160, 28};
            HBRUSH tb = CreateSolidBrush(tab.active ? RGB(255,255,255) : RGB(210,210,210));
            FillRect(hdc, &r, tb);
            DeleteObject(tb);

            DrawEdge(hdc, &r, BDR_RAISEDINNER, BF_RECT);
            DrawTextA(hdc, tab.title.c_str(), -1, &r,
                      DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);

            x += 168;
        }

        // Address bar
        RECT addr = {8, 36, client.right - 8, 64};
        HBRUSH addrBg = CreateSolidBrush(RGB(255,255,255));
        FillRect(hdc, &addr, addrBg);
        DeleteObject(addrBg);
        DrawEdge(hdc, &addr, BDR_SUNKENOUTER, BF_RECT);

        DrawTextA(hdc, state.addressBarText.c_str(), -1, &addr,
                  DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);

        // Status bar
        RECT status = {0, client.bottom - 20, client.right, client.bottom};
        HBRUSH stBg = CreateSolidBrush(RGB(230,230,230));
        FillRect(hdc, &status, stBg);
        DeleteObject(stBg);

        const char* msg = state.isLoading ? "Loading..." : "Ready";
        DrawTextA(hdc, msg, -1, &status,
                  DT_SINGLELINE | DT_VCENTER | DT_LEFT);

        ReleaseDC(hwnd_, hdc);
    }

}

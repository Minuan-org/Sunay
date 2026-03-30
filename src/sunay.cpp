#include "sunay.hpp"
#include <sstream>

namespace Sunay {

    std::string Version::toString() const {
        return std::to_string(major) + "." +
               std::to_string(minor) + "." +
               std::to_string(patch);
    }

    class EngineImpl : public Engine {
    public:
        EngineImpl() = default;

        void initialize(HWND hwnd) override {
            hwnd_ = hwnd;
            initialized_ = true;
        }

        void render(const BrowserUIState& state) override {
            if (!initialized_ || !hwnd_) return;

            HDC hdc = GetDC(hwnd_);
            if (!hdc) return;

            RECT client;
            GetClientRect(hwnd_, &client);

            // Background
            HBRUSH bgBrush = CreateSolidBrush(RGB(240, 240, 240));
            FillRect(hdc, &client, bgBrush);
            DeleteObject(bgBrush);

            // Tab bar area
            RECT tabBar = client;
            tabBar.bottom = tabBar.top + 32;
            HBRUSH tabBarBrush = CreateSolidBrush(RGB(225, 225, 225));
            FillRect(hdc, &tabBar, tabBarBrush);
            DeleteObject(tabBarBrush);

            // Draw tabs
            int tabX = 8;
            for (const auto& tab : state.tabs) {
                RECT tabRect = { tabX, tabBar.top + 4, tabX + 160, tabBar.bottom - 4 };
                HBRUSH tabBrush = CreateSolidBrush(
                    tab.active ? RGB(255, 255, 255) : RGB(210, 210, 210)
                );
                FillRect(hdc, &tabRect, tabBrush);
                DeleteObject(tabBrush);

                DrawEdge(hdc, &tabRect, BDR_RAISEDINNER, BF_RECT);

                std::string title = tab.title.empty() ? "New Tab" : tab.title;
                DrawTextA(hdc, title.c_str(), -1, &tabRect,
                          DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);

                tabX += 168;
            }

            // Address bar area
            RECT addrBar = client;
            addrBar.top = tabBar.bottom + 4;
            addrBar.bottom = addrBar.top + 28;

            HBRUSH addrBg = CreateSolidBrush(RGB(235, 235, 235));
            FillRect(hdc, &addrBar, addrBg);
            DeleteObject(addrBg);

            RECT addrField = addrBar;
            addrField.left += 8;
            addrField.right -= 8;
            HBRUSH addrFieldBg = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &addrField, addrFieldBg);
            DeleteObject(addrFieldBg);
            DrawEdge(hdc, &addrField, BDR_SUNKENOUTER, BF_RECT);

            DrawTextA(hdc, state.addressBarText.c_str(), -1, &addrField,
                      DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);

            // Status area at bottom
            RECT status = client;
            status.top = status.bottom - 20;
            HBRUSH statusBg = CreateSolidBrush(RGB(230, 230, 230));
            FillRect(hdc, &status, statusBg);
            DeleteObject(statusBg);

            std::string statusText = state.isLoading ? "Loading..." : "Ready";
            DrawTextA(hdc, statusText.c_str(), -1, &status,
                      DT_SINGLELINE | DT_VCENTER | DT_LEFT);

            ReleaseDC(hwnd_, hdc);
        }

        Version getVersion() const override {
            return Version{1, 0, 0};
        }

        void shutdown() override {
            initialized_ = false;
            hwnd_ = nullptr;
        }

    private:
        HWND hwnd_ = nullptr;
        bool initialized_ = false;
    };

    std::unique_ptr<Engine> Engine::create() {
        return std::make_unique<EngineImpl>();
    }

}

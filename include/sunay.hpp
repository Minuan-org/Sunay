#pragma once

#include <string>
#include <vector>
#include <memory>
#include <windows.h>

namespace Sunay {

    struct Version {
        int major;
        int minor;
        int patch;

        std::string toString() const;
    };

    struct Tab {
        std::string title;
        std::string url;
        bool active = false;
    };

    struct BrowserUIState {
        std::vector<Tab> tabs;
        std::string addressBarText;
        bool isLoading = false;
    };

    class Engine {
    public:
        static std::unique_ptr<Engine> create();

        virtual void initialize(HWND hwnd) = 0;
        virtual void render(const BrowserUIState& state) = 0;

        virtual Version getVersion() const = 0;
        virtual void shutdown() = 0;

        virtual ~Engine() = default;
    };

}

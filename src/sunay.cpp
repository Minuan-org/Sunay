#include "sunay.hpp"
#include "../platform/win32/win32_renderer.hpp"

namespace Sunay {

    class EngineImpl : public Engine {
    public:
        void initialize(HWND hwnd) override {
            hwnd_ = hwnd;
            renderer_ = std::make_unique<Win32Renderer>(hwnd);
        }

        void render(const BrowserUIState& state) override {
            if (renderer_) renderer_->draw(state);
        }

        Version getVersion() const override {
            return Version{1, 0, 0};
        }

        void shutdown() override {
            renderer_.reset();
            hwnd_ = nullptr;
        }

    private:
        HWND hwnd_ = nullptr;
        std::unique_ptr<Win32Renderer> renderer_;
    };

    std::unique_ptr<Engine> Engine::create() {
        return std::make_unique<EngineImpl>();
    }

}

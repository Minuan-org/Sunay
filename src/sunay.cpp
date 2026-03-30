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
        void initialize() override {
            initialized = true;
        }

        std::string render(const BrowserUIState& state) override {
            if (!initialized)
                initialize();

            std::ostringstream out;

            // Render tab bar
            out << "[Tabs]\n";
            for (const auto& tab : state.tabs) {
                if (tab.active) {
                    out << " * " << tab.title << " (" << tab.url << ")\n";
                } else {
                    out << "   " << tab.title << " (" << tab.url << ")\n";
                }
            }

            // Render address bar
            out << "\n[Address Bar]\n";
            out << " " << state.addressBarText << "\n";

            // Render status
            out << "\n[Status]\n";
            out << (state.isLoading ? " Loading...\n" : " Idle\n");

            return out.str();
        }

        Version getVersion() const override {
            return Version{1, 0, 0};
        }

        void shutdown() override {
            initialized = false;
        }

    private:
        bool initialized = false;
    };

    std::unique_ptr<Engine> Engine::create() {
        return std::make_unique<EngineImpl>();
    }

}

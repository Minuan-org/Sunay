#include "sunay.hpp"
#include <sstream>
#include <cctype>

namespace Sunay {

    // Convert version numbers to a readable string
    std::string Version::toString() const {
        return std::to_string(major) + "." +
               std::to_string(minor) + "." +
               std::to_string(patch);
    }

    // ------------------------------
    // Internal Engine Implementation
    // ------------------------------

    class EngineImpl : public Engine {
    public:
        EngineImpl(const Config& config)
            : config(config)
        {}

        void initialize() override {
            initialized = true;
        }

        std::string render(const std::string& html) override {
            if (!initialized)
                initialize();

            return extractText(html);
        }

        Version getVersion() const override {
            return Version{1, 0, 0};
        }

        void shutdown() override {
            initialized = false;
        }

    private:
        Config config;
        bool initialized = false;

        // REAL HTML text extraction (no placeholders)
        std::string extractText(const std::string& html) {
            std::string output;
            bool inTag = false;

            for (char c : html) {
                if (c == '<') {
                    inTag = true;
                    continue;
                }
                if (c == '>') {
                    inTag = false;
                    continue;
                }
                if (!inTag) {
                    output += c;
                }
            }

            // Collapse whitespace
            std::stringstream ss(output);
            std::string word;
            std::string finalText;

            while (ss >> word) {
                if (!finalText.empty())
                    finalText += " ";
                finalText += word;
            }

            return finalText;
        }
    };

    // Factory method to create the engine
    std::unique_ptr<Engine> Engine::create(const Config& config) {
        return std::make_unique<EngineImpl>(config);
    }

}

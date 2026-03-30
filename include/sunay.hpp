#ifndef SUNAY_HPP
#define SUNAY_HPP

#include <string>
#include <memory>

namespace Sunay {

    // Represents the version of the Sunay engine
    struct Version {
        int major;
        int minor;
        int patch;

        std::string toString() const;
    };

    // Configuration options for initializing the engine
    struct Config {
        bool enableJavaScript = false;
        bool enableCSS = true;
        bool enableImages = true;
    };

    // The main Sunay Engine class
    class Engine {
    public:
        // Create a new engine instance
        static std::unique_ptr<Engine> create(const Config& config = Config());

        // Initialize internal subsystems
        virtual void initialize() = 0;

        // Render a webpage from raw HTML
        virtual std::string render(const std::string& html) = 0;

        // Get engine version
        virtual Version getVersion() const = 0;

        // Shutdown and cleanup
        virtual void shutdown() = 0;

        virtual ~Engine() = default;
    };

}

#endif

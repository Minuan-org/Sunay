#include "sunay/rendering_pipeline.hpp"

#include <QObject>
#include <QUrl>

#include <iostream>
#include <type_traits>

namespace {

bool expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        return false;
    }
    return true;
}

} // namespace

int main() {
    static_assert(!std::is_copy_constructible_v<Sunay::RenderingPipeline>);
    static_assert(!std::is_copy_assignable_v<Sunay::RenderingPipeline>);

    bool passed = true;
    QObject owner;
    Sunay::RenderingPipeline renderer(nullptr, &owner);

    passed &= expect(renderer.parent() == &owner,
                     "the renderer keeps its QObject parent");
    passed &= expect(!renderer.isAttached(),
                     "a null view is reported as detached");
    passed &= expect(renderer.view() == nullptr,
                     "a null view is returned safely");

    const Sunay::RenderingOptions defaults = renderer.options();
    passed &= expect(defaults.javaScriptEnabled,
                     "JavaScript is enabled by default");
    passed &= expect(defaults.imagesEnabled,
                     "images are enabled by default");
    passed &= expect(!defaults.localContentCanAccessRemoteUrls,
                     "local pages cannot access remote URLs by default");

    Sunay::RenderingOptions restricted;
    restricted.javaScriptEnabled = false;
    restricted.imagesEnabled = false;
    restricted.localContentCanAccessRemoteUrls = false;
    renderer.setOptions(restricted);

    const Sunay::RenderingOptions readBack = renderer.options();
    passed &= expect(!readBack.javaScriptEnabled,
                     "JavaScript settings can be disabled");
    passed &= expect(!readBack.imagesEnabled,
                     "image loading can be disabled");
    passed &= expect(!readBack.localContentCanAccessRemoteUrls,
                     "remote access stays disabled when restricted");

    Sunay::RenderingOptions remoteAllowed;
    remoteAllowed.javaScriptEnabled = true;
    remoteAllowed.imagesEnabled = true;
    remoteAllowed.localContentCanAccessRemoteUrls = true;
    renderer.setOptions(remoteAllowed);
    passed &= expect(renderer.options().localContentCanAccessRemoteUrls,
                     "the explicit remote-access option is retained");

    // These methods must safely do nothing when no QWebEngineView is attached.
    renderer.renderHtml(QStringLiteral("<html><body>Test</body></html>"),
                        QUrl(QStringLiteral("about:blank")));
    renderer.navigate(QUrl(QStringLiteral("https://example.com")));
    renderer.navigate(QUrl());
    renderer.stop();

    passed &= expect(!renderer.isAttached(),
                     "null-view operations do not attach a view");

    if (passed) {
        std::cout << "Sunay rendering pipeline checks passed.\n";
        return 0;
    }
    return 1;
}

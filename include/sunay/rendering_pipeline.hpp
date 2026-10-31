#pragma once

#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>

class QWebEngineView;

namespace Sunay {

struct RenderingOptions {
    bool javaScriptEnabled = true;
    bool imagesEnabled = true;
    bool localContentCanAccessRemoteUrls = false;
};

/**
 * Sunay's embedded page-rendering pipeline for Minuan.
 *
 * Qt WebEngine (Chromium) performs HTML parsing, CSS processing, layout,
 * painting, compositing and JavaScript execution. The QWebEngineView is
 * owned by Minuan; this class provides the rendering interface.
 */
class RenderingPipeline final : public QObject {
    Q_OBJECT

public:
    explicit RenderingPipeline(QWebEngineView* view,
                               QObject* parent = nullptr);
    ~RenderingPipeline() override = default;

    RenderingPipeline(const RenderingPipeline&) = delete;
    RenderingPipeline& operator=(const RenderingPipeline&) = delete;

    bool isAttached() const noexcept;
    QWebEngineView* view() const noexcept;

    void setOptions(const RenderingOptions& options);
    RenderingOptions options() const noexcept;

    /// Render processed HTML6/HTML using an optional resource base URL.
    void renderHtml(const QString& html, const QUrl& baseUrl = QUrl());

    /// Navigate the embedded renderer to a URL.
    void navigate(const QUrl& url);

    /// Stop the current page load.
    void stop();

signals:
    void renderingStarted();
    void loadProgressChanged(int progress);
    void renderingFinished(bool success);
    void urlChanged(const QUrl& url);

private:
    void applyOptions();

    QPointer<QWebEngineView> view_;
    RenderingOptions options_;
};

} // namespace Sunay

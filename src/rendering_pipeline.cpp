#include "sunay/rendering_pipeline.hpp"

#include <QWebEngineSettings>
#include <QWebEngineView>

namespace Sunay {

RenderingPipeline::RenderingPipeline(QWebEngineView* view, QObject* parent)
    : QObject(parent), view_(view) {
    if (!view_) {
        return;
    }

    applyOptions();

    connect(view_, &QWebEngineView::loadStarted,
            this, &RenderingPipeline::renderingStarted);
    connect(view_, &QWebEngineView::loadProgress,
            this, &RenderingPipeline::loadProgressChanged);
    connect(view_, &QWebEngineView::loadFinished,
            this, &RenderingPipeline::renderingFinished);
    connect(view_, &QWebEngineView::urlChanged,
            this, &RenderingPipeline::urlChanged);
}

bool RenderingPipeline::isAttached() const noexcept {
    return !view_.isNull();
}

QWebEngineView* RenderingPipeline::view() const noexcept {
    return view_.data();
}

void RenderingPipeline::setOptions(const RenderingOptions& options) {
    options_ = options;
    applyOptions();
}

RenderingOptions RenderingPipeline::options() const noexcept {
    return options_;
}

void RenderingPipeline::applyOptions() {
    if (!view_) {
        return;
    }

    QWebEngineSettings* settings = view_->settings();

    settings->setAttribute(QWebEngineSettings::JavascriptEnabled,
                           options_.javaScriptEnabled);
    settings->setAttribute(QWebEngineSettings::AutoLoadImages,
                           options_.imagesEnabled);
    settings->setAttribute(
        QWebEngineSettings::LocalContentCanAccessRemoteUrls,
        options_.localContentCanAccessRemoteUrls);
}

void RenderingPipeline::renderHtml(const QString& html,
                                   const QUrl& baseUrl) {
    if (view_) {
        view_->setHtml(html, baseUrl);
    }
}

void RenderingPipeline::navigate(const QUrl& url) {
    if (view_ && url.isValid() && !url.isEmpty()) {
        view_->load(url);
    }
}

void RenderingPipeline::stop() {
    if (view_) {
        view_->stop();
    }
}

} // namespace Sunay

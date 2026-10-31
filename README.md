# Sunay

Sunay is Minuan's embedded page-rendering component. It is a library used by
Minuan, not a separate browser application.

## Rendering backend

Sunay embeds Qt WebEngine, which uses Chromium to provide HTML parsing,
DOM construction, CSS parsing and layout, JavaScript execution, page painting,
compositing, navigation and load-progress events.

Minuan remains responsible for browser policy, URL allowlisting, HTML6
validation and HTML6-to-HTML conversion. Sunay accepts the processed
document and displays it inside Minuan's page view.

## Build

Requirements:
- CMake 3.16 or newer
- C++20 compiler
- Qt 6 or Qt 5 with Core, Widgets and WebEngineWidgets

Build standalone:

    cmake -S . -B build
    cmake --build build --config Release

Or add Sunay to Minuan with CMake FetchContent.

## API

    #include <QWebEngineView>
    #include <sunay/rendering_pipeline.hpp>

    auto* view = new QWebEngineView(parentWidget);
    auto* pipeline = new Sunay::RenderingPipeline(view, parentWidget);
    pipeline->renderHtml(processedHtml, baseUrl);

The host application owns the QWebEngineView and controls which URLs may be
visited. Local HTML is prevented from accessing remote URLs by default.

## Scope

This is a Chromium-backed rendering integration, not a from-scratch
HTML/CSS/JavaScript engine. Minuan-specific HTML6 and CSS Next features need
translation or compatibility work before being passed to Chromium.
JS-squared-specific runtime features are not implemented by Sunay yet.

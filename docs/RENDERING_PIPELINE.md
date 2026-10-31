# Sunay Rendering Pipeline

Sunay is Minuan's rendering component, not a separate browser or standalone application.

## Architecture

Minuan remains responsible for browser policy and document preparation:
URL allowlisting, HTML6 validation, DOM processing, custom-tag conversion,
and generation of the final document. Minuan then hands the processed
document to Sunay.

Sunay embeds Qt WebEngine's Chromium renderer in Minuan's page area.
Chromium supplies the web-platform rendering stages:

1. **Document input** — Minuan supplies processed HTML and an optional base URL.
2. **HTML parsing** — Chromium parses the document and constructs its DOM.
3. **CSS processing** — stylesheets are loaded, parsed and resolved into computed styles.
4. **Layout** — the renderer calculates element geometry and text flow.
5. **Paint** — display commands are generated for visible content.
6. **Compositing** — Chromium combines paint layers and presents the page.
7. **Runtime** — JavaScript executes in the embedded page and can update the DOM.

Sunay exposes a C++ API around the renderer and forwards load progress,
completion and URL-change events to Minuan.

## Minuan integration

Create a QWebEngineView in Minuan's page area and attach the pipeline:

    #include <QWebEngineView>
    #include "sunay/rendering_pipeline.hpp"

    auto* pageView = new QWebEngineView(parentWidget);
    auto* renderer = new Sunay::RenderingPipeline(pageView, parentWidget);

    Sunay::RenderingOptions options;
    options.javaScriptEnabled = true;
    options.imagesEnabled = true;
    renderer->setOptions(options);

    // Pass output from Minuan's HTML6/DOM processing stage:
    renderer->renderHtml(QString::fromStdString(processedHtml), baseUrl);

For normal navigation, call renderer->navigate(url). Connect
renderingStarted, loadProgressChanged, and renderingFinished to Minuan's
loading indicator and navigation state.

## Implementation boundary

This initial pipeline uses Qt WebEngine/Chromium for the web-standard parser,
CSS engine, layout, JavaScript runtime, painting and compositing. Sunay does
not yet contain a from-scratch HTML/CSS/JavaScript engine. Minuan's HTML6
and CSS Next features must be translated or adapted before they are handed
to Chromium until dedicated compatibility layers are implemented.

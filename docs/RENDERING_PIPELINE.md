# Sunay Rendering Pipeline

Sunay is Minuan's rendering component, not a separate browser or standalone
application.

## Responsibilities

Minuan owns browser policy and document preparation: URL allowlisting,
HTML6 validation, custom-tag conversion, history, tabs and navigation UI.
Sunay owns the integration between Minuan's page area and Qt WebEngine.

## Rendering stages

Sunay passes processed HTML and an optional base URL to Qt WebEngine.
Chromium provides the browser rendering stages:

1. Document input — Minuan supplies processed HTML and a resource base URL.
2. HTML parsing — Chromium parses the document and constructs its DOM.
3. CSS processing — stylesheets are parsed and resolved into computed styles.
4. Layout — element geometry and text flow are calculated.
5. Paint — display commands are generated for visible content.
6. Compositing — layers are combined and presented in the page view.
7. JavaScript runtime — page scripts execute and may update the DOM.

Sunay forwards load-start, progress, completion and URL-change events to
Minuan through its C++ API.

## API

    #include <QWebEngineView>
    #include <sunay/rendering_pipeline.hpp>

    auto* pageView = new QWebEngineView(parentWidget);
    auto* renderer = new Sunay::RenderingPipeline(pageView, parentWidget);

    Sunay::RenderingOptions options;
    options.javaScriptEnabled = true;
    options.imagesEnabled = true;
    renderer->setOptions(options);

    renderer->renderHtml(processedHtml, baseUrl);

For navigation, call renderer->navigate(url). Call renderer->stop() to stop
the active load. The view is owned by Minuan; Sunay does not own it.

## Security boundary

Local content access to remote URLs is disabled by default. Minuan must
validate top-level navigation against its configured allowed-site list.
The renderer does not replace browser navigation policy.

## Implementation boundary

This version uses Qt WebEngine/Chromium for HTML, CSS, JavaScript, layout,
painting and compositing. It does not contain a custom rendering engine.
Minuan's HTML6 and CSS Next syntax must be translated or adapted before it is
handed to Chromium. JS-squared-specific runtime features are not implemented
yet.

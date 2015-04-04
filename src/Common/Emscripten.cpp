#ifndef __EMSCRIPTEN__
#include <Common/Emscripten.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <thread>
#include <chrono>
#include <stdio.h>

using namespace Common;

Awesomium::WebCore* _webCore;
Awesomium::WebView* _webView;

class ViewListener : public Awesomium::WebViewListener::View {
public:
    virtual void OnChangeTooltip(Awesomium::WebView *caller, const Awesomium::WebString &tooltip);

    virtual void OnChangeTargetURL(Awesomium::WebView *caller, const Awesomium::WebURL &url);

    virtual void OnChangeCursor(Awesomium::WebView *caller, Awesomium::Cursor cursor);

    virtual void OnChangeFocus(Awesomium::WebView *caller, Awesomium::FocusedElementType focused_type);

    virtual void OnShowCreatedWebView(Awesomium::WebView *caller, Awesomium::WebView *new_view, const Awesomium::WebURL &opener_url, const Awesomium::WebURL &target_url, const Awesomium::Rect &initial_pos, bool is_popup);

    virtual void OnChangeTitle(Awesomium::WebView *caller, const Awesomium::WebString &title);

    virtual void OnAddConsoleMessage	(	Awesomium::WebView * 	caller,
            const Awesomium::WebString & 	message,
            int 	line_number,
            const Awesomium::WebString & 	source
    ) {
        printf("Awesomium Log: %s l: %d src: %s\n", Awesomium::ToString(message).c_str(), line_number, Awesomium::ToString(source).c_str());
    }

    virtual void OnChangeAddressBar(Awesomium::WebView *caller, const Awesomium::WebURL &url);
};

class JSHandler : public Awesomium::JSMethodHandler {

public:
    virtual void OnMethodCall(Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args);

    virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args);
};



unsigned int object_id_api = 0;
unsigned int object_id_module = 0;
bool _exit_updates = false;

void Emscripten::startSimulation(Awesomium::NativeWindow hwnd, const char *page, int w, int h) {
    Awesomium::WebConfig config;
    config.remote_debugging_host = Awesomium::WSLit("127.0.0.1");
    config.remote_debugging_port = 13338;

    Awesomium::WebPreferences pref;
    pref.enable_web_security = false;
	pref.enable_gpu_acceleration = true;
	pref.enable_web_gl = true;

    _webCore = Awesomium::WebCore::Initialize(config);
    auto session = _webCore->CreateWebSession(Awesomium::WSLit("build/session.dat"), pref);

    _webView = _webCore->CreateWebView(w, h, session, Awesomium::kWebViewType_Window);
    _webView->set_parent_window(hwnd);

    Awesomium::JSObject api = _webView->CreateGlobalJavascriptObject(Awesomium::WSLit("EmscriptenSimAPI")).ToObject();
    api.SetCustomMethod(Awesomium::WSLit("Pointer_stringify"), true);
	object_id_api = api.remote_id();

    Awesomium::JSObject module = _webView->CreateGlobalJavascriptObject(Awesomium::WSLit("Module")).ToObject();
    module.SetCustomMethod(Awesomium::WSLit("_editor_update_vals"), false); // with return value to work around awesomium bug
    module.SetCustomMethod(Awesomium::WSLit("_real_main"), false);
	object_id_module = module.remote_id();

    _webView->set_view_listener(new ViewListener());
	_webView->set_js_method_handler(new JSHandler());
    _webView->LoadURL(Awesomium::WebURL(Awesomium::WSLit(page)));

    while(_webView->IsLoading()) {
        _webCore->Update();
    }
	std::this_thread::sleep_for(std::chrono::milliseconds(650));// std::literals::ms);
    _webCore->Update();
}

bool Emscripten::simulationStep() {
	if (_exit_updates) {
		_exit_updates = false;
		return false;
	}
    _webCore->Update();
	return true;
}

// ======================================================================================

void Emscripten::executeJs(const char *src) {
    _webView->ExecuteJavascript(Awesomium::WSLit(src), Awesomium::WSLit(""));
}

double Emscripten::executeJsDouble(const char *src) {
    auto result = _webView->ExecuteJavascriptWithResult(Awesomium::WSLit(src), Awesomium::WSLit(""));
    if(result.IsUndefined()) {
        return -1.0;
    }
    return result.ToDouble();
}

int Emscripten::executeJsInt(const char *src) {
    auto result = _webView->ExecuteJavascriptWithResult(Awesomium::WSLit(src), Awesomium::WSLit(""));
    if(result.IsUndefined()) {
        return -1;
    }
    return result.ToInteger();
}

// ======================================================================================

void ViewListener::OnChangeTitle(Awesomium::WebView *caller, const Awesomium::WebString &title) {

}

void ViewListener::OnChangeTooltip(Awesomium::WebView *caller, const Awesomium::WebString &tooltip) {

}

void ViewListener::OnChangeTargetURL(Awesomium::WebView *caller, const Awesomium::WebURL &url) {

}

void ViewListener::OnChangeCursor(Awesomium::WebView *caller, Awesomium::Cursor cursor) {

}

void ViewListener::OnChangeFocus(Awesomium::WebView *caller, Awesomium::FocusedElementType focused_type) {

}

void ViewListener::OnShowCreatedWebView(Awesomium::WebView *caller, Awesomium::WebView *new_view, const Awesomium::WebURL &opener_url, const Awesomium::WebURL &target_url, const Awesomium::Rect &initial_pos, bool is_popup) {

}

void ViewListener::OnChangeAddressBar(Awesomium::WebView *caller, const Awesomium::WebURL &url) {

}

// ======================================================================================
extern "C" void real_main();
extern "C" void editor_update_vals(int type);

void JSHandler::OnMethodCall(Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args) {
    //if (remote_object_id == object_id_module) {
        if (method_name == Awesomium::WSLit("_real_main")) {
            printf("Real main just got called from javascript! :)\n");
            _exit_updates = true;
        }
		else if (method_name == Awesomium::WSLit("_editor_update_vals")) {
			editor_update_vals(args[0].ToInteger());
		}
		else {
			printf("Unknown Module call: %s\n", Awesomium::ToString(method_name).c_str());
		}
    //}
}

Awesomium::JSValue JSHandler::OnMethodCallWithReturnValue(Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args) {
    //if (remote_object_id == object_id_api) {
        if (method_name == Awesomium::WSLit("Pointer_stringify")) {
            const char *ptr = (const char *) args[0].ToInteger();
            Awesomium::JSValue res(Awesomium::WSLit(ptr));
            return res;
        }
        else {
            printf("Unknown Module call: %s\n", Awesomium::ToString(method_name).c_str());
        }
    //}
}
#endif
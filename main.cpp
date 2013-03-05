/**
 * @file main.cpp
 *
 * This file contains the support code at the C++ level for
 * an HTML5/JS application that has access to device services.
 *
 * You don't need to change anything in this code file unless
 * you wish to add support for functions that are not available
 * out-of-the box in wormhole.js.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/EditBox.h>
//#include <NativeUI/EditBoxListener.h>
#include <NativeUI/Button.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/ImageButton.h>
#include <MAUtil/String.h>
#include <mastring.h>

// Namespaces we want to access.
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.
using namespace MAUtil;

/**
 * This class provides a simple webview that loads
 * the webpage at the specified url. It saves the
 * most recent entered url.
 */
class MyMoblet : public HybridMoblet, public ButtonListener {

private:
    VerticalLayout *mVerticalLayout;
    HorizontalLayout *mHorizontalLayout;
    Button *mReloadButton;
    ImageButton *mAboutIcon;
    EditBox *mUrlField;
    const String DEFAULT_URL;

public:
    MyMoblet() : DEFAULT_URL("Insert url here") {
        initialize();
        String url = DEFAULT_URL;
        String fileName = mFileUtil->getLocalPath() + "LastUrl.txt";

        // Get the most recently used url.
        bool success = mFileUtil->readTextFromFile(
                fileName,
                url);
        mUrlField->setText(url);

        if (success) showPage(url);

        // debug
        lprintfln("@@@ Filename is %s", fileName.c_str());
    }

    void initialize() {
        createUI();
        extractFileSystem();
        enableWebViewMessages();

        // Initialize the message handler. All messages from
        // JavaScript are routed through this handler.
        mMessageHandler->initialize(this);
    }

    /**
     * Creates the main UI elements.
     */
    void createUI() {
        // Create and configure the WebView.
        mWebView = new WebView();
        mWebView -> fillSpaceHorizontally();
        mWebView -> fillSpaceVertically();

        // Layouts.
        mVerticalLayout = new VerticalLayout();
        mHorizontalLayout = new HorizontalLayout();
        mHorizontalLayout -> wrapContentVertically();

        mUrlField = new EditBox();
        mUrlField -> fillSpaceHorizontally();
        // can be used to move scroll the bottom up
        // when showing the keyboard
        // mUrlField -> addEditBoxListener(this);

        mReloadButton = new Button();
        mReloadButton->setText("Reload");
        mReloadButton->addButtonListener(this);
        mReloadButton->setHeight(mUrlField->getHeight());

        mAboutIcon = new ImageButton();
        mAboutIcon->addButtonListener(this);
        mAboutIcon->setBackgroundImage(INFO_ICON);
        mAboutIcon->setSize(mReloadButton->getHeight(),mReloadButton->getHeight());
        //mInfoIcon->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);

        mVerticalLayout->addChild(mHorizontalLayout);
        mVerticalLayout->addChild(mWebView);

        mHorizontalLayout->addChild(mUrlField);
        mHorizontalLayout->addChild(mReloadButton);
        mHorizontalLayout->addChild(mAboutIcon);

        // Create and show the screen that holds the WebView.
        mScreen = new Screen();
        mScreen->setMainWidget(mVerticalLayout);
        mScreen->show();
    }

    /**
     * Display a page in the WebView of this moblet.
     * @param url   Url of page to open.
     */
    void showPage(const MAUtil::String& url) {
        // Open the page.
        getWebView()->openURL(url);

        // Test, runs custom HTML and Javascript in the webview.
        /*
            getWebView()->setHtml("<html>hello world!</html>");
            getWebView()->callJS("document.body.innerHTML = 'ciao';");
            getWebView()->callJS("var color = '#' + (Math.random() * 0xFFFFFF + 0x1000000).toString(16).substr(1,6); document.body.style.background = color;");
         */
    }

    /**
     * React upon a click on the button reload.
     * @param url   Url of page to open.
     */
    void buttonClicked(Widget* button) {
        if (button == mReloadButton) {
            String url = mUrlField->getText();
            if (url == DEFAULT_URL) {
                // invalid url
                return;
            }

            // Save the server address on file.
            mFileUtil->writeTextToFile(
                    mFileUtil->getLocalPath() + "LastUrl.txt",
                    url);

            showPage(url);
            mUrlField->hideKeyboard();

        } else if (button == mAboutIcon) {
            //Show the info screen
            maMessageBox("MoSync Live", "Load web pages live via jsbin.com");
        }
    }

    //void editBoxEditingDidBegin(EditBox* editBox) {
        // move the vertical layout up so that it is
        // visible when the keyboard shows
        //mVerticalLayout->setTopPosition(0)
    //}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain() {
    (new MyMoblet())->enterEventLoop();
    return 0;
}

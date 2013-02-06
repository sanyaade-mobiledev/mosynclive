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
#include <NativeUI/Button.h>
#include <NativeUI/ButtonListener.h>
#include <MAUtil/String.h>

// Namespaces we want to access.
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.
using namespace MAUtil;

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet, public ButtonListener
{
	VerticalLayout *mVerticalLayout;
	HorizontalLayout *mHorizontalLayout;
	Button *mReloadButton;
	EditBox *mUrlField;
	String url;

public:
	MyMoblet() {
		url = "http://jsbin.com/ebidab/1";

		initialize();

		// Show the start page.
		showPage(url); // url jsbin

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		setBeepSound(BEEP_WAV);
	}

	void initialize() {
//		if (mInitialized) { return; }
//
//		mInitialized = true;

		createUI();

		// Extract files in LocalFiles folder to the device.
		extractFileSystem();

		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		// Initialize the message handler. All messages from
		// JavaScript are routed through this handler.
		mMessageHandler->initialize(this);
	}

	/**
	 * Creates the main UI elements, but does not connect them.
	 */
	void createUI()	{
		// Create and configure the WebView.
		mWebView = new WebView();
		mWebView -> fillSpaceHorizontally();
		mWebView -> fillSpaceVertically();

		mVerticalLayout = new VerticalLayout();
		mVerticalLayout -> fillSpaceHorizontally();
		mVerticalLayout -> fillSpaceVertically();

		mHorizontalLayout = new HorizontalLayout();
		mHorizontalLayout -> fillSpaceHorizontally();
		mHorizontalLayout -> fillSpaceVertically();
		mHorizontalLayout -> wrapContentVertically();

		mUrlField = new EditBox();
		mUrlField -> setText(url);
		mUrlField -> fillSpaceHorizontally();

		mReloadButton = new Button();
		mReloadButton -> setText("Reload");
		mReloadButton -> addButtonListener(this);

		mVerticalLayout -> addChild(mWebView);
		mVerticalLayout -> addChild(mHorizontalLayout);

		mHorizontalLayout -> addChild(mUrlField);
		mHorizontalLayout -> addChild(mReloadButton);

		// Create and show the screen that holds the WebView.
		mScreen = new Screen();
		mScreen -> setMainWidget(mVerticalLayout);
		mScreen -> show();
}

	/**
	 * Display a page in the WebView of this moblet.
	 * @param url Url of page to open.
	 */
	void showPage(const MAUtil::String& url) {
		// Extract files system and perform other initialisation.
		// initialize();

		// Open the page.
		getWebView()->openURL(url);
	}

    void buttonClicked(Widget* button) {
		showPage(mUrlField -> getText()); // url jsbin
    }

};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new MyMoblet())->enterEventLoop();
	return 0;
}

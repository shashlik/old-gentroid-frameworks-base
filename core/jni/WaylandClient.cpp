#include <assert.h>
#include <string.h>
#include <signal.h>
#include <utils/Log.h>
#include "WaylandClient.h"


namespace android {

wl_callback_listener WaylandWindow::mFrameListener = {
	WaylandWindow::sRedraw
};



void WaylandWindow::init()
{
    mClient = WaylandClient::getInstance();
    mSurface = wl_compositor_create_surface(mClient->getCompositor());
    mShellSurface = wl_shell_get_shell_surface(mClient->getShell(), mSurface);
    if (mShellSurface) {
        wl_shell_surface_add_listener(mShellSurface, &mShellSurfaceListener, this);
        wl_shell_surface_set_toplevel(mShellSurface);
    } else {
       ALOGW("wl_shell_get_shell_surface FAILED \n");
       return;
    }

    wl_surface_set_user_data(mSurface, this);
    wl_shell_surface_set_title(mShellSurface, "android");
    wl_surface_commit(mSurface);
    mNative = wl_egl_window_create(mSurface, mWidth, mHeight);
    if (mNative == NULL) {
        ALOGW("wl_egl_window_create FAILED \n");
        return;
    } else {
        ALOGW("wl_egl_window_create %p \n", mNative);
    }
}

WaylandClient *WaylandClient::sInstance = NULL;

wl_registry_listener WaylandClient::mRegistryListener = {
    WaylandClient::sHandleRegistry,
    WaylandClient::sHandleRegistryRemove };

wl_seat_listener WaylandClient::mSeatListener = {
    WaylandClient::sSeatHandleCapabilities };

wl_pointer_listener WaylandClient::mPointerListener = {
    WaylandClient::sPointerHandleEnter,
    WaylandClient::sPointerHandleLeave,
    WaylandClient::sPointerHandleMotion,
    WaylandClient::sPointerHandleButton,
    WaylandClient::sPointerHandleAxis };

wl_keyboard_listener WaylandClient::mKeyboardListener = {
    WaylandClient::sKeyboardHandleKeymap,
    WaylandClient::sKeyboardHandleEnter,
    WaylandClient::sKeyboardHandleLeave,
    WaylandClient::sKeyboardHandleKey,
    WaylandClient::sKeyboardHandleModifiers };

wl_touch_listener WaylandClient::mTouchListener = {
	WaylandClient::sTouchHandleDown,
	WaylandClient::sTouchHandleUp,
	WaylandClient::sTouchHandleMotion,
	WaylandClient::sTouchHandleFrame,
	WaylandClient::sTouchHandleCancel,
};

wl_shell_surface_listener WaylandWindow::mShellSurfaceListener = {
	WaylandWindow::sShellHandlePing,
	WaylandWindow::sShellHandleConfigure,
	WaylandWindow::sShellHandlePopup
};


WaylandClient::WaylandClient() : mDisplay(NULL), mRegistry(NULL),
        mCompositor(NULL), mSeat(NULL), mPointer(NULL), mTouch(NULL),
        mKeyboard(NULL), mShell(NULL), mShm(NULL), mCursorTheme(NULL),
        mDefaultCursor(NULL)
{
}


WaylandClient *WaylandClient::getInstance()
{
    if (sInstance == NULL ) {
        sInstance = new WaylandClient();
    }

    return sInstance;
}

wl_compositor* WaylandClient::getCompositor()
{
    if (mDisplay == NULL)
        ALOGW("WaylandClient not connected\n");

    return mCompositor;
}

wl_shell* WaylandClient::getShell()
{
    if (mDisplay == NULL)
        ALOGW("WaylandClient not connected\n");

    return mShell;
}

wl_display* WaylandClient::getDisplay()
{
    if (mDisplay == NULL)
        ALOGW("WaylandClient not connected\n");

    return mDisplay;
}

wl_event_queue* WaylandClient::getEventQueue()
{
    if (mDisplay == NULL)
        ALOGW("WaylandClient not connected\n");

    return mEventQueue;
}


WaylandWindow *WaylandClient::createWindow(int width, int height)
{
        WaylandWindow *window = new WaylandWindow(width, height);
    	window->init();
    	return window;
}

void WaylandClient::connect()
{
    struct sigaction sigint;
    assert(mDisplay == NULL);
	mDisplay = wl_display_connect(NULL);
	if (mDisplay == NULL) {
	    ALOGW("wl_display_connect FAILED\n");
	    return;
	}

	mRegistry = wl_display_get_registry(mDisplay);

	wl_registry_add_listener(mRegistry,
				 &mRegistryListener, this);
	wl_display_dispatch(mDisplay);
	mEventQueue = wl_display_create_queue(mDisplay);
}

void WaylandClient::disconnect()
{
	if (mCursorTheme)
		wl_cursor_theme_destroy(mCursorTheme);

	if (mCompositor)
		wl_compositor_destroy(mCompositor);

	wl_registry_destroy(mRegistry);
	wl_display_flush(mDisplay);
	wl_display_disconnect(mDisplay);
	wl_event_queue_destroy(mEventQueue);
	mDisplay = NULL;
}

void WaylandWindow::sShellHandlePing(void *data, wl_shell_surface *surface,
	    uint32_t serial)
{
	wl_shell_surface_pong(surface, serial);
}

void WaylandWindow::sShellHandleConfigure(void *data, wl_shell_surface *surface,
		 uint32_t edges, int32_t width, int32_t height)
{
}

void WaylandWindow::sShellHandlePopup(void *data, wl_shell_surface *surface)
{
}


void WaylandClient::sTouchHandleDown(void *data, wl_touch *touch,
		  uint32_t serial, uint32_t time, wl_surface *surface,
		  int32_t id, wl_fixed_t x_w, wl_fixed_t y_w)
{
}

void WaylandClient::sTouchHandleUp(void *data, wl_touch *touch,
		uint32_t serial, uint32_t time, int32_t id)
{
}

void WaylandClient::sTouchHandleMotion(void *data, wl_touch *touch,
		    uint32_t time, int32_t id, wl_fixed_t x_w, wl_fixed_t y_w)
{
}

void WaylandClient::sTouchHandleFrame(void *data, wl_touch *touch)
{
}

void WaylandClient::sTouchHandleCancel(void *data, wl_touch *touch)
{
}


void WaylandClient::sKeyboardHandleKeymap(void *data, wl_keyboard *keyboard,
		       uint32_t format, int fd, uint32_t size)
{
}


void WaylandClient::sKeyboardHandleEnter(void *data, wl_keyboard *keyboard,
		      uint32_t serial, wl_surface *surface, wl_array *keys)
{
}

void WaylandClient::sKeyboardHandleLeave(void *data, wl_keyboard *keyboard,
		      uint32_t serial, wl_surface *surface)
{
}

void WaylandClient::sKeyboardHandleKey(void *data, wl_keyboard *keyboard,
		    uint32_t serial, uint32_t time, uint32_t key,
		    uint32_t state)
{
}

void WaylandClient::sKeyboardHandleModifiers(void *data, wl_keyboard *keyboard,
			  uint32_t serial, uint32_t mods_depressed,
			  uint32_t mods_latched, uint32_t mods_locked,
			  uint32_t group)
{
}


void WaylandClient::sHandleRegistry(void * data, wl_registry *registry,
		       uint32_t name, const char *interface, uint32_t version)
{
    WaylandClient *client = reinterpret_cast<WaylandClient*>(data);
    client->handleRegistry(registry, name, interface, version);
}



void WaylandClient::sSeatHandleCapabilities(void *data, wl_seat *seat,
			 uint32_t caps)
{
    WaylandClient *client = reinterpret_cast<WaylandClient*>(data);
    client->seatHandleCapabilities(seat, (wl_seat_capability)caps);
}

void WaylandClient::seatHandleCapabilities(wl_seat *seat,
			 enum wl_seat_capability caps)
{
	if ((caps & WL_SEAT_CAPABILITY_POINTER) && !mPointer) {
		mPointer = wl_seat_get_pointer(mSeat);
		wl_pointer_add_listener(mPointer, &mPointerListener, this);
	} else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && mPointer) {
		wl_pointer_destroy(mPointer);
		mPointer = NULL;
	}

	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !mKeyboard) {
		mKeyboard = wl_seat_get_keyboard(mSeat);
		wl_keyboard_add_listener(mKeyboard, &mKeyboardListener, this);
	} else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && mKeyboard) {
		wl_keyboard_destroy(mKeyboard);
		mKeyboard = NULL;
	}

	if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !mTouch) {
		mTouch = wl_seat_get_touch(mSeat);
		wl_touch_set_user_data(mTouch, this);
		wl_touch_add_listener(mTouch, &mTouchListener, this);
	} else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && mTouch) {
		wl_touch_destroy(mTouch);
		mTouch = NULL;
	}
}

void WaylandClient::sHandleRegistryRemove(void * data, wl_registry *registry,
		       uint32_t name)
{
    WaylandClient *client = reinterpret_cast<WaylandClient*>(data);
    client->handleRegistryRemove(registry, name);
}

void WaylandClient::handleRegistry(wl_registry *registry,
		       uint32_t name, const char *interface, uint32_t version)
{

	if (strcmp(interface, "wl_compositor") == 0) {
		mCompositor = (wl_compositor*)wl_registry_bind(registry, name,
					 &wl_compositor_interface, 1);
	} else if (strcmp(interface, "wl_seat") == 0) {
		mSeat = (wl_seat*)wl_registry_bind(registry, name,
					   &wl_seat_interface, 1);
		wl_seat_add_listener(mSeat, &mSeatListener, this);
	} else if (strcmp(interface, "wl_shell") == 0) {
      	mShell = (wl_shell*)wl_registry_bind(registry, name,
      					 &wl_shell_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
		mShm = (wl_shm*)wl_registry_bind(registry, name,
					  &wl_shm_interface, 1);
		mCursorTheme = wl_cursor_theme_load(NULL, 32, mShm);
		if (!mCursorTheme) {
			fprintf(stderr, "unable to load default theme\n");
			return;
		}
		mDefaultCursor =
			wl_cursor_theme_get_cursor(mCursorTheme, "left_ptr");
		if (!mDefaultCursor) {
			fprintf(stderr, "unable to load default left pointer\n");
			// TODO: abort ?
		}
	}

}

void WaylandClient::handleRegistryRemove(wl_registry *registry,
			      uint32_t name)
{
}

void WaylandClient::sPointerHandleEnter(void *data, wl_pointer *pointer,
		     uint32_t serial, wl_surface *surface,
		     wl_fixed_t sx, wl_fixed_t sy)
{
}

void WaylandClient::sPointerHandleLeave(void *data, wl_pointer *pointer,
		     uint32_t serial, wl_surface *surface)
{
}

void WaylandClient::sPointerHandleMotion(void *data, wl_pointer *pointer,
		      uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
}

void WaylandClient::sPointerHandleButton(void *data, wl_pointer *wl_pointer,
		      uint32_t serial, uint32_t time, uint32_t button,
		      uint32_t state)
{
}

void WaylandClient::sPointerHandleAxis(void *data, wl_pointer *wl_pointer,
		    uint32_t time, uint32_t axis, wl_fixed_t value)
{
}

void WaylandWindow::sRedraw(void *data, wl_callback *callback, uint32_t time)
{
    WaylandWindow *window = reinterpret_cast<WaylandWindow*>(data);
    window->redraw(callback, time);
}

void WaylandWindow::redraw(wl_callback *callback, uint32_t time)
{

}

};
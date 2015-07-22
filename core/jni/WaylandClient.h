#ifndef _ANDROID_WAYLANDCLIENT_H
#define _ANDROID_WAYLANDCLIENT_H


#include <stdio.h>
#include <EGL/eglplatform.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>



namespace android {


class WaylandClient;

class WaylandWindow
{
friend WaylandClient;
public:
    static void sRedraw(void *data, wl_callback *callback, uint32_t time);
    void redraw(wl_callback *callback, uint32_t time);
    wl_egl_window* getNative() { return mNative; }
private:
    WaylandClient *mClient;
    static wl_shell_surface_listener mShellSurfaceListener;
    static wl_callback_listener mFrameListener;
    wl_surface *mSurface;
    wl_shell_surface *mShellSurface;
    wl_egl_window *mNative;
    int mWidth;
    int mHeight;
    wl_callback *mCallback;

    WaylandWindow(int width, int height) : mWidth(width), mHeight(height) {}
    void init();

    static void sShellHandlePing(void *data, wl_shell_surface *surface,
        	    uint32_t serial);
    static void sShellHandleConfigure(void *data, wl_shell_surface *surface,
        		 uint32_t edges, int32_t width, int32_t height);
    static void sShellHandlePopup(void *data, wl_shell_surface *surface);

};


class WaylandClient
{
public:
    static WaylandClient *getInstance();
    void connect();
    void disconnect();
    WaylandWindow* createWindow(int width, int height);
    wl_compositor* getCompositor();
    wl_shell* getShell();
    wl_display* getDisplay();
    wl_event_queue* getEventQueue();
private:
    wl_event_queue *mEventQueue;
    static WaylandClient *sInstance;
	wl_display *mDisplay;
	wl_registry *mRegistry;
	wl_compositor *mCompositor;
	wl_seat *mSeat;
	wl_pointer *mPointer;
	wl_touch *mTouch;
	wl_keyboard *mKeyboard;
    wl_shell *mShell;
	wl_shm *mShm;
	wl_cursor_theme *mCursorTheme;
	wl_cursor *mDefaultCursor;

    static wl_registry_listener mRegistryListener;
    static wl_seat_listener mSeatListener;
    static wl_pointer_listener mPointerListener;
    static wl_keyboard_listener mKeyboardListener;
    static wl_touch_listener mTouchListener;


	WaylandClient (WaylandClient &) { assert(0); }
    WaylandClient & operator = (const WaylandClient &) { assert(0); }
	WaylandClient();


	static void sHandleRegistry(void * data, wl_registry *registry,
		       uint32_t name, const char *interface, uint32_t version);
	static void sHandleRegistryRemove(void * data, wl_registry *registry,
		       uint32_t name);

    static void sSeatHandleCapabilities(void *data, wl_seat *seat, uint32_t caps);

    static void sPointerHandleEnter(void *data, wl_pointer *pointer,
    		     uint32_t serial, wl_surface *surface,
    		     wl_fixed_t sx, wl_fixed_t sy);
    static void sPointerHandleLeave(void *data, wl_pointer *pointer,
    		     uint32_t serial, wl_surface *surface);
    static void sPointerHandleMotion(void *data, wl_pointer *pointer,
    		      uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
    static void sPointerHandleButton(void *data, wl_pointer *pointer,
    		      uint32_t serial, uint32_t time, uint32_t button,
    		      uint32_t state);
    static void sPointerHandleAxis(void *data, wl_pointer *pointer,
    		    uint32_t time, uint32_t axis, wl_fixed_t value);

    static void sKeyboardHandleKeymap(void *data, wl_keyboard *keyboard,
                                     		       uint32_t format, int fd, uint32_t size);
    static void sKeyboardHandleEnter(void *data, wl_keyboard *keyboard,
    		      uint32_t serial, wl_surface *surface, wl_array *keys);
    static void sKeyboardHandleLeave(void *data, wl_keyboard *keyboard,
    		      uint32_t serial, wl_surface *surface);
    static void sKeyboardHandleKey(void *data, wl_keyboard *keyboard,
    		    uint32_t serial, uint32_t time, uint32_t key,
    		    uint32_t state);
    static void sKeyboardHandleModifiers(void *data, wl_keyboard *keyboard,
    			  uint32_t serial, uint32_t mods_depressed,
    			  uint32_t mods_latched, uint32_t mods_locked,
    			  uint32_t group);

    static void sTouchHandleDown(void *data, wl_touch *touch,
    		  uint32_t serial, uint32_t time, wl_surface *surface,
    		  int32_t id, wl_fixed_t x_w, wl_fixed_t y_w);
    static void sTouchHandleUp(void *data, wl_touch *touch,
    		uint32_t serial, uint32_t time, int32_t id);
    static void sTouchHandleMotion(void *data, wl_touch *touch,
    		    uint32_t time, int32_t id, wl_fixed_t x_w, wl_fixed_t y_w);
    static void sTouchHandleFrame(void *data, wl_touch *touch);
    static void sTouchHandleCancel(void *data, wl_touch *touch);

    void handleRegistry(wl_registry *registry,
    		       uint32_t name, const char *interface, uint32_t version);
    void handleRegistryRemove(wl_registry *registry, uint32_t name);
    void seatHandleCapabilities(wl_seat *seat, enum wl_seat_capability caps);
};

}

#endif
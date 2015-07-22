/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.view;

import android.view.IWindowId;
import com.android.internal.view.IInputContext;
import com.android.internal.view.IInputMethodClient;
import com.android.internal.view.IInputMethodManager;

import android.content.ClipData;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.graphics.Region;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Parcel;
import android.os.Process;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.UserHandle;
import android.util.Slog;
import android.view.Display;
import android.view.IWindow;
import android.view.IWindowSession;
import android.view.InputChannel;
import android.view.Surface;
import android.view.WindowManager;
import android.view.WindowManagerLocal;

import java.io.PrintWriter;

/**
 * This class represents an active client session.  There is generally one
 * Session object per process that is interacting with the window manager.
 */

public final class WindowSessionLocal {
    static WindowSessionLocal sWindowSessionLocal;
    final WindowManagerLocal mService =  WindowManagerLocal.getInstance();
    final String mStringName = new String("WindowSessionLocal");
    int mNumWindow = 0;
    boolean mClientDead = false;

    private WindowSessionLocal() {
    }

    public static WindowSessionLocal getInstance() {
        if (sWindowSessionLocal == null) {
            sWindowSessionLocal = new WindowSessionLocal();
        }
        return sWindowSessionLocal;

    }

    public void freezeRotation(int rotation) {

    }

    /**
     * Thaw rotation changes.  (Disable "rotation lock".)
     */
    public void thawRotation() {

    }

    public void updateRotationUnchecked(boolean alwaysSendConfiguration, boolean forceRelayout) {

    }

    public void getDisplayFrame(IWindow window, Rect outDisplayFrame) { }

    public IWindowId getWindowId(IBinder token) {
        return null;
    }

    public IBinder prepareDrag(IWindow window, int flags,
                               int width, int height, Surface outSurface) {
        return null;
    }

    public boolean performDrag(IWindow window, IBinder dragToken,
                               float touchX, float touchY, float thumbCenterX, float thumbCenterY,
                               ClipData data) { return false; }

    void setTransparentRegion(IWindow window, Region region) { }

    public void setInsets(IWindow window, int touchableInsets,
                          Rect contentInsets, Rect visibleInsets, Region touchableArea) { }

    public boolean outOfMemory(IWindow window) {
        return false;
    }

    public void finishDrawing(IWindow window) {  }

    public void setInTouchMode(boolean mode) { }

    public void dragRecipientEntered(IWindow window) { }

    public void dragRecipientExited(IWindow window) { }

    public int relayout(IWindow window, int seq, WindowManager.LayoutParams attrs,
                        int requestedWidth, int requestedHeight, int viewFlags,
                        int flags, Rect outFrame, Rect outOverscanInsets, Rect outContentInsets,
                        Rect outVisibleInsets, Configuration outConfig, Surface outSurface) { return 0; }

    public void reportDropResult(IWindow window, boolean consumed) { }

    public boolean getInTouchMode() {
       return false;
    }

    public int addToDisplay(IWindow window, int seq, WindowManager.LayoutParams attrs,
                            int viewVisibility, int displayId, Rect outContentInsets,
                            InputChannel outInputChannel) { return WindowManagerLocal.ADD_OKAY; }

    public void remove(IWindow window) { }

    public boolean performHapticFeedback(IWindow window, int effectId,
                                         boolean always) { return false; }

    public void onRectangleOnScreenRequested(IBinder token, Rect rectangle, boolean immediate) { }


    public int addToDisplayWithoutInputChannel(IWindow window, int seq, WindowManager.LayoutParams attrs,
                                               int viewVisibility, int displayId, Rect outContentInsets) {
        return 0;
    }

    public void performDeferredDestroy(IWindow window) { }
}
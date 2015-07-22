/*
 * Copyright (C) 2012 The Android Open Source Project
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

package android.hardware.display;

import android.content.Context;
import android.hardware.display.DisplayManager.DisplayListener;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.text.TextUtils;
import android.util.Log;
import android.util.SparseArray;
import android.view.DisplayAdjustments;
import android.view.Display;
import android.view.DisplayInfo;
import android.view.Surface;

import java.util.ArrayList;

/**
 * Manager communication with the display manager service on behalf of
 * an application process.  You're probably looking for {@link DisplayManager}.
 *
 * @hide
 */
public final class DisplayManagerLocal {
    private static final String TAG = "DisplayManager";
    private static final boolean DEBUG = false;


    private static DisplayManagerLocal sInstance;

    private final SparseArray<DisplayInfo> mDisplayInfoCache = new SparseArray<DisplayInfo>();
    private final SparseArray<Display> mDisplays = new SparseArray<Display>();


    private DisplayManagerLocal() {
    }

    /**
     * Gets an instance of the display manager global singleton.
     *
     * @return The display manager instance, may be null early in system startup
     * before the display manager has been fully initialized.
     */
    public static DisplayManagerLocal getInstance() {
        synchronized (DisplayManagerLocal.class) {
            if (sInstance != null)
                return sInstance;
            sInstance = new DisplayManagerLocal();
            return sInstance;
        }
    }

    /**
     * Get information about a particular logical display.
     *
     * @param displayId The logical display id.
     * @return Information about the specified display, or null if it does not exist.
     * This object belongs to an internal cache and should be treated as if it were immutable.
     */
    public DisplayInfo getDisplayInfo(int displayId) {
        DisplayInfo info = new DisplayInfo();
        info.layerStack = 0;
        info.flags = 0;
        info.type = 0;
        info.address = "";
        info.name = "main";
        info.appWidth = 480;
        info.appHeight = 800;
        info.smallestNominalAppWidth = 480;
        info.smallestNominalAppHeight = 800;
        info.largestNominalAppWidth = 480;
        info.largestNominalAppHeight = 800;
        info.logicalWidth = 480;
        info.logicalHeight = 800;
        info.overscanLeft = 0;
        info.overscanTop = 0;
        info.overscanRight = 0;
        info.overscanBottom = 0;
        info.rotation = 0;
        info.refreshRate = 60.0f;
        info.logicalDensityDpi = 233;
        info.physicalXDpi = 233;
        info.physicalYDpi = 233;
        info.ownerUid = 0;
        info.ownerPackageName = "";
        return info;
    }

    /**
     * Gets all currently valid logical display ids.
     *
     * @return An array containing all display ids.
     */
    public int[] getDisplayIds() {
        int[] ids = new int[1];
        ids[0] = 0;
        return ids;
    }

    /**
     * Gets information about a logical display.
     * <p/>
     * The display metrics may be adjusted to provide compatibility
     * for legacy applications or limited screen areas.
     *
     * @param displayId The logical display id.
     * @param daj       The compatibility info and activityToken.
     * @return The display object, or null if there is no display with the given id.
     */
    public Display getCompatibleDisplay(int displayId, DisplayAdjustments daj) {
        DisplayInfo displayInfo = getDisplayInfo(displayId);
        if (displayInfo == null) {
            return null;
        }
        return new Display(this, displayId, displayInfo, daj);
    }

    /**
     * Gets information about a logical display without applying any compatibility metrics.
     *
     * @param displayId The logical display id.
     * @return The display object, or null if there is no display with the given id.
     */
    public Display getRealDisplay(int displayId) {
        return getCompatibleDisplay(displayId, DisplayAdjustments.DEFAULT_DISPLAY_ADJUSTMENTS);
    }

    /**
     * Gets information about a logical display without applying any compatibility metrics.
     *
     * @param displayId The logical display id.
     * @param IBinder   the activity token for this display.
     * @return The display object, or null if there is no display with the given id.
     */
    public Display getRealDisplay(int displayId, IBinder token) {
        return getCompatibleDisplay(displayId, new DisplayAdjustments(token));
    }

    public void registerDisplayListener(DisplayListener listener, Handler handler) {
    }

    public void unregisterDisplayListener(DisplayListener listener) {
    }

    public void startWifiDisplayScan() {
    }

    public void stopWifiDisplayScan() {
    }

    public void connectWifiDisplay(String deviceAddress) {
    }

    public void pauseWifiDisplay() {
    }

    public void resumeWifiDisplay() {
    }

    public void disconnectWifiDisplay() {
    }

    public void renameWifiDisplay(String deviceAddress, String alias) {
    }

    public void forgetWifiDisplay(String deviceAddress) {
    }

    public WifiDisplayStatus getWifiDisplayStatus() {
        return new WifiDisplayStatus();
    }

    public VirtualDisplay createVirtualDisplay(Context context, String name,
                                               int width, int height, int densityDpi, Surface surface, int flags) {
        if (TextUtils.isEmpty(name)) {
            throw new IllegalArgumentException("name must be non-null and non-empty");
        }
        if (width <= 0 || height <= 0 || densityDpi <= 0) {
            throw new IllegalArgumentException("width, height, and densityDpi must be "
                    + "greater than 0");
        }
        if (surface == null) {
            throw new IllegalArgumentException("surface must not be null");
        }

        return null;

    }

    public void releaseVirtualDisplay(IBinder token) {
    }
};


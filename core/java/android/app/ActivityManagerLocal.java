/*
 * Copyright (C) 2006 The Android Open Source Project
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

package android.app;

import android.app.ActivityManager.StackBoxInfo;
import android.content.ComponentName;
import android.content.IIntentReceiver;
import android.content.IIntentSender;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.IntentSender;
import android.content.UriPermission;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.IPackageDataObserver;
import android.content.pm.ParceledListSlice;
import android.content.pm.UserInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Debug;
import android.os.IBinder;
import android.os.ParcelFileDescriptor;
import android.os.Parcelable;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.StrictMode;
import android.text.TextUtils;
import android.util.Log;
import android.util.Singleton;

import java.util.ArrayList;
import java.util.List;

/** {@hide} */
public class ActivityManagerLocal implements IActivityManager
{


    /**
     * Retrieve the system's default/global activity manager.
     */
    static public IActivityManager getDefault() {
        return gDefault.get();
    }


    public ActivityManagerLocal() {

    }

    private static final Singleton<IActivityManager> gDefault = new Singleton<IActivityManager>() {
        protected IActivityManager create() {
            return new ActivityManagerLocal();
        }
    };


    public IBinder asBinder() {
        return null;
    }

    @Override
    public int startActivity(IApplicationThread caller, String callingPackage, Intent intent,
                             String resolvedType, IBinder resultTo, String resultWho, int requestCode,
                             int startFlags, String profileFile,
                             ParcelFileDescriptor profileFd, Bundle options) throws RemoteException { return -1; }

    @Override
    public int startActivityAsUser(IApplicationThread caller, String callingPackage, Intent intent,
                                   String resolvedType, IBinder resultTo, String resultWho, int requestCode,
                                   int startFlags, String profileFile,
                                   ParcelFileDescriptor profileFd, Bundle options, int userId) throws RemoteException { return -1; }

    @Override
    public WaitResult startActivityAndWait(IApplicationThread caller, String callingPackage,
                                           Intent intent, String resolvedType, IBinder resultTo, String resultWho,
                                           int requestCode, int startFlags, String profileFile,
                                           ParcelFileDescriptor profileFd, Bundle options, int userId) throws RemoteException {
        WaitResult res = null;
        return res;
    }

    @Override
    public int startActivityWithConfig(IApplicationThread caller, String callingPackage,
                                       Intent intent, String resolvedType, IBinder resultTo, String resultWho,
                                       int requestCode, int startFlags, Configuration config,
                                       Bundle options, int userId) throws RemoteException { return -1; }

    @Override
    public int startActivityIntentSender(IApplicationThread caller,
                                         IntentSender intent, Intent fillInIntent, String resolvedType,
                                         IBinder resultTo, String resultWho, int requestCode,
                                         int flagsMask, int flagsValues, Bundle options) throws RemoteException { return -1; }

    @Override
    public boolean startNextMatchingActivity(IBinder callingActivity,
                                             Intent intent, Bundle options) throws RemoteException { return false; }

    @Override
    public boolean finishActivity(IBinder token, int resultCode, Intent resultData)
            throws RemoteException { return false; }

    @Override
    public void finishSubActivity(IBinder token, String resultWho, int requestCode) throws RemoteException { }

    @Override
    public boolean finishActivityAffinity(IBinder token) throws RemoteException { return false; }

    @Override
    public boolean willActivityBeVisible(IBinder token) throws RemoteException { return false; }

    @Override
    public Intent registerReceiver(IApplicationThread caller, String packageName,
                                   IIntentReceiver receiver,
                                   IntentFilter filter, String perm, int userId) throws RemoteException {

        Intent intent = null;
        return intent;
    }

    @Override
    public void unregisterReceiver(IIntentReceiver receiver) throws RemoteException { }

    @Override
    public int broadcastIntent(IApplicationThread caller,
                               Intent intent, String resolvedType,  IIntentReceiver resultTo,
                               int resultCode, String resultData, Bundle map,
                               String requiredPermission, int appOp, boolean serialized,
                               boolean sticky, int userId) throws RemoteException { return -1; }

    @Override
    public void unbroadcastIntent(IApplicationThread caller, Intent intent, int userId)
            throws RemoteException { }

    @Override
    public void finishReceiver(IBinder who, int resultCode, String resultData, Bundle map, boolean abortBroadcast) throws RemoteException { }

    @Override
    public void attachApplication(IApplicationThread app) throws RemoteException { }

    @Override
    public void activityIdle(IBinder token, Configuration config, boolean stopProfiling)
            throws RemoteException { }

    @Override
    public void activityResumed(IBinder token) throws RemoteException { }

    @Override
    public void activityPaused(IBinder token) throws RemoteException { }

    @Override
    public void activityStopped(IBinder token, Bundle state,
                                Bitmap thumbnail, CharSequence description) throws RemoteException { }

    @Override
    public void activitySlept(IBinder token) throws RemoteException { }

    @Override
    public void activityDestroyed(IBinder token) throws RemoteException { }

    @Override
    public String getCallingPackage(IBinder token) throws RemoteException {
        String res = null;
        return res;
    }

    @Override
    public ComponentName getCallingActivity(IBinder token)
            throws RemoteException {
        ComponentName res = null;
        return res;
    }

    @Override
    public List getTasks(int maxNum, int flags,
                         IThumbnailReceiver receiver) throws RemoteException {
        ArrayList list = null;
        return list;
    }

    @Override
    public List<ActivityManager.RecentTaskInfo> getRecentTasks(int maxNum,
                                                               int flags, int userId) throws RemoteException {

        ArrayList<ActivityManager.RecentTaskInfo> list = null;
        return list;
    }

    @Override
    public ActivityManager.TaskThumbnails getTaskThumbnails(int id) throws RemoteException {
        ActivityManager.TaskThumbnails bm = null;
        return bm;
    }

    @Override
    public Bitmap getTaskTopThumbnail(int id) throws RemoteException {
        Bitmap bm = null;
        return bm;
    }

    @Override
    public List getServices(int maxNum, int flags) throws RemoteException {
        ArrayList list = null;
        return list;
    }

    @Override
    public List<ActivityManager.ProcessErrorStateInfo> getProcessesInErrorState()
            throws RemoteException {

        ArrayList<ActivityManager.ProcessErrorStateInfo> list  = null;
        return list;
    }

    @Override
    public List<ActivityManager.RunningAppProcessInfo> getRunningAppProcesses()
            throws RemoteException {
        ArrayList<ActivityManager.RunningAppProcessInfo> list = null;
        return list;
    }

    @Override
    public List<ApplicationInfo> getRunningExternalApplications()
            throws RemoteException {
        ArrayList<ApplicationInfo> list = null;
        return list;
    }

    @Override
    public void moveTaskToFront(int task, int flags, Bundle options) throws RemoteException { }

    @Override
    public void moveTaskToBack(int task) throws RemoteException { }

    @Override
    public boolean moveActivityTaskToBack(IBinder token, boolean nonRoot)
            throws RemoteException { return false; }

    public void moveTaskBackwards(int task) throws RemoteException  { }

    @Override
    public int createStack(int taskId, int relativeStackBoxId, int position, float weight)
            throws RemoteException { return -1; }

    @Override
    public void moveTaskToStack(int taskId, int stackId, boolean toTop) throws RemoteException { }

    @Override
    public void resizeStackBox(int stackBoxId, float weight) throws RemoteException { }

    @Override
    public List<StackBoxInfo> getStackBoxes() throws RemoteException {
        ArrayList<StackBoxInfo> list = null;
        return list;
    }

    @Override
    public StackBoxInfo getStackBoxInfo(int stackBoxId) throws RemoteException  {
        StackBoxInfo info = null;;
        return info;
    }

    @Override
    public void setFocusedStack(int stackId) throws RemoteException { }

    @Override
    public int getTaskForActivity(IBinder token, boolean onlyRoot) throws RemoteException { return -1; }

    @Override
    public void reportThumbnail(IBinder token,
                                Bitmap thumbnail, CharSequence description) throws RemoteException { }

    @Override
    public ContentProviderHolder getContentProvider(IApplicationThread caller,
                                                    String name, int userId, boolean stable) throws RemoteException {
        ContentProviderHolder cph = null;
        return cph;
    }

    @Override
    public ContentProviderHolder getContentProviderExternal(String name, int userId, IBinder token)
            throws RemoteException {
        ContentProviderHolder cph = null;
        return cph;
    }

    @Override
    public void publishContentProviders(IApplicationThread caller,
                                        List<ContentProviderHolder> providers) throws RemoteException { }

    @Override
    public boolean refContentProvider(IBinder connection, int stable, int unstable)
            throws RemoteException { return false; }

    @Override
    public void unstableProviderDied(IBinder connection) throws RemoteException { }

    @Override
    public void appNotRespondingViaProvider(IBinder connection) throws RemoteException { }

    @Override
    public void removeContentProvider(IBinder connection, boolean stable) throws RemoteException { }

    @Override
    public void removeContentProviderExternal(String name, IBinder token) throws RemoteException { }

    @Override
    public PendingIntent getRunningServiceControlPanel(ComponentName service)
            throws RemoteException {
        PendingIntent res = null;
        return res;
    }

    @Override
    public ComponentName startService(IApplicationThread caller, Intent service,
                                      String resolvedType, int userId) throws RemoteException {
        ComponentName res = null;
        return res;
    }

    @Override
    public int stopService(IApplicationThread caller, Intent service,
                           String resolvedType, int userId) throws RemoteException  { return -1; }

    @Override
    public boolean stopServiceToken(ComponentName className, IBinder token,
                                    int startId) throws RemoteException { return false; }

    @Override
    public void setServiceForeground(ComponentName className, IBinder token,
                                     int id, Notification notification, boolean removeNotification) throws RemoteException { }

    @Override
    public int bindService(IApplicationThread caller, IBinder token,
                           Intent service, String resolvedType, IServiceConnection connection,
                           int flags, int userId) throws RemoteException { return -1; }

    @Override
    public boolean unbindService(IServiceConnection connection) throws RemoteException { return false; }

    @Override
    public void publishService(IBinder token,
                               Intent intent, IBinder service) throws RemoteException { }

    @Override
    public void unbindFinished(IBinder token, Intent intent, boolean doRebind)
            throws RemoteException { }

    @Override
    public void serviceDoneExecuting(IBinder token, int type, int startId,
                                     int res) throws RemoteException { }

    @Override
    public IBinder peekService(Intent service, String resolvedType) throws RemoteException {
        IBinder binder = null;
        return binder;
    }

    @Override
    public boolean bindBackupAgent(ApplicationInfo app, int backupRestoreMode)
            throws RemoteException { return false; }

    @Override
    public void clearPendingBackup() throws RemoteException { }

    @Override
    public void backupAgentCreated(String packageName, IBinder agent) throws RemoteException { }

    @Override
    public void unbindBackupAgent(ApplicationInfo app) throws RemoteException { }

    @Override
    public boolean startInstrumentation(ComponentName className, String profileFile,
                                        int flags, Bundle arguments, IInstrumentationWatcher watcher,
                                        IUiAutomationConnection connection, int userId, String instructionSet)
            throws RemoteException { return false; }

    @Override
    public void finishInstrumentation(IApplicationThread target,
                                      int resultCode, Bundle results) throws RemoteException { }

    @Override
    public Configuration getConfiguration() throws RemoteException {
        Configuration res = null;
        return res;
    }

    @Override
    public void updateConfiguration(Configuration values) throws RemoteException { }

    @Override
    public void setRequestedOrientation(IBinder token, int requestedOrientation)
            throws RemoteException { }

    @Override
    public int getRequestedOrientation(IBinder token) throws RemoteException { return -1; }

    @Override
    public ComponentName getActivityClassForToken(IBinder token)
            throws RemoteException {
        ComponentName res = null;
        return res;
    }

    @Override
    public String getPackageForToken(IBinder token) throws RemoteException {
        String res = null;
        return res;
    }

    @Override
    public IIntentSender getIntentSender(int type,
                                         String packageName, IBinder token, String resultWho,
                                         int requestCode, Intent[] intents, String[] resolvedTypes, int flags,
                                         Bundle options, int userId) throws RemoteException {
        IIntentSender res = null;
        return res;
    }

    @Override
    public void cancelIntentSender(IIntentSender sender) throws RemoteException {  }

    @Override
    public String getPackageForIntentSender(IIntentSender sender) throws RemoteException {
        String res = null;
        return res;
    }

    @Override
    public int getUidForIntentSender(IIntentSender sender) throws RemoteException { return -1; }

    @Override
    public int handleIncomingUser(int callingPid, int callingUid, int userId, boolean allowAll,
                                  boolean requireFull, String name, String callerPackage) throws RemoteException { return -1; }

    @Override
    public void setProcessLimit(int max) throws RemoteException { }

    @Override
    public int getProcessLimit() throws RemoteException { return -1; }

    @Override
    public void setProcessForeground(IBinder token, int pid,
                                     boolean isForeground) throws RemoteException { }

    @Override
    public int checkPermission(String permission, int pid, int uid)
            throws RemoteException { return -1; }

    @Override
    public boolean clearApplicationUserData(final String packageName,
                                            final IPackageDataObserver observer, final int userId) throws RemoteException { return false; }

    @Override
    public int checkUriPermission(Uri uri, int pid, int uid, int mode)
            throws RemoteException { return -1; }

    @Override
    public void grantUriPermission(IApplicationThread caller, String targetPkg,
                                   Uri uri, int mode) throws RemoteException { }

    @Override
    public void revokeUriPermission(IApplicationThread caller, Uri uri,
                                    int mode) throws RemoteException { }

    @Override
    public void takePersistableUriPermission(Uri uri, int mode) throws RemoteException { }

    @Override
    public void releasePersistableUriPermission(Uri uri, int mode) throws RemoteException { }

    @Override
    public ParceledListSlice<UriPermission> getPersistedUriPermissions(
            String packageName, boolean incoming) throws RemoteException {
        final ParceledListSlice<UriPermission> perms = null;
        return perms;
    }

    @Override
    public void showWaitingForDebugger(IApplicationThread who, boolean waiting)
            throws RemoteException { }

    @Override
    public void getMemoryInfo(ActivityManager.MemoryInfo outInfo) throws RemoteException { }

    @Override
    public void unhandledBack() throws RemoteException { }

    @Override
    public ParcelFileDescriptor openContentUri(Uri uri) throws RemoteException {
        ParcelFileDescriptor pfd = null;
        return pfd;
    }

    @Override
    public void goingToSleep() throws RemoteException { }

    @Override
    public void wakingUp() throws RemoteException { }

    @Override
    public void setLockScreenShown(boolean shown) throws RemoteException { }

    @Override
    public void setDebugApp(
            String packageName, boolean waitForDebugger, boolean persistent)
            throws RemoteException { }

    @Override
    public void setAlwaysFinish(boolean enabled) throws RemoteException { }

    @Override
    public void setActivityController(IActivityController watcher) throws RemoteException { }

    @Override
    public void enterSafeMode() throws RemoteException { }

    @Override
    public void noteWakeupAlarm(IIntentSender sender) throws RemoteException { }

    @Override
    public boolean killPids(int[] pids, String reason, boolean secure) throws RemoteException { return false; }

    @Override
    public boolean killProcessesBelowForeground(String reason) throws RemoteException { return false; }

    @Override
    public void startRunning(String pkg, String cls, String action,
                             String indata) throws RemoteException { }

    @Override
    public boolean testIsSystemReady() {
        /* this base class version is never called */
        return true;
    }

    @Override
    public void handleApplicationCrash(IBinder app,
                                       ApplicationErrorReport.CrashInfo crashInfo) throws RemoteException { }

    @Override
    public boolean handleApplicationWtf(IBinder app, String tag,
                                        ApplicationErrorReport.CrashInfo crashInfo) throws RemoteException { return false; }

    @Override
    public void handleApplicationStrictModeViolation(IBinder app,
                                                     int violationMask,
                                                     StrictMode.ViolationInfo info) throws RemoteException { }

    @Override
    public void signalPersistentProcesses(int sig) throws RemoteException { }

    @Override
    public void killBackgroundProcesses(String packageName, int userId) throws RemoteException { }

    @Override
    public void killAllBackgroundProcesses() throws RemoteException { }

    @Override
    public void forceStopPackage(String packageName, int userId) throws RemoteException { }

    @Override
    public void getMyMemoryState(ActivityManager.RunningAppProcessInfo outInfo)
            throws RemoteException { }

    @Override
    public ConfigurationInfo getDeviceConfigurationInfo() throws RemoteException {
        ConfigurationInfo res = null;
        return res;
    }

    @Override
    public boolean profileControl(String process, int userId, boolean start,
                                  String path, ParcelFileDescriptor fd, int profileType) throws RemoteException { return false; }

    @Override
    public boolean shutdown(int timeout) throws RemoteException { return false; }

    @Override
    public void stopAppSwitches() throws RemoteException { }

    @Override
    public void resumeAppSwitches() throws RemoteException { }

    @Override
    public void killApplicationWithAppId(String pkg, int appid, String reason)
            throws RemoteException { }

    @Override
    public void closeSystemDialogs(String reason) throws RemoteException { }

    @Override
    public Debug.MemoryInfo[] getProcessMemoryInfo(int[] pids)
            throws RemoteException {
        Debug.MemoryInfo[] res = null;
        return res;
    }

    @Override
    public void killApplicationProcess(String processName, int uid) throws RemoteException { }

    @Override
    public void overridePendingTransition(IBinder token, String packageName,
                                          int enterAnim, int exitAnim) throws RemoteException { }

    @Override
    public boolean isUserAMonkey() throws RemoteException { return false; }

    @Override
    public void setUserIsMonkey(boolean monkey) throws RemoteException { }

    @Override
    public void finishHeavyWeightApp() throws RemoteException { }

    @Override
    public boolean convertFromTranslucent(IBinder token)
            throws RemoteException { return false; }

    @Override
    public boolean convertToTranslucent(IBinder token)
            throws RemoteException { return false; }

    @Override
    public void setImmersive(IBinder token, boolean immersive)
            throws RemoteException { }

    @Override
    public boolean isImmersive(IBinder token)
            throws RemoteException { return false; }

    @Override
    public boolean isTopActivityImmersive()
            throws RemoteException { return false; }

    @Override
    public void crashApplication(int uid, int initialPid, String packageName,
                                 String message) throws RemoteException { }

    @Override
    public String getProviderMimeType(Uri uri, int userId) throws RemoteException {
        String res = null;
        return res;
    }

    @Override
    public IBinder newUriPermissionOwner(String name)
            throws RemoteException {
        IBinder res = null;
        return res;
    }

    @Override
    public void grantUriPermissionFromOwner(IBinder owner, int fromUid, String targetPkg,
                                            Uri uri, int mode) throws RemoteException { }

    @Override
    public void revokeUriPermissionFromOwner(IBinder owner, Uri uri,
                                             int mode) throws RemoteException { }

    @Override
    public int checkGrantUriPermission(int callingUid, String targetPkg,
                                       Uri uri, int modeFlags) throws RemoteException { return -1; }

    @Override
    public boolean dumpHeap(String process, int userId, boolean managed,
                            String path, ParcelFileDescriptor fd) throws RemoteException { return false; }

    @Override
    public int startActivities(IApplicationThread caller, String callingPackage,
                               Intent[] intents, String[] resolvedTypes, IBinder resultTo,
                               Bundle options, int userId) throws RemoteException { return -1; }

    @Override
    public int getFrontActivityScreenCompatMode() throws RemoteException { return -1; }

    @Override
    public void setFrontActivityScreenCompatMode(int mode) throws RemoteException { }

    @Override
    public int getPackageScreenCompatMode(String packageName) throws RemoteException { return -1; }

    @Override
    public void setPackageScreenCompatMode(String packageName, int mode)
            throws RemoteException { }

    @Override
    public boolean getPackageAskScreenCompat(String packageName) throws RemoteException { return false; }

    @Override
    public void setPackageAskScreenCompat(String packageName, boolean ask)
            throws RemoteException { }

    @Override
    public boolean switchUser(int userid) throws RemoteException { return false; }

    @Override
    public int stopUser(int userid, IStopUserCallback callback) throws RemoteException { return -1; }

    @Override
    public UserInfo getCurrentUser() throws RemoteException {
        UserInfo userInfo = null;
        return userInfo;
    }

    @Override
    public boolean isUserRunning(int userid, boolean orStopping) throws RemoteException { return false; }

    @Override
    public int[] getRunningUserIds() throws RemoteException {
        int[] result = null;
        return result;
    }

    @Override
    public boolean removeSubTask(int taskId, int subTaskIndex) throws RemoteException { return false; }

    @Override
    public boolean removeTask(int taskId, int flags) throws RemoteException { return false; }

    @Override
    public void registerProcessObserver(IProcessObserver observer) throws RemoteException { }

    @Override
    public void unregisterProcessObserver(IProcessObserver observer) throws RemoteException { }

    @Override
    public boolean isIntentSenderTargetedToPackage(IIntentSender sender) throws RemoteException { return false; }

    @Override
    public boolean isIntentSenderAnActivity(IIntentSender sender) throws RemoteException { return false; }

    @Override
    public Intent getIntentForIntentSender(IIntentSender sender) throws RemoteException {
        Intent res =  null;
        return res;
    }

    @Override
    public void updatePersistentConfiguration(Configuration values) throws RemoteException { }

    @Override
    public long[] getProcessPss(int[] pids) throws RemoteException {
        long[] res = null;
        return res;
    }

    @Override
    public void showBootMessage(CharSequence msg, boolean always) throws RemoteException { }

    @Override
    public void dismissKeyguardOnNextActivity() throws RemoteException { }

    @Override
    public boolean targetTaskAffinityMatchesActivity(IBinder token, String destAffinity)
            throws RemoteException { return false; }

    @Override
    public boolean navigateUpTo(IBinder token, Intent target, int resultCode, Intent resultData)
            throws RemoteException { return false; }

    @Override
    public int getLaunchedFromUid(IBinder activityToken) throws RemoteException { return -1; }

    @Override
    public String getLaunchedFromPackage(IBinder activityToken) throws RemoteException {
        String result = null;
        return result;
    }

    @Override
    public void registerUserSwitchObserver(IUserSwitchObserver observer) throws RemoteException { }

    @Override
    public void unregisterUserSwitchObserver(IUserSwitchObserver observer) throws RemoteException { }

    @Override
    public void requestBugReport() throws RemoteException { }

    @Override
    public long inputDispatchingTimedOut(int pid, boolean aboveSystem, String reason)
            throws RemoteException { return -1; }

    @Override
    public Bundle getAssistContextExtras(int requestType) throws RemoteException {
        Bundle res = null;
        return res;
    }

    @Override
    public void reportAssistContextExtras(IBinder token, Bundle extras)
            throws RemoteException { }

    @Override
    public void killUid(int uid, String reason) throws RemoteException { }

    @Override
    public void hang(IBinder who, boolean allowRestart) throws RemoteException { }

    @Override
    public void reportActivityFullyDrawn(IBinder token) throws RemoteException { }

    @Override
    public void notifyActivityDrawn(IBinder token) throws RemoteException { }

    @Override
    public void restart() throws RemoteException { }

    @Override
    public void performIdleMaintenance() throws RemoteException { }

};

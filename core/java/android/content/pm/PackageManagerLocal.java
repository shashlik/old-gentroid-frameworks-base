package android.content.pm;

import android.content.ComponentName;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.IntentSender;
import android.content.pm.ContainerEncryptionParams;
import android.content.pm.InstrumentationInfo;
import android.content.pm.IPackageInstallObserver;
import android.content.pm.IPackageManager;
import android.content.pm.PackageCleanItem;
import android.content.pm.PackageParser;
import android.content.pm.PermissionInfo;
import android.content.pm.ProviderInfo;
import android.content.pm.VerifierDeviceIdentity;
import android.content.pm.VerificationParams;
import android.os.RemoteException;
import android.os.UserHandle;
import android.net.Uri;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

public class PackageManagerLocal implements IPackageManager {
    private static PackageManagerLocal sInstance = null;
    final HashMap<String, PackageParser.Package> mPackages =
            new HashMap<String, PackageParser.Package>();

    ApplicationInfo mAndroidApplication;



    private PackageManagerLocal() {}

    @Override
    public android.os.IBinder asBinder() { return null; }

    @Override
    public boolean isPackageAvailable(String packageName, int userId) { return false; }

    @Override
    public int getPackageUid(String packageName, int userId) { return -1; }

    @Override
    public int[] getPackageGids(String packageName) {
        // stupid thing to indicate an error.
        return new int[0];
    }

    @Override
    public String[] currentToCanonicalPackageNames(String[] names) {
        String[] out = new String[names.length];

        for (int i=names.length-1; i>=0; i--) {
            out[i] =  names[i];
        }
        return out;
    }

    @Override
    public String[] canonicalToCurrentPackageNames(String[] names) {
        String[] out = new String[names.length];
        // reader

        for (int i=names.length-1; i>=0; i--) {
            out[i] =  names[i];
        }
        return out;
    }

    @Override
    public PermissionInfo getPermissionInfo(String name, int flags) { return null; }

    @Override
    public List<PermissionInfo> queryPermissionsByGroup(String group, int flags) { return null; }

    @Override
    public PermissionGroupInfo getPermissionGroupInfo(String name, int flags) { return null; }

    @Override
    public List<PermissionGroupInfo> getAllPermissionGroups(int flags) {
            ArrayList<PermissionGroupInfo> out
                    = new ArrayList<PermissionGroupInfo>();
            return out;
    }

    @Override
    public ActivityInfo getActivityInfo(ComponentName component, int flags, int userId) { return null; }

    @Override
    public ActivityInfo getReceiverInfo(ComponentName component, int flags, int userId) { return null; }

    @Override
    public ServiceInfo getServiceInfo(ComponentName component, int flags, int userId) { return null; }

    @Override
    public ProviderInfo getProviderInfo(ComponentName component, int flags, int userId) { return null; }

    @Override
    public int checkPermission(String permName, String pkgName) { return 0; }

    @Override
    public int checkUidPermission(String permName, int uid) { return PackageManager.PERMISSION_GRANTED; }

    @Override
    public boolean addPermission(PermissionInfo info) { return true; }

    @Override
    public boolean addPermissionAsync(PermissionInfo info) { return true; }

    @Override
    public void removePermission(String name) { }

    @Override
    public void grantPermission(String packageName, String permissionName) { }

    @Override
    public void revokePermission(String packageName, String permissionName) { }

    @Override
    public boolean isProtectedBroadcast(String actionName) { return false; }

    @Override
    public int checkSignatures(String pkg1, String pkg2) { return PackageManager.SIGNATURE_UNKNOWN_PACKAGE; }

    @Override
    public int checkUidSignatures(int uid1, int uid2) { return PackageManager.SIGNATURE_UNKNOWN_PACKAGE; }

    @Override
    public String[] getPackagesForUid(int uid) { return null; }

    @Override
    public String getNameForUid(int uid) { return null; }

    @Override
    public int getUidForSharedUser(String sharedUserName) { return -1; }

    @Override
    public int getFlagsForUid(int uid) { return 0; }

    @Override
    public ResolveInfo resolveIntent(Intent intent, String resolvedType,
                                     int flags, int userId) {  return null; }

    @Override
    public List<ResolveInfo> queryIntentActivities(Intent intent,
                                                   String resolvedType, int flags, int userId) {
        return Collections.emptyList();
    }

    @Override
    public List<ResolveInfo> queryIntentActivityOptions(ComponentName caller,
                                                        Intent[] specifics, String[] specificTypes, Intent intent,
                                                        String resolvedType, int flags, int userId) {
        return Collections.emptyList();
    }

    public static PackageManagerLocal getInstance() {
        if (sInstance == null) {
            sInstance = new PackageManagerLocal();
        }
        return sInstance;
    }

    @Override
    public List<ResolveInfo> queryIntentReceivers(Intent intent, String resolvedType, int flags,
                                                  int userId) {
        return Collections.emptyList();
    }

    @Override
    public ResolveInfo resolveService(Intent intent, String resolvedType, int flags, int userId) {
        return null;
    }

    @Override
    public List<ResolveInfo> queryIntentServices(Intent intent, String resolvedType, int flags,
                                                 int userId) {
        return Collections.emptyList();
    }

    @Override
    public List<ResolveInfo> queryIntentContentProviders(
            Intent intent, String resolvedType, int flags, int userId) {
        return Collections.emptyList();
    }

    @Override
    public ParceledListSlice<PackageInfo> getInstalledPackages(int flags, int userId) { return null; }

    @Override
    public ParceledListSlice<PackageInfo> getPackagesHoldingPermissions(
            String[] permissions, int flags, int userId) { return null; }

    @Override
    public ParceledListSlice<ApplicationInfo> getInstalledApplications(int flags, int userId) { return null; }

    @Override
    public List<ApplicationInfo> getPersistentApplications(int flags) {
        final ArrayList<ApplicationInfo> finalList = new ArrayList<ApplicationInfo>();
        return finalList;
    }

    @Override
    public ProviderInfo resolveContentProvider(String name, int flags, int userId) {  return null; }

    @Override
    @Deprecated
    public void querySyncProviders(List<String> outNames, List<ProviderInfo> outInfo) { }

    @Override
    public List<ProviderInfo> queryContentProviders(String processName,
                                                    int uid, int flags) {
        ArrayList<ProviderInfo> finalList = null;
        return finalList;
    }

    @Override
    public InstrumentationInfo getInstrumentationInfo(ComponentName name,
                                                      int flags) { return null; }

    @Override
    public List<InstrumentationInfo> queryInstrumentation(String targetPackage,
                                                          int flags) {
        ArrayList<InstrumentationInfo> finalList =
                new ArrayList<InstrumentationInfo>();

        return finalList;
    }

    @Override
    public void installPackage(
            final Uri packageURI, final IPackageInstallObserver observer, final int flags,
            final String installerPackageName) {
        installPackageWithVerification(packageURI, observer, flags, installerPackageName, null,
                null, null);
    }

    @Override
    public void finishPackageInstall(int token) { }

    @Override
    public void setInstallerPackageName(String targetPackage, String installerPackageName) { }

    @Override
    public void deletePackageAsUser(final String packageName,
                                    final IPackageDeleteObserver observer,
                                    final int userId, final int flags) { }

    @Override
    public String getInstallerPackageName(String packageName) { return null; }

    @Override
    public void addPackageToPreferred(String packageName) {  }

    @Override
    public void removePackageFromPreferred(String packageName) { }

    @Override
    public List<PackageInfo> getPreferredPackages(int flags) {
        return new ArrayList<PackageInfo>();
    }

    @Override
    public void resetPreferredActivities(int userId) { }

    @Override
    public ResolveInfo getLastChosenActivity(Intent intent, String resolvedType, int flags) { return null; }

    @Override
    public void setLastChosenActivity(Intent intent, String resolvedType, int flags,
                                      IntentFilter filter, int match, ComponentName activity) {  }

    @Override
    public void addPreferredActivity(IntentFilter filter, int match,
                                     ComponentName[] set, ComponentName activity, int userId) { }

    @Override
    public void replacePreferredActivity(IntentFilter filter, int match,
                                         ComponentName[] set, ComponentName activity) { }

    @Override
    public void clearPackagePreferredActivities(String packageName) { }

    @Override
    public int getPreferredActivities(List<IntentFilter> outFilters,
                                      List<ComponentName> outActivities, String packageName) { return 0; }

    @Override
    public ComponentName getHomeActivities(List<ResolveInfo> allHomeCandidates) { return null; }

    @Override
    public int getComponentEnabledSetting(ComponentName componentName, int userId) { return -1; }

    @Override
    public void setComponentEnabledSetting(ComponentName componentName,
                                           int newState, int flags, int userId) { }

    @Override
    public void setApplicationEnabledSetting(String appPackageName,
                                             int newState, int flags, int userId, String callingPackage) { }

    @Override
    public int getApplicationEnabledSetting(String packageName, int userId) { return -1; }

    @Override
    public void setPackageStoppedState(String packageName, boolean stopped, int userId) { }

    @Override
    public void freeStorageAndNotify(final long freeStorageSize, final IPackageDataObserver observer) { }

    @Override
    public void freeStorage(final long freeStorageSize, final IntentSender pi) { }

    @Override
    public void deleteApplicationCacheFiles(final String packageName,
                                            final IPackageDataObserver observer) { }

    @Override
    public void clearApplicationUserData(final String packageName,
                                         final IPackageDataObserver observer, final int userId) { }

    @Override
    public void getPackageSizeInfo(final String packageName, int userHandle,
                                   final IPackageStatsObserver observer) { }

    @Override
    public String[] getSystemSharedLibraryNames() {  return null; }

    @Override
    public FeatureInfo[] getSystemAvailableFeatures() { return null; }

    @Override
    public boolean hasSystemFeature(String name) { return true; }

    @Override
    public void enterSafeMode() { }

    @Override
    public boolean isSafeMode() {
        return false;
    }

    @Override
    public void systemReady() { }

    @Override
    public boolean hasSystemUidErrors() {
        return false;
    }

    @Override
    public void performBootDexOpt() { }

    @Override
    public boolean performDexOpt(String packageName) { return false; }

    @Override
    public void updateExternalMediaStatus(final boolean mediaStatus, final boolean reportStatus) {
    }

    @Override
    public PackageCleanItem nextPackageToClean(PackageCleanItem lastPackage) { return null;}

    @Override
    public PackageInfo getPackageInfo(String packageName, int flags, int userId) {
        PackageParser.Package p = mPackages.get(packageName);
        if (p != null) {
            return generatePackageInfo(p, flags, userId);
        }
        return null;
    }

    public void addPackageInfo(String packageName, PackageParser.Package p) {
        mPackages.put(packageName, p);
    }

    @Override
    public void movePackage(final String packageName, final IPackageMoveObserver observer,
                            final int flags) { }

    @Override
    public void installPackageWithVerification(Uri packageURI, IPackageInstallObserver observer,
                           int flags, String installerPackageName, Uri verificationURI,
                           ManifestDigest manifestDigest, ContainerEncryptionParams encryptionParams) {
        VerificationParams verificationParams = new VerificationParams(verificationURI, null, null,
                            VerificationParams.NO_UID, manifestDigest);
        installPackageWithVerificationAndEncryption(packageURI, observer, flags,
                installerPackageName, verificationParams, encryptionParams);
    }

    @Override
    public void installPackageWithVerificationAndEncryption(Uri packageURI,
                            IPackageInstallObserver observer, int flags, String installerPackageName,
                            VerificationParams verificationParams, ContainerEncryptionParams encryptionParams) {
        installPackageWithVerificationEncryptionAndAbiOverride(packageURI, observer, flags,
                installerPackageName, verificationParams, encryptionParams, null);
    }

    @Override
    public ApplicationInfo getApplicationInfo(String packageName, int flags, int userId) {
        // writer
        synchronized (mPackages) {
            PackageParser.Package p = mPackages.get(packageName);
            if (p != null) {
                return PackageParser.generateApplicationInfo(
                        p, flags, new PackageUserState(), userId);
            }
        }
        return null;
    }

    PackageInfo generatePackageInfo(PackageParser.Package p, int flags, int userId) {
        return PackageParser.generatePackageInfo(p, null, flags,
                0, 0, null, new PackageUserState(), userId);
    }

    @Override
    public boolean setInstallLocation(int loc) { return false; }

    @Override
    public int getInstallLocation() { return -1; }

    @Override
    public void installPackageWithVerificationEncryptionAndAbiOverride(Uri packageURI,
                            IPackageInstallObserver observer, int flags, String installerPackageName,
                            VerificationParams verificationParams, ContainerEncryptionParams encryptionParams,
                            String packageAbiOverride) { }

    @Override
    public int installExistingPackageAsUser(String packageName, int userId) {
        return PackageManager.INSTALL_FAILED_INTERNAL_ERROR;
    }


    @Override
    public boolean getApplicationBlockedSettingAsUser(String packageName, int userId) { return false; }
    @Override
    public boolean setApplicationBlockedSettingAsUser(String packageName, boolean blocked,
                                                      int userId) {
        return false;
    }

    @Override
    public void verifyPendingInstall(int id, int verificationCode) throws RemoteException { }

    @Override
    public void extendVerificationTimeout(int id, int verificationCodeAtTimeout,
                                          long millisecondsToDelay) {  }

    @Override
    public boolean isFirstBoot() { return false; }

    @Override
    public boolean isOnlyCoreApps() { return false; }


    @Override
    public VerifierDeviceIdentity getVerifierDeviceIdentity() throws RemoteException { return null; }

    @Override
    public void setPermissionEnforced(String permission, boolean enforced) { }

    @Override
    @Deprecated
    public boolean isPermissionEnforced(String permission) { return true; }

    @Override
    public boolean isStorageLow() { return false; }
};

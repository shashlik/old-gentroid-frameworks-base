project (android_runtime)


find_package (ZLIB REQUIRED)
find_package (Android COMPONENTS syscore nativehelper native REQUIRED)
find_package (ICU REQUIRED)
find_package (Skia REQUIRED)
find_package (SQLite3 REQUIRED)
find_package (HarfBuzz REQUIRED)
find_package (OpenGL REQUIRED)
find_package (EGL REQUIRED)
find_package (GLES REQUIRED)
find_package (Threads REQUIRED)
find_package (JPEG REQUIRED)
find_package (Wayland COMPONENTS Client Egl Cursor REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/android/graphics ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/hwui ${PROJECT_SOURCE_DIR}/../../include ${ANDROID_INCLUDE_DIRS} 
					${SKIA_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIR} ${ICU_INCLUDE_DIRS} ${HARFBUZZ_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${EGL_INCLUDE_DIR} ${OPENGLES2_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${Wayland_INCLUDE_DIRS})

add_definitions (${SKIA_CFLAGS} ${Wayland_DEFINITIONS})

add_definitions (-DHAVE_CONFIG_H -DKHTML_NO_EXCEPTIONS -DGKWQ_NO_JAVA
 				-DNO_SUPPORT_JS_BINDING -DQT_NO_WHEELEVENT -DKHTML_NO_XBL -DUSE_OPENGL_RENDERER
				-DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES -DWL_EGL_PLATFORM)

add_definitions (-Wno-unused-parameter -Wno-int-to-pointer-cast
				 -Wno-maybe-uninitialized -Wno-parentheses)
			

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
	add_definitions ("-DPACKED=\"__attribute__ ((packed))\"")
else ()
	add_definitions ("-DPACKED=\"\"")
endif ()

set (SRC_LIST AndroidRuntime.cpp
	#Time.cpp
	WaylandClient.cpp
	com_android_internal_content_NativeLibraryHelper.cpp
	com_google_android_gles_jni_EGLImpl.cpp
	com_google_android_gles_jni_GLImpl.cpp
	android_app_NativeActivity.cpp
	android_opengl_EGL14.cpp
	android_opengl_EGLExt.cpp
	#android_opengl_GLES10.cpp
	#android_opengl_GLES10Ext.cpp
	#android_opengl_GLES11.cpp
	#android_opengl_GLES11Ext.cpp
	android_opengl_GLES20.cpp
	android_opengl_GLES30.cpp
	android_database_CursorWindow.cpp
	android_database_SQLiteCommon.cpp
	android_database_SQLiteConnection.cpp
	android_database_SQLiteGlobal.cpp
	android_database_SQLiteDebug.cpp
	#android_emoji_EmojiFactory.cpp
	android_view_DisplayEventReceiver.cpp
	#android_view_Surface.cpp
	#android_view_SurfaceControl.cpp
	#android_view_SurfaceSession.cpp
	#android_view_TextureView.cpp
	#android_view_InputChannel.cpp
	#android_view_InputDevice.cpp
	android_view_InputEventReceiver.cpp
	#android_view_InputEventSender.cpp
	#android_view_InputQueue.cpp
	#android_view_KeyEvent.cpp
	#android_view_KeyCharacterMap.cpp
	android_view_HardwareRenderer.cpp
	#android_view_GraphicBuffer.cpp
	android_view_GLES20DisplayList.cpp
	android_view_GLES20Canvas.cpp
	#android_view_MotionEvent.cpp
	#android_view_PointerIcon.cpp
	#android_view_VelocityTracker.cpp
	android_view_Wayland.cpp
	android_text_AndroidCharacter.cpp
	android_text_AndroidBidi.cpp
	#android_os_Debug.cpp
	android_os_MemoryFile.cpp
	android_os_MessageQueue.cpp
	android_os_Parcel.cpp
	#android_os_SELinux.cpp
	android_os_SystemClock.cpp
	android_os_SystemProperties.cpp
	android_os_Trace.cpp
	#android_os_UEventObserver.cpp
	android_net_LocalSocketImpl.cpp
	#android_net_NetUtils.cpp
	android_net_TrafficStats.cpp
	#android_net_wifi_WifiNative.cpp
	android_nio_utils.cpp
	#android_text_format_Time.cpp
	android_util_AssetManager.cpp
	android_util_Binder.cpp
	android_util_EventLog.cpp
	android_util_Log.cpp
	android_util_FloatMath.cpp
	android_util_Process.cpp
	android_util_StringBlock.cpp
	android_util_XmlBlock.cpp
	android/graphics/AutoDecodeCancel.cpp
	android/graphics/Bitmap.cpp
	android/graphics/BitmapFactory.cpp
	#android/graphics/Camera.cpp
	android/graphics/Canvas.cpp
	android/graphics/ColorFilter.cpp
	android/graphics/DrawFilter.cpp
	android/graphics/CreateJavaOutputStreamAdaptor.cpp
	android/graphics/Graphics.cpp
	android/graphics/HarfBuzzNGFaceSkia.cpp
	android/graphics/Interpolator.cpp
	android/graphics/LayerRasterizer.cpp
	android/graphics/MaskFilter.cpp
	android/graphics/Matrix.cpp
	android/graphics/Movie.cpp
	android/graphics/NinePatch.cpp
	android/graphics/NinePatchImpl.cpp
	android/graphics/NinePatchPeeker.cpp
	android/graphics/Paint.cpp
	android/graphics/Path.cpp
	android/graphics/PathMeasure.cpp
	android/graphics/PathEffect.cpp
	android/graphics/Picture.cpp
	android/graphics/PorterDuff.cpp
	android/graphics/BitmapRegionDecoder.cpp
	android/graphics/Rasterizer.cpp
	android/graphics/Region.cpp 
	android/graphics/Shader.cpp
	#android/graphics/SurfaceTexture.cpp
	android/graphics/TextLayout.cpp
	android/graphics/TextLayoutCache.cpp
	android/graphics/Typeface.cpp
	android/graphics/Utils.cpp
	android/graphics/Xfermode.cpp
	android/graphics/YuvToJpegEncoder.cpp
	android/graphics/pdf/PdfDocument.cpp
	#android_media_AudioRecord.cpp
	#android_media_AudioSystem.cpp
	#android_media_AudioTrack.cpp
	#android_media_JetPlayer.cpp
	#android_media_RemoteDisplay.cpp
	#android_media_ToneGenerator.cpp
	#android_hardware_Camera.cpp
	#android_hardware_camera2_CameraMetadata.cpp
	#android_hardware_SensorManager.cpp
	#android_hardware_SerialPort.cpp
	#android_hardware_UsbDevice.cpp
	#android_hardware_UsbDeviceConnection.cpp
	#android_hardware_UsbRequest.cpp
	android_util_FileObserver.cpp
	android/opengl/poly_clip.cpp
	android/opengl/util.cpp
	#android_server_NetworkManagementSocketTagger.cpp
	#android_server_Watchdog.cpp
	#android_ddm_DdmHandleNativeHeap.cpp
	#com_android_internal_os_ZygoteInit.cpp
	android_backup_BackupDataInput.cpp
	android_backup_BackupDataOutput.cpp
	android_backup_FileBackupHelperBase.cpp
	android_backup_BackupHelperDispatcher.cpp
	android_app_backup_FullBackup.cpp
	android_content_res_ObbScanner.cpp
	android_content_res_Configuration.cpp
	android_animation_PropertyValuesHolder.cpp
	com_android_internal_net_NetworkStatsFactory.cpp
	com_android_internal_os_Zygote.cpp)


add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES (${PROJECT_NAME} PROPERTIES LINK_FLAGS -Wl,--no-undefined)
target_link_libraries (${PROJECT_NAME} hwui androidfw ${ANDROID_LIBRARIES} ${SKIA_LIBRARIES} ${HARFBUZZ_LIBRARIES} ${SQLITE3_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES}
						${ZLIB_LIBRARIES} ${OPENGL_LIBRARIES} ${EGL_LIBRARY} ${OPENGLES2_LIBRARY}  ${JPEG_LIBRARY} ${Wayland_LIBRARIES} ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_LIB_DIR})



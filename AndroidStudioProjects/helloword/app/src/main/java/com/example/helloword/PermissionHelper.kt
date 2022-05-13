// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.example.helloword

import android.Manifest
import android.app.Activity
import android.content.pm.PackageManager
import android.util.Log
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat


/** Manages camera permission request and handling.  */
object PermissionHelper {
    private const val TAG = "PermissionHelper"
    private const val AUDIO_PERMISSION = Manifest.permission.RECORD_AUDIO
    private const val CAMERA_PERMISSION = Manifest.permission.CAMERA
    private const val REQUEST_CODE = 0
    fun permissionsGranted(context: Activity?, permissions: Array<String?>): Boolean {
        for (permission in permissions) {
            val permissionStatus = ContextCompat.checkSelfPermission(context!!, permission!!)
            if (permissionStatus != PackageManager.PERMISSION_GRANTED) {
                return false
            }
        }
        return true
    }

    fun checkAndRequestPermissions(context: Activity?, permissions: Array<String?>) {
        if (!permissionsGranted(context, permissions)) {
            ActivityCompat.requestPermissions(context!!, permissions, REQUEST_CODE)
        }
    }

    /** Called by context to check if camera permissions have been granted.  */
    fun cameraPermissionsGranted(context: Activity?): Boolean {
        return permissionsGranted(context, arrayOf(CAMERA_PERMISSION))
    }

    /**
     * Called by context to check if camera permissions have been granted and if not, request them.
     */
    fun checkAndRequestCameraPermissions(context: Activity?) {
        Log.d(TAG, "checkAndRequestCameraPermissions")
        checkAndRequestPermissions(context, arrayOf(CAMERA_PERMISSION))
    }

    /** Called by context to check if audio permissions have been granted.  */
    fun audioPermissionsGranted(context: Activity?): Boolean {
        return permissionsGranted(context, arrayOf(AUDIO_PERMISSION))
    }

    /** Called by context to check if audio permissions have been granted and if not, request them.  */
    fun checkAndRequestAudioPermissions(context: Activity?) {
        Log.d(TAG, "checkAndRequestAudioPermissions")
        checkAndRequestPermissions(context, arrayOf(AUDIO_PERMISSION))
    }

    /** Called by context when permissions request has been completed.  */
    fun onRequestPermissionsResult(
            requestCode: Int, permissions: Array<String?>?, grantResults: IntArray) {
        Log.d(TAG, "onRequestPermissionsResult")
        if (permissions != null) {
            if (permissions.size > 0 && grantResults.size != permissions.size) {
                Log.d(TAG, "Permission denied.")
                return
            }
        }
        for (i in grantResults.indices) {
            if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
                Log.d(TAG, permissions?.get(i) + " permission granted.")
            }
        }
        // Note: We don't need any special callbacks when permissions are ready because activities
        // using this helper class can have code in onResume() which is called after the
        // permissions dialog box closes. The code can be branched depending on if permissions are
        // available via permissionsGranted(Activity).
        return
    }
}
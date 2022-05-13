package com.example.helloword

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        PermissionHelper.checkAndRequestCameraPermissions(this);
        setContentView(R.layout.activity_main)
    }
    
}fun onRequestPermissionsResult(
    requestCode: Int, permissions: Array<String?>?, grantResults: IntArray?
) {
    //super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    if (grantResults != null) {
        PermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }
}

fun onResume() {
    //super.onResume()
    if (PermissionHelper.cameraPermissionsGranted(this)) {
        startCamera()
    }
}

fun startCamera() {}
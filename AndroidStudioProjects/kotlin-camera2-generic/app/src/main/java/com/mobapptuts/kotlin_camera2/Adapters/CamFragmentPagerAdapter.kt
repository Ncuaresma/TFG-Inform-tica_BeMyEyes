package com.mobapptuts.kotlin_camera2.Adapters

import android.support.v4.app.Fragment
import android.support.v4.app.FragmentManager
import android.support.v4.app.FragmentPagerAdapter
import com.mobapptuts.kotlin_camera2.Fragments.PreviewFragment

/**
 * Created by nigelhenshaw on 2018/01/24.
 */
class CamFragmentPagerAdapter(fragmentManager: FragmentManager): FragmentPagerAdapter(fragmentManager) {
    override fun getItem(position: Int): Fragment {
        TODO("Fragments will be added in future")
    }

    override fun getCount() = 2
}
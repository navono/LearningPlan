package com.example.navono.criminalintent;

import android.os.Bundle;
import android.support.v4.app.Fragment;

/**
 * Created by navono on 2016/7/12.
 */
public class CrimeFragment extends Fragment {
    private Crime mCrime;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mCrime = new Crime();
    }
}

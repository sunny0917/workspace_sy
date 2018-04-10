package com.example.display;

import android.app.Presentation;
import android.content.Context;
import android.os.Bundle;
import android.view.Display;

public class DifferentDislay extends Presentation{
    public DifferentDislay(Context outerContext, Display display) {
        super(outerContext,display);
        //TODOAuto-generated constructor stub  
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.diffrentdisplay_basket);
    }
}

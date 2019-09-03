package com.example.timtest;

import android.os.Bundle;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        QREncode mQREncode = new QREncode();
        ImageView mImageView = findViewById(R.id.iv_qrcode);
        try {
            mImageView.setImageBitmap(mQREncode.createQRCodeBitmap("Test", 500));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}

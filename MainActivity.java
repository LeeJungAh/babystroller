package com.example.kim.client5;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.StrictMode;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.ViewFlipper;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    final static String SERVER_IP = "http://192.168.43.44";
    EditText EditText_ID;
    EditText EditText_PAWD;

    ViewFlipper Vf;
    HttpPost httppost;
    HttpResponse response;
    HttpClient httpclient;
    List<NameValuePair> nameValuePairs;
    ProgressDialog dialog = null;
    Handler handler = new Handler();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        EditText_ID = (EditText)findViewById(R.id.editText_ID);
        EditText_PAWD = (EditText)findViewById(R.id.editText_PAWD);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy); // 강제적으로 네트워크 접속
    }



    // button functions : onButtonloginClicked, onButtonjoinClicked
    public void onButtonloginClicked(View v){
        dialog = ProgressDialog.show(MainActivity.this, "",
                "Validating user...", true);
//        로그인버튼 누르고 잠시 기다리는 동안 출력되는 다이얼로그
        new Thread(new Runnable() {
            public void run() {
                Looper.prepare();
                login();
                Looper.loop();
            }
        }).start();

//        Intent intent = new Intent(getApplicationContext(), MenuActivity.class);
//        intent.putExtra("id", EditText_ID.getText().toString());
//        startActivity(intent);
    }

    public void onButtonjoinClicked(View v){
        startActivity(new Intent(MainActivity.this, JoinActivity.class));
    }



    // login function : login
    void login() {
        try {
            httpclient = new DefaultHttpClient();
            httppost = new HttpPost(SERVER_IP+"/login.php");
            // 여기서는 가상 AVD로 구현하기 때문에 내 아이피 주소를 사용
            // 실제 구동할 때는 공용 서버 주소를 사용
            nameValuePairs = new ArrayList<NameValuePair>(2);
            nameValuePairs.add(new BasicNameValuePair("id", EditText_ID.getText().toString()));
            nameValuePairs.add(new BasicNameValuePair("password", EditText_PAWD.getText().toString()));
            httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
            response = httpclient.execute(httppost);
            runOnUiThread(new Runnable() {
                public void run() {
                    dialog.dismiss();
                }
            });

            ResponseHandler<String> responsubseHandler = new BasicResponseHandler();
            String String_resopnse = httpclient.execute(httppost, responseHandler);
            String String_compare = String_resopnse.string(0,6) + "User Found";
            Log.e("RESPONSE", String_resopnse);
            Log.e("RESPONSE", String_compare);

            if (String_resopnse.equals(String_compare)) {
                Toast.makeText(MainActivity.this, "Login Success", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(getApplicationContext(), MenuActivity.class);
                intent.putExtra("id", EditText_ID.getText().toString());
                startActivity(intent);
            } else {
                Toast.makeText(MainActivity.this, "Login Fail", Toast.LENGTH_SHORT).show();
            }

        } catch (Exception e) {
            dialog.dismiss();
            System.out.println("Exception : " + e.getMessage());
        }
    }

}




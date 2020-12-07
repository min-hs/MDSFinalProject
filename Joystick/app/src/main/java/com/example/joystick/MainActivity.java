package com.example.joystick;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {
    Button btnUp;
    Button btnDown;
    Button btnLeft;
    Button btnRight;
    Button btnExit;
    Button btnConnect;
    Button btnSpeed1;
    Button btnSpeed2;
    Button btnSpeed3;

    EditText etString;
    TextView tvString;

    String str = "";
    int speed = 1;

    Socket socket;
    BufferedWriter networkWriter;
    int port = 3501;

    protected void setSocket(String ip, int port) throws IOException {
        try {
            socket = new Socket(ip, port);
            networkWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        } catch (IOException e) {
            System.out.println(e);
            e.printStackTrace();
        }
    }

    protected void closeSocket() throws IOException {
        try {
            socket.close();
        } catch (IOException e) {
            System.out.println(e);
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnUp = (Button)findViewById(R.id.btnUp);
        btnDown = (Button)findViewById(R.id.btnDown);
        btnLeft = (Button)findViewById(R.id.btnLeft);
        btnRight = (Button)findViewById(R.id.btnRight);
        btnExit = (Button)findViewById(R.id.btnExit);
        btnConnect = (Button)findViewById(R.id.btnConnect);
        btnSpeed1 = (Button)findViewById(R.id.btnSpeed1);
        btnSpeed2 = (Button)findViewById(R.id.btnSpeed2);
        btnSpeed3 = (Button)findViewById(R.id.btnSpeed3);

        etString = (EditText)findViewById(R.id.etString);
        tvString = (TextView)findViewById(R.id.tvString);

        View.OnTouchListener dirListener = new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    int vid = v.getId();
                    switch (vid) {
                        case R.id.btnUp :
                            str = "W";
                            break;
                        case R.id.btnDown :
                            str = "S";
                            break;
                        case R.id.btnLeft :
                            str = "A";
                            break;
                        case R.id.btnRight :
                            str = "D";
                            break;
                        default :
                            break;
                    }
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) {
                    str = "X";
                }
                else {
                    return false;
                }
                str += Integer.toString(speed);
                
                // TODO 소켓만들어서 전송
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        PrintWriter out = new PrintWriter(networkWriter, true);
                        out.println(str);
                    }
                }).start();
                tvString.setText(str);
                
                return false;
            }
        };
        btnUp.setOnTouchListener(dirListener);
        btnDown.setOnTouchListener(dirListener);
        btnLeft.setOnTouchListener(dirListener);
        btnRight.setOnTouchListener(dirListener);

        View.OnClickListener speedListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int vid = v.getId();
                switch (vid) {
                    case R.id.btnSpeed1 :
                        speed = 1;
                        break;
                    case R.id.btnSpeed2 :
                        speed = 2;
                        break;
                    case R.id.btnSpeed3 :
                        speed = 3;
                        break;
                    default :
                        break;
                }
                Toast.makeText(getApplicationContext(),"Set speed to " + Integer.toString(speed), Toast.LENGTH_SHORT).show();
            }
        };
        btnSpeed1.setOnClickListener(speedListener);
        btnSpeed2.setOnClickListener(speedListener);
        btnSpeed3.setOnClickListener(speedListener);

        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO 소켓 연결하기
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String ip = etString.getText().toString();
                        try {
                            setSocket(ip, port);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
                Toast.makeText(getApplicationContext(),"Connected with server", Toast.LENGTH_SHORT).show();
            }
        });

        btnExit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    closeSocket();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                Toast.makeText(getApplicationContext(),"Disconnected with server", Toast.LENGTH_SHORT).show();
                finish();
            }
        });
    }
}
package br.newm.paker.brainfuck;

import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;

import org.apache.commons.io.IOUtils;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    EditText code_editText;
    TextView outputView;
    Button executeBtn;
    ScrollView scrollView;
    Handler handler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        code_editText = (EditText) findViewById(R.id.code_editText);
        outputView = (TextView) findViewById(R.id.outputView);
        executeBtn = (Button) findViewById(R.id.executeBtn);
        scrollView = (ScrollView) findViewById(R.id.scrollView);
        Spinner exampleSpn = (Spinner) findViewById(R.id.exampleSpn);

        executeBtn.setTag(0);

        exampleSpn.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

            private String readFile(String filename) {
                try {
                    return IOUtils.toString(getAssets().open(filename + ".txt"), "UTF-8");
                } catch (IOException e) {
                    e.printStackTrace();
                }

                return "";
            }

            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int pos, long l) {

                switch (pos) {
                    case 1:
                        code_editText.setText(readFile("helloworld"));
                        break;
                    case 2:
                        code_editText.setText(readFile("fatorial"));
                        break;
                    case 3:
                        code_editText.setText(readFile("hanoi"));
                        break;
                    case 4:
                        code_editText.setText(readFile("mandelbrot"));
                        break;
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        executeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(executeBtn.getTag().equals(1))
                    stop();
                else {
                    outputView.setText("");

                    code_editText.setEnabled(false);
                    executeBtn.setText("Stop!");
                    executeBtn.setTag(1);

                    execute(code_editText.getText().toString());
                }
            }
        });
    }

    public void print(final String str) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                outputView.append(str);
                scrollView.fullScroll(ScrollView.FOCUS_DOWN);
            }
        });
    }

    public void finish() {
        handler.post(new Runnable() {
            @Override
            public void run() {
                code_editText.setEnabled(true);
                executeBtn.setText("Execute");
                executeBtn.setTag(0);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void execute(String code);

    public native void stop();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}

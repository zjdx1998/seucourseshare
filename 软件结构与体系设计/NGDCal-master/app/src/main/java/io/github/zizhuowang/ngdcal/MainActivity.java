package io.github.zizhuowang.ngdcal;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends AppCompatActivity {

    static String regex = "百度为您找到相关结果约(.*)个";
    static Pattern pattern = Pattern.compile(regex);
    static String url = "http://www.baidu.com/s?wd=";
    EditText input1, input2, output1, output2;
    Integer numA, numB, numC;
    Double result=0.0;
    String html;
    Button button;
    TextView source;
    String in1, in2;
    Handler handler = new Handler() {
        @Override
        public void handleMessage(Message m) {
            if(m.arg1 == 1){
                numA = m.arg2;
                source.append(in1+numA.toString()+"\n");
            }
            if(m.arg1 == 2){
                numB = m.arg2;
                source.append(in2+numB.toString()+"\n");
            }
            if(m.arg1 == 3){
                numC = m.arg2;
                result = Calculate(numA,numB,numC);
                source.append(in1+"+"+in2+numC.toString()+"\n"+result.toString()+"\n");
                output2.setText(result.toString());
                if (result < 0.3) {
                    output1.setText("非常相似");
                } else if (result < 0.8) {
                    output1.setText("联系密切");
                } else if (result < 1.5) {
                    output1.setText("关系不大");
                } else {
                    output1.setText("没有联系");
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        input1 = (EditText) findViewById(R.id.input1);
        input2 = (EditText) findViewById(R.id.input2);
        output1 = (EditText) findViewById(R.id.output1);
        output2 = (EditText) findViewById(R.id.output2);
        button = (Button) findViewById(R.id.button);
        source = (TextView) findViewById(R.id.source);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                in1 = input1.getText().toString();
                in2 = input2.getText().toString();
                try{
                    Thread t1 = new Thread(){
                        @Override
                        public void run(){
                            searchResult(in1,1);
                        }
                    };t1.start();t1.join();
                    Thread t2 = new Thread(){
                        @Override
                        public void run(){
                            searchResult(in2,2);
                        }
                    };t2.start();t2.join();
                    Thread t3 = new Thread(){
                        @Override
                        public void run(){
                            searchResult(in1 + "%20" + in2,3);
                        }
                    };t3.start();t3.join();
                }catch(Exception e){}
            }
        });
    }

    /**
     * @param name
     * @return 搜索到的网页数量
     */
    private void searchResult(String name,int id) {
        try {
            URL baiduURL = new URL(url + name);
            URLConnection connection = baiduURL.openConnection();
            connection.setRequestProperty("User-Agent","Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0");
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            //逐行筛选匹配正则表达式，并将结果中的逗号删除，之后转化成数字。
            html = in.readLine();
            while (html != null) {
                Matcher matcher = pattern.matcher(html);
                while (matcher.find()) {
                    System.out.println(name.replaceAll("%20", " ") + ": " + matcher.group(1));//格式优化
                    String temp = matcher.group(1);
                    Message m = new Message();
                    m.arg1 = id;
                    m.arg2 = Integer.parseInt(temp.replaceAll(",", ""));
                    handler.sendMessage(m);
                }
                html = in.readLine();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * @param numA
     * @param numB
     * @param numC
     * @return NGD公式计算结果
     */
    public Double Calculate(Integer numA, Integer numB, Integer numC) {
        Double lnx = Math.log(numA);
        Double lny = Math.log(numB);
        Double lnSum = Math.log(25270000000.0);//由于不知具体数值，这里取谷歌搜素最大索引限制
        Double lnxy = Math.log(numC);
        //NGD公式
        if (lnx > lny) {
            return (lnx - lnxy) / (lnSum - lny);
        } else {
            return (lny - lnxy) / (lnSum - lnx);
        }
    }
}
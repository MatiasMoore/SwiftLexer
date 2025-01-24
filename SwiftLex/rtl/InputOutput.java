package rtl;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class InputOutput {

    public static void print(int i) 
    {
        System.out.println(i);
    }
    
    public static void print(Integer integer)
    {
        System.out.println(integer._value);
    }
    
    public static String readLine()
    {
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            java.lang.String s = br.readLine();
            return new String(s);
        }
        catch(IOException e) {
            e.printStackTrace();
        }
        return new String("Error!");
    }

    public static void print(String string)
    {
        System.out.println(string._value);
    }

    public static void print(Boolean bool)
    {
        if (bool._value) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }
    }

    public static void print(Float f)
    {
        System.out.println(f._value);
    }
}

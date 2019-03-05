public class returnTest {

    public static void main(String[] args) {
        topla(11,4);
    }

    public static int topla(int a, int b){
        int var = 0;
        for (int i=0; i<5; i++){
            var += a+b;
        }
        return var;
    }
}
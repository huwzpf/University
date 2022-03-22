import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main( String[] args) {

        TaskCollector in = new TaskCollector();
        OutputCollector out = new OutputCollector();


        int number_of_threads = 1;
        if(args.length != 0){
            number_of_threads = Integer.parseInt(args[0]);
        }
        Thread[] threads = new Thread[number_of_threads];

        for(int i = 0; i<number_of_threads; i++){
            threads[i] = new Thread( new Computation(in, out, i) );
            threads[i].start();
        }



        Scanner sc = new Scanner(System.in);
        int input;
        for (int i = 0; i< 10; i++){
            in.put(i);
        }

        boolean running = true;
        while(running){

            input = sc.nextInt();
            if (input == -1){
                running = false;
            }
            else {
                in.put(input);
            }
        }

        for(ReturnData num : out.get_list()){
            System.out.println(num);
        }

        for(int i=0; i<number_of_threads; i++) {
            threads[i].interrupt();
            try{
                threads[i].join();
            }catch (InterruptedException ex) {
                break;
            }
        }

    }
}
import java.util.List;

public class Computation implements Runnable{
    private int id;
    private TaskCollector input;
    private OutputCollector output;

    public Computation(TaskCollector in, OutputCollector out, int id){
        this.input = in;
        this.output = out;
        this.id = id;
    }

    @Override
    public void run() {
        while (!Thread.interrupted()) {
            try {

                InputData in = input.take();
                System.out.println("Computation for job: " + in.job_id + " started by thread: " + this.id);
                int precision = in.precision;
                double computed_value = 0;
                int counter = 1;
                while(Math.abs(4 * computed_value - Math.PI) > Math.pow(10, -precision)){
                    computed_value += Math.pow(-1, counter - 1) / (2 * counter - 1);
                    counter++;
                }
                output.put(new ReturnData(in.job_id, precision, 4 * computed_value, Math.abs(4 * computed_value - Math.PI), counter));
                System.out.println("Computation for job: " + in.job_id + " finished by thread: " + this.id);
            } catch (InterruptedException ex) {
                break;
            }
        }
    }

}

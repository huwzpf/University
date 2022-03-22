import java.util.ArrayList;
import java.util.List;

public class TaskCollector {
    private int job_count = 0;

    private final List<InputData> numbers = new ArrayList<>();

    public synchronized InputData take() throws InterruptedException {
        while (numbers.isEmpty()) {
            wait();
        }
        return numbers.remove(0);
    }

    public synchronized void put(int precision) {
        this.numbers.add(new InputData(job_count, precision));
        job_count++;
        notifyAll();
    }
}
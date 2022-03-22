import java.util.ArrayList;
import java.util.List;

public class OutputCollector {
    private final List<ReturnData> numbers = new ArrayList<>();

    public synchronized ReturnData take() throws InterruptedException {
        return numbers.remove(0);
    }

    public List<ReturnData> get_list(){
        return this.numbers;
    }

    public synchronized void put(ReturnData number) {
        this.numbers.add(number);
    }
}
import entity.Mage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import repository.MageRepository;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class MageRepositoryTest {
    private MageRepository mr;

    @BeforeEach
    void setUp(){
        mr = new MageRepository();
    }

    @Test
    public void addAlreadyExistingRecord(){
        mr.save(new Mage("Andrzej",1));
        assertThrows(IllegalArgumentException.class, () -> {
            mr.save(new Mage("Andrzej",1));
        });
    }

    @Test
    public void deleteNotExistingRecord(){
        assertThrows(IllegalArgumentException.class, () -> {
            mr.delete("Zbigniew");
        });
    }

    @Test
    public void readNotExistingRecord(){
        Optional<Mage> result = mr.find("Zbigniew");
        assertEquals(Optional.empty(),result);
    }

    @Test
    public void readExistingRecord(){
        Mage m1 = new Mage("Andrzej",1);
        mr.save(m1);
        Optional<Mage> result = mr.find("Andrzej");
        assertEquals(Optional.of(m1),result);
    }
}

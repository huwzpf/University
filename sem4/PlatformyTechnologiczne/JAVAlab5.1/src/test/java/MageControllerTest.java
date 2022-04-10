import controller.MageController;
import entity.Mage;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import repository.MageRepository;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;

import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
public class MageControllerTest {
    @Mock
    private MageRepository repository;
    private MageController mageController;

    @BeforeEach
    void setUp(){
        mageController = new MageController(repository);
    }

    @Test
    public void DeleteNotExistingRecord(){
        doThrow(new IllegalArgumentException()).when(repository).delete("Andrzej");
        // assert correct return value
        assertEquals("not found",mageController.delete("Andrzej"));
        // assert correct call to repository
        verify(repository).delete("Andrzej");
    }

    @Test
    public void DeleteExistingRecord(){
        doNothing().when(repository).delete("Andrzej");
        // assert correct response
        assertEquals("done", mageController.delete("Andrzej"));
        // assert correct call to repository
        verify(repository).delete("Andrzej");
    }


    @Test
    public void SaveRecord(){
        ArgumentCaptor<Mage> mageArgumentCaptor = ArgumentCaptor.forClass(Mage.class);
        // assert correct response
        assertEquals("done", mageController.save("Andrzej",1));
        // assert correct call to repository
        verify(repository).save(mageArgumentCaptor.capture());
        assertEquals( new Mage("Andrzej",1), mageArgumentCaptor.getValue());
    }

    @Test
    public void SaveAlreadyExistingRecord(){
        ArgumentCaptor<Mage> mageArgumentCaptor = ArgumentCaptor.forClass(Mage.class);
        doThrow(new IllegalArgumentException()).when(repository).save(any(Mage.class));
        // assert correct response
        assertEquals("bad request", mageController.save("Andrzej",1));
        // assert correct call to repository
        verify(repository).save(mageArgumentCaptor.capture());
        assertEquals( new Mage("Andrzej",1), mageArgumentCaptor.getValue());
    }


    @Test
    public void FindNotExistingRecord(){
        when(repository.find("Andrzej")).thenReturn(Optional.empty());
        // assert correct response
        assertEquals("not found", mageController.find("Andrzej"));
        // assert correct call to repository
        verify(repository).find("Andrzej");
    }

    @Test
    public void FindExistingRecord(){
        Mage m = new Mage("Andrzej", 1);
        when(repository.find("Andrzej")).thenReturn(Optional.of(m));
        // assert correct response
        assertEquals(m.toString(), mageController.find("Andrzej"));
        // assert correct call to repository
        verify(repository).find("Andrzej");
    }

}

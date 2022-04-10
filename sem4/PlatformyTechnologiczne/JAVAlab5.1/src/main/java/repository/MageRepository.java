package repository;

import entity.Mage;

import java.util.Collection;
import java.util.Optional;


public class MageRepository extends InMemoryRepository<Mage> {
    private final Collection<Mage> collection = set;

    public Optional<Mage> find(String name){
        for (Mage m: collection) {
            if(m.getName().equals(name)) {
                return Optional.of(m);
            }
        }
        return Optional.empty();
    }

    public void delete(String name) throws IllegalArgumentException{
        Optional<Mage> m = find(name);
        if (m.isPresent()){
            collection.remove(m.get());
        }
        else {
            throw new IllegalArgumentException();
        }
    }

    public void save(Mage mage) throws IllegalArgumentException{
        Optional<Mage> m = find(mage.getName());
        if (m.isPresent()){
            throw new IllegalArgumentException();
        }
        else {
            collection.add(mage);
        }
    }
}


package repository;

import java.util.*;


public abstract class InMemoryRepository <E extends Comparable<E>> {
    protected Set<E> set;
    public InMemoryRepository() {
        set = new HashSet<>();
    }
    public InMemoryRepository(boolean sorted) {
        set = sorted ? new TreeSet<>() : new HashSet<>();
    }
    public InMemoryRepository(Comparator<E> comparator) {
        set = new TreeSet<>(comparator);
    }
    public List<E> findAll() {
        return new ArrayList<>(set);
    }
    public void delete(Optional<E> entity) throws IllegalArgumentException {
        E e;
        try{
            e = entity.get();
        }
        catch (NoSuchElementException ex){
            throw new IllegalArgumentException();
        }
        set.remove(e);
    }
    public void save(E entity) throws IllegalArgumentException {
        for (E e : set){
            if (e.equals(entity)){
                throw new IllegalArgumentException();
            }
        }
        set.add(entity);
    }
}

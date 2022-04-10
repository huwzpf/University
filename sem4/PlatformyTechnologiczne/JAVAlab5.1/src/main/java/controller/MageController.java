package controller;

import entity.Mage;
import repository.MageRepository;

import java.util.List;
import java.util.Optional;

public class MageController {

    private final MageRepository repository;
    public MageController(MageRepository repository) {
        this.repository = repository;
    }
    public List<Mage> findAllMages() {
        return repository.findAll();
    }
    public String find(String name){
        String response = "not found" ;
        Optional<Mage> m = repository.find(name);
        if(m.isPresent()){
            response = m.get().toString();
        }
        return response;
    }

    public String delete(String name){
        String response = "done";
        try {
            this.repository.delete(name);
        }
        catch(IllegalArgumentException ex){
            response = "not found";
        }
        return response;
    }

    public String save(String name,int level){
        String response = "done";
        try {
            this.repository.save(new Mage(name,level));
        }
        catch(IllegalArgumentException e){
            response = "bad request";
        }
        return response;
    }
}

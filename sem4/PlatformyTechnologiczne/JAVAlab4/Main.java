package pt.lab4;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.logging.Level;

public class Main {
    public static void main(String[] args) {
        java.util.logging.Logger.getLogger("org.hibernate").setLevel(Level.OFF);

        System.out.println("Hello!");
        EntityManagerFactory emf = Persistence.createEntityManagerFactory("testPu");
        MageManager mm = new MageManager(emf);
        TowerManager tm = new TowerManager(emf);

        // create entities
        tm.add(new Tower("W1", 1));
        tm.add(new Tower("W2", 2));
        mm.add(new Mage("Tomasz", 1, tm.find("W1")));
        mm.add(new Mage("Kazimierz", 2, tm.find("W2")));
        mm.add(new Mage("Ryszard", 3, tm.find("W2")));

        // printing
        System.out.println("All mages:");
        System.out.println(mm.findAll());
        System.out.println("All towers:");
        System.out.println(tm.findAll());

        // custom queries
        System.out.println("All towers lower than 2:");
        System.out.println(tm.findAllLowerThan(2));
        System.out.println("All mages with level higher than 2 in tower W2:");
        System.out.println(mm.findWithLevelHigherThanFromTower(2, tm.find("W2")));



        //delete
        tm.delete(tm.find("W1"));
        mm.delete(mm.find("Ryszard"));
        System.out.println("After deleting tower W1 and mage Ryszard");
        System.out.println("All mages:");
        System.out.println(mm.findAll());
        System.out.println("All towers:");
        System.out.println(tm.findAll());
        emf.close();

    }
}
package pt.lab4;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

@Entity
public class Mage {
    @Id
    private String name;
    private int level;
    @ManyToOne
    private Tower tower;

    public Mage(){
        this.name = "noname";
        this.level = -1;
        this.tower = null;
    }

    public Mage(String name, int level){
        this.name = name;
        this.level = level;
        this.tower = null;
    }

    public Mage(String name, int level, Tower t){
        this.name = name;
        this.level = level;
        this.tower = t;
    }

    @Override
    public String toString() {
        return "Mage{" +
                "name='" + name + '\'' +
                ", level=" + level +
                ", tower=" + tower.getName() +
                '}';
    }

    public String getName() {
        return name;
    }

    public int getLevel() {
        return level;
    }

    public Tower getTower() {
        return tower;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    public void setTower(Tower tower) {
        this.tower = tower;
    }
}

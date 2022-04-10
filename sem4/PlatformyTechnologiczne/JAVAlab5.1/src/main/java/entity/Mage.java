package entity;

import java.util.Comparator;
import java.util.Objects;

public class Mage implements Comparator<Mage>, Comparable<Mage> {
    private String name;
    private int level;


    public Mage() {}

    public Mage(String n, int lvl) {
        this.name = n;
        this.level = lvl;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    @Override
    public String toString() {
        return "Mage{" +
                "name='" + this.name + '\'' +
                ", level=" + this.level +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Mage mage = (Mage) o;
        return this.level == mage.level && Objects.equals(this.name, mage.name);
    }

    @Override
    public int hashCode(){
        return this.name.hashCode()*this.level;
    }

    @Override
    public int compare(Mage m1, Mage m2) {
        int result = m1.getName().hashCode() - m2.getName().hashCode();
        if(result == 0){
            result = m1.getLevel() - m2.getLevel();
        }
        return result;
    }

    @Override
    public int compareTo(Mage m) {
        int result = this.name.hashCode() - m.getName().hashCode();
        if(result == 0){
            result = this.level - m.getLevel();
        }
        return result;
    }
}

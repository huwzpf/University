package pt.lab4;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

@Entity
public class Tower {
    @Id
    private String name;
    private int height;
    @OneToMany(mappedBy = "tower", fetch = FetchType.EAGER, cascade = CascadeType.REMOVE)
    private List<Mage> mages;

    public Tower(){
        this.name = "noname";
        this.height= -1;
        this.mages = new ArrayList<Mage>();
    }

    public Tower(String name, int height){
        this.name = name;
        this.height= height;
        this.mages =new ArrayList<Mage>();
    }

    public Tower(String name, int height, List<Mage> l){
        this.name = name;
        this.height= height;
        this.mages = l;
    }

    @Override
    public String toString() {
        String mages_str = "null";
       if (!this.mages.isEmpty()) {
           mages_str = "";
           for (Mage m : this.mages) {
               mages_str +=  "'" + m.getName() + "' " ;
           }
        }


        return "Tower{" +
                "name='" + name + '\'' +
                ", height=" + height +
                ", mages=" + mages_str +
                '}';
    }


    public int getHeight() {
        return height;
    }

    public List<Mage> getMages() {
        return mages;
    }

    public String getName() {
        return name;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setMages(List<Mage> mages) {
        this.mages = mages;
    }

    public void addMage(Mage m){
        this.mages.add(m);
    }
}

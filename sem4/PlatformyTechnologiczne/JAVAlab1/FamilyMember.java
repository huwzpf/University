package pt.lab1;

import java.util.Set;

public class FamilyMember implements Comparable<FamilyMember>{
    private String name;
    private int age;
    private double height;
    private Set<FamilyMember> children;

    public FamilyMember(String name, int age, double height, Set<FamilyMember> s){
        this.name = name;
        this.age = age;
        this.height = height;
        this.children = s;
    }

    public String getName(){
        return this.name;
    }

    public Set<FamilyMember> getChildren(){
        return this.children;
    }

    public int getAge(){
        return this.age;
    }

    public double getHeight(){
        return this.height;
    }

    public int getAllChildrenCount(){
        if(this.children == null)
            return 0;
        else{
            int ret = this.children.size();
            for(FamilyMember a : this.children){
                ret += a.getAllChildrenCount();
            }
            return ret;
        }
    }

    public void addApprentices(FamilyMember m){
        this.children.add(m);
    }

    public void setName(String name){
        this.name = name;
    }

    public void setAge(int age){
        this.age = age;
    }

    public void setHeight(double height){
        this.height = height;
    }

    @Override
    public boolean equals(Object obj){
        if(this == obj)
            return true;
        else if (obj == null || obj.getClass() != this.getClass())
            return false;
        else {
            FamilyMember m = (FamilyMember) obj;
            return(m.getName().equals(this.name) && m.getAge() == this.age && m.getHeight() ==this.height);
        }
    }

    @Override
    public int hashCode(){
        return this.name.hashCode() + this.age * (int)this.height;
    }

    public String toString(){
        return String.format("Family member { name = ' %s ', age = %d, height = %f}", this.name, this.age, this.height);
    }

    @Override
    public int compareTo(FamilyMember m) {
        if (this.age != m.getAge()) {
            return  Integer.compare(this.age, m.getAge());
        }
        else if (!this.name.equals(m.getName())) {
            return this.name.compareTo(m.getName());
        }
        else{
            return Double.compare(this.height, m.getHeight());
        }
    }
}

package pt.lab1;

import java.util.Comparator;

public class CmpFamilyMember implements Comparator<FamilyMember>{
    public int compare(FamilyMember a, FamilyMember b){
        if (!a.getName().equals(b.getName())) {
            return a.getName().compareTo(b.getName());
        }
        else if (a.getHeight() != b.getHeight()) {
            return Double.compare(a.getHeight(), b.getHeight());
        }
        else{
            return  Integer.compare(a.getAge(), b.getAge());
        }
    }
}


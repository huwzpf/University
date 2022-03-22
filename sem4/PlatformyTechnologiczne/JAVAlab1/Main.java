package pt.lab1;

import java.util.*;


public class Main {
    private enum EnumSort {NO_SORT, NATURAL, ALTERNATIVE}

    private static void printSet(Set<FamilyMember> s, int depth){
        for (FamilyMember familyMember :s){
            String ret = "-";
            for (int i = 0; i < depth; i++)
                ret += "-";
            ret += familyMember;
            System.out.println(ret);
            if (familyMember.getChildren() != null){
                printSet(familyMember.getChildren(), depth + 1);
            }
        }

    }

    private static Map<FamilyMember, Integer> getChildrenStatistics(List<Set<FamilyMember>> list, EnumSort sort) {
        Map<FamilyMember, Integer> ret;
        switch (sort) {
            case NATURAL ->
                    ret = new TreeMap<FamilyMember, Integer>();
            case ALTERNATIVE ->
                    ret = new TreeMap<FamilyMember, Integer>(new CmpFamilyMember());
            default ->
                    ret = new HashMap<FamilyMember, Integer>();
        }
        for(Set<FamilyMember> s : list){
            for(FamilyMember m : s){
                ret.put(m, m.getAllChildrenCount());
            }
        }
        return ret;
    }

    private static void printFamilyMemberMap(Map<FamilyMember, Integer> map){
        for(Map.Entry<FamilyMember, Integer> entry : map.entrySet()){
            System.out.println(entry.getKey() + " has " + entry.getValue() + " children");
        }
    }

    public static void main( String[] args){
        List<Set<FamilyMember>> sets_arr;
        sets_arr = new ArrayList<Set<FamilyMember>>(6);


        EnumSort sort = EnumSort.NO_SORT;
        if(args.length != 0){
            if (args[0].equals("alternative")){
                sort = EnumSort.ALTERNATIVE;
            }
            else {
                sort = EnumSort.NATURAL;
            }
        }

        switch (sort){
            case NATURAL:
                for ( int i = 0; i < 6; i ++){
                    sets_arr.add(new TreeSet<FamilyMember>());
                }
                break;
            case ALTERNATIVE:
                CmpFamilyMember c = new CmpFamilyMember();
                for ( int i = 0; i < 6; i ++){
                    sets_arr.add(new TreeSet<FamilyMember>(c));
                }
                break;
            default:
                for ( int i = 0; i < 6; i ++) {
                    sets_arr.add(new HashSet<FamilyMember>(2));
                }
                break;
        }



        sets_arr.get(0).add(new FamilyMember("Andrzej", 2, 2, null));
        sets_arr.get(0).add(new FamilyMember("Andrzej", 4, 22, null));
        sets_arr.get(1).add(new FamilyMember("Krzysztof", 2, 2, null));
        sets_arr.get(1).add(new FamilyMember("Bartosz", 4, 22, null));
        sets_arr.get(2).add(new FamilyMember("Cezary", 2, 2, null));
        sets_arr.get(2).add(new FamilyMember("Cyryl", 4, 22, null));
        sets_arr.get(3).add(new FamilyMember("Daniel", 7, 11, sets_arr.get(0)));
        sets_arr.get(3).add(new FamilyMember("Dariusz", 7, 12, sets_arr.get(1)));
        sets_arr.get(4).add(new FamilyMember("Euzebiusz", 8, 10, sets_arr.get(2)));
        sets_arr.get(5).add(new FamilyMember("Zenon", 21, 37, sets_arr.get(3)));
        sets_arr.get(5).add(new FamilyMember("Zbigniew", 37, 21, sets_arr.get(4)));

        printFamilyMemberMap(getChildrenStatistics(sets_arr, sort));
        printSet(sets_arr.get(5), 0);
    }
}
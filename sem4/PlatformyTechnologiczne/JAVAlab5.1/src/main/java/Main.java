import repository.MageRepository;
import controller.MageController;

public class Main {
    public static void Main(String[] args){
        MageRepository mr = new MageRepository();
        MageController mc = new MageController(mr);


        mc.save("Andrzej",1);
        mc.save("Bartosz",2);
        mc.save("Cyprian",3);
        System.out.println(mc.findAllMages());
        System.out.println("");

        System.out.println("Usunięcie nieistniejącego obiektu");
        System.out.println(mc.delete("Dawid"));

        System.out.println("Znalezienie nieistniejącego obiektu");
        System.out.println(mc.find("Dawid"));

        System.out.println("Dodanie istniejącego obiektu");
        System.out.println(mc.save("Andrzej",1));

        System.out.println("Zapis nowego obiektu");
        System.out.println(mc.save("Dawid",4));

        System.out.println("Znalezienie istniejącego obiektu");
        System.out.println(mc.find("Andrzej"));

        System.out.println("Usunięcie nieistniejącego obiektu");
        System.out.println(mc.delete("Andrzej"));


    }
}

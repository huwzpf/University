package pt.lab4;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import java.util.List;

public class MageManager extends PUManager<Mage, String> {
    public MageManager(EntityManagerFactory emf) {
        super(emf, Mage.class);
    }
    public List<Mage> findWithLevelHigherThanFromTower(int l, Tower t){
        EntityManager em = getEmf().createEntityManager();
        List<Mage> mages = em.createQuery("select m from Mage m where m.level > :l and m.tower = :t", Mage.class)
                .setParameter("l", l)
                .setParameter("t", t)
                .getResultList();
        em.close();
        return mages;
    }
}

package pt.lab4;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import java.util.List;

public class TowerManager extends PUManager<Tower, String> {
    public TowerManager(EntityManagerFactory emf) {
        super(emf, Tower.class);
    }
    public List<Tower> findAllLowerThan(int h){
        EntityManager em = getEmf().createEntityManager();
        List<Tower> towers = em.createQuery("select t from Tower t where t.height < :h", Tower.class)
                .setParameter("h", h)
                .getResultList();
        em.close();
        return towers;
    }
}
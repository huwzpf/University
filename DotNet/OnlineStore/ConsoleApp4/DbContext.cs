using DataModel;
using Microsoft.EntityFrameworkCore;

public class MyDbContext : DbContext
{
    public DbSet<Product> Products { get; set; }
    public DbSet<Client> Clients { get; set; }
    public DbSet<Order> Orders { get; set; }
    public DbSet<OrderItem> OrderItems { get; set; }
    public DbSet<EClient> EClients { get; set; }
    public DbSet<EOrder> EOrders { get; set; }

    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        optionsBuilder.UseSqlServer("Data Source=(LocalDB)\\MSSQLLocalDB;AttachDbFilename=C:\\Users\\Dell\\Desktop\\DB.mdf;Integrated Security=True;Connect Timeout=30");
    }


    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<Client>().HasData(
        new Client { ClientId = 1, Name = "Andrzej", Address = "Addr1" },
        new Client { ClientId = 2, Name = "Marcin", Address = "Addr2" },
        new Client { ClientId = 3, Name = "Tomasz", Address = "Addr3" },
        new Client { ClientId = 4, Name = "Marian", Address = "Addr4" },
        new Client { ClientId = 5, Name = "Janusz", Address = "Addr5" },
        new Client { ClientId = 6, Name = "Agnieszka", Address = "Addr6" },
        new Client { ClientId = 7, Name = "Katarzyna", Address = "Addr7" },
        new Client { ClientId = 8, Name = "Maja", Address = "Addr8" },
        new Client { ClientId = 9, Name = "Grzegorz", Address = "Addr9" },
        new Client { ClientId = 10, Name = "Michalina", Address = "Addr10" }
        );

        modelBuilder.Entity<Client>()
               .HasMany(c => c.Orders)
               .WithOne(o => o.Client)
               .HasForeignKey(o => o.ClientId);


        modelBuilder.Entity<Order>()
                .HasMany(z => z.Items)
                .WithOne()
                .HasForeignKey(oi => oi.OrderId);

        modelBuilder.Entity<Client>()
                    .HasDiscriminator<string>("ClientType")
                    .HasValue<Client>("Regular")
                    .HasValue<EClient>("Online");

        modelBuilder.Entity<Order>()
                    .HasDiscriminator<string>("OrderType")
                    .HasValue<Order>("Regular")
                    .HasValue<EOrder>("Online");
    }

}

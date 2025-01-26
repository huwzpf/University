using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using DataModel;

namespace RazorPages.Data;

public class ApplicationDbContext : IdentityDbContext
{
#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options): base(options){}
#pragma warning restore CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.
    public DbSet<Anion> Anions { get; set; }
    public DbSet<Cation> Cations { get; set; }
    public DbSet<Producer> Producer { get; set; }
    public DbSet<Product> Product { get; set; }
    public DbSet<Delivery> Delivery { get; set; }
    public DbSet<Order> Order { get; set; }
    public DbSet<Sales> Sales { get; set; }
    public DbSet<SalesList> SalesList { get; set; }
    public DbSet<CationsList> CationsList { get; set; }
    public DbSet<AnionsList> AnionsList { get; set; }

    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        if (!optionsBuilder.IsConfigured)
        {
            optionsBuilder.UseSqlServer("(localdb)\\mssqllocaldb;Database=WaterMgmt;Trusted_Connection=True;MultipleActiveResultSets=true");
        }
    }
}
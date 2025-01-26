using DataModel;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Repository;

public class ProductRepository : IProductRepository
{
    private readonly ApplicationDbContext _context;

    public ProductRepository(ApplicationDbContext context)
    {
        _context = context;
    }

    public async Task<List<Product>> GetProducts()
    {
        return await _context.Product.Select(x => new Product
        {
            Id = x.Id,
            Name = x.Name,
            Type = x.Type,
            Producer= x.Producer,
            PH= x.PH,
            PackingType= x.PackingType,
            Volume= x.Volume
        }).ToListAsync();
    }
}
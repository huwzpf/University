using DataModel;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Model;

public class DataRepository : IDataRepository
{
    private readonly ApplicationDbContext _context;


    public DataRepository(ApplicationDbContext context)
    {
        _context = context;
    }

	public IEnumerable<string> GetAvailableWaterTypes()
    {
        var values = Enum.GetValues(typeof(SupportedTypes));
        foreach (var item in values)
        {
            yield return item.ToString();
        }
	}

	public List<Product> GetProducts() => _context.Product.ToList();
    public List<string> GetUserNames() => _context.Users.Select(u => u.UserName).ToList();

    public async Task UpdateStorageAfterBuy(string productName, int amount){

        var product = await _context.Product.FirstAsync(p => p.Name == productName);

        if(product.AvailableAmount >= amount)
        {
            product.AvailableAmount -= amount;
        }

        _ = await _context.SaveChangesAsync();
    }

	public async Task AddSales(Sales sales)
    {
        _context.Sales.Add(sales);
		_ = await _context.SaveChangesAsync();
	}
}
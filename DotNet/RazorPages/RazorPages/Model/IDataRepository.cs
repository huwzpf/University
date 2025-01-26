using DataModel;

namespace RazorPages.Model;

public interface IDataRepository
{
	IEnumerable<string> GetAvailableWaterTypes();
	List<Product> GetProducts();
    List<string> GetUserNames();

	Task AddSales(Sales sales);
    Task UpdateStorageAfterBuy(string productName, int amount);
}
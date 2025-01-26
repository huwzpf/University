using DataModel;
using JetBrains.Annotations;

namespace RazorPages.Repository;

public interface IProductRepository
{
    [UsedImplicitly]
    Task<List<Product>> GetProducts();
}
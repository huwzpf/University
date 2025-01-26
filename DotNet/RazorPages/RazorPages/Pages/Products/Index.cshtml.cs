using DataModel;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Pages.Products;

public class IndexModel : PageModel
{
    private readonly Data.ApplicationDbContext _context;

    public IndexModel(Data.ApplicationDbContext context)
    {
        _context = context;
    }

    public IList<Product> Product { get;set; } = default!;

    public async Task OnGetAsync()
    {
        Product = await _context.Product
            .Include(x => x.Producer)
            .ToListAsync();
    }
}
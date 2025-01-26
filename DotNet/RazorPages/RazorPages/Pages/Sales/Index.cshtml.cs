using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Pages.Sales;

public class IndexModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public IndexModel(ApplicationDbContext context)
    {
        _context = context;

        Sales = new List<DataModel.Sales>();
    }

    public IList<DataModel.Sales> Sales { get; set; }

    public async Task OnGetAsync()
    {
        Sales = await _context.Sales.ToListAsync();
    }
}
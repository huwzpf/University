using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Pages.Delivery;

public class IndexModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public IndexModel(ApplicationDbContext context)
    {
        _context = context;
    }

    public IList<DataModel.Delivery> Delivery { get;set; } = default!;

    public async Task OnGetAsync()
    {
        Delivery = await _context.Delivery.ToListAsync();
    }
}
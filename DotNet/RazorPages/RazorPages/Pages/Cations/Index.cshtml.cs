using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Cations;

public class IndexModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public IndexModel(ApplicationDbContext context)
    {
        _context = context;

        Cation = new List<Cation>();
    }

    public IList<Cation> Cation { get; set; }

    public async Task OnGetAsync()
    {
        Cation = await _context.Cations.ToListAsync();
    }
}
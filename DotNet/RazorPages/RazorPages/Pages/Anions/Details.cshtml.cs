using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Anions;

public class DetailsModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DetailsModel(ApplicationDbContext context)
    {
        _context = context;

        Anion = new Anion();
    }

    public Anion Anion { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var anion = await _context.Anions.FirstOrDefaultAsync(x => x.Id == id);
        if (anion == null)
        {
            return NotFound();
        }

        Anion = anion;
        return Page();
    }
}
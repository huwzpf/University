using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Anions;

public class DeleteModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DeleteModel(ApplicationDbContext context)
    {
        _context = context;

        Anion = new Anion();
    }

    [BindProperty]
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

    public async Task<IActionResult> OnPostAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var anion = await _context.Anions.FindAsync(id);
        if (anion == null)
        {
            return RedirectToPage("./Index");
        }

        Anion = anion;
        _context.Anions.Remove(Anion);
        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
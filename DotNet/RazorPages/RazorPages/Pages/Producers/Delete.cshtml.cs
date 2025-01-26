using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Producers;

public class DeleteModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DeleteModel(ApplicationDbContext context)
    {
        _context = context;

        Producer = new Producer();
    }

    [BindProperty]
    public Producer Producer { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var producer = await _context.Producer.FirstOrDefaultAsync(x => x.Id == id);
        if (producer == null)
        {
            return NotFound();
        }
        
        Producer = producer;
        return Page();
    }

    public async Task<IActionResult> OnPostAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var producer = await _context.Producer.FindAsync(id);
        if (producer == null)
        {
            return RedirectToPage("./Index");
        }

        Producer = producer;
        _context.Producer.Remove(Producer);
        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Pages.Delivery;

public class DeleteModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DeleteModel(ApplicationDbContext context)
    {
        _context = context;

        Delivery = new DataModel.Delivery();
    }

    [BindProperty]
    public DataModel.Delivery Delivery { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var delivery = await _context.Delivery.FirstOrDefaultAsync(x => x.Id == id);
        if (delivery == null)
        {
            return NotFound();
        }

        Delivery = delivery;
        return Page();
    }

    public async Task<IActionResult> OnPostAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var delivery = await _context.Delivery.FindAsync(id);

        // ReSharper disable once InvertIf
        if (delivery != null)
        {
            Delivery = delivery;
            _context.Delivery.Remove(Delivery);

            await _context.SaveChangesAsync();
        }

        return RedirectToPage("./Index");
    }
}
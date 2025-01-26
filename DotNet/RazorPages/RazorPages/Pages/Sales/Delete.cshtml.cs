using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Pages.Sales;

public class DeleteModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DeleteModel(ApplicationDbContext context)
    {
        _context = context;

        Sales = new DataModel.Sales();
    }

    [BindProperty]
    public DataModel.Sales Sales { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var sales = await _context.Sales.FirstOrDefaultAsync(x => x.Id == id);
        if (sales == null)
        {
            return NotFound();
        }
        
        Sales = sales;
        return Page();
    }

    public async Task<IActionResult> OnPostAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }
        var sales = await _context.Sales.FindAsync(id);
        if (sales == null)
        {
            return RedirectToPage("./Index");
        }
        
        Sales = sales;
        _context.Sales.Remove(Sales);
        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
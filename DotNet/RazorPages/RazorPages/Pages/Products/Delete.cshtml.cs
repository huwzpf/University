using DataModel;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Pages.Products;

public class DeleteModel : PageModel
{
    private readonly Data.ApplicationDbContext _context;

    public DeleteModel(Data.ApplicationDbContext context)
    {
        _context = context;

        Product = new Product();
    }

    [BindProperty]
    public Product Product { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var product = await _context.Product.FirstOrDefaultAsync(x => x.Id == id);
        if (product == null)
        {
            return NotFound();
        }
         
        
        Product = product;
        return Page();
    }

    public async Task<IActionResult> OnPostAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var product = await _context.Product.FindAsync(id);
        if (product == null)
        {
            return RedirectToPage("./Index");
        }

        Product = product;
        _context.Product.Remove(Product);
        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
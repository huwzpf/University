using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using DataModel;
using RazorPages.Data;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Pages.Sales;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Sales = new DataModel.Sales();
        Products = new List<Product>();
    }

    public async Task<IActionResult> OnGetAsync()
    {
        Products = await _context.Product.ToListAsync();
        return Page();
    }

    [BindProperty]
    public DataModel.Sales Sales { get; set; }
    public List<Product> Products { get; set; }

    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        _context.Sales.Add(Sales);


        Products = await _context.Product.ToListAsync();
        if (Sales.SalesList == null)
        {
            return RedirectToPage("./Index");
        }

        foreach (var item in Sales.SalesList)
        {
            foreach (var product in Products.Where(product => product.Id == item.IdOfProduct))
            {
                product.AvailableAmount -= item.Amount;
            }
        }


        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
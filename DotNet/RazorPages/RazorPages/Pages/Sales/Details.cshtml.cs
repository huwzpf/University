using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Sales;

public class DetailsModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DetailsModel(ApplicationDbContext context)
    {
        _context = context;

        Sales = new DataModel.Sales();
        SalesList = new List<SalesList>();
    }

    public DataModel.Sales Sales { get; set; }
    public List<SalesList> SalesList { get; set; }

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
        SalesList = await _context.SalesList.Where(x => x.IdOfSale == sales).ToListAsync();

        return Page();
    }
}
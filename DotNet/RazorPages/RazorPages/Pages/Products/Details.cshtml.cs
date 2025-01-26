using DataModel;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Pages.Products;

public class DetailsModel : PageModel
{
    private readonly Data.ApplicationDbContext _context;

    public DetailsModel(Data.ApplicationDbContext context)
    {
        _context = context;

        Product = new Product();
        Cations = new List<Cation>();
        Anions = new List<Anion>();
    }

    public Product Product { get; set; }
    public List<Cation> Cations { get; set; }
    public List<Anion> Anions { get; set; }

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

        var cationsLists = await _context.CationsList.Where(x => x.IdOfProduct == product).ToListAsync();
        foreach (var cation in cationsLists.Select(x => _context.Cations.FirstOrDefault(cation => cation.Id == x.IdOfCation)))
        {
            if (cation != null) Cations.Add(cation);
        }

        var anionsLists = await _context.AnionsList.Where(x => x.IdOfProduct == product).ToListAsync();
        foreach (var anion in anionsLists.Select(x => _context.Anions.FirstOrDefault(anion => anion.Id == x.IdOfAnion)))
        {
            if (anion != null) Anions.Add(anion);
        }

        return Page();
    }
}
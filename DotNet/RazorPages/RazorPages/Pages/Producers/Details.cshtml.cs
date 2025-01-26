using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Producers;

public class DetailsModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DetailsModel(ApplicationDbContext context)
    {
        _context = context;

        Producer = new Producer();
    }

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
}
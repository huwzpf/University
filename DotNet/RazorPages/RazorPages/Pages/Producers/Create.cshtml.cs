using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Producers;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Producer = new Producer();
    }

    public IActionResult OnGet()
    {
        return Page();
    }

    [BindProperty]
    public Producer Producer { get; set; }
        

    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        if (!ModelState.IsValid)
        {
            return Page();
        }

        _context.Producer.Add(Producer);
        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
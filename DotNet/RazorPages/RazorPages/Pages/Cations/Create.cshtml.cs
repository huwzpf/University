using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using DataModel;
using RazorPages.Data;
using System.Globalization;

namespace RazorPages.Pages.Cations;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Cation = new Cation();
    }

    public IActionResult OnGet()
    {
        return Page();
    }

    [BindProperty]
    public Cation Cation { get; set; }
        

    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        if (!ModelState.IsValid)
        {
            return Page();
        }

        var concentrationAsText = Request.Form["Cation.Concentration"];

        try
        {
            double concentration = double.Parse(concentrationAsText, CultureInfo.InvariantCulture);
            Cation.Concentration = concentration;

            _context.Cations.Add(Cation);
            await _context.SaveChangesAsync();

            return RedirectToPage("./Index");
        }
        catch (Exception)
        {
            return Page();
        }
    }
}
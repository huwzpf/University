using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using DataModel;
using RazorPages.Data;
using System.Globalization;
using Microsoft.AspNetCore.Localization;

namespace RazorPages.Pages.Anions;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Anion = new Anion();
    }

    public IActionResult OnGet()
    {
        return Page();
    }

    [BindProperty]
    public Anion Anion { get; set; }

    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        if (!ModelState.IsValid)
        {
            return Page();
        }

        var concentrationAsText = Request.Form["Anion.Concentration"];

        try
        {
            double concentration = double.Parse(concentrationAsText, CultureInfo.InvariantCulture);
            Anion.Concentration = concentration;

            _context.Anions.Add(Anion);
            await _context.SaveChangesAsync();

            return RedirectToPage("./Index");
        }
        catch (Exception)
        {
            return Page();
        }
    }
}
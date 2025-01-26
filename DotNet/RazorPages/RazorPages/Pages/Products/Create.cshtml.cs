using DataModel;
using Microsoft.AspNetCore.Localization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;
using System.Globalization;

namespace RazorPages.Pages.Products;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Product = new Product();
        Anions = new List<Anion>();
        Cations = new List<Cation>();
    }

    public async Task<IActionResult> OnGetAsync()
    {
        ViewData["IdOfAnion"] = new SelectList(_context.Anions, "Id", "Name");
        ViewData["IdOfCation"] = new SelectList(_context.Cations, "Id", "Name");
        ViewData["IdOfProducer"] = new SelectList(_context.Producer, "Name", "Email");

        Cations = await _context.Cations.ToListAsync();
        return Page();
    }

    [BindProperty]
    public Product Product { get; set; }
    public List<Cation> Cations { get; set; }
    public List<Anion> Anions { get; set; }


    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        var cations = Request.Form["Cations"];
        var anions = Request.Form["Anions"];

        var culture = HttpContext.Features.Get<IRequestCultureFeature>();

        var phString = Request.Form["Product.PH"];
        var phFloat = float.Parse(phString, CultureInfo.InvariantCulture);

        var objString = Request.Form["obj"];
        var objFloat = float.Parse(objString, CultureInfo.InvariantCulture);

        if (Product.ImageFile != null)
        {
            byte[]? data;
            await using (var fs = Product.ImageFile.OpenReadStream())
            {
                using var br = new BinaryReader(fs);
                data = br.ReadBytes((int)fs.Length);
                br.Dispose();
            }
            Product.ImageData = Convert.ToBase64String(data, 0, data.Length);
        }

        double ions = 0;

        foreach (var item in cations)
        {
            var cationList = new CationsList
            {
                IdOfCation = int.Parse(item),
                IdOfProduct = Product
            };
            _context.CationsList.Add(cationList);

            var cation = _context.Cations.FirstOrDefault(x => x.Id == int.Parse(item));
            if (cation != null) ions += cation.Concentration;
        }

        foreach (var item in anions)
        {
            var anionList = new AnionsList
            {
                IdOfAnion = int.Parse(item),
                IdOfProduct = Product
            };
            _context.AnionsList.Add(anionList);

            var anion = _context.Anions.FirstOrDefault(x => x.Id == int.Parse(item));
            if (anion != null) ions += anion.Concentration;
        }

        Product.Mineralization = ions switch
        {
            <= 0.05 => "very low mineralized ",
            <= 0.5 => "low mineralized ",
            <= 1.5 => "on average mineralized ",
            _ => "highly mineralized "
        };

        Product.PH = phFloat;
        Product.Volume = objFloat;
        _context.Product.Add(Product);


        await _context.SaveChangesAsync();

        return RedirectToPage("./Index");
    }
}
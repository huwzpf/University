using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using DataModel;
using RazorPages.Data;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Pages.Delivery;

public class CreateModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public CreateModel(ApplicationDbContext context)
    {
        _context = context;

        Delivery = new DataModel.Delivery();
        Product = new Product();
        Products = new List<Product>();
        Orders = new List<Order>();
    }


    public async Task<IActionResult> OnGetAsync()
    {
        Products = await _context.Product.ToListAsync();
			
        return Page();
    }

    [BindProperty]
    public DataModel.Delivery Delivery { get; set; }
    public List<Product> Products { get; set; }


    [BindProperty]
    public Product Product { get; set; }
    [BindProperty]
    public List<Order> Orders { get; set; }

    // To protect from overposting attacks, see https://aka.ms/RazorPagesCRUD
    public async Task<IActionResult> OnPostAsync()
    {
        _context.Delivery.Add(Delivery);
        Products = await _context.Product.ToListAsync();
        
        if (Delivery.Order == null)
        {
            return RedirectToPage("./Index");
        }

        foreach (var item in Delivery.Order)
        {
            foreach (var product in Products.Where(x => x.Id == item.IdOfProduct))
            {
                product.AvailableAmount += Delivery.Pallets * item.Amount;
            }
        }

        await _context.SaveChangesAsync();
        return RedirectToPage("./Index");
    }
}
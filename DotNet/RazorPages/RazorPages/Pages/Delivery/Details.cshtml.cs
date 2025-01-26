using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using DataModel;
using RazorPages.Data;

namespace RazorPages.Pages.Delivery;

public class DetailsModel : PageModel
{
    private readonly ApplicationDbContext _context;

    public DetailsModel(ApplicationDbContext context)
    {
        _context = context;

        Delivery = new DataModel.Delivery();
        Products = new List<Product>();
        Orders = new List<Order>();
    }

    public DataModel.Delivery Delivery { get; set; }
    public List<Product> Products { get; set; }
    public List<Order> Orders { get; set; }

    public async Task<IActionResult> OnGetAsync(int? id)
    {
        if (id == null)
        {
            return NotFound();
        }

        var delivery = await _context.Delivery.FirstOrDefaultAsync(x => x.Id == id);
        if (delivery == null)
        {
            return NotFound();
        }
        Delivery = delivery;

        Orders=await _context.Order.Where(x => x.IdOfDelivery == delivery).ToListAsync();
        return Page();
    }
}
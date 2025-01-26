using System.Globalization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Options;

namespace RazorPages.Pages;

public class IndexModel : PageModel
{
    private readonly IOptions<RequestLocalizationOptions> _localizationOptions;
    

    public IndexModel(IOptions<RequestLocalizationOptions> localizationOptions)
    {
        _localizationOptions = localizationOptions;

        SupportedCultures = (_localizationOptions.Value.SupportedUICultures ?? throw new NullReferenceException()).ToList();
    }

    public IEnumerable<CultureInfo> SupportedCultures { get; set; }

    [BindProperty]
    public CultureInfo? CurrentUICulture { get; set; }

    public void OnGet()
    {
        var cookieResult = HttpContext.Request.Cookies["Culture"];

        try
        {
            CurrentUICulture = SupportedCultures.First(x => x.Name.Equals(cookieResult));
        }
        catch (Exception)
        {
            CurrentUICulture = CultureInfo.CurrentUICulture;
        }

        ViewData["Culture"] = CurrentUICulture;
    }

    public IActionResult OnPost()
    {
        Response.Cookies.Append(
            "Culture",
            CurrentUICulture?.Name ?? throw new InvalidOperationException(),
            new CookieOptions { Expires = DateTimeOffset.UtcNow.AddYears(1) }
        );

        ViewData["Culture"] = CurrentUICulture;

        Thread.CurrentThread.CurrentCulture = CurrentUICulture;
        Thread.CurrentThread.CurrentUICulture = CurrentUICulture;

        return RedirectToPage("./Index");
    }
}
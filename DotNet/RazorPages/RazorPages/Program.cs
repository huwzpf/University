using System.Globalization;
using DataModel;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;
using RazorPages.Repository;
using Microsoft.AspNetCore.Mvc;
using RazorPages.Model;
using Microsoft.AspNetCore.Localization;
using Microsoft.Extensions.Options;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");
builder.Services.AddDbContext<ApplicationDbContext>(options =>
    options.UseSqlServer(connectionString));
builder.Services.AddDatabaseDeveloperPageExceptionFilter();

builder.Services.AddDefaultIdentity<IdentityUser>(options => options.SignIn.RequireConfirmedAccount = false)
    .AddEntityFrameworkStores<ApplicationDbContext>();

//swagger
builder.Services.AddScoped<IDataRepository, DataRepository>();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();


builder.Services.AddScoped<IProducerRepository, ProducerRepository>();
builder.Services.AddScoped<ICationRepository, CationRepository>();
builder.Services.AddScoped<IAnionRepository, AnionRepository>();
builder.Services.AddScoped<IProductRepository, ProductRepository>();

builder.Services.AddLocalization(options => options.ResourcesPath = "Resources");

builder.Services.AddRazorPages();

// Add internationalization support
builder.Services.AddControllersWithViews();
builder.Services.Configure<RequestLocalizationOptions>(options =>
{
    var supportedCultures = new[]
    {
        new CultureInfo("en-GB"),
        new CultureInfo("pl-PL")
    };

    options.DefaultRequestCulture = new RequestCulture(supportedCultures.First());
    options.SupportedCultures = supportedCultures;
    options.SupportedUICultures = supportedCultures;
});

var app = builder.Build();
app.UseSwaggerUI();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseMigrationsEndPoint();
}
else
{
    app.UseExceptionHandler("/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseSwagger(c => c.SerializeAsV2 = true);

app.MapGet("/storage", ([FromServices] IDataRepository db) => db.GetProducts());
app.MapGet("/watertypes", ([FromServices] IDataRepository db) => db.GetAvailableWaterTypes().ToList());
app.MapGet("/registeredusers", ([FromServices] IDataRepository db) => db.GetUserNames());

app.MapPost("/addsale", (Sales sales, [FromServices] IDataRepository db) => 
{
	try
	{
		db.AddSales(sales);
		return Results.Ok(sales);
	}
	catch (Exception)
	{
		return Results.NotFound();
	}
});

app.MapPost("/buy", (string productName, int amount, [FromServices] IDataRepository db) => db.UpdateStorageAfterBuy(productName, amount));


//app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

var requestLocationOptions = app.Services.GetService<IOptions<RequestLocalizationOptions>>();
if (requestLocationOptions != null)
{
    app.UseRequestLocalization(requestLocationOptions.Value);
}

app.UseAuthentication();
app.UseAuthorization();

app.MapRazorPages();
app.UseSwaggerUI();
app.Run();

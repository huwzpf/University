using DataModel;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Repository;

public class CationRepository : ICationRepository
{
    private readonly ApplicationDbContext _context;

    public CationRepository(ApplicationDbContext context)
    {
        _context = context;
    }
    
    public async Task<List<Cation>> GetCations()
    {
        return await _context.Cations.Select(x => new Cation
        {
            Id = x.Id,
            Name = x.Name,
            Symbol = x.Symbol,
            Concentration = x.Concentration
        }).ToListAsync();
    }
}
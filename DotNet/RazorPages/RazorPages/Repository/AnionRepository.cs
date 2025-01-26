using DataModel;
using RazorPages.Data;
using Microsoft.EntityFrameworkCore;

namespace RazorPages.Repository;

public class AnionRepository : IAnionRepository
{
    private readonly ApplicationDbContext _context;

    public AnionRepository(ApplicationDbContext context)
    {
        _context = context;
    }

    public async Task<List<Anion>> GetAnions()
    {
        return await _context.Anions.Select(x => new Anion
        {
            Id = x.Id,
            Name = x.Name,
            Symbol = x.Symbol,
            Concentration = x.Concentration
        }).ToListAsync();
    }
}
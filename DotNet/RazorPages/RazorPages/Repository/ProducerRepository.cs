using DataModel;
using Microsoft.EntityFrameworkCore;
using RazorPages.Data;

namespace RazorPages.Repository;

public class ProducerRepository : IProducerRepository
{
    private readonly ApplicationDbContext _context;

    public ProducerRepository(ApplicationDbContext context)
    {
        _context = context;
    }
    public async Task<List<Producer>> GetProducers()
    {
        return await _context.Producer.Select(x => new Producer
        {
            Id = x.Id,
            Name = x.Name,
            Email = x.Email,
            Phone = x.Phone
        }).ToListAsync();
    }
}
using DataModel;
using JetBrains.Annotations;

namespace RazorPages.Repository;

public interface IProducerRepository
{
    [UsedImplicitly]
    Task<List<Producer>> GetProducers();
}
using DataModel;
using JetBrains.Annotations;

namespace RazorPages.Repository;

public interface ICationRepository
{
    [UsedImplicitly]
    Task<List<Cation>> GetCations();
}
using DataModel;
using JetBrains.Annotations;

namespace RazorPages.Repository;

public interface IAnionRepository
{
    [UsedImplicitly]
    Task<List<Anion>> GetAnions();
}
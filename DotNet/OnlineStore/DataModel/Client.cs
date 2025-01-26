using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class Client
    {
        public int ClientId { get; set; }
        public string Name { get; set; }
        public string Address { get; set; }
        public List<Order> Orders { get; set; }

        public decimal HowMuchOrdered()
        {
            return Orders?.Sum(o => o.TotalPrice()) ?? 0;
        }
    }

    public class EClient : Client
    {
        public string IpAddress { get; set; }
    }
}

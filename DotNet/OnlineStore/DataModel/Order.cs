using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class OrderItem
    {
        public int OrderItemId { get; set; }
        public int ProductId { get; set; }
        public int OrderId { get; set; }
        public Product Product { get; set; }
        public int Quantity { get; set; }
    }

    public class Order
    {
        public int OrderId { get; set; }
        public List<OrderItem> Items { get; set; } = new List<OrderItem>();
        public int ClientId { get; set; }
        public Client Client { get; set; }
        public bool Completed { get; set; }

        public decimal TotalPrice()
        {
            return Items.Sum(item => item.Product.Price * item.Quantity);
        }

        public int AmountOfItems()
        {
            return Items.Sum(item => item.Quantity);
        }
    }

    public class EOrder : Order
    {
        public string IpAddress { get; set; }
    }
}

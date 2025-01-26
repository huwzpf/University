using Terminal.Gui;
using System;
using System.Linq;
using DataModel;
using Microsoft.EntityFrameworkCore;

class Program
{

    static Client activeClient;
    static StatusBar activeClientStatusBar;
    static int currentPage = 0;
    const int pageSize = 5;
    static ListView productsList;
    static void Main()
    {
        Application.Init();
        var top = Application.Top;

        var win = new Window("Product Management")
        {
            X = 0,
            Y = 1,
            Width = Dim.Fill(),
            Height = Dim.Fill()
        };
        top.Add(win);

        // Add your UI elements here
        productsList = new ListView()
        {
            Width = Dim.Fill(),
            Height = Dim.Fill(),
            AllowsMarking = false,
        };
        win.Add(productsList);


        productsList.SetSource(GetAllProducts());

        activeClientStatusBar = new StatusBar(new StatusItem[] {
            new StatusItem(Key.Null, "Active Client: None - Total Ordered: $0.00", null)
        });
        top.Add(activeClientStatusBar);
        var menu = new MenuBar(new MenuBarItem[] {
            new MenuBarItem ("_Actions", new MenuItem [] {
                new MenuItem ("_Add Product", "", () => AddProduct(productsList)),
                new MenuItem ("_Delete Product", "", () => DeleteProduct(productsList)),
                new MenuItem ("_Increase Stock", "", () => IncreaseProductStock(productsList)),
                new MenuItem("_Update Product Price", "", () => UpdateProductPrice(productsList)),
                new MenuItem("_Choose Active Client", "", () => ChooseActiveClient()),
                new MenuItem("_Create Order for Active Client", "", () => CreateOrderForActiveClient()),
                new MenuItem("_View Orders", "", ViewOrders),
                new MenuItem("_Fiind client by ordered items", "", FindClientsByOrderedItem),
                new MenuItem ("_Quit", "", () => { if (Quit()) top.Running = false; }),
                new MenuItem ("_Delete All Odders", "", DeleteAllOrders),
            })
        });
        top.Add(menu);

        Application.Run();
    }
    static void DeleteAllOrders()
    {
        using (var context = new MyDbContext())
        {
            // Retrieve all orders from the database
            var allOrders = context.Orders.ToList();

            // Remove each order
            foreach (var order in allOrders)
            {
                context.Orders.Remove(order);
            }

            // Save the changes to the database
            context.SaveChanges();
        }
    }

    static void UpdateProductPrice(ListView listView)
    {
        var dialog = new Dialog("Update Product Price", 60, 10, new Button("Cancel"));

        var idLabel = new Label(2, 2, "Product ID:");
        var idText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(idLabel),
            Width = 40
        };
        dialog.Add(idLabel, idText);

        var priceLabel = new Label(2, 4, "New Price:");
        var priceText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(priceLabel),
            Width = 40
        };
        dialog.Add(priceLabel, priceText);

        var updateButton = new Button("Update")
        {
            X = Pos.Right(priceText) - 18,
            Y = Pos.Top(priceLabel) + 2
        };
        updateButton.Clicked += () =>
        {
            if (int.TryParse(idText.Text.ToString(), out int productId) &&
                decimal.TryParse(priceText.Text.ToString(), out decimal newPrice))
            {
                UpdatePrice(productId, newPrice);
                listView.SetSource(GetAllProducts());
            }
            Application.RequestStop();
        };
        dialog.AddButton(updateButton);

        Application.Run(dialog);
    }

    static void UpdatePrice(int productId, decimal newPrice)
    {
        using (var context = new MyDbContext())
        {
            var product = context.Products.FirstOrDefault(p => p.ProductId == productId);
            if (product != null)
            {
                product.Price = newPrice;
                context.SaveChanges();
            }
        }
    }

    static void UpdateActiveClientDisplay()
    {
        if (activeClient != null)
        {
            activeClientStatusBar.Items[0].Title = $"Active Client: {activeClient.Name} - Total Ordered: {activeClient.HowMuchOrdered():C}";
        }
        else
        {
            activeClientStatusBar.Items[0].Title = "Active Client: None - Total Ordered: $0.00";
        }
    }

    static void ChooseActiveClient()
    {
        var clients = GetAllClients();
        var clientNames = clients.Select(c => c.Name).ToList();

        var dialog = new Dialog("Choose Active Client", 60, 18, new Button("Cancel"));

        var searchLabel = new Label(2, 2, "Search:");
        var searchField = new TextField("")
        {
            X = 20,
            Y = Pos.Top(searchLabel),
            Width = 40
        };
        dialog.Add(searchLabel, searchField);

        var listView = new ListView(clientNames)
        {
            X = 2,
            Y = 4,
            Width = Dim.Fill() - 4,
            Height = 10
        };
        dialog.Add(listView);

        searchField.TextChanged += (args) =>
        {
            var filteredClients = clients.Where(c => c.Name.ToLower().Contains(searchField.Text.ToString().ToLower())).ToList();
            listView.SetSource(filteredClients.Select(c => c.Name).ToList());
        };

        var selectButton = new Button("Select", true)
        {
            X = Pos.Center(),
            Y = Pos.Bottom(listView) + 1
        };
        selectButton.Clicked += () =>
        {
            var selectedIndex = listView.SelectedItem;
            if (selectedIndex >= 0 && selectedIndex < clients.Count)
            {
                activeClient = clients[selectedIndex];
                MessageBox.Query("Active Client", $"Active client set to: {activeClient.Name}", "Ok");
            }

            UpdateActiveClientDisplay();
            Application.RequestStop();
        };
        dialog.AddButton(selectButton);
        Application.Run(dialog);
    }

    static List<Client> GetAllClients()
    {
        using (var context = new MyDbContext())
        {
            return context.Clients.OrderBy(c => c.Name).ToList();
        }
    }

    static bool Quit()
    {
        var n = MessageBox.Query(50, 7, "Quit", "Are you sure you want to quit?", "Yes", "No");
        return n == 0;
    }

    static void AddProduct(ListView listView)
    {
        var dialog = new Dialog("Add Product", 60, 10, new Button("Cancel"));

        var nameLabel = new Label(2, 2, "Name:");
        var nameText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(nameLabel),
            Width = 40
        };
        dialog.Add(nameLabel, nameText);

        var addButton = new Button("Add")
        {
            X = Pos.Right(nameText) - 18,
            Y = Pos.Top(nameLabel) + 2
        };
        addButton.Clicked += () =>
        {
            AddNewProduct(nameText.Text.ToString());
            listView.SetSource(GetAllProducts());
            Application.RequestStop();
        };
        dialog.AddButton(addButton);

        Application.Run(dialog);
    }

    static void AddNewProduct(string name)
    {
        using (var context = new MyDbContext())
        {
            var product = new Product { Name = name };
            context.Products.Add(product);
            context.SaveChanges();
        }
    }

    static void DeleteProduct(ListView listView)
    {
        var deleteDialog = new Dialog("Delete Product", 60, 10, new Button("Cancel"));

        var idLabel = new Label(2, 2, "Product ID:");
        var idText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(idLabel),
            Width = 40
        };
        deleteDialog.Add(idLabel, idText);

        var deleteButton = new Button("Delete")
        {
            X = Pos.Right(idText) - 18,
            Y = Pos.Top(idLabel) + 2
        };
        deleteButton.Clicked += () =>
        {
            if (int.TryParse(idText.Text.ToString(), out int productId))
            {
                DeleteProductFromDatabase(productId);
                listView.SetSource(GetAllProducts());
            }
            Application.RequestStop();
        };
        deleteDialog.AddButton(deleteButton);

        Application.Run(deleteDialog);
    }

    static void DeleteProductFromDatabase(int productId)
    {
        using (var context = new MyDbContext())
        {
            var product = context.Products.FirstOrDefault(p => p.ProductId == productId);
            if (product != null)
            {
                context.Products.Remove(product);
                context.SaveChanges();
            }
        }
    }

    static System.Collections.Generic.List<Product> GetAllProducts()
    {
        using (var context = new MyDbContext())
        {
            return context.Products.ToList();
        }
    }

    static void IncreaseProductStock(ListView listView)
    {
        var dialog = new Dialog("Increase Product Stock", 60, 10, new Button("Cancel"));

        var idLabel = new Label(2, 2, "Product ID:");
        var idText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(idLabel),
            Width = 40
        };
        dialog.Add(idLabel, idText);

        var amountLabel = new Label(2, 4, "Amount to Increase:");
        var amountText = new TextField("")
        {
            X = 20,
            Y = Pos.Top(amountLabel),
            Width = 40
        };
        dialog.Add(amountLabel, amountText);

        var increaseButton = new Button("Increase")
        {
            X = Pos.Right(amountText) - 20,
            Y = Pos.Top(amountLabel) + 2
        };
        increaseButton.Clicked += () =>
        {
            if (int.TryParse(idText.Text.ToString(), out int productId) &&
                int.TryParse(amountText.Text.ToString(), out int amount))
            {
                IncreaseStock(productId, amount);
                listView.SetSource(GetAllProducts());
            }
            Application.RequestStop();
        };
        dialog.AddButton(increaseButton);

        Application.Run(dialog);
    }

    static void IncreaseStock(int productId, int amount)
    {
        using (var context = new MyDbContext())
        {
            var product = context.Products.FirstOrDefault(p => p.ProductId == productId);
            if (product != null)
            {
                product.Stock += amount;
                context.SaveChanges();
            }
        }
    }
    public static List<Order> GetAllOrders()
    {
        using (var context = new MyDbContext())
        {
            return context.Orders.Include(o => o.Client).Include(o => o.Items).ThenInclude(i => i.Product).ToList();
        }
    }
    static void CompleteOrder(Order order)
    {
        using (var context = new MyDbContext())
        {
            using (var transaction = context.Database.BeginTransaction())
            {
                try
                {
                    // Attach the order to the context
                    context.Orders.Attach(order);

                    // Update the order's Completed status
                    order.Completed = true;

                    // Update stock for each product in the order
                    foreach (var item in order.Items)
                    {
                        var product = context.Products.FirstOrDefault(p => p.ProductId == item.ProductId);
                        if (product != null && product.Stock >= item.Quantity)
                        {
                            product.Stock -= item.Quantity;
                        }
                        else
                        {
                            MessageBox.Query("Error", "Insufficient stock for order completion.", "Ok");
                            transaction.Rollback();
                            return;
                        }
                    }

                    context.SaveChanges();
                    transaction.Commit();
                    MessageBox.Query("Success", "Order completed successfully.", "Ok");
                    productsList.SetSource(GetAllProducts());
                    UpdateActiveClientDisplay();
                }
                catch
                {
                    transaction.Rollback();
                    MessageBox.Query("Error", "An error occurred while completing the order.", "Ok");
                }
            }
        }
    }

    static void ViewOrders()
    {
        var orders = GetAllOrders();
        ShowOrdersPage(orders);

        void ShowOrdersPage(List<Order> allOrders)
        {
            var paginatedOrders = allOrders.Skip(currentPage * pageSize).Take(pageSize).ToList();
            var orderDescriptions = paginatedOrders.Select(o =>
                $"Items: {o.AmountOfItems()}, Total: {o.TotalPrice():C}, Client: {o.Client.Name}, Completed: {o.Completed}" + (o is EOrder ? ", Online Order" : "")
            ).ToList();

            var dialog = new Dialog("Orders", 80, 40);

            var listView = new ListView(orderDescriptions)
            {
                X = 1,
                Y = 1,
                Width = Dim.Fill(),
                Height = pageSize
            };
            dialog.Add(listView);

            var closeButton = new Button("Close")
            {
                X = Pos.Percent(50) - 10, // Adjust position as needed
                Y = Pos.Bottom(listView) + 2
            };
            closeButton.Clicked += () =>
            {
                Application.RequestStop();
            };
            dialog.AddButton(closeButton);


            var nextPageButton = new Button("Next")
            {
                X = Pos.Percent(70),
                Y = Pos.Bottom(listView) + 1
            };
            nextPageButton.Clicked += () =>
            {
                if ((currentPage + 1) * pageSize < allOrders.Count)
                {
                    currentPage++;
                    Application.RequestStop();
                    ShowOrdersPage(allOrders);
                }
            };
            dialog.Add(nextPageButton);

            var prevPageButton = new Button("Prev")
            {
                X = Pos.Left(nextPageButton) - 10,
                Y = Pos.Top(nextPageButton)
            };
            prevPageButton.Clicked += () =>
            {
                if (currentPage > 0)
                {
                    currentPage--;
                    Application.RequestStop();
                    ShowOrdersPage(allOrders);
                }
            };
            dialog.Add(prevPageButton);

            var completeOrderButton = new Button("Complete Order")
            {
                X = Pos.Left(prevPageButton) - 20,
                Y = Pos.Top(prevPageButton)
            };
            completeOrderButton.Clicked += () =>
            {
                var selectedIndex = listView.SelectedItem;
                if (selectedIndex >= 0 && selectedIndex < paginatedOrders.Count)
                {
                    var selectedOrder = paginatedOrders[selectedIndex];
                    CompleteOrder(selectedOrder);
                    allOrders = GetAllOrders();
                    ShowOrdersPage(allOrders);
                }
            };
            dialog.Add(completeOrderButton);

            Application.Run(dialog);
        }
    }
    static void CreateOrderForActiveClient()
    {
        if (activeClient == null)
        {
            MessageBox.Query("No Active Client", "Please select an active client first.", "Ok");
            return;
        }

        var products = GetAllProducts();
        var dialog = new Dialog($"Create Order for {activeClient.Name}", 60, 20, new Button("Cancel"));

        var productList = new ListView(products)
        {
            X = 1,
            Y = 1,
            Width = Dim.Fill(),
            Height = 6
        };
        dialog.Add(productList);

        var quantityLabel = new Label(2, 8, "Quantity:");
        var quantityField = new TextField("1")
        {
            X = 14,
            Y = Pos.Top(quantityLabel),
            Width = 5
        };
        dialog.Add(quantityLabel, quantityField);

        var orderItems = new List<OrderItem>();
        var addButton = new Button("Add to Order")
        {
            X = Pos.Left(quantityField),
            Y = Pos.Top(quantityField) + 2
        };

        var ipLabel = new Label("IP Address (for online orders):")
        {
            X = Pos.Left(quantityLabel),
            Y = Pos.Top(quantityLabel) + 2
        };
        var ipField = new TextField("")
        {
            X = Pos.Left(ipLabel),
            Y = Pos.Top(ipLabel) + 1,
            Width = 40
        };
        dialog.Add(ipLabel, ipField);

        addButton.Clicked += () =>
        {
            if (productList.SelectedItem < 0 || productList.SelectedItem >= products.Count)
                return;

            if (int.TryParse(quantityField.Text.ToString(), out int quantity))
            {
                var product = products[productList.SelectedItem];
                orderItems.Add(new OrderItem { Product = product, Quantity = quantity });
                MessageBox.Query("Item Added", $"{quantity} x {product.Name} added to the order.", "Ok");
            }
        };
        dialog.Add(addButton);

        var saveButton = new Button("Save Order")
        {
            X = Pos.Right(addButton) + 2,
            Y = Pos.Top(addButton)
        };
        saveButton.Clicked += () =>
        {
            if(orderItems.Count > 0)
            {
                if (!string.IsNullOrWhiteSpace(ipField.Text.ToString()))
                    SaveOrder(activeClient.ClientId, orderItems, ipField.Text.ToString());
                else
                    SaveOrder(activeClient.ClientId, orderItems);
            }
            else
            {
                MessageBox.Query("Trying to add empty order", "Ok");
            }

            UpdateActiveClientDisplay();
            Application.RequestStop();
        };
        dialog.Add(saveButton);

        Application.Run(dialog);
    }

    static void SaveOrder(int clientId, List<OrderItem> orderItems, string? ip = null)
    {
        using (var context = new MyDbContext())
        {
            var client = context.Clients.Find(clientId);
            Order order;

            if (ip != null)
            {
                order = new EOrder
                {
                    ClientId = clientId,
                    Completed = false,
                    IpAddress = ip };
            }
            else
            {
                order = new Order() {
                    ClientId = clientId,
                    Completed = false,
                };
            }

            foreach (var item in orderItems)
            {
                context.Attach(item.Product);
                order.Items.Add(new OrderItem { ProductId = item.Product.ProductId, Quantity = item.Quantity });
            }

            if (client.Orders == null)
            {
                client.Orders = new List<Order>();
            }

            context.Orders.Add(order);
            client.Orders.Add(order);
            context.Clients.Attach(client);
            context.SaveChanges();
            activeClient = client;
        }
    }

    static void FindClientsByOrderedItem()
    {
        var dialog = new Dialog("Find Clients by Ordered Item", 60, 20);


        var searchLabel = new Label(2, 2, "Item Name:");
        var searchField = new TextField("")
        {
            X = 20,
            Y = Pos.Top(searchLabel),
            Width = 40
        };
        dialog.Add(searchLabel, searchField);

        var listView = new ListView(new List<string>())
        {
            X = 1,
            Y = 6,
            Width = Dim.Fill() - 2,
            Height = 10
        };
        dialog.Add(listView);

        var searchButton = new Button("Search")
        {
            X = Pos.Left(searchField),
            Y = Pos.Top(searchField) + 2
        };
        searchButton.Clicked += () =>
        {
            var clients = GetClientsByOrderedItem(searchField.Text.ToString());
            listView.SetSource(clients.Select(c => $"{c.Name} - {c.Address}").ToList());
        };
        dialog.Add(searchButton);

        var closeButton = new Button("Close")
        {
            X = Pos.Percent(50) - 10,
            Y = Pos.Bottom(searchButton) + 2
        };
        closeButton.Clicked += () =>
        {
            Application.RequestStop();
        };
        Application.Run(dialog);
    }
    public static List<Client> GetClientsByOrderedItem(string itemName)
    {
        using (var context = new MyDbContext())
        {
            // Query clients who have orders containing the specified item
            var clients = context.Clients.Where(c => c.Orders.Any(o => o.Items.Any(i => i.Product.Name == itemName))).ToList();
            return clients;
        }
    }
}
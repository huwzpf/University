using DataModel;
using Newtonsoft.Json;

var client = new HttpClient();

while (true)
{
    displayMenu();

    var cmd = Console.ReadLine();

	if (string.IsNullOrWhiteSpace(cmd))
	{
		Console.WriteLine("Invalid input.");
		break;
	}

	if (cmd.Equals("q")) break;

    Console.Clear();

    await act(cmd);
}

async Task act(string? cmd)
{
    switch (cmd)
    {
        case "d":
            await ListProducts();

            break;
        case "u":
            var jsonResponse = await fetchDataFromUri("http://localhost:5115/registeredusers");
            var userNames = JsonConvert.DeserializeObject<List<string>>(jsonResponse ?? "");

            Console.WriteLine("Users:");
            DisplayListWithIndex(userNames);

            break;
        case "b":
            var products = await ListProducts();

            Console.WriteLine("\nChoose product from list - type in number");
            var input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input))
            {
				Console.WriteLine("Invalid input.");
				break;
			}

            var parsed = int.TryParse(input, out int productIndex);

            if (!parsed || productIndex < 0 || productIndex >= products.Count)
            {
                Console.WriteLine("Choose product from list.");
                break;
            }

            Console.WriteLine("Type in amount");
            parsed = int.TryParse(Console.ReadLine(), out int amount);

            if (!parsed || amount < 1 || amount > products[productIndex].AvailableAmount)
            {
                Console.WriteLine("Type in proper amount.");
                break;
            }

            var result = client
                .PostAsync($"http://localhost:5115/buy?productName={products[productIndex].Name}&amount={amount}", new StringContent(""))
                .Result;

            Console.WriteLine((result.StatusCode == System.Net.HttpStatusCode.OK) ? "Succesfully bought!" : "There was an error while buying.");

            break;
        default:
			Console.WriteLine("Invalid input.");
			break;
    }
}

async Task<List<Product>> ListProducts()
{
    var jsonResponse = await fetchDataFromUri("http://localhost:5115/storage");
    var products = JsonConvert.DeserializeObject<List<Product>>(jsonResponse ?? "");

    Console.WriteLine("products:");
    var strProducts = products?.Select(p => $"Name: {p.Name}, amount: {p.AvailableAmount}").ToList();
    DisplayListWithIndex(strProducts);

    return products ?? new();
}

void displayMenu()
{
    Console.WriteLine("__________________________________");
    Console.WriteLine("| Choose menu option:            |");
    Console.WriteLine("|--------------------------------|");
    Console.WriteLine("| d - display storage state      |");
    Console.WriteLine("| u - display registered users   |");
    Console.WriteLine("| b - buy                        |");
    Console.WriteLine("| q - quit                       |");
    Console.WriteLine("|________________________________|");
}

async Task<string?> fetchDataFromUri(string uri)
{
    var requestContent = (await client.GetAsync(uri)).Content;
    return await requestContent.ReadAsStringAsync();
}

static void DisplayListWithIndex(List<string>? items) =>
    items?.Select((item, index) => $"{index}: {item}").ToList().ForEach(Console.WriteLine);

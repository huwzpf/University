using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;
using System.Linq;
using System.Xml.Linq;
using System.Xml.XPath;

namespace ConsoleApp
{
    [XmlRoot(ElementName = "engine")]
    public class Engine
    {
        public double displacement;

        public double horsePower;

        [XmlAttribute]
        public string model;


        public Engine() { }
        public Engine(double displacement, double horsePower, string model)
        {
            this.displacement = displacement;
            this.horsePower = horsePower;
            this.model = model;
        }

    }


    [XmlType("car")]
    public class Car
    {
        public string model;
        public int year;

        [XmlElement(ElementName = "engine")]
        public Engine motor;

        public Car() { }

        public Car(string model, Engine motor, int year)
        {
            this.model = model;
            this.motor = motor;
            this.year = year;
        }
    }
    class Program
    {

       
        static void Main(string[] args)
        {

            List<Car> myCars = new List<Car>(){
                new Car("E250", new Engine(1.8, 204, "CGI"), 2009),
                new Car("E350", new Engine(3.5, 292, "CGI"), 2009),
                new Car("A6", new Engine(2.5, 187, "FSI"), 2012),
                new Car("A6", new Engine(2.8, 220, "FSI"), 2012),
                new Car("A6", new Engine(3.0, 295, "TFSI"), 2012),
                new Car("A6", new Engine(2.0, 175, "TDI"), 2011),
                new Car("A6", new Engine(3.0, 309, "TDI"), 2011),
                new Car("S6", new Engine(4.0, 414, "TFSI"), 2012),
                new Car("S8", new Engine(4.0, 513, "TFSI"), 2012)
                };

            var myCarQuery = myCars
                                                .Where(s => s.model.Equals("A6"))
                                                .Select(car =>
                                                    new
                                                    {
                                                        engineType = String.Compare(car.motor.model, "TDI") == 0
                                                                   ? "diesel"
                                                                   : "petrol",
                                                        hppl = car.motor.horsePower / car.motor.displacement,
                                                    });
            foreach (var elem in myCarQuery)
            {
                Console.WriteLine(elem.ToString());
            }
            Console.WriteLine();
            var groupedQuery = myCarQuery
                .GroupBy(elem => elem.engineType)
                .Select(elem => $"{elem.First().engineType}: {elem.Average(s => s.hppl).ToString()}");
            foreach (var elem in groupedQuery)
            {
                Console.WriteLine(elem);
            }
            Console.WriteLine();


            var filePath = "CarsCollection.xml";
            Serialize(myCars, filePath);
            var deserializedList = Deserialize(filePath);
            Console.WriteLine("Models of cars from deserialized list: ");
            foreach (var elem in deserializedList)
            {
                Console.WriteLine(elem.model);
            }

            Console.WriteLine();

            XElement rootNode = XElement.Load(filePath);
            double avgHP = (double)rootNode.XPathEvaluate("sum(//car/engine[@model!=\"TDI\"]/horsePower) div count(//car/engine[@model!=\"TDI\"]/horsePower)");
            Console.WriteLine("Average HP: " + avgHP);
            Console.WriteLine(" ");

            IEnumerable<XElement> models = rootNode.XPathSelectElements("//car/model[not(. = following::car/model)]");
            Console.WriteLine("Distinct models");
            foreach (var model in models)
            {
                Console.WriteLine(model.Value);
            }

            Console.WriteLine(" ");

            SaveCollectionASXML(myCars);
            SaveCollectionASTable(myCars);
            ModifyCarsCollections();
            Console.ReadLine();
        }


        public static List<Car> Deserialize(string filePath)
        {
            List<Car> list = new List<Car>();
            XmlSerializer serializer = new XmlSerializer(typeof(List<Car>),
                new XmlRootAttribute("cars"));
            using (Stream reader = new FileStream(filePath, FileMode.Open))
            {
                list = (List<Car>)serializer.Deserialize(reader);
            }
            return list;
        }

        public static void Serialize(List<Car> listOfCars, string filename)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(List<Car>),
                new XmlRootAttribute("cars"));
            using (TextWriter writer = new StreamWriter(filename))
            {
                serializer.Serialize(writer, listOfCars);
            }
        }



        private static void SaveCollectionASXML(List<Car> myCars)
        {
            XElement rNode = new XElement("cars", myCars.Select(n =>
                new XElement("car",
                    new XElement("model", n.model),
                    new XElement("engine",
                        new XAttribute("model", n.motor.model),
                        new XElement("displacement", n.motor.displacement),
                        new XElement("horsePower", n.motor.horsePower)),
                    new XElement("year", n.year))));
            rNode.Save("myCarsFromLinq.xml");
        }

        private static void SaveCollectionASTable(List<Car> myCars)
        {
            XElement table = new XElement("table", new XAttribute("style", "border: 2px solid black"), myCars.Select(car =>
                                                       new XElement("tr", new XAttribute("style", "border: 2px solid black"),
                                                           new XElement("td", new XAttribute("style", "border: 2px solid black"), car.model),
                                                           new XElement("td", new XAttribute("style", "border: 2px solid black"), car.motor.model),
                                                           new XElement("td", new XAttribute("style", "border: 2px solid black"), car.motor.displacement),
                                                           new XElement("td", new XAttribute("style", "border: 2px solid black"), car.motor.horsePower),
                                                           new XElement("td", new XAttribute("style", "border: 2px solid black"), car.year))));
            XElement template = XElement.Load("template.html");
            XElement body = template.Element("{http://www.w3.org/1999/xhtml}body");
            body.Add(table);
            template.Save("templateModified.html");
        }

        private static void ModifyCarsCollections()
        {
            XElement template = XElement.Load("CarsCollection.xml");
            foreach (var car in template.Elements())
            {

                var HPfield = car.Element("engine").Element("horsePower");
                HPfield.Name = "hp";



                var yearField = car.Element("year");
                XAttribute attribute = new XAttribute("year", yearField.Value);
                car.Element("model").Add(attribute);
                yearField.Remove();
            }
            template.Save("CarsCollectionModified.xml");
        }
    }
}

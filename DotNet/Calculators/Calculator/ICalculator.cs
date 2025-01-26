namespace Calculator;
using System.Windows.Controls;
public interface ICalculator
{
    string Name { get; set; }
    UserControl GetUserControl();
}
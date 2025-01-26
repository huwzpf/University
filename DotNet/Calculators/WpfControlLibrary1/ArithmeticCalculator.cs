using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Globalization;
using Calculator;
using System.Windows.Controls;

namespace Arithmetic;

[Export(typeof(ICalculator))]
public class ArithmeticCalculator : ICalculator
{
    private string name = "Arithmetic Calculator";

    public string Name
    {
        get { return name; }
        set { name = value; }
    }

    public UserControl GetUserControl()
    {
        return new ArithmeticCalc();
    }
}
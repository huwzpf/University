using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Globalization;
using Calculator;
using System.Windows.Controls;

namespace Set;

[Export(typeof(ICalculator))]
public class SetCalculator : ICalculator
{
    private string name = "Set Calculator";

    public string Name
    {
        get { return name; }
        set { name = value; }
    }

    public UserControl GetUserControl()
    {
        return new SetCalc();
    }
}
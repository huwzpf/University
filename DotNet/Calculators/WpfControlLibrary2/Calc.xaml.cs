using System.Windows;
using System.Windows.Controls;
using Calculator;

namespace MEF;

public partial class Calc : UserControl
{
    private ICalculator _calculator;
    
    public Calc(ICalculator calculator)
    {
        InitializeComponent();
        _calculator = calculator;
    }

    private void Calculate(object sender, RoutedEventArgs e)
    {
        Solution.Text = _calculator.Calculate(AText.Text, OpText.Text, BText.Text);
    }
}
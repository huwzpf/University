using System.Windows;
using System.Windows.Controls;
using Calculator;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;


namespace Arithmetic;
public partial class ArithmeticCalc : UserControl
{
    public ArithmeticCalc()
    {
        InitializeComponent();
    }

    void Add(object sender, RoutedEventArgs e)
    {
        Result.Text = "Arithmetic Add";
    }

    void Multiply(object sender, RoutedEventArgs e)
    {
        Result.Text = "Arithmetic Multiply";

    }
}
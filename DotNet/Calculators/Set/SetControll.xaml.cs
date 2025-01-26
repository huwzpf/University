using System.Windows;
using System.Windows.Controls;
using Calculator;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;



namespace Set;
public partial class SetCalc : UserControl
{
    public SetCalc()
    {
        InitializeComponent();
    }

    void Add(object sender, RoutedEventArgs e)
    {
        Result.Text = "Set Add";
    }

    void Multiply(object sender, RoutedEventArgs e)
    {
        Result.Text = "Set Multiply";

    }
}
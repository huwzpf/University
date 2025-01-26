using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using Calculator;

namespace MEF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [ImportMany(typeof(ICalculator))]
        private List<ICalculator> calculators = new List<ICalculator>();

        private PluginManager pluginManager;
        
        public List<ICalculator> Calculators
        {
            get { return calculators; }
        }

        public MainWindow()
        {
            InitializeComponent();
            
            pluginManager = new PluginManager();
            pluginManager.FilesChanged += OnFilesChanged;

            Console.WriteLine("Number of Calculators: {0}", pluginManager.Calculators.Count);
            pluginManager.SetUpPluginManager();
            Console.WriteLine("Number of loaded Calculators: {0}", pluginManager.Calculators.Count);

            foreach (var plugin in pluginManager.Calculators)
            {
                TabItem pluginItem = new TabItem();
                pluginItem.Header = plugin.Name;
                pluginItem.Content = plugin.GetUserControl();

                Tabs.Items.Add(pluginItem);
            }
            
        }

        private void OnFilesChanged(object? sender, EventArgs e)
        {
            RefeshTabs();
        }

        private void RefeshTabs()
        {
            Dispatcher.BeginInvoke(
                new ThreadStart(() =>
                {
                    Tabs.Items.Clear();
                    foreach (var plugin in pluginManager.Calculators)
                    {
                        TabItem pluginItem = new TabItem();
                        pluginItem.Header = plugin.Name;
                        pluginItem.Content = plugin.GetUserControl();

                        Tabs.Items.Add(pluginItem);
                    }
                }));
        }
    }
}
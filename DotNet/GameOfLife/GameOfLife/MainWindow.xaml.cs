using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace GameOfLife
{
    public partial class MainWindow : Window
    {
        private MainViewModel _viewModel;
        private DispatcherTimer _timer;

        public MainWindow(int boardWidth, int boardHeight, List<int> populatedNumbersList, List<int> unpopulatedNumbersList, List<int>? initialCells = null)
        {
            InitializeComponent();
            _viewModel = new MainViewModel(boardWidth, boardHeight, populatedNumbersList, unpopulatedNumbersList, initialCells);
            this.DataContext = _viewModel;

            _timer = new DispatcherTimer();
            _timer.Interval = TimeSpan.FromSeconds(0.2);
            _timer.Tick += (s, e) => { _viewModel.NextGeneration(); };
        }
        private void Start_Click(object sender, RoutedEventArgs e)
        {
            _timer.Start();
            Start.IsEnabled = false;
            Stop.IsEnabled = true;
            NextStep.IsEnabled = false;
            PreviousStep.IsEnabled = false;
            LoadState.IsEnabled = false;
            SaveState.IsEnabled = false;
        }

        private void Stop_Click(object sender, RoutedEventArgs e)
        {
            _timer.Stop();
            Start.IsEnabled = true;
            Stop.IsEnabled = false;
            NextStep.IsEnabled = true;
            PreviousStep.IsEnabled = true;
            LoadState.IsEnabled = true;
            SaveState.IsEnabled = true;
        }

        private void NextStep_Click(object sender, RoutedEventArgs e)
        {
            _viewModel.NextGeneration();
            PreviousStep.IsEnabled = true;
        }

        private void Border_MouseUp(object sender, MouseButtonEventArgs e)
        {
            var border = sender as Border;
            if (!_timer.IsEnabled && border != null && border.DataContext is Cell cell)
            {
                if(cell.IsAlive)
                {
                    _viewModel.DeadCellsCount++;
                }
                else
                {
                    _viewModel.CreatedCellsCount++;
                }

                cell.IsAlive = !cell.IsAlive;
            }
        }
        private void PreviousStep_Click(object sender, RoutedEventArgs e)
        {
            if (!_viewModel.PreviousGeneration())
            {
                PreviousStep.IsEnabled = false;
            }
        }
        private void LoadState_Click(object sender, RoutedEventArgs e)
        {
            if(_viewModel.LoadGameState())
            {
                PreviousStep.IsEnabled = false;
            }
        }
        private void SaveState_Click(object sender, RoutedEventArgs e)
        {
            _viewModel.SaveGameState();
        }
    }
}

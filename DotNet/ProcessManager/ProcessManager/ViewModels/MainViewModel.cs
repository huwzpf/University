using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ProcessManager.Models;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Windows.Input;
using ProcessManager.Commands;
using System.Windows.Threading;
using System.Windows.Data;
using System.ComponentModel;
using System.Windows;
using System.Diagnostics;

namespace ProcessManager.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        // Implement the interface
        public event PropertyChangedEventHandler PropertyChanged;
        public ICommand SortByIdCommand { get; private set; }
        public ICommand SortByNameCommand { get; private set; }
        public ObservableCollection<ProcessModel> Processes { get; set; }
        public ICollectionView ProcessView { get; private set; }
        public ICommand RefreshCommand { get; private set; }
        public ICommand ToggleAutoRefreshCommand { get; private set; }
        public ICommand KillProcessCommand { get; private set; }
        public ICommand ChangePriorityCommand { get; private set; }
        public ObservableCollection<PriorityModel> Priorities { get; private set; }
        private int _refreshInterval = 5; // Default interval in seconds
        private DispatcherTimer _refreshTimer;
        private string _filterText;

        private ProcessModel _selectedProcess;

        public ProcessModel SelectedProcess
        {
            get => _selectedProcess;
            set
            {
                _selectedProcess = value;
                OnPropertyChanged(nameof(SelectedProcess));
            }
        }

        private PriorityModel _selectedPriority;
        public PriorityModel SelectedPriority
        {
            get => _selectedPriority;
            set
            {
                if (_selectedPriority != value)
                {
                    _selectedPriority = value;
                    OnPropertyChanged(nameof(SelectedPriority));
                }
            }
        }

        // Helper method to raise the event
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }


        private void KillSelectedProcess()
        {
            try
            {
                SelectedProcess.Kill();
                LoadProcesses();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Error Killing Process", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void ChangeProcessPriority()
        {
            if (SelectedProcess == null || SelectedPriority == null)
            {
                return;
            }

            try
            {
                SelectedProcess.SetPriority(SelectedPriority.Priority);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Error Setting Priority", MessageBoxButton.OK,
                    MessageBoxImage.Error);

            }
        }
        public string FilterText
        {
            get => _filterText;
            set
            {
                _filterText = value;
                ProcessView.Refresh();
            }
        }
        public int RefreshInterval
        {
            get => _refreshInterval;
            set
            {
                _refreshInterval = value;
                // Here you might raise PropertyChanged if you're implementing INotifyPropertyChanged
                _refreshTimer.Interval = TimeSpan.FromSeconds(_refreshInterval);
            }
        }

        private bool _isAutoRefreshEnabled;
        public bool IsAutoRefreshEnabled
        {
            get => _isAutoRefreshEnabled;
            set
            {
                _isAutoRefreshEnabled = value;
                // Here you might raise PropertyChanged if you're implementing INotifyPropertyChanged
                if (_isAutoRefreshEnabled)
                {
                    StartAutoRefresh();
                }
                else
                {
                    StopAutoRefresh();
                }
            }
        }

        public MainViewModel()
        {
            Processes = new ObservableCollection<ProcessModel>();
            ProcessView = CollectionViewSource.GetDefaultView(Processes);

            ProcessView.Filter = FilterProcess;
            _refreshTimer = new DispatcherTimer();
            _refreshTimer.Tick += (s, e) => LoadProcesses();
            _refreshTimer.Interval = TimeSpan.FromSeconds(5);

            RefreshCommand = new RelayCommand(LoadProcesses);
            ToggleAutoRefreshCommand = new RelayCommand(ToggleAutoRefresh);
            SortByIdCommand = new RelayCommand(SortById);
            SortByNameCommand = new RelayCommand(SortByName);
            KillProcessCommand = new RelayCommand(KillSelectedProcess, ()=> SelectedProcess != null);
            ChangePriorityCommand = new RelayCommand(ChangeProcessPriority, () => (SelectedProcess != null) && (SelectedPriority != null));
            

            Priorities = new ObservableCollection<PriorityModel>
            {
                new PriorityModel(ProcessPriorityClass.Normal, "Normal"),
                new PriorityModel(ProcessPriorityClass.Idle, "Idle"),
                new PriorityModel(ProcessPriorityClass.High, "High"),
                new PriorityModel(ProcessPriorityClass.RealTime, "RealTime"),
                new PriorityModel(ProcessPriorityClass.BelowNormal, "Below Normal"),
                new PriorityModel(ProcessPriorityClass.AboveNormal, "Above Normal")
            };

            LoadProcesses();
        }

        private bool FilterProcess(object item)
        {
            if (string.IsNullOrEmpty(FilterText))
                return true;

            var process = item as ProcessModel;
            if (process == null)
                return false;

            bool nameContainsFilter = process.Name.IndexOf(FilterText, StringComparison.OrdinalIgnoreCase) >= 0;

            bool idContainsFilter = process.Id.ToString().Contains(FilterText);

            return nameContainsFilter || idContainsFilter;
        }

        private void SortById()
        {
            Sort("Id");
        }

        private void SortByName()
        {
            Sort("Name");
        }

        private void Sort(string sortBy)
        {
            var direction = ListSortDirection.Ascending;
            if (ProcessView.SortDescriptions.Count > 0 &&
                ProcessView.SortDescriptions[0].PropertyName == sortBy &&
                ProcessView.SortDescriptions[0].Direction == ListSortDirection.Ascending)
            {
                direction = ListSortDirection.Descending;
            }

            ProcessView.SortDescriptions.Clear();
            ProcessView.SortDescriptions.Add(new SortDescription(sortBy, direction));
        }


        private void ToggleAutoRefresh()
        {
            IsAutoRefreshEnabled = !IsAutoRefreshEnabled;
        }

        private void StartAutoRefresh()
        {
            _refreshTimer.Start();
        }

        private void StopAutoRefresh()
        {
            _refreshTimer.Stop();
        }

        private void LoadProcesses()
        {
            Processes.Clear();
            foreach (var process in Process.GetProcesses())
            {
                Processes.Add(new ProcessModel(process));
            }
        }
    }
}

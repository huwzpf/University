using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace GameOfLife
{
    public class MainViewModel : INotifyPropertyChanged
    {
        public List<Cell> Cells { get; set; }
        public int BoardWidth { get; set; } = 10;
        public int BoardHeight { get; set; } = 10;

        public List<int> PopulatedNumbersList { get; set; } = new List<int>() { 2, 3 };
        public List<int> UnpopulatedNumbersList { get; set; } = new List<int>() { 3 };
        private int _generationsCount = 0;
        public int GenerationsCount
        {
            get { return _generationsCount; }
            set
            {
                _generationsCount = value;
                OnPropertyChanged(nameof(GenerationsCount));
            }
        }
        private int _createdCellsCount = 0;
        public int CreatedCellsCount
        {
            get { return _createdCellsCount; }
            set
            {
                _createdCellsCount = value;
                OnPropertyChanged(nameof(CreatedCellsCount));
            }
        }
        private int _deadCellsCount = 0;
        public int DeadCellsCount
        {
            get { return _deadCellsCount; }
            set
            {
                _deadCellsCount = value;
                OnPropertyChanged(nameof(DeadCellsCount));
            }
        }


        public List<int> GetNeighbourIndices(int index)
        {
            List<int> ret = new List<int>();

            int x = index % BoardWidth;
            int y = index / BoardWidth;

            if (y > 0)
            {
                ret.Add(index - BoardWidth);
                if (x > 0)
                {
                    ret.Add(index - BoardWidth - 1);
                }
                if (x < BoardWidth - 1)
                {
                    ret.Add(index - BoardWidth + 1);
                }
            }

            if (y < BoardHeight - 1)
            {
                ret.Add(index + BoardWidth);
                if (x > 0)
                {
                    ret.Add(index + BoardWidth - 1);
                }
                if (x < BoardWidth - 1)
                {
                    ret.Add(index + BoardWidth + 1);
                }
            }

            if (x > 0)
            {
                ret.Add(index - 1);
            }
            if (x < BoardWidth - 1)
            {
                ret.Add(index + 1);
            }

            return ret;
        }

        public void InitializeCells(List<int>? initialCells = null)
        {
            Cells = new List<Cell>(Enumerable.Range(0, BoardWidth * BoardHeight).Select(i => new Cell()));
            foreach (var tuple in Cells.Select((cell, index) => (cell, index)))
            {
                foreach (var neighbour in GetNeighbourIndices(tuple.index))
                {
                    if (neighbour >= 0 && neighbour < Cells.Count)
                    {
                        tuple.cell.neighbors.Add(Cells[neighbour]);
                    }
                }
            }
            if(initialCells != null)
            {
                foreach (int cell in initialCells)
                {
                    if (cell < 0 || cell >= Cells.Count)
                    {
                        continue;
                    }
                    Cells[cell].IsAlive = true;
                    CreatedCellsCount++;
                }
            }
        }

        public MainViewModel(int boardWidth, int boardHeight, List<int> populatedNumbersList, List<int> unpopulatedNumbersList, List<int>? initialCells = null)
        {
            BoardWidth = boardWidth;
            BoardHeight = boardHeight;

            if (populatedNumbersList.Count > 0)
            {
                PopulatedNumbersList = populatedNumbersList;
            }

            if (unpopulatedNumbersList.Count > 0)
            {
                UnpopulatedNumbersList = unpopulatedNumbersList;
            }
            if(initialCells != null)
            { 
            }
            else
            {
                InitializeCells();
            }
        }
        
        private void CalculateNextSteps()
        {
            foreach (var cell in Cells)
            {
                int aliveNeighbours = cell.CountAliveNeighbours();
                if (cell.IsAlive)
                {
                    if (PopulatedNumbersList.Contains(aliveNeighbours))
                    {
                        cell.isAliveNextStep = true;
                    }
                    else
                    {
                        DeadCellsCount++;
                    }
                }
                else
                {
                    if (UnpopulatedNumbersList.Contains(aliveNeighbours))
                    {
                        cell.isAliveNextStep = true;
                        CreatedCellsCount++;
                    }
                }
            }
        }

        public void NextGeneration()
        {
            GenerationsCount++;
            CalculateNextSteps();

            foreach (var cell in Cells)
            {
                cell.SaveState();
                cell.IsAlive = cell.isAliveNextStep;
                cell.isAliveNextStep = false;
            }
        }

        public bool PreviousGeneration()
        {
            bool hasPreviousGeneration = true;

            foreach (var cell in Cells)
            {
                if(cell.PreviousStates.Count == 0)
                {
                    // All cells have the same number of previous states, so if one cell has no previous states, all cells have no previous states
                    return false;
                }
                if (cell.PreviousStates.Count == 1)
                {
                    // All cells have the same number of previous states, so if one cell has only one previous state, all cells have only one previous state
                    hasPreviousGeneration = false;
                }

                bool lastState = cell.PreviousStates.Last();

                if (cell.IsAlive && !lastState)
                {
                    // In last generation cell was switched from dead to alive, so we need to decrease created cells count
                    CreatedCellsCount--;
                }
                else if (!cell.IsAlive && lastState)
                {
                    // In last generation cell was switched from alive to dead, so we need to decrease dead cells count
                    DeadCellsCount--;
                }

                cell.IsAlive = lastState;
                cell.PreviousStates.RemoveLast();
            }

            GenerationsCount--;
            return hasPreviousGeneration;
        }

        public void SaveGameState()
        {
            var dialog = new SaveFileDialog
            {
                Filter = "JSON Files (*.json)|*.json",
                DefaultExt = "json",
                AddExtension = true
            };

            if (dialog.ShowDialog() == true)
            {
                GameState gameState = new GameState
                {
                    BoardHeight = BoardHeight,
                    BoardWidth = BoardWidth,
                    CellStates = Cells.Select(c => c.IsAlive).ToList()
                };

                string jsonString = JsonSerializer.Serialize(gameState);
                File.WriteAllText(dialog.FileName, jsonString);
            }
        }
        public bool LoadGameState()
        {
            var dialog = new OpenFileDialog
            {
                Filter = "JSON Files (*.json)|*.json",
                DefaultExt = "json",
                AddExtension = true
            };

            if (dialog.ShowDialog() == false)
            {
                return false;
            }

            string jsonString = File.ReadAllText(dialog.FileName);

            if(jsonString == null || jsonString.Length == 0)
            {
                return false;
            }

            GameState gameState = JsonSerializer.Deserialize<GameState>(jsonString);
            BoardWidth = gameState.BoardWidth;
            BoardHeight = gameState.BoardHeight;

            InitializeCells();  // Reinitialize the cells.

            GenerationsCount = 0;
            CreatedCellsCount = 0;
            DeadCellsCount = 0;

            for (int i = 0; i < gameState.CellStates.Count; i++)
            {
                if(gameState.CellStates[i])
                {
                    CreatedCellsCount++;
                }
                Cells[i].IsAlive = gameState.CellStates[i];
            }

            // Inform the UI to update.
            OnPropertyChanged(nameof(Cells));
            OnPropertyChanged(nameof(BoardWidth));
            OnPropertyChanged(nameof(BoardHeight));

            return true;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

    }
}

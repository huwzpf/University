using GameOfLife;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class Cell : INotifyPropertyChanged
{
    private bool _isAlive;
    public bool IsAlive
    {
        get { return _isAlive; }
        set
        {
            _isAlive = value;
            OnPropertyChanged(nameof(IsAlive));
        }
    }
    public bool isAliveNextStep = false;

    public List<Cell> neighbors = new List<Cell>();
    public LinkedList<bool> PreviousStates = new LinkedList<bool>();

    public void SaveState()
    {
        if (PreviousStates.Count > 10)
        {
            PreviousStates.RemoveFirst();
        }
        PreviousStates.AddLast(IsAlive);
    }

    public int CountAliveNeighbours()
    {
        int count = 0;
        foreach (var neighbour in neighbors)
        {
            if (neighbour.IsAlive)
            {
                count++;
            }
        }
        return count;
    }

    public event PropertyChangedEventHandler PropertyChanged;
    protected virtual void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

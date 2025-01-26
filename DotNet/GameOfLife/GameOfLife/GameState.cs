using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameOfLife
{
    public class GameState
    {
        public int BoardWidth { get; set; } = 0;
        public int BoardHeight { get; set; } = 0;
        public List<bool> CellStates { get; set; } = new List<bool>();
    }
}

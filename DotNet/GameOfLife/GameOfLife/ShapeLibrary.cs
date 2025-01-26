using System.Collections.Generic;

namespace GameOfLife
{
    internal class ShapeLibrary
    {
        private int getIndexOfCell(int x, int y, int boardWidth, int xOffset, int yOffset)
        {
            return ((y + yOffset) * boardWidth) + x + xOffset;
        }

        public List<int> GetBlinker(int boardWidth, int xOffset, int yOffset)
        {
            List<int> ret = new List<int>();
            /*
             * 0 0 0 0 0
             * 0 0 0 0 0
             * 0 1 1 1 0
             * 0 0 0 0 0
             * 0 0 0 0 0
            */

            ret.Add(getIndexOfCell(1, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(3, 2, boardWidth, xOffset, yOffset));

            return ret;
        }

        public List<int> GetGlider(int boardWidth, int xOffset, int yOffset)
        {
            List<int> ret = new List<int>();
            /*
             * 0 0 0 0 0
             * 0 0 1 0 0
             * 0 0 0 1 0
             * 0 1 1 1 0
             * 0 0 0 0 0
             */

            ret.Add(getIndexOfCell(2, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(3, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(1, 3, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 3, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(3, 3, boardWidth, xOffset, yOffset));

            return ret;
        }

        public List<int> GetLwss(int boardWidth, int xOffset, int yOffset)
        {
            List<int> ret = new List<int>();
            /*
             * 0 0 0 0 0 0 0
             * 0 0 1 1 1 1 0
             * 0 1 0 0 0 1 0
             * 0 0 0 0 0 1 0
             * 0 1 0 0 1 0 0
             * 0 0 0 0 0 0 0
             */
            ret.Add(getIndexOfCell(2, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(3, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(4, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(5, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(1, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(5, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(5, 3, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(1, 4, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(4, 4, boardWidth, xOffset, yOffset));

            return ret;
        }

        public List<int> GetRpentomino(int boardWidth, int xOffset, int yOffset)
        {
            List<int> ret = new List<int>();
            /*
            * 0 0 0 0 0
            * 0 0 1 1 0
            * 0 1 1 0 0
            * 0 0 1 0 0
            * 0 0 0 0 0
            */

            ret.Add(getIndexOfCell(2, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(3, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(1, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 3, boardWidth , xOffset, yOffset));

            return ret;
        }

        public List<int> GetDiehard(int boardWidth, int xOffset, int yOffset)
        {
            List<int> ret = new List<int>();
            /*
            * 0 0 0 0 0 0 0 0 0 0
            * 0 0 0 0 0 0 0 1 0 0
            * 0 1 1 0 0 0 0 0 0 0
            * 0 0 1 0 0 0 1 1 1 0
            * 0 0 0 0 0 0 0 0 0 0
            */

            ret.Add(getIndexOfCell(7, 1, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(1, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 2, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(2, 3, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(6, 3, boardWidth , xOffset, yOffset));
            ret.Add(getIndexOfCell(7, 3, boardWidth, xOffset, yOffset));
            ret.Add(getIndexOfCell(8, 3, boardWidth, xOffset, yOffset));

            return ret;
        }
    }
}

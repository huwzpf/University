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
using System.Windows.Shapes;

namespace GameOfLife
{
    public partial class NewGameWindow : Window
    {
        public NewGameWindow()
        {
            InitializeComponent();
        }

        private void InitializeGame(List<int>? initialCells = null)
        {
            int boardWidth, boardHeight;
            bool correctInput = true;
            if (!int.TryParse(BoardWidthTextBox.Text, out boardWidth) || boardWidth <= 0)
            {
                MessageBox.Show("Please enter a valid board size.");
                correctInput = false;
            }

            if (!int.TryParse(BoardHeightTextBox.Text, out boardHeight) || boardHeight <= 0)
            {
                MessageBox.Show("Please enter a valid board size.");
                correctInput = false;
            }

            List<int> populatedNumbersList = PopulatedNumbersTextBox.Text.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries)
            .Select(n => Convert.ToInt32(n)).Distinct().ToList();

            List<int> unpopulatedNumbersList = UnpopulatedNumbersTextBox.Text.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries)
            .Select(n => Convert.ToInt32(n)).Distinct().ToList();

            if (correctInput)
            {
                MainWindow gameWindow = new MainWindow(boardWidth, boardHeight, populatedNumbersList, unpopulatedNumbersList, initialCells);
                gameWindow.Show();
                this.Close();
            }
        }
        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            InitializeGame();
        }

        private void NumbersTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string text = textBox.Text;
            string validText = "";
            bool hasInvalidCharacter = false;

            foreach (char c in text)
            {
                // Allow only sequences of numbers in range (0,8) separated by commas
                if ((validText.Length == 0 || validText[validText.Length - 1] == ',')&& char.IsDigit(c) && c >= '0' && c <= '8')
                {
                    validText += c;
                }
                else if (validText.Length > 0 && char.IsDigit(validText[validText.Length - 1]) && c == ',')
                {
                    validText += c;
                }
                else
                {
                    hasInvalidCharacter = true;
                }
            }

            // If there was an invalid character, revert the text and set the caret to the end
            if (hasInvalidCharacter)
            {
                textBox.Text = validText;
                textBox.CaretIndex = validText.Length;
            }
        }

        private void SizeTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string text = textBox.Text;
            string validText = "";
            bool hasInvalidCharacter = false;

            foreach (char c in text)
            {
                // Allow only sequences of numbers larger than 0
                if (char.IsDigit(c))
                {
                    validText += c;
                }
                else
                {
                    hasInvalidCharacter = true;
                }
            }

            // If there was an invalid character, revert the text and set the caret to the end
            if (hasInvalidCharacter)
            {
                textBox.Text = validText;
                textBox.CaretIndex = validText.Length;
            }
        }
        private void CreateBlinker_Click(object sender, RoutedEventArgs e)
        {
            InitializeGame();
        }
    }
}

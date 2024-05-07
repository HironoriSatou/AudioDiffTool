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
using CLIWrapper;

namespace AudioDiffToolGUI
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        public string filename1 = "defalut.wav";
        public string filename2 = "defalut.wav";
        public CLIWrapperClass cLIWrapperClass = new CLIWrapperClass();

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // Configure open file dialog box
            var dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.FileName = "Document"; // Default file name
            dialog.DefaultExt = ".wav"; // Default file extension
            dialog.Filter = "Text documents (.wav)|*.wav"; // Filter files by extension

            // Show open file dialog box
            bool? result = dialog.ShowDialog();

            // Process open file dialog box results
            if (result == true)
            {
                // Open document
                Button button = (Button)sender;
                if ((string)button.Content == "Input File 1")
                {
                    filename1 = dialog.FileName;
                    TextBox1.Text = filename1;
                }
                else
                {
                    filename2 = dialog.FileName;
                    TextBox2.Text = filename2;
                }
            }
        }

        private void Button_Click_exe(object sender, RoutedEventArgs e)
        {
            // ADT main Process
            // TODO: call ADT process function (filename1, filename2)
            // Display result
            cLIWrapperClass.CompareSound(filename1, filename2);
            Result1.Text = cLIWrapperClass.num_ch.ToString();
            Result2.Text = cLIWrapperClass.max_diff_dB.ToString();
        }
    }
}

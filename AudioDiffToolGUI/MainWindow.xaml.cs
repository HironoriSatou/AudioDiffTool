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

        public string filename1 = "defalut.wav";
        public string filename2 = "defalut.wav";
        public CLIWrapperClass cLIWrapperClass = new CLIWrapperClass();
        public MainWindow()
        {
            InitializeComponent();            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // Configure open file dialog box
            var dialog = new Microsoft.Win32.OpenFileDialog
            {
                FileName = "Document", // Default file name
                DefaultExt = ".wav", // Default file extension
                Filter = "Text documents (.wav)|*.wav" // Filter files by extension
            };

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
                    Input_File1_Path.Text = filename1;
                }
                else
                {
                    filename2 = dialog.FileName;
                    Input_File2_Path.Text = filename2;
                }
            }
        }

        private void Button_Click_exe(object sender, RoutedEventArgs e)
        {
            // ADT main Process
            // TODO: call ADT process function (filename1, filename2)
            // Display result
            cLIWrapperClass.CompareSound(filename1, filename2);
            // disp file information
            Input_File1_num_ch.Text = cLIWrapperClass.GetNumCh(0).ToString();
            Input_File2_num_ch.Text = cLIWrapperClass.GetNumCh(1).ToString();
            Input_File1_num_samples.Text = cLIWrapperClass.GetNumSamples(0).ToString();
            Input_File2_num_samples.Text = cLIWrapperClass.GetNumSamples(1).ToString();
            Input_File1_fs.Text = cLIWrapperClass.GetFs(0).ToString();
            Input_File2_fs.Text = cLIWrapperClass.GetFs(1).ToString();
            Input_File1_bit.Text = cLIWrapperClass.GetBit(0).ToString();
            Input_File2_bit.Text = cLIWrapperClass.GetBit(1).ToString();
            //disp compare result
            Result_max_index.Text = cLIWrapperClass.max_diff_index.ToString();
            Result_max_ch.Text = cLIWrapperClass.max_diff_ch.ToString();
            Result_max_diff.Text = cLIWrapperClass.max_diff_dB.ToString();
        }

        private void Button_Click_Chart(object sender, RoutedEventArgs e)
        {
            ChartWindow chartWindow = new ChartWindow(cLIWrapperClass);
            var temp = cLIWrapperClass.GetAudioBuffer();
            chartWindow.Show();
        }

        private void TextBox_PreviewDragOver(object sender, DragEventArgs e)
        {
            // マウスカーソルをコピー中に変更
            e.Effects = DragDropEffects.Copy;
            // ドラッグ中のオブジェクトがファイルの場合、受け付ける
            e.Handled = e.Data.GetDataPresent(DataFormats.FileDrop);
        }

        private void TextBox_Drop(object sender, DragEventArgs e)
        {
            TextBox txt = (TextBox)sender;
            // ドロップしたファイル一覧を取得
            var files = e.Data.GetData(DataFormats.FileDrop) as string[];
            if (files == null)
            {
                return;
            }

            var sb = new StringBuilder();
            sb.Append(files[0]);
            if (txt.Name == "Input_File1_Path")
            {
                filename1 = sb.ToString();
                Input_File1_Path.Text = sb.ToString();
            }
            else 
            {
                filename2 = sb.ToString();
                Input_File2_Path.Text = sb.ToString();
            }
        }

        private void Button_version(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("version");
        }
    }
}

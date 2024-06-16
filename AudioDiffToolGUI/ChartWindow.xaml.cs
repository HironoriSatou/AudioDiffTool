using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Forms.DataVisualization.Charting;
using CLIWrapper;
using System.Collections;

namespace AudioDiffToolGUI
{
    /// <summary>
    /// ChartWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class ChartWindow : Window
    {

        public CLIWrapperClass _cLIWrapperClass;
        public ChartWindow(CLIWrapperClass cLIWrapperClass)
        {
            _cLIWrapperClass = cLIWrapperClass;
            InitializeComponent();
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var windowsFormsHost = (WindowsFormsHost)GraphArea.Children[0];
            var graph = (Chart)windowsFormsHost.Child;

            // ChartArea追加
            graph.ChartAreas.Add("Graph1");
            
            // Seriesの作成と値の追加
            int axisX_max = 1024;
            int axisX_min = 0;
            graph.ChartAreas[0].AxisX.Maximum = axisX_max; //そのグラフの最小値
            graph.ChartAreas[0].AxisX.Minimum = axisX_min; //そのグラフの最大値
            graph.ChartAreas[0].AxisY.Maximum = 1.0;
            graph.ChartAreas[0].AxisY.Minimum = -1.0;
            //Series temp = new Series();
            Series[] plotSeries = new Series[2];
            for (int i = 0; i < plotSeries.Length; i++) {
                plotSeries[i] = new Series();
                plotSeries[i].ChartType = SeriesChartType.Line;
                plotSeries[i].MarkerStyle = MarkerStyle.Circle;
                plotSeries[i].MarkerSize = 2;
            }            

            float[][][] audio_buffer = _cLIWrapperClass.GetAudioBuffer();
            float y = 0;
            for (int i = 0; i < plotSeries.Length; i++)
            {
                for (int x = axisX_min; x < axisX_max; x++)
                {
                    y = audio_buffer[i][0][x]; //plot ch 0
                    plotSeries[i].Points.AddXY(x, y);
                    plotSeries[i].BorderWidth = 1;
                }
                graph.Series.Add(plotSeries[i]);
            }
        }
    }
}

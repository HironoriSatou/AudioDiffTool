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

namespace AudioDiffToolGUI
{
    /// <summary>
    /// ChartWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class ChartWindow : Window
    {
        public ChartWindow()
        {
            InitializeComponent();
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var windowsFormsHost = (WindowsFormsHost)GraphArea.Children[0];
            var graph = (Chart)windowsFormsHost.Child;

            // ChartArea追加
            graph.ChartAreas.Add("Graph1");
            // Seriesの作成と値の追加
            Series seriesStep = new Series();
            seriesStep.ChartType = SeriesChartType.Line;
            graph.ChartAreas[0].AxisX.Maximum = 2; //そのグラフの最小値
            graph.ChartAreas[0].AxisX.Minimum = -2; //そのグラフの最大値
            graph.ChartAreas[0].AxisX.Interval = 1; //目盛りの間隔（最大値と最小値の設定が必要）

            int y = 0; //ステップ関数の初期値
            for (double x = -2; x <= 2; x = x + 0.001)
            {
                if (x > 0)
                {
                    y = 1;  //0を超えたら1を出力
                }
                seriesStep.Points.AddXY(x, y);
                seriesStep.BorderWidth = 3;
            }
            graph.Series.Add(seriesStep);
        }
    }
}

#include "QualityView.h"


QualityView::QualityView(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//初始化customplot
	customplot = new QCustomPlot();
	setCentralWidget(customplot);
	
	//设置customplot背景样式
	customplot->setBackground(QBrush(QColor(238,248,255)));

	//初始化柱形图类bar
	dis_quality_num = new QCPBars(customplot->yAxis, customplot->xAxis2);
	dis_quality_num->setName("gamma(for shaping)");
	dis_quality_num->setPen(QPen(QColor(0, 168, 140).lighter(130)));
	dis_quality_num->setBrush(QColor(0, 168, 140));

	//初始化图例
	customplot->legend->setVisible(true);
	customplot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignHCenter);
	customplot->legend->setBrush(QColor(255, 255, 255, 100));
	customplot->legend->setBorderPen(Qt::NoPen);
	QFont legendFont = font();
	legendFont.setPointSize(11);
	customplot->legend->setFont(legendFont);
	
	//初始化y轴:0.0-1.0 step=0.1
	QVector<QString> labels;
	ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
	labels << "0-0.1" << "0.1-0.2" << "0.2-0.3" << "0.3-0.4" << "0.4-0.5" << "0.5-0.6" << "0.6-0.7" << "0.7-0.8" << "0.8-0.9" << "0.9-1.0";
	QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
	textTicker->addTicks(ticks, labels);
	customplot->yAxis->setTicker(textTicker);
	customplot->yAxis->setRange(0, 11);
	customplot->yAxis->setBasePen(QPen(Qt::gray));
	customplot->yAxis->setTickLabelColor(Qt::black);
	customplot->yAxis->setLabelColor(Qt::gray);
	customplot->yAxis->setRangeReversed(true);//y轴反转
	customplot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

	//初始化x轴
	customplot->xAxis2->setPadding(5);
	customplot->xAxis2->setLabel("Number of Elements for Cases(INT)");
	customplot->xAxis2->setBasePen(QPen(Qt::gray));
	customplot->xAxis2->setTickPen(QPen(Qt::black));
	customplot->xAxis2->setSubTickPen(QPen(Qt::black));
	customplot->xAxis2->setTickLabelColor(Qt::black);
	customplot->xAxis2->setLabelColor(Qt::black);
	customplot->xAxis2->setLabelFont(legendFont);
	customplot->xAxis2->setUpperEnding(QCPLineEnding::esSpikeArrow);
	customplot->xAxis2->setVisible(true);
	customplot->xAxis->setVisible(false);
}

QualityView::~QualityView()
{
	delete customplot;
}

void QualityView::setData(QVector<qreal> quality)
{
	dis_quality_num->setData(ticks, quality);	
	//找到最大值据此设置x轴范围
	auto maxVal = std::max_element(std::begin(quality), std::end(quality));
	customplot->xAxis2->setRange(0, (*maxVal) * 1.5);
	customplot->replot();
}

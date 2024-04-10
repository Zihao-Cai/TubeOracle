#include <QtWidgets/QMainWindow>
#include "ui_TubeOracle.h"
#include "options.h"
#include "elementsColor.h"
#include "addCircle.h"
#include "addPoint.h"
#include "addRectangle.h"
#include "distribution.h"
#include "GModel.h"
#include "SolverWidget.h"
#include "key.h"
#include "about.h"
#include "QualityView.h"


class TubeOracle : public QMainWindow
{
    Q_OBJECT

public:
    TubeOracle(QWidget *parent = nullptr);
    ~TubeOracle();
    
    
private:
    Ui::TubeOracleClass ui;
    options* optionsWindow = nullptr;
    elementsColor* elementColorWindow = nullptr;//help菜单选项面板
    GModel* cur_model = nullptr;
    MygraphicsView* graView = nullptr;
    SolverWidget* solverWidget = nullptr;
    key* mykey = nullptr;
    about* myabout = nullptr;
    distribution* trans_window = nullptr;
    QualityView* qualityview = nullptr;
 

    bool flag_working;//工作台操作开关
    string project_dir;
    string project_name;
    void consoleinit();//状态栏初始化声明
    bool line_flag;
    bool sirc_flag;
    bool loop_flag;
    bool plane_flag;
    bool trans_flag;
    QList<ItemPoint*> line_box;
    QList<ItemPoint*> sircle_box;
    QList<CurveLoop*> plane_box;
    QList<QGraphicsObject*> loop_box;
    QList<QGraphicsObject*> trans_box;
    
    void open_pnt_sel();
    void close_pnt_sel();
    bool circle_legal(ItemPoint* start, ItemPoint* center, ItemPoint* end);
    QList<int> loop_legal();
    CurveLoop* find_loop(ItemLine* curve);
    CurveLoop* find_loop(ItemCurve* curve);
    QGraphicsObject* find_curve(int tag);
    void open_curves_sel();
    void close_curves_sel();
    void reset();
    void loadconfig();
    void keyPressEvent(QKeyEvent* event);

    


private slots:
    void globalconfig();//options面板值改变时对全局配置文件进行更新
    void newProject();
    void on_creted_pro(bool iscr, QString prodir, QString proname);
    void openProject();
    void showOptionsWindow();//打开options窗口
    void showElementColorWindow();//element color窗口
    void showKeyWindow();
    void showAboutWindow();
    void opendoc();

    void showaddPointWindow(QTreeWidgetItem* item, int row);//打开设置所要添加点的参数窗口
    void addLine(QTreeWidgetItem* item, int row);//添加直线
    void showaddRectangleWindow(QTreeWidgetItem* item, int row);//打开设置所要添加矩形的参数窗口
    void showaddCircleWindow(QTreeWidgetItem* item, int row);//打开设置所要添加圆的参数窗口
    void addCirclearc(QTreeWidgetItem* item, int row);//添加圆弧线
    void addCurveLoop(QTreeWidgetItem* item, int row);//添加Loop
    void addPlaneSurface(QTreeWidgetItem* item, int row);//添加曲面
    void on_resetView(QTreeWidgetItem* item, int row);//重置视图


    void Delete(QTreeWidgetItem* item, int row);//删除

    void Generate2D(QTreeWidgetItem* item, int row);//生成网格
    void showdistributionWindow(QTreeWidgetItem* item, int row);//打开网格约束窗口
    void Recombine2D(QTreeWidgetItem* item, int row);//网格重组
    void Optimize2D(QTreeWidgetItem* item, int row);//网格优化处理
    void Inspect(QTreeWidgetItem* item, int row);//网格质量检查

    void Solver(QTreeWidgetItem* item, int row);//求解

    void transfinite_get(QGraphicsObject* cvs);
    void on_settransfinite();
    void on_recv_curves(QGraphicsObject* cvs);
    bool on_addLoop();
    bool on_addSurface();
    bool on_addpnt(double pointx, double pointy);
    bool on_addline(ItemPoint* point);
    bool on_addcirclearc(ItemPoint* point);
    bool on_addrect(double x, double y, double width, double height);
    bool on_addcircle(double cx, double cy, double radius);
    void quit_sel();
    void view_filtrate(QTreeWidgetItem* item, int column);
    void label_filtrate();
    void surface_effect();
    void algorithm_config();
    void on_selNodes(Node* node);
    
};


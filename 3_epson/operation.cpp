#include "operation.h"
#include "ui_operation.h"

extern QString com1,com2,com3,com4,com5,com6,com7,com8,com9,com10,com11,com12,com13,com14;


Operation::Operation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Operation)
{
    ui->setupUi(this);
    setFixedSize(891, 641); // 设置窗口大小
    move(20,0);
    Drawing();
    InitLed();
    InitButton();

    Btncom();



}

Operation::~Operation()
{
    delete ui;
}

void Operation::Btncom()
{
    connect(ui->Btn1_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked1);
    connect(ui->Btn1_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked1);
    connect(ui->Btn2_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked2);
    connect(ui->Btn2_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked2);
    connect(ui->Btn3_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked3);
    connect(ui->Btn3_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked3);

    connect(ui->Btn4_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked4);
    connect(ui->Btn4_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked4);
    connect(ui->Btn5_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked5);
    connect(ui->Btn5_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked5);
    connect(ui->Btn6_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked6);
    connect(ui->Btn6_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked6);

    connect(ui->Btn7_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked7);
    connect(ui->Btn7_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked7);
    connect(ui->Btn8_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked8);
    connect(ui->Btn8_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked8);
    connect(ui->Btn9_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked9);
    connect(ui->Btn9_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked9);

    connect(ui->Btn10_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked10);
    connect(ui->Btn10_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked10);
    connect(ui->Btn11_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked11);
    connect(ui->Btn11_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked11);
    connect(ui->Btn12_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked12);
    connect(ui->Btn12_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked12);

    connect(ui->Btn13_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked13);
    connect(ui->Btn13_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked13);

    connect(ui->Btn14_1, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked14);
    connect(ui->Btn14_2, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked14);
    connect(ui->Btn14_3, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked14);
    connect(ui->Btn14_4, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked14);
    connect(ui->Btn14_5, &QRadioButton::clicked, this, &Operation::onRadioButtonClicked14);

}


void Operation::onRadioButtonClicked1()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId1 = btnGroup1->checkedId();



    if(checkedGenderId1 == 0) {
        com1="1";
        emit Btnclicked(1);
        setLED(ui->led1, 3, 16);
    } else if(checkedGenderId1 == 1) {
        com1="0";
        emit Btnclicked(1);
        setLED(ui->led1, 0, 16);
    }
}

void Operation::onRadioButtonClicked2()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId2 = btnGroup2->checkedId();


    if(checkedGenderId2 == 0) {
        com2="1";
        emit Btnclicked(2);
        setLED(ui->led2, 3, 16);
    } else if(checkedGenderId2 == 1) {
        com2="0";
        emit Btnclicked(2);
        setLED(ui->led2, 0, 16);
    }

}

void Operation::onRadioButtonClicked3()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId3 = btnGroup3->checkedId();

    if(checkedGenderId3 == 0) {
        com3="1";
        emit Btnclicked(3);
        setLED(ui->led3, 3, 16);
    } else if(checkedGenderId3 == 1) {
        com3="0";
        emit Btnclicked(3);
        setLED(ui->led3, 0, 16);
    }

}


void Operation::onRadioButtonClicked4()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId4 = btnGroup4->checkedId();


    if(checkedGenderId4 == 0) {
        com4="1";
        emit Btnclicked(4);
        setLED(ui->led4, 3, 16);
    } else if(checkedGenderId4 == 1) {
        com4="0";
        emit Btnclicked(4);
        setLED(ui->led4, 0, 16);
    }

}

void Operation::onRadioButtonClicked5()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId5 = btnGroup5->checkedId();


    if(checkedGenderId5 == 0) {
        com5="1";
        emit Btnclicked(5);
        setLED(ui->led5, 3, 16);
    } else if(checkedGenderId5 == 1) {
        com5="0";
        emit Btnclicked(5);
        setLED(ui->led5, 0, 16);
    }

}

void Operation::onRadioButtonClicked6()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId6 = btnGroup6->checkedId();

    if(checkedGenderId6 == 0) {
        com6="1";
        emit Btnclicked(6);
        setLED(ui->led6, 3, 16);
    } else if(checkedGenderId6 == 1) {
        com6="0";
        emit Btnclicked(6);
        setLED(ui->led6, 0, 16);
    }

}

void Operation::onRadioButtonClicked7()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId7 = btnGroup7->checkedId();


    if(checkedGenderId7 == 0) {
        com7="1";
        emit Btnclicked(7);
        setLED(ui->led1_2, 3, 16);
    } else if(checkedGenderId7 == 1) {
        com7="0";
        emit Btnclicked(7);
        setLED(ui->led1_2, 0, 16);
    }

}

void Operation::onRadioButtonClicked8()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId8 = btnGroup8->checkedId();

    if(checkedGenderId8 == 0) {
        com8="1";
        emit Btnclicked(8);
        setLED(ui->led2_2, 3, 16);
    } else if(checkedGenderId8 == 1) {
        com8="0";
        emit Btnclicked(8);
        setLED(ui->led2_2, 0, 16);
    }


}

void Operation::onRadioButtonClicked9()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId9 = btnGroup9->checkedId();

    if(checkedGenderId9 == 0) {
        com9="1";
        emit Btnclicked(9);
        setLED(ui->led3_2, 3, 16);
    } else if(checkedGenderId9 == 1) {
        com9="0";
        emit Btnclicked(9);
        setLED(ui->led3_2, 0, 16);
    }

}

void Operation::onRadioButtonClicked10()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId10 = btnGroup10->checkedId();


    if(checkedGenderId10 == 0) {
        com10="1";
        emit Btnclicked(10);
        setLED(ui->led4_2, 3, 16);
    } else if(checkedGenderId10 == 1) {
        com10="0";
        emit Btnclicked(10);
        setLED(ui->led4_2, 0, 16);
    }


}

void Operation::onRadioButtonClicked11()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId11 = btnGroup11->checkedId();

    if(checkedGenderId11 == 0) {
        com11="1";
        emit Btnclicked(11);
        setLED(ui->led5_2, 3, 16);
    } else if(checkedGenderId11 == 1) {
        com11="0";
        emit Btnclicked(11);
        setLED(ui->led5_2, 0, 16);
    }


}

void Operation::onRadioButtonClicked12()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId12 = btnGroup12->checkedId();


    if(checkedGenderId12 == 0) {
        com12="1";
        emit Btnclicked(12);
        setLED(ui->led6_2, 3, 16);
    } else if(checkedGenderId12 == 1) {
        com12="0";
        emit Btnclicked(12);
        setLED(ui->led6_2, 0, 16);
    }
}

void Operation::onRadioButtonClicked13()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId13 = btnGroup13->checkedId();


    if(checkedGenderId13 == 0) {
        com13="1";
        emit Btnclicked(13);
        setLED(ui->led7, 3, 16);
    } else if(checkedGenderId13 == 1) {
        com13="0";
        emit Btnclicked(13);
        setLED(ui->led7, 0, 16);
    }
}

void Operation::onRadioButtonClicked14()
{
    //查询案件组中的ID,以此来判断哪个按钮被按下
    int checkedGenderId14 = btnGroup14->checkedId();


    if(checkedGenderId14 == 0) {
        com14="1";
        emit Btnclicked(14);
        setLED(ui->led7_2, 1, 16);
    }
    else if(checkedGenderId14 == 1) {
        com14="2";
        emit Btnclicked(14);
        setLED(ui->led7_2, 5, 16);
    }
    else if(checkedGenderId14 == 2) {
        com14="3";
        emit Btnclicked(14);
        setLED(ui->led7_2, 4, 16);
    }
    else if(checkedGenderId14 == 3) {
        com14="4";
        emit Btnclicked(14);
        setLED(ui->led7_2, 2, 16);
    }
    else if(checkedGenderId14 == 4) {
        com14="5";
        emit Btnclicked(14);
        setLED(ui->led7_2, 6, 16);
    }
}

void Operation::InitButton()
{

    //分组1
    btnGroup1 = new QButtonGroup(this);
    btnGroup1->addButton(ui->Btn1_1,0);
    btnGroup1->addButton(ui->Btn1_2,1);

    //分组2
    btnGroup2 = new QButtonGroup(this);
    btnGroup2 ->addButton(ui->Btn2_1,0);
    btnGroup2 ->addButton(ui->Btn2_2,1);

    btnGroup3 = new QButtonGroup(this);
    btnGroup3->addButton(ui->Btn3_1,0);
    btnGroup3->addButton(ui->Btn3_2,1);

    btnGroup4 = new QButtonGroup(this);
    btnGroup4->addButton(ui->Btn4_1,0);
    btnGroup4->addButton(ui->Btn4_2,1);

    btnGroup5 = new QButtonGroup(this);
    btnGroup5->addButton(ui->Btn5_1,0);
    btnGroup5->addButton(ui->Btn5_2,1);

    btnGroup6 = new QButtonGroup(this);
    btnGroup6->addButton(ui->Btn6_1,0);
    btnGroup6->addButton(ui->Btn6_2,1);

    btnGroup7 = new QButtonGroup(this);
    btnGroup7->addButton(ui->Btn7_1,0);
    btnGroup7->addButton(ui->Btn7_2,1);

    btnGroup8 = new QButtonGroup(this);
    btnGroup8->addButton(ui->Btn8_1,0);
    btnGroup8->addButton(ui->Btn8_2,1);

    btnGroup9 = new QButtonGroup(this);
    btnGroup9->addButton(ui->Btn9_1,0);
    btnGroup9->addButton(ui->Btn9_2,1);

    btnGroup10 = new QButtonGroup(this);
    btnGroup10->addButton(ui->Btn10_1,0);
    btnGroup10->addButton(ui->Btn10_2,1);

    btnGroup11 = new QButtonGroup(this);
    btnGroup11->addButton(ui->Btn11_1,0);
    btnGroup11->addButton(ui->Btn11_2,1);

    btnGroup12 = new QButtonGroup(this);
    btnGroup12->addButton(ui->Btn12_1,0);
    btnGroup12->addButton(ui->Btn12_2,1);

    btnGroup13 = new QButtonGroup(this);
    btnGroup13->addButton(ui->Btn13_1,0);
    btnGroup13->addButton(ui->Btn13_2,1);

    btnGroup14 = new QButtonGroup(this);
    btnGroup14->addButton(ui->Btn14_1,0);
    btnGroup14->addButton(ui->Btn14_2,1);
    btnGroup14->addButton(ui->Btn14_3,2);
    btnGroup14->addButton(ui->Btn14_4,3);
    btnGroup14->addButton(ui->Btn14_5,4);



    ui->Btn1_2->setChecked(1);                       //默认选中
    ui->Btn2_2->setChecked(1);
    ui->Btn3_2->setChecked(1);                       //默认选中
    ui->Btn4_2->setChecked(1);
    ui->Btn5_2->setChecked(1);                       //默认选中
    ui->Btn6_2->setChecked(1);
    ui->Btn7_2->setChecked(1);                       //默认选中
    ui->Btn8_2->setChecked(1);
    ui->Btn9_2->setChecked(1);                       //默认选中
    ui->Btn10_2->setChecked(1);
    ui->Btn11_2->setChecked(1);                       //默认选中
    ui->Btn12_2->setChecked(1);
    ui->Btn13_2->setChecked(1);
    ui->Btn14_1->setChecked(1);


}

void Operation::InitLed()
{
    setLED(ui->led1, 0, 16);
    setLED(ui->led2, 0, 16);
    setLED(ui->led3, 0, 16);
    setLED(ui->led4, 0, 16);
    setLED(ui->led5, 0, 16);
    setLED(ui->led6, 0, 16);
    setLED(ui->led7, 0, 16);

    setLED(ui->led1_2, 0, 16);
    setLED(ui->led2_2, 0, 16);
    setLED(ui->led3_2, 0, 16);
    setLED(ui->led4_2, 0, 16);
    setLED(ui->led5_2, 0, 16);
    setLED(ui->led6_2, 0, 16);
    setLED(ui->led7_2, 0, 16);
}

void Operation::setLED(QLabel* label, int color, int size)
{
    // 将label中的文字清空
    label->setText("");
    // 先设置矩形大小
    // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
    // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    // 再设置边界形状及边框
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    // 最后设置背景颜色
    QString background = "background-color:";
    switch (color) {
    case 0:
        // 灰色
        background += "rgb(190,190,190)";
        break;
    case 1:
        // 红色
        background += "rgb(255,0,0)";
        break;
    case 2:
        // 绿色
        background += "rgb(0,255,0)";
        break;
    case 3:
        // 蓝色
        background += "rgb(0,0,255)";
        break;
    case 4:
        // 黄色
        background += "rgb(255,255,0)";
        break;
    case 5:
        // 橙色
        background += "rgb(255,97,0)";
        break;
    case 6:
        //紫色
        background += "rgb(153,51,250)";
        break;
    default:
        break;
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

void Operation::Drawing()
{
    ui->centralwidget->setStyleSheet("background-color: rgb(255,114,86);");
    QString style="font: 50 13pt '微软雅黑 Light';"
                    "font-weight: bold;"
                    "border:none;"
                    "color:rgb(255,255,255);"
                    "background-color: transparent;";
    ui->label->setStyleSheet(style);
    ui->label_2->setStyleSheet(style);
    ui->label_3->setStyleSheet(style);
    ui->label_4->setStyleSheet(style);
    ui->label_5->setStyleSheet(style);
    ui->label_6->setStyleSheet(style);

    ui->label_7->setStyleSheet(style);
    ui->label_8->setStyleSheet(style);
    ui->label_9->setStyleSheet(style);
    ui->label_10->setStyleSheet(style);
    ui->label_11->setStyleSheet(style);
    ui->label_12->setStyleSheet(style);
    ui->label_13->setStyleSheet(style);
    ui->label_14->setStyleSheet(style);

    ui->Btn1_1->setStyleSheet(style);
    ui->Btn1_2->setStyleSheet(style);

    ui->Btn2_1->setStyleSheet(style);
    ui->Btn2_2->setStyleSheet(style);

    ui->Btn3_1->setStyleSheet(style);
    ui->Btn3_2->setStyleSheet(style);

    ui->Btn4_1->setStyleSheet(style);
    ui->Btn4_2->setStyleSheet(style);

    ui->Btn5_1->setStyleSheet(style);
    ui->Btn5_2->setStyleSheet(style);

    ui->Btn6_1->setStyleSheet(style);
    ui->Btn6_2->setStyleSheet(style);

    ui->Btn7_1->setStyleSheet(style);
    ui->Btn7_2->setStyleSheet(style);

    ui->Btn8_1->setStyleSheet(style);
    ui->Btn8_2->setStyleSheet(style);

    ui->Btn9_1->setStyleSheet(style);
    ui->Btn9_2->setStyleSheet(style);

    ui->Btn10_1->setStyleSheet(style);
    ui->Btn10_2->setStyleSheet(style);

    ui->Btn11_1->setStyleSheet(style);
    ui->Btn11_2->setStyleSheet(style);

    ui->Btn12_1->setStyleSheet(style);
    ui->Btn12_2->setStyleSheet(style);

    ui->Btn13_1->setStyleSheet(style);
    ui->Btn13_2->setStyleSheet(style);

    ui->Btn14_1->setStyleSheet(style);
    ui->Btn14_2->setStyleSheet(style);
    ui->Btn14_3->setStyleSheet(style);
    ui->Btn14_4->setStyleSheet(style);
    ui->Btn14_5->setStyleSheet(style);

    ui->topiclabel->setStyleSheet("font: 50 25pt '微软雅黑 Light';"
                                  "font-weight: bold;"
                                  "border:none;"
                                  "color:rgb(255,255,255);"
                                  "background-color: transparent;");


}




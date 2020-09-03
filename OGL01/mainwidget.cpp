#include "mainwidget.h"
#include "./ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
  ui->setupUi(this);
  resize(1024, 768);
  setWindowTitle("Qt + OpenGL 学习 01");
}

MainWidget::~MainWidget() { delete ui; }

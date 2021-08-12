/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     linxun <linxun@uniontech.com>
*
* Maintainer: linxun <linxun@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwidnow.h"

#include <DTitlebar>

#include <QImageReader>

MainWidnow::MainWidnow(QWidget *parent):DMainWindow (parent)
{
    initUI();
    connect(m_mainPage, &MainPage::buttonPress, this, &MainWidnow::showClickedPage);
}

void MainWidnow::initUI()
{
   m_titlebar=titlebar();
   m_titlebar->installEventFilter(this);
   m_titlebar->setBackgroundTransparent(true);

   m_stackedWidget = new DStackedWidget(this);
   m_mainPage = new MainPage(m_stackedWidget);
   m_gameWindow = new GameWindow(16,10,m_stackedWidget);

   m_stackedWidget->addWidget(m_mainPage);
   m_stackedWidget->addWidget(m_gameWindow);
   setCentralWidget(m_stackedWidget);
}

bool MainWidnow::eventFilter(QObject *obj, QEvent *event)
{
    //对titlebar透明度进行处理
    if(obj==m_titlebar){
    if(event->type()==QEvent::Paint){
        DGuiApplicationHelper::ColorType themtype = DGuiApplicationHelper::instance()->themeType();
        QColor broundColor;
         if (themtype == DGuiApplicationHelper::ColorType::DarkType) {
             broundColor = m_titlebar->palette().color(QPalette::Normal, QPalette::Dark);
         } else if (themtype == DGuiApplicationHelper::ColorType::LightType) {
             broundColor = m_titlebar->palette().color(QPalette::Normal, QPalette::Light);
         }
        QPainter painter(m_titlebar);
        painter.setRenderHint(QPainter::Antialiasing, true);
        broundColor.setAlphaF(0.8);
        painter.setBrush(broundColor);
        painter.setPen(Qt::NoPen);
        painter.fillRect(titlebar()->rect(), broundColor);
        painter.drawRect(titlebar()->rect());
        return  true;
     }
    }
    return  DMainWindow::eventFilter(obj,event);
}

void MainWidnow::paintEvent(QPaintEvent *event)
{
    //绘制背景图片
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QImageReader imageReader(":/assets/images/background.png");
    QSize backgroundSize;
    backgroundSize.setWidth(rect().width());
    backgroundSize.setHeight(rect().height()-m_titlebar->rect().height());
    imageReader.setScaledSize(backgroundSize);
    p.drawPixmap(rect().x(),rect().y()+m_titlebar->rect().height(),QPixmap::fromImageReader(&imageReader));
    DWidget::paintEvent(event);
}

void MainWidnow::showClickedPage()
{
    m_stackedWidget->setCurrentWidget(m_gameWindow);
}

#pragma once

#include <QWidget>
#include "ui_ViewCourses.h"
#include"student.h"
class ViewCourses : public QWidget
{
	Q_OBJECT

public:
	ViewCourses(QWidget *parent = Q_NULLPTR);
	ViewCourses(Student* S,QWidget* parent = Q_NULLPTR);
	~ViewCourses();
	Student* stud;

private:
	Ui::ViewCourses ui;
private slots:
	void RadioButton_Clicked();
	void Widget_Item_Clicked(QTreeWidgetItem* item,int index);
};

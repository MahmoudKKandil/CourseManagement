#include <QMessageBox>
#include "adminPanel.h"
#include"CSVFile.h"
#include"CourseManagement.h"
adminPanel::adminPanel(User* admin,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->admin = admin;
	LoadCrs(Database::Courses,ui.treeCrss);
	hidelabels();
}
void adminPanel::LoadCrs(vector<Course*> c, QTreeWidget*Tree,bool AddChilds)
{
	Tree->clear();
	auto itms = Course::CreateTree(c,AddChilds);
	for (int i = 0; i < itms.size(); i++)
		Tree->addTopLevelItem(itms[i]);
}
void adminPanel::itemClicked(QTreeWidgetItem* item, int index)
{
	c = Database::GetCourse(item->data(0, Qt::UserRole).toString().toStdString());
	if (c == nullptr)
		return;
	switch (ui.tabWidget->currentIndex())
	{
	case 3:

		return;
	case 0:
	{
		ui.crsname->setText(QString::fromStdString(c->Name));
		ui.crscode->setText(QString::fromStdString(c->Code));
		ui.crshrs->setText(QString::number(c->Hours));
		ui.crsnumstd->setText(QString::number(c->MaxNumOfStudents));
		ui.crsinprog->setPlainText(getStuds(c));
		break;
	}
	default:
		break;
	}
}
void adminPanel::on_updatecrsinfo_clicked()
{
	reply = QMessageBox::question(this, "Update Course", "Update course info?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		if (ui.crsname->text().isEmpty())
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle(" ");
			msgBox.setText("There's no course selected, Select a course and try again");
			msgBox.exec();
			return;
		}
		c->Name = ui.crsname->text().toStdString();
		c->Code = ui.crscode->text().toStdString();
		c->Hours = ui.crshrs->text().toInt();
		c->MaxNumOfStudents = ui.crsnumstd->text().toInt();
		Database::Save();
		LoadCrs(Database::Courses, ui.treeCrss);
	}
}
adminPanel::~adminPanel()
{
}
void adminPanel::on_getuserinfo_clicked()
{
	S = Database::GetStudentByUsername(ui.susername->text().toStdString());
	if (S == nullptr)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("No User Found");
		msgBox.setText("Username is not correct");
		msgBox.exec();
		hidelabels();
		Clear();
		return;
	}
	ui.sname->setText(QString::fromStdString(S->Name));
	ui.spassword->setText(QString::fromStdString(S->Password));
	ui.syear->setText(QString::number(S->Academicyear));
	if (S->Role == 0)
	{
		showlabels();
		ui.treeWidget->clear();
		{
			LoadCrs(S->GetFinishedCourses(),ui.treeWidget);
		}
		ui.treeWidget_2->clear();
		{
			LoadCrs(S->GetCoursesInProgress(), ui.treeWidget_2,false);
		}
	}
}
void adminPanel::showlabels()
{
	ui.lfinished->show();
	ui.linprogress->show();
	ui.courseisfinished->show();
	ui.treeWidget->show();
	ui.treeWidget_2->show();
}
void adminPanel::hidelabels()
{
	ui.lfinished->hide();
	ui.linprogress->hide();
	ui.courseisfinished->hide();
	ui.treeWidget->hide();
	ui.treeWidget_2->hide();
}
void adminPanel::on_updateuinfo_clicked()
{
	reply = QMessageBox::question(this, "Update Account", "Update account info?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		if (ui.sname->text().isEmpty())
		{
			return;
		}
		if (Database::GetUserByUsername(ui.sname->text().toStdString()) != NULL)
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("");
			msgBox.setText("Username Is Already Found, choose another username");
			msgBox.exec();
			return;
		}
		S->Username = ui.susername->text().toStdString();
		S->Name = ui.sname->text().toStdString();
		S->Password = ui.spassword->text().toStdString();
		S->Academicyear = ui.syear->text().toInt();
		Clear();
		hidelabels();
		Database::Save();
		Q.setWindowTitle(" ");
		Q.setText("Student Data Updated");
		Q.exec();
	}
}
void adminPanel::Clear()
{
	ui.syear->clear();
	ui.sname->clear();
	ui.susername->clear();
	ui.spassword->clear();
}
QString adminPanel::getStuds(Course* c)
{
	vector<Student*>Studs = c->GetStudents();
	QString result;
	string temp;
	for (auto stud : Studs)
	{
		temp += stud->Name;
		temp += "\n";
	}
	result = QString::fromStdString(temp);
	return result;
}
void adminPanel::on_addsbtn_clicked()
{
	reply = QMessageBox::question(this, "Add Account", "Add new account?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		QMessageBox msgBox;
		if (ui.newsname->text().isEmpty() || ui.newspw->text().isEmpty() || ui.newsrolecb->currentIndex() < 0 ||
			ui.newsun->text().isEmpty() || ui.newsyear->text().isEmpty())
		{
			msgBox.setWindowTitle("No Data Entered");
			msgBox.setText("Please Enter User Data");
			msgBox.exec();
			return;
		}
		if (Database::GetUserByUsername(ui.newsname->text().toStdString())!=NULL)
		{
			msgBox.setWindowTitle("");
			msgBox.setText("Username Is Already Found, choose another username");
			msgBox.exec();
			return;
		}
		if (ui.newsrolecb->currentIndex() == 1)
		{
			auto u = new User(ui.newsname->text().toStdString(), ui.newsun->text().toStdString(), ui.newspw->text().toStdString(),
				ui.newsrolecb->currentIndex()); //admin
			msgBox.setWindowTitle("User Added");
			msgBox.setText("Admin Added Successfuly");
			msgBox.exec();
		}
		else if (ui.newsrolecb->currentIndex() == 0)
		{
			auto u = new Student(ui.newsname->text().toStdString(), ui.newsun->text().toStdString(),
				ui.newspw->text().toStdString(), ui.newsyear->text().toInt(), vector<string>(),
				vector<string>()); //student
			msgBox.setWindowTitle("User Added");
			msgBox.setText("Student Added Successfuly");
			msgBox.exec();
		}
	}
}
void adminPanel::on_addcbtn_clicked()
{
	reply = QMessageBox::question(this, "Add Course", "Add new course?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		QMessageBox msgBox;
		if (ui.newcname->text().isEmpty() || ui.newchrs->text().isEmpty() || ui.newcid->text().isEmpty() ||
			ui.newcstd->text().isEmpty())
		{
			msgBox.setWindowTitle("No Data Entered");
			msgBox.setText("Please Enter Course Data");
			msgBox.exec();
			return;
		}
		auto PreReq = CSVFile::ParseLine(ui.newcprc->text().toStdString());
		auto c = new Course(ui.newcname->text().toStdString(), ui.newcid->text().toStdString(), ui.newcstd->text().toInt(),
			ui.newchrs->text().toInt(), PreReq);
		msgBox.setWindowTitle("Course Added");
		msgBox.setText("Course Added Successfuly");
		msgBox.exec();
	}
}
void adminPanel::on_courseisfinished_clicked()
{
	reply = QMessageBox::question(this, "Complete Course", "Move course to student finished courses?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		if ( S == NULL || c == NULL)
		{
			
			QMessageBox msgBox;
			msgBox.setWindowTitle("No Selected Course");
			msgBox.setText("There's no selected course, Select a course and try again");
			msgBox.exec();
			return;
		}
		S->MoveCourseToFinished(c->Code);
		on_getuserinfo_clicked();
		Database::Save();
	}
}
void adminPanel::on_logout_clicked()
{
	reply = QMessageBox::question(this, "Logout", "Logout?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		admin = NULL;
		adminPanel::close();
		auto bk = new CourseManagement();
		bk->show();
	}
}
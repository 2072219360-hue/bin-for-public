#include <QtGui>
#include <QtSql>

#include "huang32DBsqliteMasterSlaveemployeeform.h"
#include "huang32DBsqliteMasterSlavemainform.h"

bool mscreateConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db/personnel.dat");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}

void mscreateFakeData()
{
    QStringList names;
    names << "Eabha Biddell" << "Prentice Hutchison"
          << "Rameesha Davidge" << "Digby Roson" << "Nettah Newarch"
          << "Lewisha Middleton" << "Ahmed Hanmer"
          << "Jordyn-Leigh Lamant" << "Lindsay Bigham"
          << "Kaylay Weir" << "Sofia Weir" << "Coel Spurlock"
          << "Youcef Culpan" << "Lucy-Jasmine Blanchard"
          << "Ally Hodgkin" << "Ara Collinge" << "Luka Dempster"
          << "Samanta Winster" << "Keri Palin" << "Ruiridh Bisset"
          << "Norman Epworth" << "Kezia Raw"
          << "Kaylan-Thomas Swynford" << "Kashaf Benning"
          << "Norma Yair" << "Edan Bassett" << "Akshat Mcglasson"
          << "Philippa Upton" << "Tylor Rockliff" << "Aqdas Buckman"
          << "Briana Dowell" << "Querida North" << "Chelsay Botts"
          << "Kishanth Calloway" << "Jan Covington"
          << "Teighan Monson" << "Claudia Mendel" << "Kerra Doe"
          << "Kara Depp" << "Harlie Soole" << "Viggo Streeter"
          << "Ava Cofel" << "Catherine Balderston"
          << "Brendan Gosnay" << "Zhaoyun Haygarth" << "Deri Pepler"
          << "Vicki Hopwood" << "Amitra Bindless" << "Cerhys Hayton"
          << "Gwendoline Westall";

    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(QObject::tr("Staff Manager"));
    progress.setLabelText(QObject::tr("Creating database..."));
    progress.setMinimum(0);
    progress.setMaximum(names.count() + 6);
    progress.setValue(1);
    qApp->processEvents();
    QSqlQuery query;
    query.exec("DROP TABLE department");
    query.exec("DROP TABLE employee");
    query.exec("DROP TABLE location");

    progress.setValue(2);
    qApp->processEvents();
    query.exec("CREATE TABLE location ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL)");
    progress.setValue(3);
    qApp->processEvents();
    query.exec("CREATE TABLE department ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL, "
               "locationid INTEGER NOT NULL, "
               "FOREIGN KEY (locationid) REFERENCES location)");
    progress.setValue(4);
    qApp->processEvents();
    query.exec("CREATE TABLE employee ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL, "
               "departmentid INTEGER NOT NULL, "
               "extension INTEGER NOT NULL, "
               "email VARCHAR(40) NOT NULL, "
               "startdate DATE NOT NULL, "
               "FOREIGN KEY (departmentid) REFERENCES department)");

    progress.setValue(5);
    qApp->processEvents();
    query.exec("INSERT INTO location (name) VALUES ("
               "'Floor 18, 1129 Evanston Heights, New York, NY')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'The Shed, Elmtree Drive, Boston, MA')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'14 Valentine Buildings, Amor Street, Cambridge, MA')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'Bunker Building, Silo Avenue, Los Angeles, CA')");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Sales', 1)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Marketing', 2)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Processing', 1)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Support', 4)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Research', 3)");

    progress.setValue(6);
    qApp->processEvents();
    int count = 0;
    query.prepare("INSERT INTO employee (name, departmentid, "
                  "extension, email, startdate) "
                  "VALUES (:name, :departmentid, :extension, "
                  ":email, :startdate)");
    foreach (QString name, names) {
        query.bindValue(":name", name);
        query.bindValue(":departmentid", 1 + (std::rand() % 5));
        query.bindValue(":extension", 400 + (std::rand() % 100));
        query.bindValue(":email", name.toLower().replace(" ", ".") +
                                  "@company.com");
        query.bindValue(":startdate",
                QDate::currentDate().addDays(-(std::rand() % 3600)));
        query.exec();
        ++count;
        progress.setValue(count + 6);
    }
    progress.setValue(progress.maximum());
    qApp->processEvents();
}

void initMasterSlaveDBSqlite()
{
    bool existingData = QFile::exists("db/personnel.dat");
    if (!mscreateConnection())
        return;
    if (!existingData)
        mscreateFakeData();

}

MasterSlaveDBSqliteMainForm::MasterSlaveDBSqliteMainForm(QWidget *parent)
{
    createDepartmentPanel();
    createEmployeePanel();

    splitter = new QSplitter(Qt::Vertical);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(departmentPanel);
    splitter->addWidget(employeePanel);

    addButton = new QPushButton(tr("&Add Dept."));
    deleteButton = new QPushButton(tr("&Delete Dept."));
    editButton = new QPushButton(tr("&Edit Employees..."));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::AcceptRole);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addDepartment()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteDepartment()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEmployees()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Staff Manager"));
    departmentView->setCurrentIndex(departmentModel->index(0, 0));
}

void MasterSlaveDBSqliteMainForm::updateEmployeeView()
{
    QModelIndex index = departmentView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = departmentModel->record(index.row());
        int id = record.value("id").toInt();
        employeeModel->setFilter(QString("departmentid = %1").arg(id));
        employeeLabel->setText(tr("E&mployees in the %1 Department")
                               .arg(record.value("name").toString()));
    } else {
        employeeModel->setFilter("departmentid = -1");
        employeeLabel->setText(tr("E&mployees"));
    }
    employeeModel->select();
    employeeView->horizontalHeader()->setVisible(
            employeeModel->rowCount() > 0);
}

void MasterSlaveDBSqliteMainForm::addDepartment()
{
    int row = departmentModel->rowCount();
    departmentModel->insertRow(row);
    QModelIndex index = departmentModel->index(row, Department_Name);
    departmentView->setCurrentIndex(index);
    departmentView->edit(index);
}

void MasterSlaveDBSqliteMainForm::deleteDepartment()
{
    QModelIndex index = departmentView->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = departmentModel->record(index.row());
    int id = record.value(Department_Id).toInt();
    int numEmployees = 0;

    QSqlQuery query(QString("SELECT COUNT(*) FROM employee "
                            "WHERE departmentid = %1").arg(id));
    if (query.next())
        numEmployees = query.value(0).toInt();
    if (numEmployees > 0) {
        int r = QMessageBox::warning(this, tr("Delete Department"),
                    tr("Delete %1 and all its employees?")
                    .arg(record.value(Department_Name).toString()),
                    QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }

        query.exec(QString("DELETE FROM employee "
                           "WHERE departmentid = %1").arg(id));
    }

    departmentModel->removeRow(index.row());
    departmentModel->submitAll();
    QSqlDatabase::database().commit();

    updateEmployeeView();
    departmentView->setFocus();
}

void MasterSlaveDBSqliteMainForm::editEmployees()
{
    int employeeId = -1;
    QModelIndex index = employeeView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = employeeModel->record(index.row());
        employeeId = record.value(Employee_Id).toInt();
    }

    EmployeeForm form(employeeId, this);
    form.exec();
    updateEmployeeView();
}

void MasterSlaveDBSqliteMainForm::createDepartmentPanel()
{
    departmentPanel = new QWidget;

    departmentModel = new QSqlRelationalTableModel(this);
    departmentModel->setTable("department");
    departmentModel->setRelation(Department_LocationId,
            QSqlRelation("location", "id", "name"));
    departmentModel->setSort(Department_Name, Qt::AscendingOrder);
    departmentModel->setHeaderData(Department_Name, Qt::Horizontal,
                                   tr("Dept."));
    departmentModel->setHeaderData(Department_LocationId,
                                   Qt::Horizontal, tr("Location"));
    departmentModel->select();

    departmentView = new QTableView;
    departmentView->setModel(departmentModel);
    departmentView->setItemDelegate(new QSqlRelationalDelegate(this));
    departmentView->setSelectionMode(
            QAbstractItemView::SingleSelection);
    departmentView->setSelectionBehavior(QAbstractItemView::SelectRows);
    departmentView->setColumnHidden(Department_Id, true);
    departmentView->resizeColumnsToContents();
    departmentView->horizontalHeader()->setStretchLastSection(true);

    departmentLabel = new QLabel(tr("Depar&tments"));
    departmentLabel->setBuddy(departmentView);

    connect(departmentView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateEmployeeView()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(departmentLabel);
    layout->addWidget(departmentView);
    departmentPanel->setLayout(layout);
}

void MasterSlaveDBSqliteMainForm::createEmployeePanel()
{
    employeePanel = new QWidget;

    employeeModel = new QSqlRelationalTableModel(this);
    employeeModel->setTable("employee");
    employeeModel->setRelation(Employee_DepartmentId,
            QSqlRelation("department", "id", "name"));
    employeeModel->setSort(Employee_Name, Qt::AscendingOrder);
    employeeModel->setHeaderData(Employee_Name, Qt::Horizontal,
                                 tr("Name"));
    employeeModel->setHeaderData(Employee_Extension, Qt::Horizontal,
                                 tr("Ext."));
    employeeModel->setHeaderData(Employee_Email, Qt::Horizontal,
                                 tr("Email"));

    employeeView = new QTableView;
    employeeView->setModel(employeeModel);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->horizontalHeader()->setStretchLastSection(true);
    employeeView->setColumnHidden(Employee_Id, true);
    employeeView->setColumnHidden(Employee_DepartmentId, true);
    employeeView->setColumnHidden(Employee_StartDate, true);

    employeeLabel = new QLabel(tr("E&mployees"));
    employeeLabel->setBuddy(employeeView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(employeeLabel);
    layout->addWidget(employeeView);
    employeePanel->setLayout(layout);
}

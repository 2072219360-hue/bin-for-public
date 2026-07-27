#include "huangzbWinMPIRunDialog.h"

THuangzbWinMPIRunDialog::THuangzbWinMPIRunDialog(QWidget *parent/* =0 */):QDialog(parent)
{
	setWindowTitle(tr("本地MPI并行执行设置"));

	createBaseInfo();
	createDetailInfo();

	QVBoxLayout *layout=new QVBoxLayout;
	layout->addWidget(baseWidget);
	layout->addWidget(detailWidget);
	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->setSpacing(10);
	setLayout(layout);

	
}

void THuangzbWinMPIRunDialog::createBaseInfo()
{
	
	baseWidget = new QWidget;

	QLabel *nameLabel = new QLabel(tr("MPI进程数:"));
	MPINPEdit = new QLineEdit;
	QLabel *gridLabel = new QLabel(tr("网格文件:"));
	QLabel *workdirLabel = new QLabel(tr("工作目录:"));
	QLabel *exeLabel = new QLabel(tr("解算器:"));

	QPushButton *okButton = new QPushButton(tr("确定"));
	connect(okButton,SIGNAL(clicked()),this,SLOT(accept()));

	QPushButton *detailButton = new QPushButton(tr("更多配置"));
	connect(detailButton,SIGNAL(clicked()),this,SLOT(slotExtension()));

	QDialogButtonBox *btnBox = new QDialogButtonBox(Qt::Vertical);
	btnBox->addButton(okButton,QDialogButtonBox::ActionRole);
	//btnBox->addButton(detailButton,QDialogButtonBox::ActionRole);

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(nameLabel,0,0);
	grid->addWidget(MPINPEdit,0,1);
	grid->addWidget(exeLabel,1,0,1,2);
	grid->addWidget(gridLabel,2,0,1,2);
	grid->addWidget(workdirLabel,3,0,1,2);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addLayout(grid);
	hbox->addStretch();
	hbox->addWidget(btnBox);
	baseWidget->setLayout(hbox);

}

void THuangzbWinMPIRunDialog::createDetailInfo()
{
	detailWidget = new QWidget;

	QLabel *label1 = new QLabel(tr("Age:"));
	QLineEdit *ageEdit = new QLineEdit;
	ageEdit->setText("30");
	QLabel *label2 = new QLabel(tr("Department:"));
	QComboBox *deptComboBox = new QComboBox;
	deptComboBox->addItem(tr("dept 1"));
	deptComboBox->addItem(tr("dept 2"));
	deptComboBox->addItem(tr("dept 3"));
	deptComboBox->addItem(tr("dept 4"));
	QLabel *label3 = new QLabel(tr("email:"));
	QLineEdit *edit = new QLineEdit;

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(label1,0,0);
	grid->addWidget(ageEdit,0,1);
	grid->addWidget(label2,1,0);
	grid->addWidget(deptComboBox,1,1);
	grid->addWidget(label3,2,0);
	grid->addWidget(edit,2,1);

	detailWidget->setLayout(grid);
	detailWidget->hide();
}

void THuangzbWinMPIRunDialog::slotExtension()
{
	if (detailWidget->isHidden())
		detailWidget->show();
	else
		detailWidget->hide();
}


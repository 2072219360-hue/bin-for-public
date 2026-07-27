#include "GraphWindow.h"

namespace platforms {
	namespace echv {
		GraphWindow::GraphWindow(QString arg_graphPath, QWidget *parent)
			: QWidget(parent)
		{
			setMinimumSize(300, 300);
			setMaximumSize(2000, 2000);
			//setFixedSize(500, 500);
			//resize(1000, 1000);

			/*QPalette palette = this->palette();
			palette.setColor(QPalette::Window, Qt::lightGray);
			this->setPalette(palette);
			this->setAutoFillBackground(true);*/

			//setStyleSheet("QWidget { background-color : blue;}");

			//QGridLayout *layout = new QGridLayout(this);
			graphLabel = new QLabel(this);
			this->graphPath = arg_graphPath;

			showGraph();

		}
		void GraphWindow::showGraph() {
			QFileInfo file(graphPath);
			if (!file.isFile()) {
				QPalette palette = this->palette();
				palette.setColor(QPalette::Window, Qt::darkYellow);
				this->setPalette(palette);
				this->setAutoFillBackground(true);

			}
			else {
				//方法1
				QPalette PAllbackground = this->palette();
				//检查图片格式
				/*QImageReader reader(graphPath);
				QImage image = reader.read();
				if (image.isNull()) {
					qDebug() << "Error loading image:" << reader.errorString();
				}
				QByteArrayList supportedFormats = QImageReader::supportedImageFormats();
				qDebug() << "Supported formats:" << supportedFormats;*/

				/*QFile file(graphPath);
				file.open(QIODevice::ReadOnly);
				QByteArray data = file.readAll();
				QImage ImgAllbackground = QImage::fromData(data);*/

				QImage ImgAllbackground(graphPath);

				QImage fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				qDebug() << "width:" << this->width() << " height:" << this->height();
				PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
				this->setPalette(PAllbackground);
				this->setAutoFillBackground(true);

				//方法2
				//QPixmap pix = QPixmap();
				//pix.load(graphPath);
				//// 与窗口等大
				//graphLabel->resize(this->width(), this->height());
				//// 利用setPixmap方法设置背景图片'
				//graphLabel->setPixmap(pix.scaled(graphLabel->size(),
				//	Qt::IgnoreAspectRatio,
				//	Qt::SmoothTransformation));
				//// 这一步自动填充背景不能省略
				//graphLabel->setAutoFillBackground(true);

			}
		}
		void GraphWindow::resizeEvent(QResizeEvent *event) {
			showGraph();
		}
		GraphWindow::~GraphWindow()
		{
			delete graphLabel;
		}
	}
}


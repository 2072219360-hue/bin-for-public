#pragma once

#include <QWidget>
#include<string>
#include <QFileInfo>
#include <QPixmap>
#include<QLabel>
#include <QVBoxLayout>
#include<QPushButton>
#include <QPalette>
#include<QDebug>
#include<QImage>
#include <QPainter>
#include<QImageReader>
#include <QResizeEvent>
#include "ECHVSignalInstance.h"
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
using namespace std;

namespace platforms {
	namespace echv {
		// 用来展示最终的图片
		class GraphWindow : public QWidget
		{
			Q_OBJECT

		public:
			GraphWindow(QString arg_graphPath ="", QWidget *parent = nullptr);
			~GraphWindow();
			void showGraph();

		private:
			
			QLabel *graphLabel;
			QString graphPath;
		protected:
			void resizeEvent(QResizeEvent *event) override;
		};
	}
}




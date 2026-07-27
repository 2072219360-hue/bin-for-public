#pragma once
#include <qwidget.h>
#include <QMetaType>
#include <QHBoxLayout>
#include "scenewidget.h"
namespace platforms {
	namespace sspns {
		class PostGridSimplify :
			public QWidget
		{
			Q_OBJECT
		public:
			PostGridSimplify();
			~PostGridSimplify();
			void initControl();
			void initConnect();

		private:
			reveal::SceneWidget* m_sceneGridSimplify;
			QWidget* m_operatePlane;
			QHBoxLayout*m_mainLayout;
		};
	}
}


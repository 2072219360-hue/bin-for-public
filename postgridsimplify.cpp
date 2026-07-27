#include "postgridsimplify.h"
namespace platforms {
	namespace sspns {
		PostGridSimplify::PostGridSimplify() 
		{
			initControl();
		}

		PostGridSimplify::~PostGridSimplify()
		{
		}
		void PostGridSimplify::initControl()
		{
			m_sceneGridSimplify = new reveal::SceneWidget();
			m_operatePlane = new QWidget;
			m_operatePlane->setFixedWidth(1000);
			m_mainLayout = new QHBoxLayout;
			m_mainLayout->addWidget(m_sceneGridSimplify);
			m_mainLayout->addWidget(m_operatePlane);
			this->setLayout(m_mainLayout);
		}
		void PostGridSimplify::initConnect()
		{
		}
	}
}
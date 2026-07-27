#include "btnWiget.h"

namespace platforms {
	namespace sspns {
		btnWigdet::btnWigdet(int _columnIndex , int _btnNum , std::initializer_list<QString> args )
			:m_columnIndex(_columnIndex),m_btnNum(_btnNum) {
			m_btn.resize(_btnNum);
			m_layout = new QGridLayout;
			for (int i = 0; i < args.size(); i++) {
				m_btn[i] = new QPushButton(*(args.begin()+i));
				m_layout->addWidget(m_btn[i],i/2,i%2);
			}
			this->setLayout(m_layout);
		}
		void btnWigdet::setBodyMode() {
			m_layout->removeWidget(m_btn[0]);
			m_layout->removeWidget(m_btn[1]);
			m_layout->addWidget(m_btn[1], 0, 0, 1,2);
		}
		void btnWigdet::dynamicConnect(int index, QObject* reciver, std::function<void(int)> f) {
			if (index >= m_btn.size()) return;
			connect(m_btn[index], &QPushButton::clicked, reciver, [=]() {
				f(m_columnIndex);
			});
		}
		btnWigdet::~btnWigdet() {
			for (QPushButton* button : m_btn) {
				delete button;
			}
			m_btn.clear();
		}

	}
}

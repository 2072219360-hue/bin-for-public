#pragma once
#include <qwidget.h>
namespace platforms {
	namespace detail {
		class SingletonBase :
			public QWidget
		{
			Q_OBJECT
		protected:
			SingletonBase();
			
		public:
			SingletonBase(const SingletonBase &) = delete;
			SingletonBase &operator = (const SingletonBase &) = delete;
			virtual ~SingletonBase() = default;
		};

	}/// detail

}/// platforms


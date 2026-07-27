#pragma once
#include<QVector>
#include<QString>
namespace platforms {
	namespace sspns {
		enum processStatus{
			IS_FREE = 0,
			IS_RUNNING_HEAD = 1,
			IS_RUNNING_BODY = 2,
			BODY_FAIL=3,
			HEAD_FAIL=4,
		};
		static QVector<QString> statusName = {u8"都未运行",u8"正在运行头部",u8"正在运行身部",u8"pns身部运行失败",u8"头部运行失败" };

	}
}

#include "argData.h"


namespace platforms {
	namespace echv {
		argData::argData(string arg_filePath , int arg_xSize , int arg_ySize,  int arg_zSize , double arg_interval)
			:filePath(arg_filePath),xSize(arg_xSize), ySize(arg_ySize), zSize(arg_zSize), interval(arg_interval){

		}

		argData::~argData() {

		}
		void argData::copy(argData *arg) {
			this->filePath = arg->filePath;
			this->xSize = arg->xSize;
			this->ySize = arg->ySize;
			this->zSize = arg->zSize;
			this->interval = arg->interval;

		}
		string argData::getFilePath() {
			return this->filePath;
		}
	}
}
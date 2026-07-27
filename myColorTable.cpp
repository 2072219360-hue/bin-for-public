#include "myColorTable.h"
myColorTable::myColorTable() {
	colorNum = 9 * 9 * 9;
	unsigned char(*colorTableTemp)[3] = new unsigned char[colorNum][3]{ {0,0,0} };
	int index = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				if (i <= 2 || j <= 2 || k <= 2) continue;
				if (i == 9) colorTableTemp[index][0] = 255;
				else colorTableTemp[index][0] = i * 32;
				if (j == 9)colorTableTemp[index][1] = 255;
				else colorTableTemp[index][1] = j * 32;
				if (k == 9) colorTableTemp[index][2] = 255;
				else colorTableTemp[index][2] = k * 32;
				index++;
			}
		}
	}
	colorNum = index;
	//color 1 相邻的颜色很接近
	/*for (int i = 0; i < colorNum; i++) {
		colorTable[i][0] = colorTableTemp[i][0];
		colorTable[i][1] = colorTableTemp[i][1];
		colorTable[i][2] = colorTableTemp[i][2];
	}*/
	//color 2  gap个一组
	//colorTable = new unsigned char[colorNum][3]{ {0,0,0} };
	colorTable = new unsigned char[index][3]{ {0,0,0} };
	int gap = 20;
	index = 0;
	for (int i = 0; i < gap; i++) {
		for (int j = 0; j < (colorNum + gap - 1) / gap; j++) {
			int id = i + j * gap;
			if (id >= colorNum) continue;
			colorTable[index][0] = colorTableTemp[id][0];
			colorTable[index][1] = colorTableTemp[id][1];
			colorTable[index][2] = colorTableTemp[id][2];

			index++;
		}
	}
	colorNum = index;
	//printf("index=%d colorNum=%d\n", index, colorNum);

	/*for (int i = 0; i < colorNum/2; i++) {
		unsigned char temp[3];
		temp[0] = colorTable[i][0];
		temp[1] = colorTable[i][1];
		temp[2] = colorTable[i][2];

		colorTable[i][0] = colorTable[colorNum - 1 - i][0];
		colorTable[i][1] = colorTable[colorNum - 1 - i][1];
		colorTable[i][2] = colorTable[colorNum - 1 - i][2];

		colorTable[colorNum - 1 - i][0] = temp[0];
		colorTable[colorNum - 1 - i][1] = temp[1];
		colorTable[colorNum - 1 - i][2] = temp[2];
	}*/
	delete[]colorTableTemp;
}
myColorTable::~myColorTable() {
	delete[]colorTable;
}

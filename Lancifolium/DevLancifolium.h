#ifndef DEVLANCIFOLIUM_H
#define DEVLANCIFOLIUM_H

#include <algorithm>
#include "GeneBase.h"
#include "FunLancifolium.h"
#include "FileBuff.h"

#define TRIANGLE 1 /* 三角形 */
#define DIAMOND 2 /* 方塊 */
#define FORK 3 /* 叉 */
#define CIRCLE 4 /* 圓 */

#define BUFFER_LENGTH 999999 /* 評論長度 */

struct DevLancifolium { // 全局結構
	int siz; // 棋盤規格

	//struct GnNode *root; // 根節點
	GnTree *gntree; // 棋譜

	char reader;
	FileOpenBuff read;

	struct GnNode *curNode; // 當前節點指針
	struct GnNode *tmpNode; // 臨時指針
	std::stack<struct GnNode *> branchStack; // 分支棧

	/* 初始 */
	void init();
	void clearall();
	DevLancifolium();
	~DevLancifolium();


	/* 讀取棋譜函數 */
	int openfile(char *filename);

	/* 棋譜信息函數 */
	int dealSize();

	/* 讀取處理 */
	int dealAddStones(struct GnNode *tmpnode, int colour);

	int dealMove(struct GnNode *tmpnode, int colour);

	int dealCommentNodename(struct GnNode *tmpnode, int tmpkind);

	int dealLabels(struct GnNode *tmpnode, int form);

	/* 節點處理函數 */
	int configNode();

	/* 讀取棋譜主函數 */
	int configManual(char *filename);
};

#endif // DEVLANCIFOLIUM_H

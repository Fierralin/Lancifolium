#include "DevLancifolium.h"

void DevLancifolium::init() {
	gntree = NULL; siz = 19;
}

int DevLancifolium::openfile(char *filename)  { // 讀入文件
	return read.openfile(filename);
} // finished openfile

void DevLancifolium::clearall() {
	//while (!branchStack.empty()) branchStack.pop();
	gntree->deleteroot(gntree->treeroot);
	init();
}

DevLancifolium::DevLancifolium() {
	init();
}

DevLancifolium::~DevLancifolium() {
	printf("\nRunning DevLancifolium Destructor. \n");
	//clearall();
}


/* 處理節點內容函數 */
int DevLancifolium::dealSize() {
	char tmpnum[3];//, reader;
	int tmpi;
	reader = read.getc(); // 棄了'['
	tmpi = 0;
	while (isdigit(reader)) {
		tmpnum[tmpi++] = reader; reader = read.getc();
		if (tmpi > 2) break;
	}
	tmpnum[tmpi] = '\0';
	siz = atoi(tmpnum);
	if (siz < 4) siz = 4; else if (siz > 26) siz = 26;
	reader = read.getc(); //
}

int DevLancifolium::dealAddStones(struct GnNode *tmpnode, int colour) {
	int tmpx, tmpy;

	if (colour == 0) return 0;
	else if (colour == 1) { // 添加黑子
		while (reader == '[') {
			tmpx = toupper(read.getc()) - 'A';
			tmpy = toupper(read.getc()) - 'A';
			if ((tmpx >= 0) && (tmpx < siz) && (tmpy >= 0) && (tmpy < siz)) {
				tmpnode->addblacks.push_back(tmpx * 100 + tmpy);
			}
			reader = read.getc(); // 棄了']'
			reader = read.getc(); // '['
		}
	}
	else { // 添加白子
		while (reader == '[') {
			tmpx = toupper(read.getc()) - 'A';
			tmpy = toupper(read.getc()) - 'A';
			if ((tmpx >= 0) && (tmpx < siz) && (tmpy >= 0) && (tmpy < siz)) {
				tmpnode->addwhites.push_back(tmpx * 100 + tmpy);
			}
			reader = read.getc(); // 棄了']'
			reader = read.getc(); // '['
		}
	}
} // finished dealAddStones

int DevLancifolium::dealMove(GnNode *tmpnode, int colour) {
	int tmpx, tmpy;
	reader = toupper(read.getc());
	while (!isalpha(reader) && reader != EOF) reader = read.getc();
	tmpx = reader - 'A';
	reader = toupper(read.getc());
	tmpy = reader - 'A';
	if (reader == EOF) return 0;
	tmpnode->mov = tmpx * 100 + tmpy;
	tmpnode->stoneProp = colour;

	reader = read.getc(); // ']'
	reader = read.getc(); // 棄了']'
}

int DevLancifolium::dealCommentNodename(GnNode *tmpnode, int tmpkind) {
	char tmpsave, buff[BUFFER_LENGTH]; /* 此處不宜過大，否則會出問題的 */
	int tmpi = 0;

	reader = read.getc(); // 棄了'['
	tmpsave = '\0';
	while ((reader != ']') || (tmpsave == '\\')) {
		if (tmpsave == '\\') { // 此處要改
			if (reader == 'n') buff[tmpi - 1] = '\n';
			else if (reader == 't') buff[tmpi - 1] = '\t';
			else if ((reader == '[') || (reader == ']')
				|| (reader == '\\')) buff[tmpi - 1] = reader;
			else buff[tmpi++] = reader;
			tmpsave = reader;
		}
		else {
			tmpsave = reader;
			if (tmpi < BUFFER_LENGTH) buff[tmpi++] = reader;
		}
		reader = read.getc(); // 下一個
	}
	buff[tmpi] = '\0';
	//printf("$$buff: %s", buff); ////////////
	gntree->insertcomment(buff, tmpnode, tmpkind);
	reader = read.getc(); // 棄了']'
} // finished dealCommentNodename

int DevLancifolium::dealLabels(struct GnNode *tmpnode, int form) {
	/*
	 * labels can be LB[ab][cd]... MA[ab][cd]
	 * however, labels can also be: [pg:A]
	**/
	int tmpform;
	int tmplab;
	while (reader == '[') {
		tmplab = toupper(read.getc()) - 'A';
		tmplab *= 100;
		tmplab += toupper(read.getc()) - 'A';
		while (reader != ']') reader = read.getc(); // ']'
		reader = read.getc(); // '[' or not
		tmpnode->labels.push_back(tmplab);
	}

	switch (form) {
	case 0: // 字母標籤
		for (int tmpi = 0; tmpi < tmpnode->labels.size(); tmpi++) {
			tmpnode->labels[tmpi] += ('A' + tmpi) * 10000;
			printf("----alphabet labels: %d \n", tmpnode->labels[tmpi]); /*----------------*/
		}
		break;
	case TRIANGLE: tmpform = TRIANGLE;
	case DIAMOND: tmpform = DIAMOND;
	case FORK: tmpform = FORK;
	case CIRCLE: tmpform = CIRCLE;
		for (int tmpi = 0; tmpi < tmpnode->labels.size(); tmpi++)
			tmpnode->labels[tmpi] += tmpform * 10000;
		break;
	default: break;
	}

	return 0;
}


/* 處理節點和棋譜的函數 */
int DevLancifolium::configNode() { // 處理一個非根節點，curNode指之
	char operate[10]; int tmpi;
	reader = read.getc(); // 棄掉';' 始以';'，終以';', '(', ')'

	while ((reader != ';') && (reader != '(') && (reader != ')')) { // 內容
		while (iswhite(reader)) reader = read.getc(); // 空白
		tmpi = 0; // 獲取標誌
		while (isalpha(reader)) { // 當是字母的時候
			operate[tmpi++] = toupper(reader); reader = read.getc();
		}
		operate[tmpi] = '\0'; // LB, C, N, AB, AW, SZ,
		while (reader != '[' && reader != EOF) reader = read.getc(); // 找到'['
		if (reader == EOF) return 0; // EOF

		switch (operatecase(operate)) {
		case 1202: dealLabels(curNode, 0); break; /* LB 字母 0 */
		case 2018: dealLabels(curNode, TRIANGLE); break; /* TR 三角 1 */
		case 1917: dealLabels(curNode, DIAMOND); break; /* SQ 方塊 2 */
		case 1301: dealLabels(curNode, FORK); break; /* MA 叉 3 */
		case 318:  dealLabels(curNode, CIRCLE); break; /* CR 圓 4 */
		case 3:    dealCommentNodename(curNode, 1); break; /* C comment */
		case 14:   dealCommentNodename(curNode, 2); break; /* N nodename */
		case 102:  dealAddStones(curNode, BLACKSTONE); break; /* AB 添加黑子 */
		case 123:  dealAddStones(curNode, WHITESTONE); break; /* AW 添加白子 */
		case 1926: dealSize(); break; /* SZ deal size */
		case 2:    dealMove(curNode, BLACKSTONE); break; /* B 黑走子 */
		case 23:   dealMove(curNode, WHITESTONE); break; /* W 白走子 */
		default: /* 其他忽略 */
			while (reader != ']' && reader != EOF) reader = read.getc();
			reader = read.getc(); // 棄了']'
		}

		while (iswhite(reader)) reader = read.getc(); // 去掉空白，必不可少
	} // finished while ';' '('
	std::sort(curNode->addblacks.begin(), curNode->addblacks.end(), sortcmp);
	std::sort(curNode->addwhites.begin(), curNode->addwhites.end(), sortcmp);
	std::sort(curNode->labels.begin(), curNode->labels.end(), sortcmp);
} // finished configNode

int DevLancifolium::configManual(char *filename) {
	if (openfile(filename)) return 1; /* 文件讀取失敗 */
	if (gntree == NULL) gntree = new GnTree;
	if (gntree->treeroot != NULL) return 2; /* 棋譜樹不爲空 */
	this->init(); /* init */
	gntree->treeroot = new struct GnNode;
	gntree->treeroot->init(NULL);
	while (!branchStack.empty()) branchStack.pop(); // 清空棧

	reader = read.getc();
	while (reader != '(' && reader != EOF) reader = read.getc(); /* 找到第一個左括號 */
	while (reader != ';' && reader != EOF) reader = read.getc(); /* 找到分號 */
	curNode = gntree->treeroot; /**/
	configNode();

	while (true) { // 處理棋譜
		if (reader == ';') { // ';'
			tmpNode = curNode;
			curNode = new struct GnNode;
			tmpNode->insertNextNode(curNode);
			configNode(); /**/
		}
		else if (reader == '(') { // '('
			tmpNode = curNode;
			curNode = new struct GnNode;
			tmpNode->insertNextNode(curNode); /* 挿入分支 */
			branchStack.push(tmpNode); /* 上一分支節點入棧 */
			reader = read.getc();
			while ((reader != ';') && (reader != '(') && (reader != ')')
				&& (reader != EOF)) reader = read.getc();
			if (reader == ';') configNode(); /* 當是節點的時候直接設置 */
		}
		else if (reader == ')') { // ')'
			if (branchStack.empty()) { // 根節點的')'，讀譜結束
				printf("\nFinished Reading Manual. \n");
				break;
			}
			else {
				curNode = branchStack.top();
				branchStack.pop();
				reader = read.getc(); // 棄掉')'
				while ((reader != ';') && (reader != '(') && (reader != ')')
					&& (reader != EOF)) reader = read.getc();
			}
		} // finished if-else ';', '(', ')'.
		if (reader == EOF) break; // EOF
	} // finished while true
} // finished configManual



#include <bits/stdc++.h>
#include "btree.hpp"

int main() {
	long double stdMapTime = 0;
	long double bTreeTime = 0;
	long double start, end;
	std::map<std::string, unsigned long long> mp;

	btree::BTree btree;
	char q[KEY_SIZE + 1];
	unsigned short curSize = 0;
	unsigned long long curValue;
	Pair curItem;
	// TBtreeNode<TItem>* curSearchNode;
	unsigned long long curSearchPos;
	while (scanf("%s", q) > 0) {
		if (q[0] == '+') {
			scanf("%s%llu", q, &curValue);
			for (unsigned short i = 0; i < KEY_SIZE + 1; ++i) {
				if ('A' <= q[i] and q[i] <= 'Z') {
					q[i] = q[i] - 'A' + 'a';
				}
				curItem.key[i] = q[i];
				if (q[i] == 0) {
					curSize = i;
					break;
				}
			}
			// curItem.keySize = curSize;
			curItem.value = curValue;
			// curSearchNode = nullptr;
			curSearchPos = 0;

			// B-tree
			start = clock();
			unsigned long long* res = btree.search(curItem.key);
			if (res == nullptr) {
				btree.insert(curItem);
			}
			end = clock();
			bTreeTime = bTreeTime + end - start;

			// std::map
			start = clock();
			if (mp.find(curItem.key) == mp.end()) {
				mp[curItem.key] = curItem.value;
			}
			end = clock();
			stdMapTime = stdMapTime + end - start;
		} else if (q[0] == '-') {
			scanf("%s", q);
			for (unsigned short i = 0; i < KEY_SIZE + 1; ++i) {
				if ('A' <= q[i] and q[i] <= 'Z') {
					q[i] = q[i] - 'A' + 'a';
				}
				curItem.key[i] = q[i];
				if (q[i] == 0) {
					curSize = i;
					break;
				}
			}
			// curItem.keySize = curSize;
			// curSearchNode = nullptr;
			curSearchPos = 0;

			// B-tree
			start = clock();
			unsigned long long* res = btree.search(curItem.key);
			if (res != nullptr) {
				btree.remove(curItem.key);
			}
			end = clock();
			bTreeTime = bTreeTime + end - start;

			// std::map
			start = clock();
			if (mp.find(curItem.key) != mp.end()) {
				mp.erase(curItem.key);
			}
			end = clock();
			stdMapTime = stdMapTime + end - start;

		} else {
			for (unsigned short i = 0; i < KEY_SIZE + 1; ++i) {
				if ('A' <= q[i] and q[i] <= 'Z') {
					q[i] = q[i] - 'A' + 'a';
				}
				curItem.key[i] = q[i];
				if (q[i] == 0) {
					curSize = i;
					break;
				}
			}
			// curItem.keySize = curSize;
			// curSearchNode = nullptr;
			curSearchPos = 0;
			// B-tree
			start = clock();
			btree.search(curItem.key);
			end = clock();
			bTreeTime = bTreeTime + end - start;

			// std::map
			start = clock();
			mp.find(curItem.key);
			end = clock();
			stdMapTime = stdMapTime + end - start;
		}
		// Clear(curItem.key);
	}
	printf("B-tree: %.3Lf ms\n", bTreeTime / 1000.0);
	printf("std::map: %.3Lf ms\n", stdMapTime / 1000.0);
	return 0;
}
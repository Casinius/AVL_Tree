#include <iostream>
#include "avl_tree.h"
template<avl_request_type T>
void print(avl_tree<T>& tree) {

	int idx = 0;
	for (avl_node<T> x : tree) {
		//std::cout << "     " << x.value << "\n";

		std::cout << x.self << "  |  x.value:" << x.value << "   |   ";
		if (x.left != -1 && x.right == -1) {

			std::cout << "left:" << tree.at(x.left).value;

		}
		if (x.right != -1 && x.left == -1) {

			std::cout  << "right:" << tree.at(x.right).value;

		}
		if (x.right != -1 && x.left != -1) {

			std::cout  << "left:" << tree.at(x.left).value << " | right:" << tree.at(x.right).value;

		}
		std::cout << "\n";
		idx++;
	}
}

int main(int argc, char** argv) {
	avl_tree<int> aa;
	aa.make_avl_bst({1, 5, 4, 2, 3, 6, 77, 56, 32, 22, 41});
	printf("\n");
	print(aa);
	//for (auto x : aa) {
	//	std::cout << x.value << "\n";
	//}

	//printf("{}", aa.size());
	return 0;
}

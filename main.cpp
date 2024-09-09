#include <iostream>
#include "avl_tree.h"
template<avl_request_type T>
void print(avl_tree<T>& tree) {
	int l = 0, r = 0;
	for (avl_node<T> x : tree) {
		//std::cout << "     " << x.value << "\n";
		if (x.left != nullptr && x.right == nullptr) {
			for (int i = tree.size(); i > l; i--) {
				std::cout << " ";
			}
			std::cout << x.left->value;

			l++;
		}
		if (x.right != nullptr && x.left == nullptr) {
			std::cout << "                 ";
			for (int i = 0; i < r; i++) {
				std::cout << " ";
			}
			std::cout << x.right->value;
			r++;
		}
		std::cout << "\n";
	}
}

int main(int argc, char** argv) {
	avl_tree<int> aa;
	aa.make_avl_bst({1, 5, 4, 2, 3, 6});
	printf("\n");
	print(aa);
	//for (auto x : aa) {
	//	std::cout << calc_factor(&(x)) << "   " << x.value << "\n\n";
	//}

	//printf("{}", aa.size());
	return 0;
}

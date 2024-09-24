<<<<<<< HEAD:avl_tree.h
#include<numeric>
#include<vector>
#include<cstdint>
template<typename T>
concept avl_request_type = requires(T a, T b) {
	a > b;
	a < b;
	a >= b;
	a <= b;
	a == b;
};
#define INVLD_DEFAULT -1
enum child_code {
	c_left,
	c_all,
	c_none,
	c_right
};

template<avl_request_type T>
struct avl_node {
  public:
	T value;
	int64_t left{INVLD_DEFAULT};
	int64_t right{INVLD_DEFAULT};
	int64_t father{INVLD_DEFAULT};
	int64_t self{INVLD_DEFAULT};
	int64_t height{INVLD_DEFAULT};
	avl_node(): left(INVLD_DEFAULT), right(INVLD_DEFAULT), father(INVLD_DEFAULT), self(INVLD_DEFAULT), height(INVLD_DEFAULT) {}
	void clear() {
		left = INVLD_DEFAULT;
		right = INVLD_DEFAULT;
		father = INVLD_DEFAULT;
	}
	bool is_valid_height() {
		if (height != -1) {
			return true;
		} else {
			return false;
		}
	}
	~avl_node() {
		this->clear();
	}
	int get_child_code() {

		if (this->right != INVLD_DEFAULT && this->left == INVLD_DEFAULT) {
			return child_code::c_right;
		}
		if (this->right == INVLD_DEFAULT && this->left == INVLD_DEFAULT) {
			return child_code::c_none;
		}
		if (this->right == INVLD_DEFAULT && this->left != INVLD_DEFAULT) {
			return child_code::c_left;
		}
		if (this->right != INVLD_DEFAULT && this->left != INVLD_DEFAULT) {
			return child_code::c_all;
		}
		throw child_code::c_none;
	}

	bool is_fathers_left() {
		if (this->father == this) {
			return true;
		}
		if (this->father == this) {
			return false;
		}
		return false;
	}
};


template<avl_request_type T>
struct avl_tree : public std::vector<avl_node<T>> {
	void make_avl_bst(std::initializer_list<T> list) {
		avl_node<T> buf;
		int64_t self = 0;
		for (T x : list) {
			buf.self = self;
			buf.value = x;
			this->emplace_back(buf);
			buf.clear();
			++self;
		}
		int64_t std_idx = 0;
		std::vector<int64_t> path;
		for (int64_t i = 1; i < this->size(); i++) {
			while (this->at(std_idx).left != INVLD_DEFAULT && this->at(std_idx).right != INVLD_DEFAULT) {
				path.emplace_back(this->at(i).self);
				if (this->at(std_idx).value > this->at(i).value) {
					if (this->at(std_idx).left != INVLD_DEFAULT) {

					}
				}
				if (this->at(std_idx).value < this->at(i).value) {

				}
			}

		}
	}
};
=======
#include<iostream>
#include<vector>
#include<initializer_list>
#include<memory>
template<typename T>
concept avl_request_type = requires(T a, T b) {
	a > b;
	a < b;
	a >= b;
	a <= b;
	a == b;
};

enum child_code {
	left,
	all,
	none,
	right
};

template<avl_request_type T>
struct avl_node {
  public:
	T value;
	avl_node* left{nullptr};
	avl_node* right{nullptr};
	avl_node* father{nullptr};
	int height{-1};
	avl_node(): left(nullptr), right(nullptr), father(nullptr) {}
	void clear() {
		left = nullptr;
		right = nullptr;
		father = nullptr;
	}
	bool is_valid_height() {
		if (height != -1) {
			return true;
		} else {
			return false;
		}
	}
	~avl_node() {
		this->clear();
	}
	int get_child_code() {

		if (this->right != nullptr && this->left == nullptr) {
			return child_code::right;
		}
		if (this->right == nullptr && this->left == nullptr) {
			return child_code::none;
		}
		if (this->right == nullptr && this->left != nullptr) {
			return child_code::left;
		}
		if (this->right != nullptr && this->left != nullptr) {
			return child_code::all;
		}
		throw child_code::none;
	}

	bool is_fathers_left() {
		if (this->father->left == this) {
			return true;
		}
		if (this->father->right == this) {
			return false;
		}
		return false;
	}
};

template<avl_request_type T>
int calc_factor(avl_node<T>* root) {
	int left_deep = 0;
	int right_deep = 0;
	if (root != nullptr) {
		if (root->left != nullptr) {
			++left_deep;
			left_deep += calc_factor(root->left);
		}
		if (root->right != nullptr) {
			--right_deep;
			right_deep += calc_factor(root->right);
		}
	}

	return left_deep + right_deep;
}
#include<functional>
template<avl_request_type T>
struct avl_tree : public std::vector<avl_node<T>> {

	avl_node<T>* rotate(bool is_left, avl_node<T>* current_root = nullptr) {
		if (current_root != nullptr) {
			std::cout << "Root UnNull\n";
			if (current_root->right != nullptr) {
				std::cout << "Right UnNull\n";
				if (is_left) {//向左旋转
					std::cout << "Is Left\n";
					avl_node<T>* old_grandfa = current_root->father;
					//avl_node<T>* old_left = current_root->left;
					avl_node<T>* new_root = current_root->right;
					avl_node<T>* new_root_left = new_root->left;//左冲突
					if (current_root->is_fathers_left()) {
						old_grandfa->left = new_root;

					} else {
						old_grandfa->right = new_root;
					}
					new_root->father = old_grandfa;//接上老父节点
					current_root->father = new_root;
					new_root->left = current_root;//原来的父节点变左节点


					auto opt_node = current_root;
					int final_code = child_code::none;
					while (new_root_left != nullptr) {
						std::cout << "Geting Root\n";
						if (new_root_left->value > opt_node->value) {
							if (opt_node->right == nullptr) {
								final_code = child_code::right;
								break;
							}
							opt_node = opt_node->right;
						}
						if (new_root_left->value < opt_node->value) {
							if (opt_node->left == nullptr) {
								final_code = child_code::left;
								break;
							}
							opt_node = opt_node->left;
						}
					}
					std::cout << "FinalCode" << final_code << "\n";
					switch (final_code) {
						case child_code::left:
							opt_node->left = new_root_left;
							break;
						case child_code::right:
							opt_node->right = new_root_left;
							break;
						default:
							//TODO
							break;
					}
					return new_root;
				}
			} else {
				static_assert("RightPtr Nullptr");
			}
			if (current_root->left != nullptr) {
				std::cout << "Left UnNull\n";
				if (!is_left) {//向右旋转
					std::cout << "Is Right\n";
					avl_node<T>* new_root = current_root->left;
					avl_node<T>* old_grandfa = current_root->father;
					avl_node<T>* new_root_right = new_root->right;
					if (current_root->is_fathers_left()) {
						old_grandfa->left = new_root;

					} else {
						old_grandfa->right = new_root;
					}
					new_root->father = old_grandfa;//接上老父节点
					current_root->father = new_root;
					new_root->right = current_root;//原来的父节点变左节点

					auto opt_node = current_root;
					int final_code = child_code::none;
					while (new_root_right != nullptr) {
						if (new_root_right->value > opt_node->value) {
							if (opt_node->right == nullptr) {
								final_code = child_code::right;
								break;
							}
							opt_node = opt_node->right;
						}
						if (new_root_right->value < opt_node->value) {
							if (opt_node->left == nullptr) {
								final_code = child_code::left;
								break;
							}
							opt_node = opt_node->left;
						}
					}
					switch (final_code) {
						case child_code::left:
							opt_node->left = new_root_right;
							break;
						case child_code::right:
							opt_node->right = new_root_right;
							break;
						default:
							//TODO
							break;
					}
					return new_root;

				}
			} else {
				static_assert("LeftPtr Nullptr");
			}

		} else {
			static_assert("Nullptr Expection!");
		}
		static_assert("Not Rotate!");
		throw this;
	}

	void op_iter_node(std::function<void(avl_node<T>* to_opt)> opt_func, avl_node<T>* current_root = nullptr) {
		avl_node<T>* root = current_root;

		while (root != nullptr) {
			if (root->left != nullptr) {
				opt_func(root->left);
				opt_iter_node(opt_func, root->left);
			}
			if (root->right != nullptr) {
				opt_func(root->right);
				opt_iter_node(opt_func, root->right);
			}
		}

	}

	void make_avl_bst(std::initializer_list<T> list) {
		avl_node<T> buf;
		for (T x : list) {
			buf.value = x;
			this->emplace_back(buf);
			buf.clear();
		}

		std::vector<avl_node<T>*> path;
		avl_node<T>* opt_node = &(this->at(0));
		int final_code = child_code::none;
		//std::cout << "Begin Find Node\n";
		for (int i = 1; i < (int)this->size(); ++i) {
			while (opt_node != nullptr) {
				//std::cout << "FindNode\n";
				path.emplace_back(opt_node);
				if (this->at(i).value > opt_node->value) {
					if (opt_node->right == nullptr) {
						final_code = child_code::right;
						break;
					}
					opt_node = opt_node->right;
				}
				if (this->at(i).value < opt_node->value) {
					if (opt_node->left == nullptr) {
						final_code = child_code::left;
						break;
					}
					opt_node = opt_node->left;
				}
				if (this->at(i).value == opt_node->value) {
					throw this;
				}
			}
			std::cout << "Code:" << final_code << "\n";
			switch (final_code) {
				case child_code::left:
					opt_node->left = &(this->at(i));
					this->at(i).father = opt_node->left;
					break;
				case child_code::right:
					opt_node->right = &(this->at(i));
					this->at(i).father = opt_node->right;
					break;
				default:
					//TODO
					break;
			}
		}

		int a = 0;
		for (int i = path.size() - 1; i >= 0; --i) {
			if (path.at(i)->get_child_code() == child_code::none) {
				continue;
			} else {
				a = calc_factor(path.at(i));
				std::cout << a << "\n";
				if (a == 2) {
					//rotate(false, path.at(i));
				}
				if (a == -2) {
					//rotate(true, path.at(i));
				}
			}
		}




	}

};






template<avl_request_type T>
void swap(T& v1, T& v2) {
	T v3;
	v3 = v1;
	v1 = v2;
	v2 = v3;
}
template<avl_request_type T>
void quick_sort(std::vector<T>& tmp_list) {
	using index = int;
	const T base_line = tmp_list[(tmp_list.size() - 1) / 2];
	index backward = (tmp_list.size() - 1);
	index forward = 0;
}






/*
template<avl_request_type T>
int calculate_route(avl_node<T>* central) {
enum type {
LL,
RR,

RL,
LR,

LB,
RB,

Balanced
};
avl_node<T>* current_opt = central;

if (current_opt != nullptr) {
//left
if (current_opt->father->left == current_opt && current_opt->father->right == nullptr) {
if (current_opt->left != nullptr && current_opt->right == nullptr) {
return type::LL;
}
if (current_opt->left != nullptr && current_opt->right != nullptr) {
return type::LB;
}
if (current_opt->left == nullptr && current_opt->right != nullptr) {
return type::LR;
}
}
//right
if (current_opt->father->right == current_opt && current_opt->father->left == nullptr) {
if (current_opt->left != nullptr && current_opt->right == nullptr) {
return type::RL;
}
if (current_opt->left != nullptr && current_opt->right != nullptr) {
return type::RB;
}
if (current_opt->left == nullptr && current_opt->right != nullptr) {
return type::RR;
}
}
}
return type::Balanced;
}
template<avl_request_type T>
void optimize_adjust_unit(int route, avl_node<T>* new_father) {
enum type {
LL,
RR,

RL,
LR,

LB,
RB,

Balanced
};
switch (route) {
case LL:

avl_node<T>* grand_father = nullptr;
if (new_father->father->father != nullptr) {
grand_father = new_father->father->father;
}
new_father->father->left = nullptr;
new_father->right = new_father->father;
new_father->father = grand_father;
break;
case LR:


break;
case LB:

break;


case RR:
break;
case RL:
break;
case RB:
break;



case Balanced:
break;

default:
//TODO
break;
}
}
*/
>>>>>>> 161a4644153b17508081f2be969a5c319d07fd76:src/avl_tree.hpp

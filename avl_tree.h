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

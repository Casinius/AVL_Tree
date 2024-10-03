#include<numeric>
#include<vector>
#include<cstdint>
#include<iostream>
template<typename T>
concept avl_request_type = requires(T a, T b) {
	a > b;
	a < b;
	a >= b;
	a <= b;
	a == b;
};
#define INVLD_DEFAULT -1
enum class child_code : int {
	c_left = 1,
	c_all = 2,
	c_none = 3,
	c_right = 4,
	not_init = -100
};

template<avl_request_type T>
struct avl_node {

  public:
	T value;
	int64_t left{INVLD_DEFAULT};
	int64_t right{INVLD_DEFAULT};
	int64_t father{INVLD_DEFAULT};
	int64_t self{INVLD_DEFAULT};
	//int64_t height{INVLD_DEFAULT};
	avl_node(): left(INVLD_DEFAULT), right(INVLD_DEFAULT), father(INVLD_DEFAULT), self(INVLD_DEFAULT) {}
	void clear() {
		left = INVLD_DEFAULT;
		right = INVLD_DEFAULT;
		father = INVLD_DEFAULT;
	}
	/*
	bool is_valid_height() {
	if (height != -1) {
	return true;
	} else {
	return false;
	}
	}
	*/
	bool has_left() {
		if (this->left != INVLD_DEFAULT) {
			return true;
		} else {
			return false;
		}
	}
	bool has_right() {
		if (this->right != INVLD_DEFAULT) {
			return true;
		} else {
			return false;
		}
	}
	~avl_node() {
		this->clear();
	}
	child_code get_child_code() {

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
  public:
	enum class rotate_situ : int {
		LRiR = 1,
		LRiL = 2,
		LLiL = 3,
		RRiR = 4,
		Balanced = 5,
		unDefine = 6
	};
  private:
	rotate_situ fast_detect_right_branch(const int idx) {
		int r_idx = this->at(idx).right;
		int l_idx = this->at(idx).left;

		if (this->at(idx).has_right()) {
			if (this->at(r_idx).has_left()) {
				return rotate_situ::LRiR;
			}
			if (this->at(r_idx).has_right()) {
				return rotate_situ::RRiR;
			}
		}
		if (this->at(idx).get_child_code() == child_code::c_none) {
			return rotate_situ::Balanced;
		}
	}
	rotate_situ fast_detect_left_branch(const int idx) {
		int r_idx = this->at(idx).right;
		int l_idx = this->at(idx).left;
		if (this->at(idx).has_left()) {
			if (this->at(l_idx).has_left()) {
				return rotate_situ::LLiL;
			}
			if (this->at(l_idx).has_right()) {
				return rotate_situ::LRiL;
			}
		}
		if (this->at(idx).get_child_code() == child_code::c_none) {
			return rotate_situ::Balanced;
		}
	}
	std::vector<rotate_situ> fast_detect(const int idx) {
		std::vector<rotate_situ> ret;
		int r_idx = this->at(idx).right;
		int l_idx = this->at(idx).left;
		if (this->at(idx).has_left()) {
			if (this->at(l_idx).has_left()) {
				ret.emplace_back(rotate_situ::LLiL);
			}
			if (this->at(r_idx).has_right()) {
				ret.emplace_back(rotate_situ::LRiL);
			}
		}
		if (this->at(idx).has_right()) {
			if (this->at(r_idx).has_left()) {
				ret.emplace_back(rotate_situ::LRiR);
			}
			if (this->at(r_idx).has_right()) {
				ret.emplace_back(rotate_situ::RRiR);
			}
		}
		if (this->at(idx).get_child_code() == child_code::c_none) {
			ret.emplace_back(rotate_situ::Balanced);
		}
		return std::move(ret);
	}
  private:
	int calc_factor(int idx) {
		int l = 0, r = 0;
		switch (this->at(idx).get_child_code()) {
			case child_code::c_left:
				l++;
				calc_factor(this->at(idx).left);
				break;
			case child_code::c_right:
				r++;
				calc_factor(this->at(idx).right);
				break;
			case child_code::c_all:
				l++;
				r++;
				calc_factor(this->at(idx).left);
				calc_factor(this->at(idx).right);
				break;
			case child_code::c_none:
				break;
				break;
			default:
				//TODO
				break;
		}
		return l - r;
	}
	constexpr int64_t no_neg(int64_t& a, int64_t& b) {
		return (a - b > 0) ? (a - b) : (b - a);
	}
	void rotate_left(int cur_idx) {
		int father = this->at(cur_idx).father;
		int current = cur_idx;
		int new_root = this->at(current).left;
		if (new_root == INVLD_DEFAULT) {
			throw this;
		} else {
			if (this->at(current).is_fathers_left()) {
				this->at(father).left = new_root;
			} else {
				this->at(father).right = new_root;
			}

			this->at(current).father = new_root;
			if (this->at(new_root).left == INVLD_DEFAULT) {
				this->at(new_root).left = current;
			} else {
				int std_idx = current;
				int new_root_left = this->at(new_root).left;
				this->at(new_root).left = current;
				while (1) {
					if (this->at(new_root_left).value > this->at(std_idx).value) {
						if (this->at(std_idx).right == INVLD_DEFAULT) {
							this->at(std_idx).right = new_root_left;
							this->at(new_root_left).father = std_idx;
							break;
						}
						std_idx = this->at(std_idx).right;
					}
					if (this->at(new_root_left).value < this->at(std_idx).value) {
						if (this->at(std_idx).left == INVLD_DEFAULT) {
							this->at(std_idx).left = new_root_left;
							this->at(new_root_left).father = std_idx;
							break;
						}
						std_idx = this->at(std_idx).left;
					}
					if (this->at(new_root_left).value == this->at(std_idx).value) {
						throw this;
					}
				}
			}
		}
	}
	void rotate_right(int cur_idx) {
		int father = this->at(cur_idx).father;
		int current = cur_idx;
		int new_root = this->at(current).left;
		if (new_root == INVLD_DEFAULT) {
			throw this;
		} else {
			if (this->at(current).is_fathers_left()) {
				this->at(father).left = new_root;
			} else {
				this->at(father).right = new_root;
			}

			this->at(current).father = new_root;
			if (this->at(new_root).right == INVLD_DEFAULT) {
				this->at(new_root).right = current;
			} else {
				int std_idx = current;
				int new_root_right = this->at(new_root).right;
				this->at(new_root).right = current;
				while (1) {
					if (this->at(new_root_right).value > this->at(std_idx).value) {
						if (this->at(std_idx).right == INVLD_DEFAULT) {
							this->at(std_idx).right = new_root_right;
							this->at(new_root_right).father = std_idx;
							break;
						}
						std_idx = this->at(std_idx).right;
					}
					if (this->at(new_root_right).value < this->at(std_idx).value) {
						if (this->at(std_idx).left == INVLD_DEFAULT) {
							this->at(std_idx).left = new_root_right;
							this->at(new_root_right).father = std_idx;
							break;
						}
						std_idx = this->at(std_idx).left;
					}
					if (this->at(new_root_right).value == this->at(std_idx).value) {
						throw this;
					}
				}
			}

		}
	}

  public:
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
		child_code final_code = child_code::not_init;
		//std::vector<int64_t> path;
		for (int64_t i = 1; i < (int64_t)this->size(); ++i) {
			//std::cout << this->at(i).value << " | " << this->at(i).left << " | " << this->at(i).right << ":loopin\n";
			//std_idx error
			std_idx = 0;
			while (1) {
				//path.emplace_back(std_idx);
				if (this->at(i).value > this->at(std_idx).value) {
					if (this->at(std_idx).right == INVLD_DEFAULT) {
						final_code = child_code::c_right;
						break;
					}
					std_idx = this->at(std_idx).right;
				}
				if (this->at(i).value < this->at(std_idx).value) {
					if (this->at(std_idx).left == INVLD_DEFAULT) {
						final_code = child_code::c_left;
						break;
					}
					std_idx = this->at(std_idx).left;
				}
				if (this->at(i).value == this->at(std_idx).value) {
					throw this;
				}
			}
			switch (final_code) {
				case child_code::c_left:
					this->at(std_idx).left = i;
					this->at(i).father = std_idx;
					break;
				case child_code::c_right:
					this->at(std_idx).right = i;
					this->at(i).father = std_idx;
					break;
				default:
					//TODO
					break;
			}
		}//节点关系
		for (int64_t i = 0; i < (int64_t)this->size(); ++i) {
			rotate_situ lres = fast_detect_left_branch(i), rres = fast_detect_right_branch(i);
			if (lres == rotate_situ::LRiL) {

			}
			if (lres == rotate_situ::LRiL) {

			}
			if (rres == rotate_situ::RRiR) {

			}
			if (rres == rotate_situ::LRiR) {

			}

		}
	}
};




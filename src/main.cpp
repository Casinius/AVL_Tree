#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
template <class T>
concept avl_t = requires(T a, T b) {
  a + b;
  a > b;
};
using pos = int64_t;
template <avl_t T> struct avl_node {

  T data;
  pos fa{-1};
  pos left{-1};
  pos right{-1};
  bool hasL() { return (this->left != -1); }
  bool hasR() { return (this->right != -1); }
  bool hasF() { return (this->fa != -1); }
};

template <avl_t T> struct avl_tree : std::vector<avl_node<T>> {
  enum class whichside_t { left, right, no_father };
  pos getN_upper(pos cur, uint n) {
    pos p = cur;
    for (uint i = 0; i < n; ++i) {
      p = this->at(p).fa;
    }
    return p;
  }
  pos get_upper(pos cur) {
    if (this->at(cur).hasF()) {
      return this->at(cur).fa;
    }
    return cur;
  }
  /*
   whichside_t _which_side(pos cur) {
    if (cur>this->size()-1) {
      throw;
    }
    if (!this->at(cur).hasF()) {
      return whichside_t::no_father;
    } else {
      pos father = this->at(cur).fa;
      if (this->at(father).hasL()) {
        return whichside_t::left;
      }
      if (this->at(father).hasR()) {
        return whichside_t::right;
      }
    }
    throw;
  }
  */

  int child_height(pos root) {
    if (root >= this->size()) {
      // throw "xx";
      return 0;
    }
    if ((this->at(root).hasL() && !this->at(root).hasR()) ||
        (!this->at(root).hasL() && this->at(root).hasR())) {
      //std::cout<<"single\n";
      int num = 1;
      for (int i = root + 1; i < this->size(); ++i) {//为什么不行
        // std::cout<<"i:"<<i<<"\n";
        num += this->at(i).hasL() - this->at(i).hasR();
        // std::cout<<"after i:"<<i<<"\n";
      }
      return num;
    }
    if (this->at(root).hasL() && this->at(root).hasR()) {
      // std::cout<<"both\n";
      int num = 1;
      num += child_height(this->at(root).left) + 1;
      num -= child_height(this->at(root).right) + 1;
      return num;
    }
    throw;
  }
  void _cast_push_internal(T &&v) {
    this->emplace_back(avl_node<T>{.data = v});
  }
  void auto_connect(pos root, pos child) {
    if (this->at(root).data > this->at(child).data) {
      if (this->at(root).hasL()) {
        auto_connect(this->at(root).left, child);
      } else {

        this->at(root).left = child;
        this->at(child).fa = root;
      }
    }
    if (this->at(root).data < this->at(child).data) {
      if (this->at(root).hasR()) {
        auto_connect(this->at(root).right, child);
      } else {
        this->at(root).right = child;
        this->at(child).fa = root;
      }
    }
    assert(true);
  }
  void rotateFtR(pos cur) {
    if (this->child_height(cur) > 1 && this->at(cur).hasF()) {
      pos orig_fa = this->at(cur).fa; // also new right
      pos orig_grandfa = this->at(orig_fa).fa;
      this->at(cur).fa = orig_grandfa;
      this->at(cur).right = orig_fa;
      this->at(orig_fa).fa = cur;
      this->at(orig_fa).left = -1;
      auto_connect(cur, orig_fa);
    }
  }
  void rotateFtL(pos cur) {
    if (this->child_height(cur) < -1 && this->at(cur).hasF()) {
      pos orig_fa = this->at(cur).fa; // also new right
      pos orig_grandfa = this->at(orig_fa).fa;
      this->at(cur).fa = orig_grandfa;
      this->at(cur).left = orig_fa;
      this->at(orig_fa).fa = cur;
      this->at(orig_fa).right = -1;
      auto_connect(cur, orig_fa);
    }
  }
};
template <avl_t T>
auto gen_BStree = [](std::vector<T> list) {
  std::sort(list.begin(), list.end());
  // try gen
  avl_tree<T> tr;

  for (int i = 0; i < list.size(); ++i) {
    // std::cout << list[i] << "\n";
    tr._cast_push_internal(std::move(list[i]));
    if (i != list.size() - 1) {
      tr.back().right = i + 1;
    }
    if (i != 0) {
      tr.back().fa = i - 1;
    }
  }

  /*


  */

  pos cur_p = 0;
  // 无需考虑有子树的情况 no need to worry about original child_tr of cur

  // while (std::abs((long)(tr.child_height(cur_p))) > 1) {
  while ((tr.child_height(cur_p)) < -1) {
    tr.rotateFtL(cur_p);
    cur_p = tr[cur_p].right;
    if (cur_p == -1)
      break;
  }
  while (cur_p!=-1) {
    cur_p = tr[cur_p].fa;
    if ((tr.child_height(cur_p)) < -1) {
      tr.rotateFtL(cur_p);
    }
  }

  //}
  return tr;
};
template <class T> void ptf(avl_tree<T> t) {
  for (size_t i = 0; i < t.size(); ++i) {
    std::cout << "self:" << t[i].data << " | ";
    if (t[i].hasL() && t[i].hasR()) {
      std::cout << "left:" << t[t[i].left].data << " | "
                << "right:" << t[t[i].right].data << "\n";
    }
    if (t[i].hasL() && !t[i].hasR()) {
      if (t[i].hasF()) {
        std::cout << "fath:" << t[t[i].fa].data << " | ";
      }
      std::cout << "left:" << t[t[i].left].data << "\n";
    }
    if (t[i].hasR() && !t[i].hasL()) {
      if (t[i].hasF()) {
        std::cout << "fath:" << t[t[i].fa].data << " | ";
      }
      std::cout << "right:" << t[t[i].right].data << "\n";
    }
    std::cout << "\n";
  }
}
template <class T> void ptf_valid(avl_tree<T> t) {
  for (int x = 0; x < t.size() - 1; ++x) {
    std::cout << "i:" << x << " | h:" << t.child_height(x) << "\n";
    // std::cout<<"i:"<<x<<" | h:"<<std::abs((long)(t.child_height(x)))<<"\n";
  }
}
int main(int argc, char **argv) {
  auto tr = gen_BStree<int>({1, 2, 3, 4, 12, 5, 7});
  std::cout<<tr.child_height(4)<<"\n";
  ptf(tr);
  // std::cout<<"\n";

  return 0;
}

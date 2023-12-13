//
// Created by Lenovo on 2023/12/10.
//

#ifndef BOOKSTORE__STORE_HPP_
#define BOOKSTORE__STORE_HPP_
#include "MemoryRiver.hpp"
#include<vector>
#include<cstring>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

void copy(char *a, const char *b) {
  int i = 0;
  while (b[i] != '\0') {
	a[i] = b[i];
	i++;
  }
  a[i] = '\0';
}

struct key_value {
  char name[65] = {'\0'};
  int value = 0;
};

struct Node {
  char head_name[65] = {'\0'};
  int next_index = -1;
  int pre_index = -1;
  int lump_head;
  int sizeof_lump;
};

class store {
 public:
  store() = default;
  //建一个新的块
  int new_node(int pre, int next) {
	int begin;
	key_value init;
	begin = lump.write(init);
	for (int i = 0; i < lump_size - 1; ++i) {
	  lump.write(init);
	}
	Node node;
	node.lump_head = begin;
	node.next_index = next;
	node.pre_index = pre;
	node.sizeof_lump = 0;
	int k = link.write(node);
	return k;
  }

  //未来可能要分文件
  void initialise() {
	link.initialise("LINK");
	lump.initialise("LUMP");
	link.write_info(-1, 3);
	link.write_info(-1,2);
  }

  void insert(key_value date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	//对应链表为空
	if (head == -1) {
	  int rear = new_node(-1,-1);
	  link.write_info(rear, 2);
	  link.write_info(rear, 3);
	  Node node;
	  link.read(node,rear);
	  copy(node.head_name,date.name);
	  node.sizeof_lump++;
	  link.update(node,rear);
	  lump.update(date, node.lump_head);
	  return;
	}
	  //链表非空
	else {
	  int rear = head;
	  Node node_left, node_right;
	  link.read(node_right, rear);		int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;

	  if (std::strcmp(node_right.head_name, date.name) > 0) {
		lump.Insult(date, node_right.lump_head, node_right.lump_head + sizeof(key_value) * node_right.sizeof_lump);
		copy(node_right.head_name, date.name);
		node_right.sizeof_lump++;
		link.update(node_right, rear);
		crack(node_right,rear);
	  } else {
		//寻找插入位置
		while (rear != tail) {
		  node_left = node_right;
		  link.read(node_right, node_left.next_index);

		  if (std::strcmp(node_left.head_name, date.name) <= 0 && std::strcmp(node_right.head_name, date.name) >= 0) {
			int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
			while (begin < final) {
			  int mid = (begin + final) / 2;
			  key_value read_value;
			  lump.read(read_value, (mid * sizeof(key_value)));
			  int result = std::strcmp(read_value.name, date.name);
			  if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
				begin = mid + 1;
			  } else {
				final = mid;
			  }
			}
			//如果放在末尾，但是下一个节点开头仍是这个名字
			if (final == node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			  if (std::strcmp(node_right.head_name, date.name) == 0) {
				key_value right_head;
				lump.read(right_head, node_right.lump_head);
				if (right_head.value > date.value) {
				  lump.update(date, final * sizeof(lump_size));
				  node_left.sizeof_lump++;
				  link.update(node_left, rear);
				  crack(node_left,rear);
				}
			  }
			  //如果该元素的值居然比下一个块的开头的值大,只能继续
			} else {
			  lump.Insult(date,
						  final * sizeof(key_value),
						  node_left.lump_head + node_left.sizeof_lump * sizeof(key_value));
			  //按理说不可能插到头部了
			  /* if(final==k*lump_size){
				copy(node_left.head_name,date.name);
			  }*/
			  node_left.sizeof_lump++;
			  link.update(node_left, rear);
			  crack(node_left,rear);
			}
		  }
		  if (std::strcmp(node_left.head_name, date.name) > 0) {
			break;
		  }
		  rear = node_left.next_index;
		}
		//遍历到了结尾,特判
		if (rear == tail) {
		  node_left = node_right;
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			int result = std::strcmp(read_value.name, date.name);
			if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  lump.Insult(date, final * sizeof(key_value), node_left.lump_head + node_left.sizeof_lump * sizeof(key_value));
		  //按理说不可能插到头部了
		  /* if(final==k*lump_size){
			copy(node_left.head_name,date.name);
		  }*/
		  node_left.sizeof_lump++;
		  link.update(node_left, rear);
		  //crack(node_left,rear);
		}
	  }
	}
  }
  void crack(Node node, int rear) {
	if (node.sizeof_lump >= max_number) {
	  int head, tail;
	  link.get_info(head, 2);
	  link.get_info(tail, 3);
	  int new_tail = new_node(rear, node.next_index);
	  Node new_node;
	  link.read(new_node, new_tail);
	  int begin = new_node.lump_head;
	  int transfer_begin = node.lump_head;
	  int half = node.sizeof_lump / 2 ;
	  while (half < node.sizeof_lump) {
		key_value transfer;
		lump.read(transfer, transfer_begin + half * sizeof(key_value));
		if(half==node.sizeof_lump/2){
		  copy(new_node.head_name,transfer.name);
		}
		lump.update(transfer, begin + half * sizeof(key_value));
	  }
	  new_node.sizeof_lump = node.sizeof_lump - node.lump_head/2;
	  node.sizeof_lump = node.lump_head/2;
	  link.update(node, rear);
	  link.update(new_node,new_tail);
	}
  }

  void erase(key_value date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	if (head == -1) {
	  return;
	}
	int rear = head;
	Node node_left, node_right;
	link.read(node_right, rear);
	if (std::strcmp(node_right.head_name, date.name) > 0) {
	  return;
	} else {
	  while (rear != tail) {
		node_left = node_right;
		link.read(node_right, node_left.next_index);

		if (std::strcmp(node_left.head_name, date.name) <= 0 && std::strcmp(node_right.head_name, date.name) >= 0) {
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			int result = std::strcmp(read_value.name, date.name);
			if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  if (final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			key_value read_date;
			lump.read(read_date, final * sizeof(key_value));
			if (std::strcmp(read_date.name, date.name) == 0 && read_date.value == date.value) {

			  //删除节点
			  lump.Delete(final * sizeof(key_value), node_left.lump_head + lump_size * sizeof(key_value));
			  node_left.sizeof_lump--;
			  if (node_left.sizeof_lump == 0) {
				if (rear == head) {
				  head = node_left.next_index;
				  link.write_info(head, 2);
				} else {
				  Node node_pre;
				  link.read(node_pre, node_left.pre_index);
				  node_pre.next_index = node_left.next_index;
				  node_right.pre_index = node_left.pre_index;
				  link.update(node_pre, node_left.pre_index);
				  link.update(node_right, node_left.next_index);
				}
			  } else {
				if (final * sizeof(key_value) == node_left.lump_head) {
				  lump.read(read_date, node_left.lump_head);
				  copy(node_left.head_name, read_date.name);
				  link.update(node_left, rear);
				}
			  }

			}
		  }
		}
		if (std::strcmp(node_left.head_name, date.name) > 0) {
		  break;
		}
		rear = node_left.next_index;
	  }
	  if (rear == tail) {
		node_left = node_right;
		int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		while (begin < final) {
		  int mid = (begin + final) / 2;
		  key_value read_value;
		  lump.read(read_value, (mid * sizeof(key_value)));
		  int result = std::strcmp(read_value.name, date.name);
		  if (result < 0 || ((result == 0) && (read_value.value < date.value))) {
			begin = mid + 1;
		  } else {
			final = mid;
		  }
		}
		if (final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
		  key_value read_date;
		  lump.read(read_date, final * sizeof(key_value));
		  if (std::strcmp(read_date.name, date.name) == 0 && read_date.value == date.value) {
			lump.Delete(final * sizeof(key_value), node_left.lump_head + lump_size * sizeof(key_value));
			node_left.sizeof_lump--;
			if (node_left.sizeof_lump == 0) {
			  Node node_pre;
			  link.read(node_pre, node_left.pre_index);
			  node_pre.next_index = -1;
			  link.update(node_pre, node_left.pre_index);
			} else {
			  if (final * sizeof(key_value) == node_left.lump_head) {
				lump.read(read_date, node_left.lump_head);
				copy(node_left.head_name, read_date.name);
				link.update(node_left, rear);
			  }
			}
		  }
		}
	  }

	}
  }

  void find(const string &date) {
	int head, tail;
	link.get_info(head, 2);
	link.get_info(tail, 3);
	if (head == -1) {
	  return;
	}
	int rear = head;
	Node node_left, node_right;
	link.read(node_right, rear);
	if (node_right.head_name > date) {
	  std::cout << "null" << '\n';
	  return;
	} else {
	  bool flag = false;
	  while (rear != tail) {
		node_left = node_right;
		link.read(node_right, node_left.next_index);

		if (node_left.head_name <= date && node_right.head_name >= date) {
		  int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		  while (begin < final) {
			int mid = (begin + final) / 2;
			key_value read_value;
			lump.read(read_value, (mid * sizeof(key_value)));
			if (read_value.name < date) {
			  begin = mid + 1;
			} else {
			  final = mid;
			}
		  }
		  key_value read_value;
		  lump.read(read_value, final * sizeof(key_value));
		  while (read_value.name == date && final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
			flag = true;
			std::cout << read_value.value << ' ';
			final++;
		  }
		}
		if (node_left.head_name > date) {
		  break;
		}
		rear = node_left.next_index;
	  }
	  if (rear == tail) {
		node_left=node_right;
		int begin = node_left.lump_head / sizeof(key_value), final = begin + node_left.sizeof_lump;
		while (begin < final) {
		  int mid = (begin + final) / 2;
		  key_value read_value;
		  lump.read(read_value, (mid * sizeof(key_value)));
		  if (read_value.name < date) {
			begin = mid + 1;
		  } else {
			final = mid;
		  }
		}
		key_value read_value;
		lump.read(read_value, final * sizeof(key_value));
		while (read_value.name == date && final != node_left.lump_head / sizeof(key_value) + node_left.sizeof_lump) {
		  flag = true;
		  std::cout << read_value.value << ' ';
		  final++;
		  lump.read(read_value, final * sizeof(key_value));
		}
	  }
	  if (!flag) {
		std::cout << "null" << '\n';
	  } else {
		std::cout << '\n';
	  }
	}
  }

 private:
  MemoryRiver<Node, 3> link;
  //第一个是元素个数?，第二个是头节点,第三个尾节点
  MemoryRiver<key_value, 0> lump;
  const int max_number = 200;
  const int lump_size = 300;
};

#endif //BOOKSTORE__STORE_HPP_

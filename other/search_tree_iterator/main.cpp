// Дано дерево поиска: каждый элемент дерева содержит кроме значения три указателя:
// на левый, правый и родительский элементы, только у корневого элемента указатель на родительский элемент может быть nullptr.
// Дерево построено с использованием оператора less, нужно проверить его на корректность.
//  а. Структура дерева (Дополнительная задача)
//  б. Корректность содержимого (Основная задача)
// Для простоты считаем, что элементы не повторяются.
// Реализовать функцию проверки, которая принимает в качестве аргумента указатель на корень дерева элементов типа int.
//     Функция возвращает false в случае обнаружения нарушений целостности дерева и true в противном случае.
// Предположим, Ваше решение будет использоваться сторонними разработчиками.

//         7
//     4        12
//  1     6  10    13
//0   2 5

//       10
//    5       12
//  1   7   6    13

#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>
#include <iterator>
#include <optional>

struct Node {
	Node *up;
	Node *left;
	Node *right;

	int value;
};

struct TreeUtil {
	// шагаем влево, пока нода не окажется листом
	static Node *stepLeft(Node *cur)
	{
		while (cur && cur->left) {
			cur = cur->left;
		}

		return cur;
	}

	// шагаем вверх, пока не окажемся левым поддеревом
	static Node *stepUp(Node *cur)
	{
		assert(cur);

		Node *prev;
		do {
			prev = cur;
			cur = cur->up;
		} while (cur && cur->left != prev);

		return cur;
	}

	static void replaceChild(Node *parent, Node *child, Node *new_child)
	{
		if (parent->left == child) {
			parent->left = new_child;
		} else if (parent->right == child) {
			parent->right = new_child;
		} else {
			assert(false && "Node is not child of parent");
		}

		if (new_child) {
			new_child->up = parent;
		}
	}
};

struct SearchTreeUtil {
	static Node *next(Node *node)
	{
		// Шагнуть вправо
		//  искать левый лист
		// иначе
		//  шагать вверх пока не станем левым поддеревом
		assert(node);
		if (node->right) {
			node = TreeUtil::stepLeft(node->right);
		} else {
			node = TreeUtil::stepUp(node);
		}
		return node;
	}

	static Node *extract(Node *root, Node *node)
	{
		struct NodeFinalizer {
			Node *node;

			~NodeFinalizer()
			{
				node->left = nullptr;
				node->right = nullptr;
			}
		};
		NodeFinalizer finalizer{node};

		// Нода является листом, просто ее удаляем
		if (!node->left && !node->right) {
			// Нода является вершиной
			if (!node->up) {
				return nullptr;
			}

			TreeUtil::replaceChild(node->up, node, node->right);
			return root;
		}

		if (node->left && node->right) {
			Node *successor = TreeUtil::stepLeft(node->right);
			assert(!successor->left);

			// Если преемник не является потомком удаляемого узла, то отсоединяем его от других узлов
			if (successor != node->right) {
				TreeUtil::replaceChild(successor->up, successor, successor->right);
				TreeUtil::replaceChild(successor, successor->right, node->right);
			}

			// Заменяем удаляемый узел на преемника
			TreeUtil::replaceChild(successor, successor->left, node->left);
			if (node->up) {
				TreeUtil::replaceChild(node->up, node, successor);
			} else {
				root = successor;
			}

			return root;
		}


		Node *child = node->left ? node->left : node->right;
		if (!node->up) {
			child->up = nullptr;
			return child;
		}

		TreeUtil::replaceChild(node->up, node, child);
		return root;
	}
};

struct TreeIterator : std::forward_iterator_tag {
	using difference_type = std::ptrdiff_t;
	using value_type = int;

	TreeIterator() = default;

	explicit TreeIterator(Node *node) : _node(node){}

	Node *node() const
	{
		return _node;
	}

	int operator*() const
	{
		return _node->value;
	}

	TreeIterator &operator++()
	{
		_node = SearchTreeUtil::next(_node);
		return *this;
	}

	TreeIterator operator++(int)
	{
		auto tmp = *this;
		++*this;
		return tmp;
	}

	bool operator==(const TreeIterator &other) const
	{
		return this->_node == other._node;
	}

private:
	Node *_node;

	void step(Node *next, Node **prev)
	{
		*prev = _node;
		_node = next;
	}
};

static_assert(std::forward_iterator<TreeIterator>);

class Tree {
public:
	using iterator = TreeIterator;

	explicit Tree(Node *root) : _root(root) {}

	Tree(const Tree &other) = delete;
	Tree& operator=(const Tree &other) = delete;

	Tree(Tree &&other) noexcept
	{
		_root = other._root;
		other._root = nullptr;
	}

	Tree& operator=(Tree &&other) noexcept
	{
		_root = other._root;
		other._root = nullptr;
		return *this;
	}

	~Tree()
	{
		iterator it = begin();
		while (it != end()) {
			it = erase(it);
		}
	}

	iterator begin() const
	{
		// Находим самый левый элемент (_root, если левая ветвь пустая)
		// Если _root == nullptr, в iterator передастся nullptr
		return iterator(TreeUtil::stepLeft(_root));
	}

	iterator end() const
	{
		return iterator(nullptr);
	}

	iterator erase(iterator it)
	{
		Node *node = it.node();
		if (!node) return iterator(nullptr);
		assert(node->up || _root == node);

		iterator next = it;
		++next;

		_root = SearchTreeUtil::extract(_root, node);
		delete node;

		return next;
	}

private:
	Node *_root;
};

inline constexpr std::nullopt_t X = std::nullopt;

struct NodeBuildInfo : std::optional<int> {
	using std::optional<int>::optional;
};

std::ostream &operator<<(std::ostream &os, const NodeBuildInfo &info)
{
	if (info.has_value()) {
		os << info.value();
	} else {
		os << "X";
	}
	return os;
}

/**
 * Создает дерево из вектора информации о топологии дерева.
 *
 * @param nodes содержит информацию о топологии дерева. Индекс вектора указывает на расположение элемента в дереве:
 *		    0 - вершина дерева
 *       1,   2 - левый и правый потомки вершины
 *     3, 4, 5, 6 - и т.д.
 */
std::optional<Tree> build_tree(const std::vector<NodeBuildInfo> &nodes)
{
	if (nodes.empty() || !nodes[0].has_value()) {
		return Tree{nullptr};
	}

	// Создаем вектор указателей на узлы
	std::vector<Node*> tree_nodes(nodes.size(), nullptr);
	tree_nodes[0] = new Node{nullptr, nullptr, nullptr, nodes[0].value()};

	// Создаем узлы для всех существующих значений
	for (size_t i = 1; i < nodes.size(); ++i) {
		if (nodes[i].has_value()) {
			size_t up_idx = (i - 1) / 2;
			bool is_left = (i % 2) == 1;

			Node *up_node = tree_nodes[up_idx];
			if (!up_node) {
				std::cout << "build tree failed: up_idx = " << up_idx << ", i = " << i << ", nodes = {";
				std::copy(std::begin(nodes), std::end(nodes), std::ostream_iterator<NodeBuildInfo>(std::cout, ", "));
				std::cout << "}" << std::endl;

				// Создаем дерево для деаллокации нод
				if (up_idx) Tree{tree_nodes[0]};

				return std::nullopt;
			}

			tree_nodes[i] = new Node{up_node, nullptr, nullptr, nodes[i].value()};
			if (is_left) {
				up_node->left = tree_nodes[i];
			} else {
				up_node->right = tree_nodes[i];
			}
		}
	}

	return Tree{tree_nodes[0]};
}

void test(const std::vector<NodeBuildInfo> &nodes, bool expect_is_valid_search_tree)
{
	std::optional<Tree> search_tree = build_tree(nodes);
	if (!search_tree) {
		return;
	}

	bool is_valid_search_tree = std::ranges::is_sorted(*search_tree);

	if (is_valid_search_tree != expect_is_valid_search_tree) {
		std::cout << "test failed: is_valid_search_tree = " << is_valid_search_tree
				<< ", expected = " << expect_is_valid_search_tree << ", nodes = {";
		std::copy(std::begin(nodes), std::end(nodes), std::ostream_iterator<NodeBuildInfo>(std::cout, ", "));
		std::cout << "}" << std::endl;
	} else {
		// std::cout << "test passed: is_valid_search_tree = " << is_valid_search_tree
		// 		<< ", expected = " << expect_is_valid_search_tree << ", nodes = {";
		// std::copy(std::begin(nodes), std::end(nodes), std::ostream_iterator<NodeBuildInfo>(std::cout, ", "));
		// std::cout << "}" << std::endl;
	}
}

int main()
{
	test({ }, true);
	test({ 1 }, true);
	test({1, 2}, false);
	test({2, 1}, true);
	test({2, 1, 3}, true);
	test({2, X, 3}, true);
	test({2, 3, X}, false);
}

/**
 * @file code.cpp
 * @author Meesum Ali Qazalbash (mesumali26.ma@gmail.com)
 * @brief Huffman Coder class implementation file
 * @version 0.1
 * @date 2023-03-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE

#include <execution>  // for parallel for_each loop
#include <fstream>    // for file operations
#include <iostream>   // for input/output operations
#include <map>        // for map
#include <queue>      // for priority queue
#include <set>        // for set

#define MULTITHREAD 1  // enable/disable multithreading

/**
 * @brief Node of the Huffman tree
 *
 */
class Node {
public:

    char     value;
    uint32_t frequency;
    Node    *left, *right;

    /**
     * @brief Construct a new Node object
     *
     */
    Node() = default;

    /**
     * @brief Construct a new Node object
     *
     * @param v
     * @param f
     */
    Node(char v, uint32_t f) : value(v), frequency(f), left(nullptr), right(nullptr){};

    /**
     * @brief Construct a new Node object
     *
     * @param v
     * @param f
     * @param l
     * @param r
     */
    Node(char v, uint32_t f, Node *l, Node *r) : value(v), frequency(f), left(l), right(r){};

    /**
     * @brief Less than operator
     *
     * @param n
     * @return true
     * @return false
     */
    bool operator<(const Node &n) const { return this->frequency > n.frequency; }
};

/**
 * @brief Huffman Coder class
 *
 */
class Huffman_Coder {
public:

    /**
     * @brief Construct a new Huffman Coder object
     *
     * @param filename
     */
    Huffman_Coder(const std::string &filename) {
        std::string text = read_file(filename);
        this->root       = huffman_tree(text);
        this->mapping    = generate_codes();
    }

    /**
     * @brief output stream operator
     *
     * @param out
     * @param tree
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &out, const Huffman_Coder &tree);

    /**
     * @brief Save the mapping to a file
     *
     * @param filename
     */
    void save(const std::string filename) {
        std::ofstream MyWriteFile(filename);
        for (const std::pair<char, std::string> i : this->mapping) {
            MyWriteFile << i.first << " " << i.second << std::endl;
        }
        MyWriteFile.close();
    }

private:

    Node *root;  // root of the Huffman tree

    std::map<char, std::string> mapping;  // mapping of characters to their codes

    /**
     * @brief Count the frequency of each character in the text
     *
     * @param text
     * @return std::map<uint32_t, std::set<char>>
     */
    std::map<uint32_t, std::set<char>> counter_bucket(const std::string &text) {
        std::map<char, uint32_t>           count;   // count of each character
        std::map<uint32_t, std::set<char>> bucket;  // bucket of characters

#if MULTITHREAD

        // parallel for_each loop to count the frequency of each character
        std::for_each(std::execution::par, text.begin(), text.end(), [&count](const char i) {
            if (i >= 0) {
                count[i] = count[i] + 1U;
            }
        });

        // parallel for_each loop to insert the characters into the buckets of
        // their frequencies
        std::for_each(std::execution::par, count.begin(), count.end(),
                      [&bucket](const std::pair<char, uint32_t> i) { bucket[i.second].insert(i.first); });

#else

        // sequential for loop to count the frequency of each character
        for (const char i : text) {
            if (i >= 0) {
                count[i] = count[i] + 1;
            }
        }

        // sequential for loop to insert the characters into the buckets of
        // their frequencies
        for (const std::pair<char, uint32_t> i : count) {
            bucket[i.second].insert(i.first);
        }
#endif

        return bucket;
    }

    /**
     * @brief Generate the Huffman tree
     *
     * @param text
     * @return Node*
     */
    Node *huffman_tree(const std::string &text) {
        // bucket of characters with their frequencies
        const std::map<uint32_t, std::set<char>> bucket = counter_bucket(text);
        // priority queue of nodes
        std::priority_queue<Node> queue;

#if MULTITHREAD

        // parallel for_each loop to insert the characters into the priority
        // queue
        std::for_each(std::execution::par, bucket.begin(), bucket.end(),
                      [&queue](const std::pair<uint32_t, std::set<char>> i) {
                          for (const char j : i.second) {
                              queue.push(Node(j, i.first));
                          }
                      });

#else

        // sequential for loop to insert the characters into the priority queue
        for (const std::pair<uint32_t, std::set<char>> i : bucket) {
            for (const char j : i.second) {
                queue.push(Node(j, i.first));
            }
        }

#endif

        // generate the Huffman tree
        while (queue.size() > 1) {
            // create a new node with the top of the priority queue
            Node *left = new Node(queue.top());
            queue.pop();
            // create a new node with the top of the priority queue
            Node *right = new Node(queue.top());
            queue.pop();

            // insert the new node into the priority queue with the sum of the
            // two nodes frequencies as the new frequency of the node
            queue.push(Node('\0', left->frequency + right->frequency, left, right));
        }

        Node *root = new Node(queue.top());  // root of the Huffman tree
        return root;
    }

    /**
     * @brief Generate the mapping of characters to their codes
     *
     * @return std::map<char, std::string>
     */
    std::map<char, std::string> generate_codes() {
        // mapping of characters to their codes
        std::map<char, std::string> code;
        // queue of nodes and their codes
        std::queue<std::pair<Node *, std::string>> queue;
        queue.push(std::make_pair(root, ""));

        Node       *node = nullptr;  // node of the Huffman tree
        std::string prefix;          // code of the node

        // generate the mapping of characters to their codes
        while (!queue.empty()) {
            // get the node and its code
            node   = queue.front().first;
            prefix = queue.front().second;
            queue.pop();

            // if the node is a leaf, insert the character and its code into the
            // mapping else, insert the left and right nodes into the queue with
            // a 0 or 1 as their code respectively
            if (node->left == nullptr && node->right == nullptr) {
                code[node->value] = prefix;
            } else {
                queue.push(std::make_pair(node->left, prefix + "0"));
                queue.push(std::make_pair(node->right, prefix + "1"));
            }
        }
        return code;
    }

    /**
     * @brief Read a file
     *
     * @param filename
     * @return std::string
     */
    const std::string read_file(const std::string filename) {
        std::string text = "", line;

        std::ifstream MyReadFile(filename);

        while (std::getline(MyReadFile, line)) text += line;

        MyReadFile.close();

        return text;
    }
};

/**
 * @brief Print the Huffman tree
 *
 * @param prefix
 * @param node
 * @param is_left
 */
void print(const std::string &prefix, const Node *const node, const bool is_left) {
    if (node == nullptr) {
        return;
    }

    if (node->right) {
        print(prefix + (is_left ? "│   " : "    "), node->right, false);
    }

    std::cout << prefix + (is_left ? "└── " : "┌── ") + node->value + "\n";

    if (node->left) {
        print(prefix + (is_left ? "    " : "│   "), node->left, true);
    }
}

/**
 * @brief Print the Huffman tree
 *
 * @param out
 * @param tree
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &out, const Huffman_Coder &tree) {
    print("", tree.root, false);
    return out;
}

/**
 * @brief main function
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./code <input-filename> <code-filename>" << std::endl;
        return 1;
    }

    if (argv[1] == argv[2]) {
        std::cout << "Input and output file cannot be the same" << std::endl;
        return 1;
    }

    Huffman_Coder tree(argv[1]);

    tree.save(argv[2]);

    std::cout << tree << std::endl;

    return 0;
}

#endif  // HUFFMAN_CODER

/* *
 * Шаблон с ?
 * Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
 * Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает,
 * что все обычные символы совпадают с соответствующими из текста, а вместо символа “?” в тексте
 * встречается произвольный символ. Гарантируется, что сам “?” в тексте не встречается.
 * Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в
 * исходном тексте. m ≤ 5000, n ≤ 2000000.
 * */


#include <iostream>
#include <map>
#include <memory>
#include <vector>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    bool is_terminal = false;
    std::shared_ptr<Node> parent, link;
    char toParent;
    std::map<char, std::shared_ptr<Node>> edges;
    std::vector<int> indices;
};

bool equals(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right)
{
    if(left == right) {
        return true;
    }
    if(left && right) {
        return std::tie(left->go, left->is_terminal, left->toParent, left->edges) ==
               std::tie(right->go, right->is_terminal, right->toParent, right->edges);
    }
    return false;
}


class Trie {
public:
    Trie();
    Trie(const Trie&) = delete;
    Trie(Trie&&) = delete;
    Trie& operator=(const Trie&) = delete;
    Trie& operator=(Trie&&) = delete;
    ~Trie() = default;

    bool Add(const std::string& key, int index);
    void Process(const std::string &text, const std::string &pattern);

private:
    std::shared_ptr<Node> root;
    std::shared_ptr<Node> GetLink(const std::shared_ptr<Node> &v);
    std::shared_ptr<Node> Go(const std::shared_ptr<Node> &v, const char symbol);
    std::vector<std::pair<int, int> > GetPartitions(const std::string &pattern);

    static void print(const std::shared_ptr<Node>& node, const std::string& current);

    static std::pair<bool, bool> remove(
            std::shared_ptr<Node>& node, const std::string& key, int current_index);
};

Trie::Trie() {
    root = std::make_shared<Node>();
}

bool Trie::Add(const std::string& key, const int index) {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            std::shared_ptr<Node> tmp = current->go[symbol] = std::make_shared<Node>();
            tmp->parent = current;
            tmp->toParent = symbol;
            tmp->link = nullptr;
            current = tmp;
        } else {
            current = next->second;
        }
    }
    current->indices.push_back(index);
    current->is_terminal = true;
    return true;
}

std::shared_ptr<Node> Trie::GetLink(const std::shared_ptr<Node> &v) {
    if(v->link == nullptr) {
        if(equals(v, root) || equals(v->parent, root)) {
            v->link = root;
        } else {
            v->link = Go( GetLink(v->parent), v->toParent );
        }
    }
    return v->link;
}

std::shared_ptr<Node> Trie::Go(const std::shared_ptr<Node> &v, const char symbol) {
    auto edge = v->edges.find(symbol);
    if(edge == v->edges.end()) {
        auto next = v->go.find(symbol);
        if(next != v->go.end()) {
            v->edges[symbol] = v->go[symbol];
        } else {
            v->edges[symbol] = equals(v, root) ? root : Go(GetLink(v), symbol);
        }
    }
    return v->edges[symbol];
}

std::pair<bool, bool> Trie::remove(
        std::shared_ptr<Node>& node, const std::string& key, int current_index) {
    if (current_index == key.length()) {
        if (!node->is_terminal) return std::make_pair(false, false);
        node->is_terminal = false;
        return std::make_pair(true, node->go.empty());
    }

    auto next = node->go.find(key[current_index]);
    if (next == node->go.end()) {
        return std::make_pair(false, false);
    }

    auto result = remove(next->second, key, current_index + 1);

    if (!result.first) {
        return result;
    }
    if (!result.second) {
        return result;
    }
    node->go.erase(key[current_index]);
    return std::make_pair(true, !node->is_terminal && node->go.empty());
}

void Trie::print(const std::shared_ptr<Node>& node, const std::string& current) {
    if (node->is_terminal) {
        std::cout << current << std::endl;
    }
    for (const auto go : node->go) {
        print(go.second, current + go.first);
    }
}

std::vector<std::pair<int, int> > Trie::GetPartitions(const std::string &pattern) {
    std::vector<std::pair<int, int> > result;
    std::pair<int, int> now;

    if (pattern[0] != '?') {
        now.first = 0;
        if(pattern[1] == '?') {
            now.second = 0;
            result.push_back(now);
        }
    }

    for (int i = 1; i < pattern.length() - 1; i++) {
        if (pattern[i - 1] == '?' && pattern[i] != '?') {
            now.first = i;
        }
        if (pattern[i + 1] == '?' && pattern[i] != '?') {
            now.second = i;
            result.push_back(now);
        }
    }

    if (pattern[pattern.length() - 2] == '?' && pattern[pattern.length() - 1] != '?') {
        now.first = pattern.length() - 1;
    }
    if (pattern[pattern.length() - 1] != '?') {
        now.second = pattern.length() - 1;
        result.push_back(now);
    }

    return result;
}

void Trie::Process(const std::string &text, const std::string &pattern) {
    std::vector<std::pair<int, int> > partitions = GetPartitions(pattern);
    int index = 0;
    for(auto p : partitions) {
        Add(pattern.substr(p.first, p.second - p.first + 1), index++);
    }
    std::vector<int> entries(text.length());
    std::shared_ptr<Node> v = root;

    for (int i = 0; i < text.length(); i++) {
        v = Go(v, text[i]);
        std::shared_ptr<Node> u = v;
        do {
            if (u->is_terminal) {
                for (auto indice : u->indices) {
                    int startIndex = i - partitions[indice].second + partitions[indice].first;

                    if ((startIndex - partitions[indice].first >= 0) &&  (startIndex - partitions[indice].first + pattern.length() - 1 < text.length())) {
                        entries[startIndex - partitions[indice].first]++;
                    }
                }
            }
            u = GetLink(u);
        } while ( !equals(u, root) );
    }

    std::vector<int> result;
    for (int i = 0; i < int(entries.size()) - int(pattern.size())+1; i++) {
        if (entries[i] == partitions.size()) {
            result.push_back(i);
            std::cout << i << ' ';
        }
    }
}

int main() {
    Trie trie;
    std::string pattern, s;
    std::cin >> pattern >> s;
    trie.Process(s, pattern);
    return 0;
}
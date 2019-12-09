/* Алгоритм сжатия данных Хаффмана
 * Реализуйте алгоритм построения оптимального префиксного кода Хаффмана.
 * При помощи алгоритма реализуйте две функции для создания архива из одного файла и извлечения файла из архива.
 * */

#include "Huffman.h"
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>


class BitsWriter {
 public:
  void WriteBit(bool bit);
  void WriteByte(unsigned char byte);

  std::vector<unsigned char> GetResult();

 private:
  std::vector<unsigned char> buffer_;
  unsigned char accumulator_ = 0;
  int bits_count_ = 0;
};

class BitsReader {
 public:
  bool ReadBit(bool &res);
  BitsReader(IInputStream& stream) {
    byte a;
      while (stream.Read(a)) {
          buffer_.push(a);
      }
  }

 private:
  std::queue<byte> buffer_;
  int bits_count_ = 0;
};

bool BitsReader::ReadBit(bool &res) {
    if (buffer_.empty())
        return false;
    res = (buffer_.front() & (1<<bits_count_++)) > 0;
    if (bits_count_ == 8) {
        bits_count_ = 0;
        buffer_.pop();
    }
    return true;
}

void BitsWriter::WriteBit(bool bit) {
  // Ставим бит в аккумулятор на нужное место
  accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
  ++bits_count_;
  if (bits_count_ == 8) {
    bits_count_ = 0;
    buffer_.push_back(accumulator_);
    accumulator_ = 0;
  }
}

void BitsWriter::WriteByte(unsigned char byte) {
  if (bits_count_ == 0) {
    buffer_.push_back(byte);
  } else {
    accumulator_ |= byte << bits_count_;
    buffer_.push_back(accumulator_);
    accumulator_ = byte >> (8 - bits_count_);
  }
}

std::vector<unsigned char> BitsWriter::GetResult() {
  if (bits_count_ != 0) {
    // Добавляем в буфер аккумулятор, если в нем что-то есть.
    buffer_.push_back(accumulator_);
  }
  return std::move(buffer_);
}

using namespace std;

typedef struct node {
  struct node* left;
  struct node* right;
  long long quantity;
  byte value;
  vector<bool> code;
  node() {
      left = nullptr;
      right = nullptr;
  }
  node(byte val, long long quantity) : quantity(quantity) {
    value = val;
    left = nullptr;
    right = nullptr;
  }
  bool operator<(const node &right)  const {
    return quantity > right.quantity;
  }
} node;

bool Compare(node* a, node* b)
{
    return a->quantity > b->quantity;
}

node* join_nodes(node* a, node* b) {
  node* res = new node();
  res->quantity = a->quantity + b->quantity;
  res->left = a;
  res->right = b;
  return res;
}

void encode_tree(node* root, vector<bool>& code, map<byte, vector<bool> > &revert) {
    if (!root->left && !root->right) {
        revert[root->value] = code;
    }
    if (root->left) {
        code.push_back(false);
        encode_tree(root->left, code, revert);
        code.pop_back();
    }
    if (root->right) {
        code.push_back(true);
        encode_tree(root->right, code, revert);
        code.pop_back();
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
  map<byte, long long> quantity;
  vector<byte> bin;
  byte val;
  while (original.Read(val)) {
    quantity[val]++;
    bin.push_back(val);
  }
  priority_queue<node*, vector<node*>, function<bool(node*, node*)> > res(Compare);
  for (auto it = quantity.begin(); it != quantity.end(); ++it) {
    res.push(new node(it->first, it->second));
  }
  
  while(res.size() > 1) {
    node* now1 = res.top(); res.pop(); 
    node* now2 = res.top(); res.pop();
    res.push(join_nodes(now1, now2));
  }
  node* root = res.top();
    
    vector<bool> code;
    map<byte, vector<bool> > revert;
  
    encode_tree(root, code, revert);
  
    unsigned int num_of_symbols = bin.size();
    compressed.Write((byte)(num_of_symbols));
    compressed.Write((byte)(num_of_symbols >> 8));
    compressed.Write((byte)(num_of_symbols >> 16));
    compressed.Write((byte)(num_of_symbols >> 24));
    
    compressed.Write((byte)(revert.size()-1));
    for (auto it=revert.begin(); it != revert.end(); ++it) {
        BitsWriter tmp;
        vector<bool> now = it->second;
        tmp.WriteByte(it->first);
        tmp.WriteByte((byte)(now.size() - 1));
        for (int i = 0; i < now.size(); i++)
            tmp.WriteBit(now[i]);
        vector<unsigned char> now_res = tmp.GetResult();
        for (int i = 0; i < now_res.size(); i++) {
            compressed.Write((byte)(now_res[i]));
        }
    }
    BitsWriter bw;
    for (int i = 0; i < bin.size(); i++) {
        for(int k = 0; k < revert[bin[i]].size(); k++)
            bw.WriteBit(revert[bin[i]][k]);
    }
    vector<unsigned char> result = bw.GetResult();
    for (int i = 0; i < result.size(); i++) {
        compressed.Write((byte)(result[i]));
    }
}
void Decode(IInputStream& compressed, IOutputStream& original) {
    byte tmp;
    
    compressed.Read(tmp);
    unsigned int num_of_symbols = 0;
    num_of_symbols |= tmp;
    compressed.Read(tmp);
    num_of_symbols |= tmp << 8;
    compressed.Read(tmp);
    num_of_symbols |= tmp << 16;
    compressed.Read(tmp);
    num_of_symbols |= tmp << 24;
    
    compressed.Read(tmp);
    int n = tmp; n++;
    map<vector<bool>, byte> direct;
    for (int i = 0; i < n; i++) {
        byte encoded; compressed.Read(encoded);
        compressed.Read(tmp);
        int len = tmp; len++;
        vector<bool> code;
        for (int k = 0; k < len/8; k++) {
            byte now; compressed.Read(now);
            for (int j = 0; j < 8; j++) {
                code.push_back(now & 1);
                now >>= 1;
            }
        }
        if (len % 8) {
            byte now; compressed.Read(now);
            for (int j = 0; j < len%8; j++) {
                code.push_back(now & 1);
                now >>= 1;
            }
        }
        direct[code] = encoded;
    }
    BitsReader br(compressed);
    
    vector<bool> now;
    while (num_of_symbols) {
        bool readed;
        if (!br.ReadBit(readed))
            break;
        now.push_back(readed);
        if (direct.find(now) != direct.end()) {
            num_of_symbols--;
            original.Write(direct[now]);
            now.resize(0);
        }
    }
}

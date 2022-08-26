#include <queue>
#include <string>

struct HuffManTreeNode {
    char c;
    int freq;
    std::string code;
    struct HuffManTreeNode *l, *r;

    HuffManTreeNode(){
        freq = 0;
        l = nullptr;
        r = nullptr;
    };

    HuffManTreeNode(char symbol, int frequency, struct HuffManTreeNode * left = nullptr, struct HuffManTreeNode * right = nullptr, std::string huffCode = ""):
        c(symbol),
        freq(frequency),
        l(left),
        r(right),
        code(huffCode){};
};

typedef HuffManTreeNode* HuffNodePtr;

class HuffManTree {
private:
    struct LessThanByFreq {
        bool operator()(const HuffNodePtr& lhs, const HuffNodePtr& rhs) const {
            return lhs->freq > rhs->freq;
        }
    };
    std::priority_queue<HuffNodePtr, std::vector<HuffNodePtr>, LessThanByFreq> pq;
    std::vector<std::string> codeDict;
    std::vector<int> freqVector;
    HuffNodePtr root;
    void BuildPriorityQueue(std::ifstream &inFile);
    void UpdateCode(std::priority_queue<HuffNodePtr, std::vector<HuffNodePtr>, LessThanByFreq> pq);
    void UpdateCodeRecur(const HuffNodePtr root, std::string hCode);
    void storeTreeFile(std::ofstream &treeFile);
    void restoreTree(std::ifstream &treeFile);
    std::string getEncodedString(std::ifstream &inFile);
    std::string getDecodedString(std::ifstream &encodedFile);
    bool encodeComplete;
    long long int origLength;
public:
    
    HuffManTree();
    void Encode(std::ifstream &origFile, std::ofstream &encodedFile, std::ofstream &treeFile);
    void Decode(std::ifstream &encodedFile, std::ofstream &newFile, std::ifstream &treeFile);
};

// Translate string into 1 byte integer
// Example: s: "00000010" -> result:0x02
// will cut the string inplace
inline int string2byte(std::string &s, char &result)
{
    if (s.size()<8)
        return -1;
    result = 0;
    std::string head = s.substr(0, 8);
    s.erase(0, 8);
    
    for (auto& c : head) {
        result = result<<1;
        if(c=='1')
            result++;
    }
    return 0;
}

// Append 1 byte integer to string
// Example: bits: 0x02, s:"" -> s:"00000010"
// bits will be modified inplace
// use origLength as stop condition
inline int byte2string(char bits, std::string& result, const HuffNodePtr root, HuffNodePtr& temp, const long long int origLength) {
    for(int i = 0; i < 8; i++) {
        if(!temp || result.size()>=origLength)
            return 0;
        
        bool toRight = bits & (char)0x80;
        temp = toRight ? temp->r : temp->l;
        
        if(!temp->r && !temp->l){
            //leaf node
            result += temp->c;
            if(result.size() >= origLength)
                return 0;
            temp = root;
        }
        bits = bits << 1;
    }
    return 0;
}
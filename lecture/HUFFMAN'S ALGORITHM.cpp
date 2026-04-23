#include <iostream>
#include <vector>
#include <queue>

// 노드(Node) 구조체 정의
struct Node {
    char data;          // 문자 (내부 노드일 경우 임의의 기호)
    int freq;           // 빈도수 (수도코드의 f[...])
    Node* left;         // 왼쪽 자식 (수도코드의 left[...])
    Node* right;        // 오른쪽 자식 (수도코드의 right[...])

    // 생성자
    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// 최소 힙(Min-Heap)을 위한 비교 연산자 구조체
struct Compare {
    bool operator()(Node* l, Node* r) {
        // 빈도수(freq)가 작은 노드가 큐의 top()에 오도록 설정
        return l->freq > r->freq;
    }
};

// 허프만 코드 트리 생성 함수 (수도코드 매핑)
Node* HuffmanCode(std::vector<Node*>& C) {
    // 1. n <- |C|
    int n = C.size(); 
    
    // 2. Q <- C (최소 힙 우선순위 큐 초기화)
    std::priority_queue<Node*, std::vector<Node*>, Compare> Q(C.begin(), C.end());

    // 3. for i = 1 ... n - 1
    for (int i = 1; i < n; ++i) {
        // 4. allocate a new node z
        // (내부 노드는 특정 문자가 없으므로 임의의 문자 '$'와 빈도 0으로 초기화)
        Node* z = new Node('$', 0); 
        
        // 5. left[z] <- Extract-Min(Q)
        z->left = Q.top();
        Q.pop();
        
        // 6. right[z] <- Extract-Min(Q)
        z->right = Q.top();
        Q.pop();
        
        // 7. f[z] <- f[left[z]] + f[right[z]]
        z->freq = z->left->freq + z->right->freq;
        
        // 8. Insert(Q, z)
        Q.push(z);
    }

    // 9. return Extract-Min(Q)
    Node* root = Q.top();
    Q.pop();
    
    return root; // 완성된 트리의 루트 반환
}

// 인코딩
// 1. 허프만 코드 테이블을 생성하는 재귀 함수
void buildCodeTable(Node* root, std::string currentCode, std::unordered_map<char, std::string>& huffmanTable) {
    // 예외 처리: 노드가 비어있으면 종료
    if (root == nullptr) {
        return;
    }

    // 리프 노드에 도달한 경우 (실제 문자가 저장된 노드)
    if (root->left == nullptr && root->right == nullptr) {
        // 테이블에 문자(key)와 그에 해당하는 코드(value)를 저장
        huffmanTable[root->data] = currentCode;
    }

    // 왼쪽 자식으로 내려갈 때는 코드에 '0'을 덧붙임
    buildCodeTable(root->left, currentCode + "0", huffmanTable);
    
    // 오른쪽 자식으로 내려갈 때는 코드에 '1'을 덧붙임
    buildCodeTable(root->right, currentCode + "1", huffmanTable);
}

// 2. 원본 문자열을 인코딩하는 함수
std::string encodeHuffman(const std::string& text, Node* root) {
    // 문자와 코드를 매핑할 해시 테이블(Map) 준비
    std::unordered_map<char, std::string> huffmanTable;
    
    // 루트부터 시작하여 빈 문자열("")로 코드 테이블 생성 시작
    buildCodeTable(root, "", huffmanTable);

    std::string encodedString = "";
    
    // 원본 문자열을 한 글자씩 읽으면서
    for (char ch : text) {
        // 테이블에서 해당 문자의 코드를 찾아 결과 문자열에 이어 붙임
        encodedString += huffmanTable[ch];
    }

    return encodedString; // 최종 인코딩된 문자열 반환
}

// 허프만 디코딩
std::string decodeHuffman(Node* root, const std::string& encodedString) {
    std::string decodedString = "";
    Node* current = root; // 항상 root에서 탐색 시작

    // 인코딩된 문자열(예: "0100011010")을 한 글자씩 읽기
    for (char bit : encodedString) {
        
        // 1. '0'이면 왼쪽 자식으로, '1'이면 오른쪽 자식으로 이동
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        }

        // 2. 리프 노드에 도달했는지 확인 (왼쪽, 오른쪽 자식이 모두 없는 경우)
        if (current->left == nullptr && current->right == nullptr) {
            // 해당 문자를 결과 문자열에 추가
            decodedString += current->data; 
            
            // 3. 다음 문자를 해독하기 위해 다시 root로 돌아감 (자료의 빨간 글씨 부분)
            current = root; 
        }
    }

    return decodedString; // 최종 해독된 문자열 반환
}

// 동적 할당된 노드들의 메모리 누수를 막기 위한 트리 해제 함수
void freeTree(Node* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    // 1. 테스트할 원본 문자열 준비
    std::string text = "huffman coding is fun!";
    std::cout << "[1] 원본 문자열: " << text << "\n\n";

    // 2. 문자별 빈도수 계산 (해시 테이블 활용)
    std::unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // 3. 빈도수를 바탕으로 초기 리프 노드 배열(C) 생성
    std::vector<Node*> C;
    for (auto const& pair : freqMap) {
        C.push_back(new Node(pair.first, pair.second));
    }

    // 4. 허프만 트리 생성
    // 주의: HuffmanCode 함수 내부에서 priority_queue가 C의 노드들을 
    // 조합하여 새로운 트리를 만들고 루트를 반환합니다.
    Node* root = HuffmanCode(C);
    std::cout << "[2] 허프만 트리 생성 완료!\n\n";

    // 5. 인코딩 테스트
    std::string encodedText = encodeHuffman(text, root);
    std::cout << "[3] 인코딩된 이진 문자열:\n" << encodedText << "\n\n";

    // 6. 디코딩 테스트
    std::string decodedText = decodeHuffman(root, encodedText);
    std::cout << "[4] 디코딩된 문자열:\n" << decodedText << "\n\n";

    // 결과 확인
    if (text == decodedText) {
        std::cout << "성공: 원본과 디코딩된 문자열이 완벽히 일치합니다!\n";
    } else {
        std::cout << "실패: 무언가 잘못되었습니다.\n";
    }

    // 7. 사용이 끝난 트리 메모리 해제
    freeTree(root);

    return 0;
}
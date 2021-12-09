#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <chrono>
using namespace std::chrono;
using namespace std;

//declaring the structure of a node
struct Node
{
	char c;
	int fre;
	Node *left_node, *right_node;
};

// To organize the heap, using a comparison object.
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// The node with the highest priority has the lowest frequency.
		return l->fre > r->fre;
	}
};

Node* getNode(char c, int fre, Node* left_node, Node* right_node);
void encode_string(Node* root, string str,
			unordered_map<char, string> &lossless);
void decode_string(Node* root, int &index, string str);   
void buildLosslessCompressionTree(string text);         

// lossless data encoding algorithm
int main()
{
	auto begin = high_resolution_clock::now();
    ifstream f1;
    string str1, str2;
	float time;
     f1.open ("input.txt");
      if (f1.is_open()) {
      while ( getline( f1, str2 ) ) {
         str1 +=str2;
      }
   }
  f1.close();
	
	
	buildLosslessCompressionTree(str1);
  
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - begin);
  time=(float)duration.count();
  cout<<"time taken to run: ";
  cout<<time/1000<<" seconds"<<endl;

  return 0;

	return 0;
}

// Function for allocating a new tree node
Node* getNode(char c, int fre, Node* left_node, Node* right_node)
{
	Node* node = new Node();

	node->c = c;
	node->fre = fre;
	node->left_node = left_node;
	node->right_node = right_node;

	return node;
}

// traverse the Tree and save the Codes to a map

void encode_string(Node* root, string str,
			unordered_map<char, string> &lossless)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left_node && !root->right_node) {
		lossless[root->c] = str;
	}

	encode_string(root->left_node, str + "0", lossless);
	encode_string(root->right_node, str + "1", lossless);
}

// traverse the whole Tree and decode the encoded string
void decode_string(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	// found a leaf node
	if (!root->left_node && !root->right_node)
	{
		cout << root->c;
		return;
	}

	index++;

	if (str[index] =='0')
		decode_string(root->left_node, index, str);
	else
		decode_string(root->right_node, index, str);
}

// Builds lossless compression Tree and decodes the given input string
void buildLosslessCompressionTree(string text)
{
	// Count the number of times each character(frequency) appears and save the data in to a map
	 unordered_map<char, int> fre;

	int i=0;
	while(text[i]!='\0')
	{
			fre[text[i]]+=1;
			i++;
	}

	// Creating a priority queue 'priority' to store the present nodes from the tree;
	priority_queue<Node*, vector<Node*>, comp> priority;

	// Creating a leaf node for each character and pushing it in to the priority queue created earlier.
	for (auto item: fre) {
		priority.push(getNode(item.first, item.second, nullptr, nullptr));
	}
	while (priority.size() != 1)
	{
		// From the queue, remove the two nodes which has the highest priority (lowest frequency).
		Node *left_node = priority.top(); priority.pop();
		Node *right_node = priority.top();	priority.pop();

		//creating a new node, that is created by adding up the frequencies of the earlier popped nodes and add the new node to the existing priority queue.
		int sum = left_node->fre + right_node->fre;
		priority.push(getNode('\0', sum, left_node, right_node));
	}

	// root stores a pointer to root of Huffman Tree i.e top node of the priority queue
	Node* root = priority.top();

	// traverse the Tree and store Codes in a map . Also prints them
	unordered_map<char, string> lossless;
	encode_string(root, "", lossless);

cout << "\nGiven string is :" << text << '\n';
	cout << "Binary Codes for the characters are :\n" << '\n';
	for (auto item: lossless) {
		cout << item.first << " " << item.second << '\n';
	}

	// printing encoded string
	string str = "";
	for(int i=0;text[i]!='\0';i++)
	{
		str=str+lossless[text[i]];
	}
    cout << "";
     ofstream f;
     f.open("output.txt");
  f << str;
  cout<<"written to file";
  f.close();
	cout << "\nEncoded string is :\n" << str << '\n';

}


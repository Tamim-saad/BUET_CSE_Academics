/*
  To Run:
g++ 2005095.cpp -o 2005095
.\2005095
*/

#include <bits/stdc++.h>
using namespace std;

// Define color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

// Structure to hold a single data instance
struct Instance {
  map<string, string> attributes;
  string label;
};

// Function to read data from file
vector<Instance> readData(const string &filename) {
  vector<Instance> data;
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Unable to open file " << filename << endl;
    return data;
  }

  string line;
  int lineNumber = 0;
  int malformedLines = 0;
  while (getline(file, line)) {
    lineNumber++;
    if (line.empty())
      continue;
    stringstream ss(line);
    string token;
    Instance instance;
    // Attributes: buying, maint, doors, persons, lug_boot, safety, label
    vector<string> attrs;
    while (getline(ss, token, ',')) {
      attrs.push_back(token);
    }

    // Ensure the line has exactly 7 tokens
    if (attrs.size() != 7) {
      cerr << "Warning: Malformed line at " << lineNumber << " skipped."
           << endl;
      malformedLines++;
      continue;
    }

    instance.attributes["buying"] = attrs[0];
    instance.attributes["maint"] = attrs[1];
    instance.attributes["doors"] = attrs[2];
    instance.attributes["persons"] = attrs[3];
    instance.attributes["lug_boot"] = attrs[4];
    instance.attributes["safety"] = attrs[5];
    instance.label = attrs[6];
    data.push_back(instance);
  }
  file.close();

  if (malformedLines > 0) {
    cerr << "Total malformed lines skipped: " << malformedLines << endl;
  }

  return data;
}

// Function to calculate entropy
double entropy(const vector<Instance> &data) {
  map<string, int> freq;
  for (const auto &instance : data)
    freq[instance.label]++;
  double ent = 0.0;
  int total = data.size();
  for (const auto &pair : freq) {
    double p = static_cast<double>(pair.second) / total;
    if (p > 0) { // Avoid log2(0)
      ent -= p * log2(p);
    }
  }
  return ent;
}

// Function to calculate Gini impurity
double giniImpurity(const vector<Instance> &data) {
  map<string, int> freq;
  for (const auto &instance : data)
    freq[instance.label]++;
  double gini = 1.0;
  int total = data.size();
  for (const auto &pair : freq) {
    double p = static_cast<double>(pair.second) / total;
    gini -= p * p;
  }
  return gini;
}

// Function to split data based on an attribute
map<string, vector<Instance>> splitData(const vector<Instance> &data,
                                        const string &attribute) {
  map<string, vector<Instance>> splits;
  for (const auto &instance : data) {
    // Use "unknown" if the attribute value is missing
    string attrValue = "unknown";
    if (instance.attributes.find(attribute) != instance.attributes.end()) {
      attrValue = instance.attributes.at(attribute);
    }
    splits[attrValue].push_back(instance);
  }
  return splits;
}

// Structure for Decision Tree Node
struct TreeNode {
  string attribute;
  string label;
  bool isLeaf;
  map<string, TreeNode *> children;

  // Destructor to deallocate child nodes
  ~TreeNode() {
    for (auto &child : children) {
      delete child.second;
    }
  }
};

// Decision Tree Classifier
class DecisionTree {
public:
  string selectionCriteria; // "info_gain" or "gini"
  string selectionStrategy; // "best" or "top3_rand"
  TreeNode *root;
  mt19937 &gen; // Reference to external random number generator

  DecisionTree(const string &criteria, const string &strategy,
               mt19937 &generator)
      : selectionCriteria(criteria), selectionStrategy(strategy), root(nullptr),
        gen(generator) {}

  ~DecisionTree() {
    if (root != nullptr) {
      delete root;
    }
  }

  // Build tree
  void buildTree(const vector<Instance> &data,
                 const vector<string> &attributes) {
    root = buildRecursive(data, attributes);
  }

  // Predict label for a single instance
  string predict(const Instance &instance) const {
    TreeNode *node = root;
    while (!node->isLeaf) {
      string attr = node->attribute;
      if (instance.attributes.find(attr) != instance.attributes.end() &&
          node->children.find(instance.attributes.at(attr)) !=
              node->children.end()) {
        node = node->children.at(instance.attributes.at(attr));
      } else {
        // If attribute value not seen during training, return majority label of
        // current node
        return getMajorityLabel(node);
      }
    }
    return node->label;
  }

private:
  // Recursive function to build the tree
  TreeNode *buildRecursive(const vector<Instance> &data,
                           const vector<string> &attributes) {
    TreeNode *node = new TreeNode();

    // Check if all instances have the same label
    if (allSameLabel(data)) {
      node->isLeaf = true;
      node->label = data[0].label;
      return node;
    }

    // If no more attributes to split, return a leaf with majority label
    if (attributes.empty()) {
      node->isLeaf = true;
      node->label = getMajorityLabel(data);
      return node;
    }

    // Select attribute based on the selection strategy
    string selectedAttr;
    if (selectionStrategy == "best") {
      selectedAttr = selectBestAttribute(data, attributes);
    } else if (selectionStrategy == "top3_rand") {
      selectedAttr = selectTop3Random(data, attributes);
    } else {
      // Default to best attribute if unknown strategy
      selectedAttr = selectBestAttribute(data, attributes);
    }

    // If no attribute provides information gain, return a leaf with majority
    // label
    if (selectedAttr.empty()) {
      node->isLeaf = true;
      node->label = getMajorityLabel(data);
      return node;
    }

    node->isLeaf = false;
    node->attribute = selectedAttr;

    // Split data based on the selected attribute
    map<string, vector<Instance>> splits = splitData(data, selectedAttr);

    // Remove the selected attribute from the list for child nodes
    vector<string> remainingAttributes = attributes;
    remainingAttributes.erase(find(remainingAttributes.begin(),
                                   remainingAttributes.end(), selectedAttr));

    // Recursively build child nodes
    for (auto &pair : splits) {
      node->children[pair.first] =
          buildRecursive(pair.second, remainingAttributes);
    }

    return node;
  }

  // Check if all instances have the same label
  bool allSameLabel(const vector<Instance> &data) const {
    string firstLabel = data[0].label;
    return all_of(data.begin(), data.end(), [&](const Instance &inst) {
      return inst.label == firstLabel;
    });
  }

  // Get the majority label from a set of instances
  string getMajorityLabel(const vector<Instance> &data) const {
    map<string, int> labelCounts;
    for (const auto &instance : data) {
      labelCounts[instance.label]++;
    }
    // Find the label with the maximum count
    return max_element(labelCounts.begin(), labelCounts.end(),
                       [](const pair<string, int> &a,
                          const pair<string, int> &b) -> bool {
                         return a.second < b.second;
                       })
        ->first;
  }

  // Get the majority label from a subtree node
  string getMajorityLabel(TreeNode *node) const {
    map<string, int> labelCounts;
    traverseAndCount(node, labelCounts);
    // Find the label with the maximum count
    return max_element(labelCounts.begin(), labelCounts.end(),
                       [](const pair<string, int> &a,
                          const pair<string, int> &b) -> bool {
                         return a.second < b.second;
                       })
        ->first;
  }

  // Traverse the tree and count label frequencies
  void traverseAndCount(TreeNode *node, map<string, int> &labelCounts) const {
    if (node->isLeaf) {
      labelCounts[node->label]++;
      return;
    }
    for (auto &child : node->children) {
      traverseAndCount(child.second, labelCounts);
    }
  }

  // Calculate Information Gain or Gini Gain for an attribute
  double calculateGain(const vector<Instance> &data,
                       const string &attribute) const {
    double gain;
    if (selectionCriteria == "info_gain") {
      gain = entropy(data);
      map<string, vector<Instance>> splits = splitData(data, attribute);
      int total = data.size();
      for (const auto &pair : splits) {
        double proportion = static_cast<double>(pair.second.size()) / total;
        gain -= proportion * entropy(pair.second);
      }
    } else if (selectionCriteria == "gini") {
      gain = giniImpurity(data);
      map<string, vector<Instance>> splits = splitData(data, attribute);
      int total = data.size();
      for (const auto &pair : splits) {
        double proportion = static_cast<double>(pair.second.size()) / total;
        gain -= proportion * giniImpurity(pair.second);
      }
    } else {
      // Default to entropy if unknown criteria
      gain = entropy(data);
      map<string, vector<Instance>> splits = splitData(data, attribute);
      int total = data.size();
      for (const auto &pair : splits) {
        double proportion = static_cast<double>(pair.second.size()) / total;
        gain -= proportion * entropy(pair.second);
      }
    }
    return gain;
  }

  // Select the best attribute based on the highest gain
  string selectBestAttribute(const vector<Instance> &data,
                             const vector<string> &attributes) const {
    double bestGain = -1.0;
    string bestAttr;
    for (const auto &attr : attributes) {
      double gain = calculateGain(data, attr);
      if (gain > bestGain) {
        bestGain = gain;
        bestAttr = attr;
      }
    }
    return bestAttr;
  }

  // Select one attribute randomly from the top three based on gain
  string selectTop3Random(const vector<Instance> &data,
                          const vector<string> &attributes) const {
    // Calculate gains for all attributes
    vector<pair<string, double>> gains;
    for (const auto &attr : attributes) {
      double gain = calculateGain(data, attr);
      gains.emplace_back(attr, gain);
    }

    // Sort attributes based on gain in descending order
    sort(gains.begin(), gains.end(),
         [](const pair<string, double> &a, const pair<string, double> &b) {
           return a.second > b.second;
         });

    // Select top three attributes
    int top = min(3, static_cast<int>(gains.size()));
    vector<string> topAttrs;
    for (int i = 0; i < top; i++)
      topAttrs.push_back(gains[i].first);

    // Uniformly select one attribute from the top three
    uniform_int_distribution<> dis(0, topAttrs.size() - 1);
    return topAttrs[dis(gen)];
  }
};

// Function to split data into train and test sets
pair<vector<Instance>, vector<Instance>>
trainTestSplit(const vector<Instance> &data, double train_ratio, mt19937 &gen) {
  vector<Instance> shuffled = data;
  shuffle(shuffled.begin(), shuffled.end(), gen);
  int train_size = static_cast<int>(train_ratio * shuffled.size());
  vector<Instance> train(shuffled.begin(), shuffled.begin() + train_size);
  vector<Instance> test(shuffled.begin() + train_size, shuffled.end());
  return {train, test};
}

// Function to calculate accuracy
double calculateAccuracy(const DecisionTree &tree, const vector<Instance> &test,
                         int &unseenCount) {
  int correct = 0;
  for (const auto &instance : test) {
    string pred = tree.predict(instance);
    if (pred == instance.label)
      correct++;
    else if (pred == "unknown")
      unseenCount++;
  }
  return (static_cast<double>(correct) / test.size()) * 100.0;
}

int main() {
  // Read data
  vector<Instance> data = readData("car evaluation dataset/car.data");
  if (data.empty()) {
    cerr << "Error: No data to process." << endl;
    return 1;
  }

  // Attributes
  vector<string> attributes = {"buying",  "maint",    "doors",
                               "persons", "lug_boot", "safety"};

  // Define strategies and criteria
  vector<string> strategies = {"best", "top3_rand"};
  vector<string> criteria = {"info_gain", "gini"};

  // To store results
  map<string, map<string, double>>
      results; // strategy -> criteria -> avg accuracy

  // Initialize results to 0
  for (const auto &strategy : strategies) {
    for (const auto &criterion : criteria) {
      results[strategy][criterion] = 0.0;
    }
  }

  // Number of runs
  int num_runs = 20;

  // Random number generator seeded with current time
  mt19937 gen(static_cast<unsigned long>(
      chrono::high_resolution_clock::now().time_since_epoch().count()));

  // Run experiments
  for (int i = 0; i < num_runs; i++) {
    // Split data into train and test sets
    auto split = trainTestSplit(data, 0.8, gen);
    const vector<Instance> &train = split.first;
    const vector<Instance> &test = split.second;

    for (const auto &strategy : strategies) {
      for (const auto &criterion : criteria) {
        // Initialize and build the decision tree, passing 'gen'
        DecisionTree tree(criterion, strategy, gen);
        tree.buildTree(train, attributes);

        // Calculate accuracy on the test set
        int unseenCount = 0;
        double acc = calculateAccuracy(tree, test, unseenCount);
        results[strategy][criterion] += acc;

        // Optionally, log unseen attribute values
        // cout << "Run " << i+1 << " | Strategy: " << strategy
        //      << " | Criterion: " << criterion << " | Unseen: " << unseenCount
        //      << endl;
      }
    }
  }

  // Calculate average accuracy
  for (const auto &strategy : strategies) {
    for (const auto &criterion : criteria) {
      results[strategy][criterion] /= num_runs;
    }
  }

  // Print the results table with colors
  cout << fixed << setprecision(2);
  cout << BOLD << BLUE << "Average accuracy over " << num_runs << " runs\n"
       << RESET;
  cout << BOLD << BLUE
       << "--------------------------------------------------------------------"
          "--------------\n"
       << RESET;
  cout << BOLD << YELLOW << "Attribute selection strategy\t\t" << GREEN
       << "Information gain\t" << RED << "Gini impurity\n"
       << RESET;
  cout << BOLD << BLUE
       << "--------------------------------------------------------------------"
          "--------------\n"
       << RESET;
  for (const auto &strategy : strategies) {
    if (strategy == "best")
      cout << BOLD << CYAN << "Always select the best attribute\t\t" << RESET;
    else
      cout << BOLD << CYAN << "Select one randomly from top three\t\t" << RESET;
    cout << GREEN << results[strategy]["info_gain"] << "\t\t\t";
    cout << RED << results[strategy]["gini"] << "\n" << RESET;
  }
  cout << endl;

  return 0;
}
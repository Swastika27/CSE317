#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ctime>
#include <map>
#include <cmath>
#include <limits>
#include <cstring>
#include <random>
#include <algorithm>

using namespace std;

vector<vector<string>> examples;
vector<set<string>> attribute_values;
vector<vector<string>> training_data;
vector<vector<string>> test_data;

class Node
{
private:
    int attr_index;             // index of the attribute to check
    map<string, Node> children; // value, child node map
    bool is_leaf;
    string label;

public:
    Node()
    {
        attr_index = -1;
        is_leaf = false;
        label = "unused";
    }
    bool mapping_exists(string s)
    {
        return (children.find(s) != children.end());
    }
    int get_attr_index() { return attr_index; }
    void set_attr_index(int i) { attr_index = i; }
    bool get_is_leaf() { return is_leaf; }
    void set_leaf() { is_leaf = true; }
    string get_label() { return label; }
    void set_label(string s) { label = s; }
    void add_child(string value, Node n)
    {
        children[value] = n;
    }
    string infer(vector<string> data)
    {
        if (is_leaf)
        {
            return label;
        }
        if (children.find(data[attr_index]) != children.end())
        {
            return children[data[attr_index]].infer(data);
        }
        else
        {
            return label; // Default to current node's label
        }
    }
    void print(int n)
    {

        if (!is_leaf)
        {
            for (map<string, Node>::iterator it = children.begin(); it != children.end(); it++)
            {
                for (int i = 0; i < n; i++)
                {
                    cout << ".";
                }
                cout << attr_index << " ";
                cout << it->first << "\n";
                it->second.print(n + 1);
            }
        }
        else
        {
            cout << label << "\n";
        }
    }
};

class DecisionTree
{
private:
    Node root;
    Node make_leaf_node(string label)
    {
        Node n;
        n.set_leaf();
        n.set_label(label);
        return n;
    }
    string plurality_value(vector<vector<string>> examples)
    {
        if (examples.size() == 0)
        {
            return "";
        }

        int label_index = examples[0].size() - 1; // label is at the last index

        map<string, int> frequency;
        // initialize frequency map
        for (auto v : attribute_values.back())
        {
            frequency[v] = 0;
        }

        // calculate frequency
        for (int i = 0; i < examples.size(); i++)
        {
            string str = examples[i].back();
            frequency[str]++;
        }

        // find the max frequency string
        int max_freq = 0;
        string max_str = "";
        for (map<string, int>::iterator it = frequency.begin(); it != frequency.end(); it++)
        {
            // check
            // cout << it->first << " -> " << it->second << "\n";
            // update
            if (it->second > max_freq)
            {
                max_freq = it->second;
                max_str = it->first;
            }
        }
        return max_str;
    }
    bool has_same_label(vector<vector<string>> examples)
    {
        // if all examples have same classification, return true
        if (examples.size() <= 0)
            return false;

        string label = examples[0].back();
        for (int i = 0; i < examples.size(); i++)
        {
            if (label != examples[i].back())
            {
                return false;
            }
        }
        return true;
    }
    bool is_empty(bool *included_attr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (!included_attr[i])
                return false;
        }
        return true;
    }

    map<string, vector<vector<string>>> divide_example_by_attr(vector<vector<string>> examples, int attr_index)
    {
        map<string, vector<vector<string>>> child_examples;

        // divide data based on attribute value
        for (auto v : attribute_values[attr_index])
        {
            child_examples[v] = {};
        }
        for (vector<vector<string>>::iterator it = examples.begin(); it != examples.end(); it++)
        {
            child_examples[it->at(attr_index)].push_back(*it);
        }

        return child_examples;
    }

    double get_entropy(vector<vector<string>> examples)
    {
        // cout << "inside get entropy\n";
        if (examples.size() == 0)
        {
            // cout << "example size is 0";
            return 0;
        }
        map<string, int> count_label;
        for (auto v : attribute_values.back())
        {
            count_label[v] = 0;
        }

        for (int i = 0; i < examples.size(); i++)
        {
            // cout << "count label loop\n";
            count_label[*(attribute_values.back().find(examples[i].back()))]++;
        }

        double entropy = 0;
        for (map<string, int>::iterator it = count_label.begin(); it != count_label.end(); it++)
        {
            // cout << "insdie entropy loop\n";
            if (it->second < 1)
                continue;
            double probability = (1.0 * it->second) / examples.size();
            entropy -= (probability * log2(probability));
        }

        return entropy;
    }

    double calculate_gini_impurity(vector<vector<string>> examples)
    {
        if (examples.size() == 0)
            return 0;
        map<string, int> count_label;
        for (auto v : attribute_values.back())
        {
            count_label[v] = 0;
        }

        for (int i = 0; i < examples.size(); i++)
        {
            count_label[*(attribute_values.back().find(examples[i].back()))]++;
        }

        double prob_square = 0;
        for (map<string, int>::iterator it = count_label.begin(); it != count_label.end(); it++)
        {
            if (it->second < 1)
                continue;
            double probability = (1.0 * it->second) / examples.size();
            prob_square += (probability * probability);
        }

        return 1 - prob_square;
    }

    double get_information_gain(vector<vector<string>> examples, int attr_index)
    {
        double prev_entropy = get_entropy(examples);

        // divide data based on attribute
        map<string, vector<vector<string>>> child_examples = divide_example_by_attr(examples, attr_index);

        double children_entropy = 0;
        for (map<string, vector<vector<string>>>::iterator it = child_examples.begin(); it != child_examples.end(); it++)
        {
            children_entropy += (1.0 * it->second.size() / examples.size()) * get_entropy(it->second);
        }

        return prev_entropy - children_entropy;
    }

    double get_gini_impurity(vector<vector<string>> examples, int attr_index)
    {
        double prev_impurity = calculate_gini_impurity(examples);

        // divide data based on attribute
        map<string, vector<vector<string>>> child_examples = divide_example_by_attr(examples, attr_index);

        double children_impurity = 0;
        for (map<string, vector<vector<string>>>::iterator it = child_examples.begin(); it != child_examples.end(); it++)
        {
            children_impurity += (1.0 * it->second.size() / examples.size()) * calculate_gini_impurity(it->second);
        }
        return prev_impurity - children_impurity;
    }

public:
    DecisionTree() {}
    Node DTlearn(vector<vector<string>> examples, bool *included_attr, vector<vector<string>> parent_examples, int attr_select_criteria, int mod)
    {
        if (examples.size() <= 0)
        {
            // cout << "make leaf node parent plurality\n";
            return make_leaf_node(plurality_value(parent_examples));
        }
        if (has_same_label(examples))
        {
            // cout << "make leaf node class\n";
            return make_leaf_node(examples[0].back());
        }
        if (is_empty(included_attr, attribute_values.size() - 1))
        {
            // cout << "make leaf node own plurality\n";
            return make_leaf_node(plurality_value(examples));
        }

        Node n;
        map<string, vector<vector<string>>> child_examples;
        int best_attr = -1;
        int second_best = -1;
        int third_best = -1;
        double max_information_gain = numeric_limits<double>::min();
        double max_impurity_diff = numeric_limits<double>::min();
        bool included_attr_copy[attribute_values.size() - 1];

        switch (attr_select_criteria)
        {
        case 1: // information gain
            // cout << "information gain\n";
            for (int i = 0; i < attribute_values.size() - 1; i++)
            {
                // cout << " i = " << i << "\n";
                if (!included_attr[i] && get_information_gain(examples, i) > max_information_gain)
                {
                    max_information_gain = get_information_gain(examples, i);
                    third_best = second_best;
                    second_best = best_attr;
                    best_attr = i;
                }
            }

            break;

        case 2: // gini impurity

            for (int i = 0; i < attribute_values.size() - 1; i++)
            {
                // cout << "inside min impurity i = " << i << ", gini = " << get_gini_impurity(examples, i) << "\n";
                if (!included_attr[i] && get_gini_impurity(examples, i) > max_impurity_diff)
                {
                    max_impurity_diff = get_gini_impurity(examples, i);
                    third_best = second_best;
                    second_best = best_attr;
                    best_attr = i;
                }
            }
            break;
        default:
            cout << "unknown selection criteria\n";
            break;
        }
        if (mod == 1)
        {
            srand(time(0));
            switch (rand() % 3)
            {
            case 1:
                if (second_best != -1)
                {
                    best_attr = second_best;
                }
                break;
            case 2:
                if (third_best != -1)
                {
                    best_attr = third_best;
                }
                break;
            default:
                break;
            }
        }
        // memcpy(included_attr_copy, included_attr, sizeof(bool) * (attribute_values.size() - 1));
        for (int i = 0; i < attribute_values.size() - 1; i++)
        {
            included_attr_copy[i] = included_attr[i];
        }
            included_attr_copy[best_attr] = true;

            n.set_attr_index(best_attr);

            child_examples = divide_example_by_attr(examples, best_attr);

            for (map<string, vector<vector<string>>>::iterator it = child_examples.begin(); it != child_examples.end(); it++)
            {
                if (!n.mapping_exists(it->first))
                {
                    // cout << "hi\n";
                    Node subtree = DTlearn(it->second, included_attr_copy, examples, attr_select_criteria, mod);
                    n.add_child(it->first, subtree);
                }
                else
                {
                    cout << "mapping already exists\n";
                }
            }
            return n;
    }
    void learn(vector<vector<string>> examples, int attr_selection_criteria, int mod) // criteria=1 -> information gain, criteria=2 -> gini impurity
    {
        // cout << "learn called\n";
        bool included_attr[attribute_values.back().size() - 1];
        for (int i = 0; i < attribute_values.back().size() - 1; i++)
        {
            included_attr[i] = false;
        }
        root = DTlearn(examples, included_attr, {}, attr_selection_criteria, mod);
        // cout << "root is " << root.get_attr_index() << "\n";
        // root.print(0);
    }
    string infer(vector<string> attr_val) { return root.infer(attr_val); }
};

void read_file(string filename)
{
    ifstream file(filename); // open file

    if (!file.is_open())
    {
        cout << "can not open the specified file\n";
        return;
    }

    string line; // to read each line

    while (getline(file, line))
    { // read until the file has lines
        vector<string> csvalue;
        string value;
        int column_count = 0;

        stringstream ss(line); // read a line
        while (getline(ss, value, ','))
        { // split by comma
            csvalue.push_back(value);

            if (attribute_values.size() <= column_count)
            {
                attribute_values.push_back(set<string>()); // add a new set to attibute values if necessary
            }
            attribute_values[column_count++].insert(value);
        }

        examples.push_back(csvalue); // store the example
    }

    file.close();
}

void split(double training_pct, double test_pct)
{
    if (training_pct + test_pct > 1.0001)
    {
        cout << "invalid split percentage " << (training_pct + test_pct) << "\n";
        return;
    }

    int training_example_count = (int)(examples.size() * training_pct);
    int test_example_count = examples.size() - training_example_count;
    training_data = {};
    test_data = {};

    random_device rd;
    mt19937 gen(rd());
    shuffle(examples.begin(), examples.end(), gen);

    test_data.assign(examples.begin(), examples.begin() + test_example_count);
    training_data.assign(examples.begin() + test_example_count + 1, examples.end());
    // cout << "training " << training_data.size() << ", test " << test_data.size() << "\n";
}

double get_avg_accuracy(vector<vector<string>> examples, double training_pct, int attr_select_criteria, int run_count, int mod)
{
    DecisionTree decision_tree;

    double accuracy = 0;
    for (int c = 0; c < run_count; c++)
    {
        split(training_pct, 1 - training_pct);
        decision_tree.learn(training_data, attr_select_criteria, mod);
        int accurate_inference_count = 0;
        for (int i = 0; i < test_data.size(); i++)
        {
            // int data_index = test_data_indices[i];
            vector<string> attr_list;
            for (int j = 0; j < test_data[i].size() - 1; j++)
            {
                attr_list.push_back(test_data[i][j]);
            }

            string inferred_label = decision_tree.infer(test_data[i]);
            string original_label = test_data[i].back();
            // cout << original_label << " ";
            // cout << inferred_label << "\n";
            if (inferred_label == original_label)
            {
                accurate_inference_count++;
            }
        }
        // cout << "\naccurate inference " << accurate_inference_count << " out of " << test_data.size() << "\n";
        accuracy += (1.0 * accurate_inference_count) / test_data.size();
    }
    return (accuracy / run_count) * 100;
}
int main()
{
    string filename = "car evaluation dataset/car.data";
    // cout << "Provide dataset file name:\n";
    // cin >> filename;
    read_file(filename);
    int attr_select_option = 2;
    // cout << "Provide the attribute selection option:\n1 -> Information gain\n2 -> Gini impurity\n";
    // cin >> attr_select_option;

    printf("%100s\n", "Average accuracy over 20 runs");
    printf("%50s", "Attribute selection strategy");
    printf("%25s", "Information gain");
    printf("%25s\n", "Gini impurity");

    printf("%50s", "Always select the best attribute");
    printf("%25lf", get_avg_accuracy(examples, 0.8, 1, 20, 0));
    printf("%25lf\n", get_avg_accuracy(examples, 0.8, 2, 20, 0));

    printf("%50s", "Select randomly from top three attributes");
    printf("%25lf", get_avg_accuracy(examples, 0.8, 1, 20, 1));
    printf("%25lf\n", get_avg_accuracy(examples, 0.8, 2, 20, 1));

    return 0;
}

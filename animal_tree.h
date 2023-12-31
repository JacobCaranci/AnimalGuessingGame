#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Node {
 public:
  Node(std::string value) { value_ = value; }

  friend class Animal_Tree;

 private:
  std::string value_;
  std::shared_ptr<Node> left_child_ = nullptr;
  std::shared_ptr<Node> right_child_ = nullptr;
  bool animal = false;
};

class Animal_Tree {
 public:
  // Initializes tree with a question and an animal
  Animal_Tree(std::string animal, std::string question) {
    root_ = std::make_shared<Node>(question);
    root_->left_child_ = std::make_shared<Node>(animal);
    root_->left_child_->animal = true;
  }

  // Loads the tree with information from a text file
  void load_data() {
    std::ifstream path_desript{"data.txt"};

    std::shared_ptr<Node> curr = root_;

    std::string info;
    while (path_desript) {
      path_desript >> info;

      data_in.push_back(info);
    }

    if (data_in.size() != 0) {
      for (int i = 0; i < data_in.size(); i++) {
        data.push_back(data_in.at(i));
      }
    }

    for (int i = 0; i < data_in.size(); i++) {
      if ((data_in.at(i) == "right") && (curr->right_child_ != nullptr)) {
        curr = curr->right_child_;
      }

      if ((data_in.at(i) == "left") && (curr->left_child_ != nullptr)) {
        curr = curr->left_child_;
      }
      if (data_in.at(i) == "NEW_LINE_IND") {
        if (curr->left_child_ != nullptr) {
          add(data_in.at(i + 1), data_in.at(i + 2), curr, "right");
        } else {
          add(data_in.at(i + 1), data_in.at(i + 2), curr, "left");
        }

        curr = root_;
      }
    }
  }

  // Save any new questions/animals it learned during current session onto text
  // file for future session
  void put_new_questions_in_data_base() {
    std::ofstream text_file{"data.txt"};
    if (data_in.size() != 0) {
      for (int i = 0; i > data_in.size(); i++) {
        data.push_back(data_in.at(i));
      }
    }

    int newline_ind = 0;
    for (auto text : data) {
      text_file << text << " ";
      if (text == "NEW_LINE_IND") {
        newline_ind += 1;
      } else if (newline_ind == 1) {
        newline_ind += 1;
      } else if (newline_ind == 2) {
        text_file << '\n';
        newline_ind = 0;
      }
    }
  }

  // Main function of the tree which asks users to think of an animal
  void question() {
    std::cout << "***********************************************" << '\n';
    std::cout << "Think of an animal and I will try to guess it" << '\n';
    std::cout << "***********************************************" << '\n'
              << '\n';

    std::shared_ptr<Node> curr = root_;

    std::vector<bool> path;

    while (true) {
      if (curr->animal == true) {
        std::cout << "Is the animal a " << curr->value_
                  << "? (y) for yes, (n) for no" << '\n';

        std::string answer;
        std::cin >> answer;

        if (answer == "y") {
          std::cout << "I KNEW IT. THANKS FOR PLAYING!" << '\n';
          return;
        } else {
          // INPUTING DATA ON DATABASE

          insert_data(path, "no", curr);

          return;
        }
      }

      else {
        std::cout << curr->value_ << "? (y) for yes, (n) for no" << '\n';
        std::string answer;
        std::cin >> answer;

        if (answer == "y") {
          curr = curr->left_child_;
          path.push_back(true);
        } else if (answer == "n") {
          if (curr->right_child_ == nullptr) {
            // release path into database
            insert_data(path, "yes", curr);

            return;
          }
          curr = curr->right_child_;
          path.push_back(false);
        }
      }
    }
  }

 private:
  std::shared_ptr<Node> root_ = nullptr;
  std::vector<std::string> data_in;
  std::vector<std::string> data;

  // helper functions

  // inserts path of yes/no question to animal in a vector
  void insert_data(std::vector<bool> path, std::string right,
                   std::shared_ptr<Node> node) {
    std::cout << "This animal has not been added yet. What animal were "
                 "you thinking of? "
              << '\n';
    std::string animal;
    std::cin >> animal;
    std::cout << "Enter a question that resembles the animal" << '\n';
    std::string question;
    std::cin >> question;

    std::string direction = "left";

    if (right == "yes") {
      path.push_back(false);

      if (node->animal != true) {
        direction = "right";
      }
    }

    for (auto move : path) {
      if (move == false) {
        data.push_back("right");
      } else {
        data.push_back("left");
      }
    }

    data.push_back("NEW_LINE_IND");
    data.push_back(animal);
    data.push_back(question);

    add(animal, question, node, direction);
    std::cout << "Animal and question added to game" << '\n' << '\n';
  }

  // Adds an animal and a question to the tree
  void add(std::string animal, std::string question, std::shared_ptr<Node> node,
           std::string direction) {
    if (direction == "left") {
      node->right_child_ = std::make_shared<Node>(node->value_);
      node->value_ = question;
      node->left_child_ = std::make_shared<Node>(animal);
      node->right_child_->animal = true;
      node->left_child_->animal = true;
      node->animal = false;
    }

    else if (direction == "right") {
      node->right_child_ = std::make_shared<Node>(question);
      node->right_child_->left_child_ = std::make_shared<Node>(animal);
      node->right_child_->left_child_->animal = true;
    }
  }
};
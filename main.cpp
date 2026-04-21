#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

const string SHOPPING_LIST_FILE = "shopping_list.txt";

struct ItemNode {
  string name;
  string category;
  bool purchased;
  ItemNode *next;
};

ItemNode *head = nullptr;

string toLowerCase(const string &str) {
  string result = str;
  transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return tolower(c); });
  return result;
}

ItemNode *createNode(string name, string category, bool purchased) {
  ItemNode *newNode = new ItemNode;
  newNode->name = name;
  newNode->category = category;
  newNode->purchased = purchased;
  newNode->next = nullptr;

  return newNode;
}

bool validateCategory(const string &category) {
  if (category.empty()) {
    return false;
  }

  for (unsigned char c : category) {
    if (!isalpha(c) && c != ' ') {
      return false;
    }
  }

  return true;
}

void clearList() {
  while (head != nullptr) {
    ItemNode *tmp = head;
    head = head->next;
    delete tmp;
  }
}

void addItem() {
  cout << "\nAdd an item\n";

  string name = "";
  string category = "";
  bool purchased = false;

  cout << "Enter item name:";
  getline(cin >> ws, name);

  cout << "Enter category:";
  getline(cin >> ws, category);

  name = toLowerCase(name);
  category = toLowerCase(category);

  if (!validateCategory(category)) {
    cout << "Invalid category. Please try again.\n";
    return;
  }

  ItemNode *newNode = createNode(name, category, purchased);

  if (head == nullptr || head->category > newNode->category) {
    newNode->next = head;
    head = newNode;

    cout << "Item added.\n";
    return;
  }

  ItemNode *current = head;
  while (current->next != nullptr &&
         toLowerCase(current->next->category) <= category) {
    current = current->next;
  }
  newNode->next = current->next;
  current->next = newNode;

  cout << "Item added.\n";
}

void displayList(bool onlyNeeded = false) {
  if (head == nullptr) {
    cout << "Shopping list is empty.\n";
    return;
  }

  ItemNode *current = head;
  string currentCategory = "";
  bool firstPrinted = true;

  while (current != nullptr) {
    if (onlyNeeded && current->purchased) {
      current = current->next;
      continue;
    }

    if (firstPrinted || currentCategory != current->category) {
      currentCategory = current->category;
      firstPrinted = false;
      cout << "\n[" << currentCategory << "]\n";
    }

    cout << "- " << current->name << " ["
         << (current->purchased ? "Purchased" : "Needed") << "]\n";

    current = current->next;
  }
}

void markItemPurchased() {
  cout << "Enter item name to mark as purchased: ";

  string name = "";
  getline(cin >> ws, name);

  const string normalizedName = toLowerCase(name);

  ItemNode *current = head;
  while (current != nullptr) {
    if (toLowerCase(current->name) == normalizedName) {
      if (current->purchased) {
        cout << "Item is already purchased.\n";
        return;
      }

      current->purchased = true;
      cout << "Item marked as purchased.\n";
      return;
    }

    current = current->next;
  }

  cout << "Item not found.\n";
}

void removeItem() {
  if (head == nullptr) {
    cout << "Shopping list is empty.\n";
    return;
  }

  cout << "Enter item name to remove: ";
  string name = "";
  getline(cin >> ws, name);

  const string normalizedName = toLowerCase(name);

  ItemNode *current = head;
  if (toLowerCase(current->name) == normalizedName) {
    head = current->next;
    delete current;

    cout << "Item removed.\n";
    return;
  }

  while (current->next != nullptr &&
         toLowerCase(current->next->name) != normalizedName) {
    current = current->next;
  }

  if (current->next == nullptr) {
    cout << "Item not found.\n";
    return;
  }

  ItemNode *tmp = current->next;
  current->next = tmp->next;
  delete tmp;
  cout << "Item removed.\n";
}

void saveToFile() {
  ofstream outfile(SHOPPING_LIST_FILE);
  if (!outfile) {
    cout << "Failed to save file: " << SHOPPING_LIST_FILE;
    return;
  }

  ItemNode *current = head;
  while (current != nullptr) {
    outfile << current->name << "," << current->category << ","
            << current->purchased << "\n";

    current = current->next;
  }

  cout << "Shopping list saved to: " << SHOPPING_LIST_FILE << "\n";
  outfile.close();
}

bool confirmFileLoad() {
  while (true) {
    char answer = '\0';

    if (!(cin >> answer)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      cout << "Please enter y or n: ";
      continue;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    answer = static_cast<char>(tolower(static_cast<unsigned char>(answer)));

    if (answer == 'y') {
      return true;
    }

    if (answer == 'n') {
      return false;
    }

    cout << "Only accept y or n \n";
  }
}

void loadFile() {
  if (head != nullptr) {
    cout << "Shopping list is not empty. Do you want to continue? (y/n) ";

    bool canLoadFile = confirmFileLoad();
    if (!canLoadFile) {
      return;
    }

    clearList();
  }

  ifstream inputFile(SHOPPING_LIST_FILE);
  if (!inputFile) {
    cout << "Failed to open file: " << SHOPPING_LIST_FILE;
    return;
  }

  string line = "";
  string name = "";
  string category = "";
  string purchasedText = "";
  bool purchased = false;

  while (getline(inputFile, line)) {
    size_t pos1 = line.find(',');
    size_t pos2 = line.rfind(',');

    if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
      continue;
    }

    name = line.substr(0, pos1);
    category = line.substr(pos1 + 1, pos2 - pos1 - 1);

    purchasedText = line.substr(pos2 + 1);
    try {
      int purchasedValue = stoi(purchasedText);

      if (purchasedValue != 0 && purchasedValue != 1) {
        continue;
      }

      purchased = (purchasedValue == 1);
    } catch (...) {
      continue;
    }

    ItemNode *newNode = createNode(name, category, purchased);

    if (head == nullptr || category < head->category) {
      newNode->next = head;
      head = newNode;
      continue;
    }

    ItemNode *current = head;
    while (current->next != nullptr && current->next->category <= category) {
      current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
  }

  cout << "Shopping list loaded from: " << SHOPPING_LIST_FILE << "\n";
  inputFile.close();
}

void menu() {
  loadFile();
  int choice = -1;

  do {
    cout << "\nShopping List Manager\n";
    cout << "1. Add Item\n";
    cout << "2. Display List\n";
    cout << "3. Display Needed Items Only\n";
    cout << "4. Mark Item as Purchased\n";
    cout << "5. Remove Item\n";
    cout << "6. Save List\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";

    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      cout << "Please enter a number.\n";
      continue;
    }

    switch (choice) {
    case 1:
      addItem();
      break;

    case 2:
      displayList();
      break;

    case 3:
      displayList(true);
      break;

    case 4:
      markItemPurchased();
      break;

    case 5:
      removeItem();
      break;

    case 6:
      saveToFile();
      break;

    case 0:
      cout << "Exiting...\n";
      break;

    default:
      cout << "Invalid choice.\n";
    }
  } while (choice != 0);
}

// Main function
int main() {
  menu();
  clearList();

  return 0;
}

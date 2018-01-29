#include "Tag.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <queue>

// Required functions from a4.cpp. Do not remove these.
bool html_is_valid(const std::string& document);

Tag* generate_DOM_tree(const std::string& document);

void determine_visible_objects(Tag* const root);

std::string print_visible_elements(Tag* const root);

Tag* getElementByID(Tag* const root, const std::string& id);

// -------------------------------------------------------------------

// Helper functions for tests. Feel free to add more.
const std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ifstream::ate);
    if (!file.is_open()) {
        std::cout << "File: " << filename << " failed to open." << std::endl;
        return 0;
    }
    size_t stringLength = file.tellg();
    file.seekg(0, file.beg);

    char* const data = new char[stringLength + 1];
    data[stringLength] = '\0';
    file.read(data, stringLength);
    file.close();

    const std::string fileContent(data);
    delete[] data;
    return fileContent;
}

bool test_html_is_valid(const bool& validHTML, const std::string& fileContent, const std::string& filename) {
    if (validHTML && html_is_valid(fileContent)) {
        std::cout << "Correctly validated the valid file: " << filename << std::endl;
        return true;
    }
    else if (!validHTML && !html_is_valid(fileContent)) {
        std::cout << "Correctly detected the invalid file: " << filename << std::endl;
        return true;
    }
    else {
        if (validHTML) {
            std::cout << "Incorrectly said a valid file was invalid for the file: " << filename << std::endl;
        }
        else {
            std::cout << "Incorrectly said an invalid file was valid for the file: " << filename << std::endl;
        }
        return false;
    }
}

void cleanup_tree(Tag* root) {
    if (root == nullptr) {
        return;
    }
    std::queue<Tag*> nodesToCleanup;
    nodesToCleanup.push(root);
    while (!nodesToCleanup.empty()) {
        Tag* element = nodesToCleanup.front();
        nodesToCleanup.pop();
        for (Tag* child : element->_children) {
            nodesToCleanup.push(child);
        }
        delete element;
    }
}


// -------------------------------------------------------------------


int main() {
    std::string source_folder = "sample-pages/";
    // Add more files here and whether or not they are valid.
    const std::vector<std::string> sample_files{"invalid-no-doctype.html", "invalid-no-title.html", "valid-hello.html", "valid-no-content.html"};
    const std::vector<bool> validity{false, false, true, true};
    bool passed = false;

    for (size_t i = 0; i < sample_files.size(); ++i) {
        passed = false;
        const std::string filename = source_folder + sample_files.at(i);
        const std::string fileContent = read_file(filename);

        // std::cout << "\"" << fileContent << "\"" << std::endl;

        const bool& validHTML = validity.at(i);
        passed = test_html_is_valid(validHTML, fileContent, filename);


        if (passed && validHTML) {
            Tag* domTreeRoot = generate_DOM_tree(fileContent);
            determine_visible_objects(domTreeRoot);
            print_visible_elements(domTreeRoot);
            Tag* element = getElementByID(domTreeRoot, "");
            if (element != nullptr) {
                std::cout << "Should return nullptr when empty ID is searched. Got id: \"" << element->_id << "\"" << std::endl;
            }

            if(element->_tagname == Tag::TAGNAME::CONTENT) {
                std::cout << "Element returned has TAGNAME set to CONTENT." << std::endl;
            }
            cleanup_tree(domTreeRoot);
        }
    }
    return 0;
}
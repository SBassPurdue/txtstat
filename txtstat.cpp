
//c headers
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

//c++ headers
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

//forward declaration
template<typename T> bool compare_values(std::pair<T, int>, std::pair<T, int>);

int main (int argc, char **argv) {

  FILE *in_stream = NULL;

  if (argc == 1) {
    in_stream = stdin;
  }
  else if (argc == 2) {
    in_stream = fopen(argv[1], "r");
  }
  else {
    fprintf(stderr, "Usage: %s [file_name=stdin]\n", argv[0]);
    return 0;
  }

  assert(in_stream);

  // Read input from stream (either file or stdin)

  #define BUFFER_SIZE 4096
  char text_str[BUFFER_SIZE] = {0}; //will NOT be null-terminated
  char c = '\0';
  char digraph[2] = {0};
  char trigraph[3] = {0};
  int word_length = 0; //counts consecutive characters to determine di/trigraphs
  //int buffer_num = 0; //used for logging
  int count;
  std::map<char, int> char_map;
  std::map<std::string, int> digraph_map;
  std::map<std::string, int> trigraph_map;

  while (true) {
    //printf("Reading into buffer %d\n", buffer_num++);
    count = fread(text_str, sizeof(char), BUFFER_SIZE / sizeof(char), in_stream);
    assert(!ferror(in_stream));

    for (int i = 0; i < count; i++) {

      // Parse input into characters, digraphs, and trigraphs

      if (isalpha(text_str[i])) {
        word_length++;
        text_str[i] = toupper(text_str[i]); //ignore case

        //read into di/trigraphs from back-to-front--avoids needing to reverse when storing
        c = text_str[i];

        digraph[0] = digraph[1];
        digraph[1] = text_str[i];

        trigraph[0] = trigraph[1];
        trigraph[1] = trigraph[2];
        trigraph[2] = text_str[i];

        // Store char/di/trigraph in maps

        char_map[c]++; //will initialize values that do not exist

        if (word_length >= 2) { //valid digraph
          std::string digraph_str(digraph, 2);
          digraph_map[digraph_str]++;
        }

        if (word_length >= 3) { //valid trigraph
          std::string trigraph_str(trigraph, 3);
          trigraph_map[trigraph_str]++;
        }

      } /* if isalpha */
      else {
        word_length = 0;
      }

    } /* for i */

    if (feof(in_stream)) {
      if (in_stream != stdin) { fclose(in_stream); }
      in_stream = NULL;
      break;
    }

  } /* while true */

  // Sort the values in descending order by frequency and print

  //characters
  std::vector<std::pair<char, int>> char_vec;
  for (std::map<char, int>::iterator it = char_map.begin(); it != char_map.end(); it++) {
    char_vec.emplace_back(it->first, it->second);
  }

  std::stable_sort(char_vec.begin(), char_vec.end(), compare_values<char>);

  for (std::vector<std::pair<char, int>>::iterator it = char_vec.begin(); it != char_vec.end(); it++) {
    printf("%c %d\n", it->first, it->second);
  }
  printf("\n");

  //digraphs
  std::vector<std::pair<std::string, int>> digraph_vec;
  for (std::map<std::string, int>::iterator it = digraph_map.begin(); it != digraph_map.end(); it++) {
    digraph_vec.emplace_back(it->first, it->second);
  }

  std::stable_sort(digraph_vec.begin(), digraph_vec.end(), compare_values<std::string>);

  for (std::vector<std::pair<std::string, int>>::iterator it = digraph_vec.begin(); it != digraph_vec.end(); it++) {
    printf("%s %d\n", it->first.c_str(), it->second);
  }
  printf("\n");

  //trigraphs
  std::vector<std::pair<std::string, int>> trigraph_vec;
  for (std::map<std::string, int>::iterator it = trigraph_map.begin(); it != trigraph_map.end(); it++) {
    trigraph_vec.emplace_back(it->first, it->second);
  }

  std::stable_sort(trigraph_vec.begin(), trigraph_vec.end(), compare_values<std::string>);

  for (std::vector<std::pair<std::string, int>>::iterator it = trigraph_vec.begin(); it != trigraph_vec.end(); it++) {
    printf("%s %d\n", it->first.c_str(), it->second);
  }
  printf("\n");

} /* main() */

template <class T> bool compare_values(std::pair<T, int> a, std::pair<T, int> b) {
  return a.second > b.second;
} /* compare_values() */


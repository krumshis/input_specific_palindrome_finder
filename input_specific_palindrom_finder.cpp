#include <iostream>
#include <algorithm>
#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class FindPalindromes {
 public:
    FindPalindromes(int n, std::vector<string>* words)
    : n_size(n), m_input_words(words) {
        // Process and annotate input in the hope of reducing time in future.
        // Find all palindromic words and gather them in separate vector.
        // For each word of the input, that has its inversion in the input as well,
        // enter two map entries into m_inversion_map
        m_self_palindromes = std::make_unique<std::vector<string>>();
        m_reduced_input = std::make_unique<std::vector<string>>();
        m_inversion_map = std::make_unique<std::map<string, string>>();

     for (int i = 0; i < n_size; i++) {
            string str(m_input_words->at(i));
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            if (IsPalindrome(str)) {
                m_self_palindromes->push_back(m_input_words->at(i));
            } else if (m_inversion_map->empty() ||   // Find is logarithmic in size.
                       m_inversion_map->find(str) == m_inversion_map->end()) {
                // Word and its inversion are not in the map, which means one of two things: the word
                // might not have its inversion in input or the inversion has not been found yet.
                // In either case we need first to produce an inversion and search for it.
                string rstr = "";
                for (auto rit = str.rbegin(); rit != str.rend(); rit++) {
                    rstr += *rit;
                }
                // The input has been sorted in ascending order. If rstr is lexigraphically
                // less than str - don't bother, it should have beeb accounted for already.
                // Otherwise search from iterator it on to save time.
                if (rstr.compare(str) > 0) {
                    // Make first letter a capital one - they are all dwarf's names :).
                    std::transform(rstr.begin(), rstr.begin() + 1, rstr.begin(), ::toupper);
                    // Binary search is logarithmic in size, also we slash some time by starting
                    // from the current work, instead of the beginning, and by not searchin at
                    // all if inversion compares as smaller than word - this means that wod should
                    // have already been processed if it existed, meaning it doesn't exist.
                    if (std::binary_search (m_input_words->begin() + i, m_input_words->end(), rstr)) {
                       // Inversion found, its index/position doesn't matter
                        // enter the pair into the map.
                        m_inversion_map->insert(std::pair<string, string>(m_input_words->at(i), rstr));
                        m_inversion_map->insert(std::pair<string, string>(rstr, m_input_words->at(i)));
                        m_reduced_input->push_back(m_input_words->at(i));
                        m_reduced_input->push_back(rstr);
                    }
                }
            } else {
                continue;
            }
        }
        n_reduced_size = m_reduced_input->size();
    }

    ~FindPalindromes() {}
    
    bool IsPalindrome(const string& line) {
        int len = line.length();
        int half_len = len >> 1;
        if (len < 2) {
            return true;
        } else {
            for (int i = 0; i < half_len; i++) {
                if (line[i] != line[len - 1 - i])
                   return false;
            }
            // If we finished the loop, it must be a palindrome!
            return true;
        }
        return false;
    }
	
	void PrintLeftSide(std::vector<string>* current_combination) {
        for (auto it = current_combination->begin(); it != current_combination->end(); it++) {
            cout << *it << " ";
        }
    }

	void PrintInversedRightSide(std::vector<string>* current_combination) {
        for (auto rit = current_combination->rbegin(); rit != current_combination->rend(); rit++) {
            cout << m_inversion_map->at(*rit) << " ";
        }
    }

	void PrintSelfPalidromicWords() {
        if (!m_self_palindromes->empty()) {
            for (const auto& word : *m_self_palindromes) {
                n_palindromes++;
                    cout << n_palindromes << ". ";
                    cout << word << " ";
            }
        }
    }
	
	void CreateAllHalfCombinations(int r) {
        std::vector<string> current_combination;
        current_combination.resize(r);
        MakeSingleCombination(&current_combination, 0, r, r);
    }

    void MakeSingleCombination(std::vector<string>* current_combination, /* combination of words */
                               /* index in input vector, where we can start looking for next word */
                               int start,
                               int r_left,  /* number of words in combination we still need to fill */
                               int r /* size of combination */) {
        // Current combination is ready to be processed for permutations and possible palindromes.
        if (r_left == 0) {
            // We got a set of words that constitute half of a palindrome for each permutation.
            do {
                // Print palindrome, don't forget to print the mirror image.
                n_palindromes++;
                cout << n_palindromes << ". ";
                PrintLeftSide(current_combination);
                PrintInversedRightSide(current_combination);
                cout << "\n";
                // Insert each palindromic word and print.
                if (!m_self_palindromes->empty()) {
                    for (const auto& word : *m_self_palindromes) {
                        n_palindromes++;
                        cout << n_palindromes << ". ";
                        PrintLeftSide(current_combination);
                        cout << word << " ";
                        PrintInversedRightSide(current_combination);
                        cout << "\n";
                    }
                }
            } while (std::next_permutation(current_combination->begin(), current_combination->end()));
            return;
        } else if (start + r_left <= n_reduced_size) {  // can't go if we pushed to the end of available array
            for (int i = start; i < n_reduced_size; i++) {
                // If the next coming word is an inversion of some other word that we already
                // have in our combination, we can't take it and need to skip it. Remember -
                // we are making a half of a mirror image. Use binary search to find possible inversion.
                // Unless, of course, it's the first word coming into combination.
                if (r_left == r ||
                    (!std::binary_search (current_combination->begin(),
                                         current_combination->begin() + (r - r_left),
                                         m_reduced_input->at(i)) &&
                    !std::binary_search (current_combination->begin(),
                                        current_combination->begin() + (r - r_left),
                                        m_inversion_map->at(m_reduced_input->at(i))))) {
                    current_combination->at(r - r_left) = m_reduced_input->at(i);
                    MakeSingleCombination(current_combination, i + 1, r_left - 1, r);
                }
            }
        } else {
            // Can't move forward - return.
            return;
        }
    }
	
	int GetReducedInputSize() { return n_reduced_size; }
    int GetPalindromeNumber() { return n_palindromes; }

 private:
    int n_size = 0;   // size of original input;
    std::vector<string>* m_input_words;  // original input words;
    int n_self_palindromes = 0;  // number of palindromic words in input;
    std::unique_ptr<std::vector<string>> m_self_palindromes = nullptr;  // vector of palindromic words derived from input;
    std::unique_ptr<std::map<string, string>> m_inversion_map = nullptr; // map of words to their inversions;
    int n_reduced_size = 0; // size of reduced input vector;
    std::unique_ptr<std::vector<string>> m_reduced_input = nullptr; // new input, containing only words with inversions;
    int n_palindromes = 0;   // number of found palindromes
};

int main() {
    int n;  // number of words in the intput;
    cin >> n;
    string input_word = "";
    std::vector<string> words;
    for (int i = 0; i < n; i++) {
        cin >> input_word;
        words.push_back(input_word);
    }
    // Sort element of input into ascending order.
    std::sort(words.begin(), words.end());

    clock_t start = clock ();  // Time preparation step.
    FindPalindromes my_palindromes(n, &words);
    clock_t timeInputProcessing = ( clock() - start ) * 1000000.0/ CLOCKS_PER_SEC;

    clock_t work_start = clock();   // Time actual work.
    int new_n = my_palindromes.GetReducedInputSize()/2;
    for (int r = 1; r <= new_n; r++) {
        my_palindromes.CreateAllHalfCombinations(r);
    }
    clock_t timePalindromeFinding = ( clock() - work_start ) * 1000000.0/ CLOCKS_PER_SEC;
    my_palindromes.PrintSelfPalidromicWords();

    cout << "\nNum of palindromes: " << my_palindromes.GetPalindromeNumber() << "\n";
    cout << "Input preparation took " << timeInputProcessing << " microseconds\n";
    cout << "Actual palindrome construction took " << timePalindromeFinding << " microseconds\n";
    cout << "Total time elapsed was " << timeInputProcessing + timePalindromeFinding
         << " microseconds\n";
    return 0;
}

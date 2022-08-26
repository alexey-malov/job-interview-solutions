// task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include <map>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <chrono>

using namespace std;

using CharFreqMap = map<char, int>;

CharFreqMap GetFreqMap(const string& word) {
	CharFreqMap char_freq;
	for (char ch : word) {
		++char_freq[ch];
	}
	return char_freq;
}

bool IsSubWord(const CharFreqMap& freq_map, const string& test_word) {
	for (auto [ch, freq] : GetFreqMap(test_word)) {
		if (auto it = freq_map.find(ch);
			it == freq_map.end() || freq > it->second) {
			return false;
		}
	}
	return true;
}

bool CanMakeSubWord(const string& word, const string& test_word) {
	return IsSubWord(GetFreqMap(word), test_word);
}

set<string> GetSubWords(const string& word, const vector<string>& dictionary) {
	const auto char_freq_map = GetFreqMap(word);

	set<string> sub_words;
	for (const string& w : dictionary) {
		if (w.length() <= word.length() && IsSubWord(char_freq_map, w)) {
			sub_words.emplace(w);
		}
	}

	return sub_words;
}

vector<string> ReadDictionary(const string& file_name) {
	ifstream in{ file_name };
	string word;
	set<string> dictionary;
	while (getline(in, word)) {
		if (!word.empty()) {
			dictionary.emplace_hint(dictionary.end(), word);
		}
	}
	return { dictionary.begin(), dictionary.end() };
}

int main()
{
	using namespace std::chrono;

	assert(CanMakeSubWord("telegramma"s, "telega"s));
	assert(!CanMakeSubWord("telegramma"s, "telo"s));
	assert(CanMakeSubWord("telegramma"s, "lemma"s));
	assert(!CanMakeSubWord("telegramma"s, "lemmma"s));

#if 0
	vector<string> dictionary = {
		"telega"s, "lemma"s, "gramm"s, "tema"s, "telo"s, "gamma"s
	};
#endif

	auto dictionary = ReadDictionary("dict.txt");

	const auto start_time = steady_clock::now();
	const auto sub_words = GetSubWords("adventure"s, dictionary);
	const auto dur = steady_clock::now() - start_time;

	for (const auto& sub_word : sub_words) {
		std::cout << sub_word << std::endl;
	}

	std::cout << sub_words.size() << " words of "sv << dictionary.size() << " have been found in "sv
		<< duration<double>(dur).count() << " seconds" << std::endl;
}


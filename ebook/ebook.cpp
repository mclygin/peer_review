#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <numeric>
#include <utility>
#include "log_duration.h"

using namespace std;

struct EbookStats {
	vector<int> readers;
	vector<int> pages_stat;
};


namespace ebook {
	vector<string> ReadRequests(istream& input = cin);
	void ProcessRequests(const vector<string>& requests, ostream& out = cout);
	void Cheer(int reader_id, EbookStats& stats, ostream& out);
	void Read(int reader_id, int page, EbookStats& stats);
}

vector<string> ebook::ReadRequests(istream& input) {

	vector<string> requests;
	int count_requests;
	input >> count_requests;
	requests.reserve(count_requests);

	string str;
	getline(input, str);
	while(count_requests--) {
		getline(input, str);
		requests.push_back(move(str));
	}
	return requests;
}

void ebook::ProcessRequests(const vector<string>& requests, ostream& out) {

	EbookStats stats;

	for (const auto& request : requests) {
		if (request[0] == 'C') {
			Cheer(stoi(request.substr(request.find_first_of(' ') + 1)), stats, out);
		}
		else {
			int reader_id = stoi(request.substr(request.find_first_of(' ') + 1, request.find_last_of(' ')));
			int page = stoi(request.substr(request.find_last_of(' ')));
			Read(reader_id, page, stats);
		}
	}
}

void ebook::Cheer(int reader_id, EbookStats& stats, ostream& out) {
	if (stats.readers.size() < reader_id + 1 || stats.readers[reader_id] == 0) {
		out << 0 << endl;
	}
	else {
		int read_less = accumulate(stats.pages_stat.begin(), stats.pages_stat.begin() + stats.readers[reader_id], 0);
		int total_readers = accumulate(stats.pages_stat.begin(), stats.pages_stat.end(), 0) - 1;
		if (total_readers == 0) {
			out << 1 << endl;
		}
		else {
			out << std::setprecision(6) << (read_less * 1.0) / total_readers << endl;
		}
	}
}

void ebook::Read(int reader_id, int page, EbookStats& stats) {
	if (stats.readers.size() < reader_id + 1) {
		stats.readers.resize(reader_id + 1);
	}
	if (stats.readers[reader_id] != 0) {
		--stats.pages_stat[stats.readers[reader_id]];
	}
	if (stats.pages_stat.size() < page + 1) {
		stats.pages_stat.resize(page + 1);
	}
	++stats.pages_stat[page];
	stats.readers[reader_id] = page;
}


int main() {
	ebook::ProcessRequests(ebook::ReadRequests());
	return 0;
}
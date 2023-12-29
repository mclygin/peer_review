#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <utility>
#include <cassert>
#include <functional>

using namespace std;

class Domain {

public:

    Domain(string&& domain) :
        domain_(domain) {
        std::reverse(domain_.begin(), domain_.end()); // переворачиваем строки, чтобы в дальнейшем искать по префиксам
    }

    const string& Get(void) const {
        return domain_;
    }

    bool operator==(const Domain& rhs) const {
        return domain_ == rhs.Get();
    }

    bool IsSubdomain(const Domain& other) const {
        if (domain_.size() < other.domain_.size()) {
            return false;
        }
        for (size_t i = 0; i < std::min(domain_.size(), other.domain_.size()); ++i) {   //провер€ем побуквенно домены
            if (domain_[i] != other.domain_[i]) {
                return false;
            }
        }
        if (domain_.size() == other.domain_.size()) {   //в переданном домене есть все буквы текущего домена, если размер доменов совпадает, значит это одинаковые домены
            return true;
        }
        else {                                     
            return domain_[other.domain_.size()] == '.' ? true : false; //если все буквы с переданным доменом совпали, тогда текущей домен будет субдоменом, если следующий символ будет точкой
        }
    }
private:
    string domain_;
};

bool CompareDomain(const Domain& lhs, const Domain& rhs) {
    return std::lexicographical_compare(lhs.Get().begin(), lhs.Get().end(), rhs.Get().begin(), rhs.Get().end());
}

class DomainChecker {
public:

    template<typename InputIt>
    DomainChecker(InputIt range_begin, InputIt range_end) :

        forbidden_domains_({ range_begin, range_end }) {

        sort(forbidden_domains_.begin(), forbidden_domains_.end(), CompareDomain);  //сортируем дл€ использовани€ upper_bound

        auto last = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {return rhs.IsSubdomain(lhs); });
        forbidden_domains_.erase(last, forbidden_domains_.end()); //удал€ем субдомены, если они есть
    }

    bool IsForbidden(const Domain& other) const {
        if (forbidden_domains_.size() == 0) {
            return false;
        }
        auto it = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), other, CompareDomain);
        if (it != forbidden_domains_.begin()) {
            --it;           //полученный итератор указывал на домен лексикографически больший провер€емого, поэтому нужный домен будет предыдущим
        }
        return other.IsSubdomain(*it);
    }
private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
const vector<Domain> ReadDomains(istream& input, size_t number) {
    vector<Domain> domains;
    domains.reserve(number);

    for (size_t i = 0; i < number; ++i) {

        string str;
        getline(input, str);

        domains.push_back({ std::move(str) });
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void Test(void) {
    {//ѕроверка с пустым вектором запрещенных доменов
        const vector<Domain> forbidden_domains;
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
        const vector<Domain> test_domains{ {"aa"},{"a.a"} };
        for (const Domain& domain : test_domains) {
            assert(!checker.IsForbidden(domain));
        }

    }
    {
        const vector<Domain> forbidden_domains{
            {"aa.aaaa.a.a.a.aaaa.a"},
            {"aa"},
            {"a.aa.aaaa.a.a.a.aaaa.a"},
            {"a.aa"},
            {"a.aa.aaaa.aa.a.aa"},
            {"aaaa.a.a.aaaa.a"},
            {"aa"},
            {"aaaa.a.a.a.aaaa.a"},
            {"aa.aaaa.a.a.a.aaaa.a"},
            {"aaaa.aa.aaa.aaaa.a"}
        };
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
        const vector<Domain> test_domains{ {"a.a.a.aaaa.a"},{"a.a"}, {"b.a.a"} };
        assert(!checker.IsForbidden(test_domains[0])); // "a.a.a.aaaa.a" не поддомен
        assert(!checker.IsForbidden(test_domains[1])); // "a.a"  не поддомен
        assert(!checker.IsForbidden(test_domains[2])); // "b.a.a" не поддомен
    }
}

int main() {
    Test();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
    return 0;
}
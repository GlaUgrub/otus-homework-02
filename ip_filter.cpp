#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <map>

const int numbers_in_ip = 4;

class IpV4 : public std::array<uint8_t, numbers_in_ip>
{
public:
    IpV4(std::vector<std::string> const& in)
    {
        if (in.size() != numbers_in_ip)
            throw std::length_error("Incorrect length of IP address");

        for (int i = 0; i < numbers_in_ip; ++i)
        {
            const std::string& part = in.at(i);
            const int num = std::stoi(part);

            if (num < 0 || num > 0xff)
                throw std::out_of_range("Number within ip is out of range 0-255");

            this->at(i) = num;
            string_representation.at(i) = part;
        }
    }

    IpV4(IpV4 const& in) = default;

    template<typename T>
    void print(T& stream) const
    {
        for (const auto& part : string_representation)
        {
            stream << part;
            if (&part != &string_representation.back())
                stream << '.';
        }
    }

protected:
    using IpV4Str = std::array<std::string, numbers_in_ip>;
    IpV4Str string_representation;

};

static auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

using IpId = uint32_t;
using IpPool = std::map<IpId, IpV4>;

static IpId get_ip_id(IpV4 const& ip)
{
    IpId id = ip.at(0);
    for (auto num = ip.begin() + 1; num != ip.end(); ++num)
    {
        id <<= 8;
        id += *num;
    }

    return id;
}

template<typename T>
static void set_ip_pool(T& stream, IpPool& ip_pool)
{
    for(std::string line; std::getline(stream, line);)
    {
        auto v = split(line, '\t');
        IpV4 ip = split(v.at(0), '.');
        ip_pool.emplace(get_ip_id(ip), ip);
    }
}

/*void filter(const T& t, Args... args) {
    std::cout << t << " ";
    filter(args...);
}*/

int main()
{
    try
    {
        // TODO: switch back to reading std::cin
        IpPool ip_pool;
        const std::string in_file = "/home/glaugrub/otus/otus-homework-02/ip_filter_short.tsv";
        std::ifstream input(in_file);

        set_ip_pool(input, ip_pool);

        // TODO: switch back to writing to std::cout
        const std::string out_file = "/home/glaugrub/otus/otus-homework-02/ip_filter_short_out.txt";
        std::ofstream output(out_file);

        for (decltype(auto) ip = ip_pool.crbegin(); ip != ip_pool.crend(); ++ip)
        {
            ip->second.print(output);
            output << std::endl;
        }

        // filters
        //template <>
        //auto filter()
        //IpPool first_1;


        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

#include <array>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

#include <boa/boa.h>
#include <cockpit/cockpit.h>
#include <simple-terminal/terminal.h>

auto constexpr VAR_X = 'x';
auto constexpr VAR_Y = 'y';

auto constexpr KEY_ARROW_UP    = 0x415B1B;
auto constexpr KEY_ARROW_DOWN  = 0x425B1B;
auto constexpr KEY_ARROW_RIGHT = 0x435B1B;
auto constexpr KEY_ARROW_LEFT  = 0x445B1B;

auto constexpr KEY_PAGE_UP   = 0x7E355B1B;
auto constexpr KEY_PAGE_DOWN = 0x7E365B1B;

class format_string
{
    std::vector<std::string> constants_;
    std::vector<std::pair<bool, char>> variables_;

public:

    explicit format_string(std::string const& string)
    {
        std::stringstream ss_constant, ss_escaped;

        auto escape = false;
        for (auto const cur_char : string)
        {
            switch (cur_char)
            {
            case '{':
                escape = true;
                continue;
            case '}':
                escape = false;

                auto escaped = ss_escaped.str();

                if (escaped.empty())
                    continue;

                char escaped_sign_char = escaped.front();
                auto escaped_sign_positive = true;
                switch (escaped_sign_char)
                {
                case '-':
                    escaped_sign_positive = false;
                case '+':
                    escaped = escaped.substr(1);
                    break;
                }

                if (escaped.empty())
                    continue;

                constants_.push_back(ss_constant.str());
                ss_constant.str("");

                variables_.emplace_back(escaped_sign_positive, escaped.front());
                ss_escaped.str("");

                continue;
            }

            if (escape)
                ss_escaped << cur_char;
            else
                ss_constant << cur_char;
        }

        constants_.push_back(ss_constant.str());
    }

    inline std::string str(std::unordered_map<char, int> const& value_map) const
    {
        std::vector<std::string> var_values;
        for (auto const& [sign, var] : variables_)
        {
            auto const it_value = value_map.find(var);

            var_values.push_back(
                it_value == value_map.end()
                    ? "0"
                    : std::to_string((sign ? 1 : -1) * it_value->second));
            break;
        }

        std::stringstream ss_result;
        ss_result << constants_.front();
        for (unsigned i = 0; i < var_values.size(); ++i)
            ss_result << var_values.at(i) << constants_.at(i + 1);

        return ss_result.str();
    }
};

std::string pipe_command_output(std::string command)
{
    std::array<char, 128> buffer;

    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    std::string output;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            output += buffer.data();
    }

    return output;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    std::unordered_map<char, int> value_map;

    // Get auxiliary python file from binary directory
    boa::python_file const py_aux(std::string(argv[0]) + "_aux.py");

    format_string const command(argv[1]);

    auto [n_lines, n_columns] = term::get_size();

    // Create space not to overwrite existing output
    std::cout << std::string(n_lines, '\n') << std::flush;

    cockpit cockpit(1000, 1,
        [command, &value_map, &n_lines, &n_columns]
        {
            auto const size = term::get_size();
            n_lines = size.first;
            n_columns = size.second;

            // Print prompt
            term::set_cursor(n_lines, 1);
            term::clear_line();
            std::cout << std::string(":").substr(0, n_columns);

            return pipe_command_output(command.str(value_map));
        });
    cockpit.start();

    auto character = '\0';
    do
    {
        if (std::isprint(character))
            std::cout << character << std::flush;

        auto const key = py_aux.call_function<uint32_t>("read_key");

        switch (key)
        {
        case KEY_ARROW_UP:
            --value_map[VAR_Y];
            break;
        case KEY_ARROW_DOWN:
            ++value_map[VAR_Y];
            break;
        case KEY_ARROW_RIGHT:
            ++value_map[VAR_X];
            break;
        case KEY_ARROW_LEFT:
            --value_map[VAR_X];
            break;
        case KEY_PAGE_UP:
            value_map[VAR_Y] -= n_lines;
            break;
        case KEY_PAGE_DOWN:
            value_map[VAR_Y] += n_lines;
            break;
        }

        character = static_cast<char>(key);
    }
    while (character != 'q');

    cockpit.stop();

    // Clear exit text
    term::set_cursor(n_lines, 1);
    term::clear_line();

    std::cout << std::flush;

    return 0;
}

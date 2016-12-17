# 03-lambda-stuff
Small implementation of lambdas in C++03. Modeled after Args, Vals, Refs, and Functions from Boost.Phoenix.

## Example usage:

```cpp
size_t count_matching_substrings(const std::vector<std::string> &strs, const std::string &substring)
{
    return std::count_if(strs.begin(), strs.end(),
                         NotEq(Find(arg1,
                                    substring,
                                    0),
                               std::string::npos));
}

std::vector<std::string> find_strings_of_length(const std::vector<std::string &strs, size_t length)
{
    std::vector<std::string> found;
    
    // Whoops, that's C++11.
    std::copy_if(strs.begin(), strs.end(), 
                 std::back_inserter(found),
                 Eq(Length(arg1)
                    length));
    return found;
}

```

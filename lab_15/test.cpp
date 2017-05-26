#include <gtest/gtest.h>
#include "linq.h"


TEST(until, until_predicate) {
    std::vector<int> xs = {1, 2, 3};
    std::vector<int> res(2);
    std::vector<int> ans = {1, 2};

    from(xs.begin(), xs.end())
            .until([](int x) { return x % 3 == 0; })
            .copy_to(res.begin());
    ASSERT_EQ(res, ans);
}

TEST(until, until_eq) {
    std::vector<int> xs = {1, 2, 3, 4, 4};
    std::vector<int> ans = {1, 2, 3};

    std::vector<int> res = from(xs.begin(), xs.end())
            .until_eq(4)
            .to_vector();
    ASSERT_EQ(res, ans);
}

TEST(take, take) {
    std::vector<int> xs = {1, 2, 3, 4, 4};
    std::vector<int> ans = {1, 2};

    std::vector<int> res = from(xs.begin(), xs.end())
            .take(2)
            .to_vector();
    ASSERT_EQ(res, ans);
    std::vector<int> res2 = from(xs.begin(), xs.end())
            .take(0)
            .to_vector();
    ASSERT_EQ(res2, std::vector<int>());
}

TEST(drop, drop) {
    std::vector<int> xs = {1, 2, 3, 4, 4};
    std::vector<int> ans = {3, 4, 4};

    std::vector<int> res = from(xs.begin(), xs.end())
            .drop(2)
            .to_vector();
    ASSERT_EQ(res, ans);
    std::vector<int> res2 = from(xs.begin(), xs.end())
            .drop(0)
            .to_vector();
    ASSERT_EQ(res2, xs);
}

TEST(where, where_predicate) {
    std::vector<int> xs = {1, 2, 3, 4, 4};
    std::vector<int> ans = {2, 4, 4};

    std::vector<int> res = from(xs.begin(), xs.end())
            .where([](int x) { return x % 2 == 0; })
            .to_vector();
    ASSERT_EQ(res, ans);
}

TEST(where, where_neq) {
    std::vector<int> xs = {4, 2, 3, 4, 4, 5};
    std::vector<int> ans = {2, 3, 5};

    std::vector<int> res = from(xs.begin(), xs.end())
            .where_neq(4)
            .to_vector();
    ASSERT_EQ(res, ans);
}

TEST(select, factorial) {
    class Factorial{
    public:
        int operator()(int x){
            if (x == 1)
                return 1;
            return x * Factorial()(x - 1);
        }
    };
    std::vector<int> xs = {1, 2, 3, 4, 5};
    std::vector<int> ans = {1, 2, 6, 24, 120};

    std::vector<int> res = from(xs.begin(), xs.end())
            .select<int>(Factorial())
            .to_vector();
    ASSERT_EQ(res, ans);
}

TEST(select, convert_to_bool) {
    std::vector<int> xs = {-1, -2, 0, 4, 5};
    std::vector<bool> ans = {1, 1, 0, 1, 1};

    std::vector<bool> res = from(xs.begin(), xs.end())
            .select<bool>([](int x) { return x;})
            .to_vector();
    ASSERT_EQ(res, ans);
}
template <class T>
class My{
public:
    My() { x = 0; }
    T x;
    bool operator==(My<T> other) const { return x == other.x; }
};

TEST(select, my_class) {
    My<int> my;
    std::vector<My<int>> xs = {my, my, my};
    std::vector<My<int>> ans = {my, my, my};

    std::vector<My<int>> res = from(xs.begin(), xs.end())
            .select<My<int>>([](My<int> i) { return i; })
            .to_vector();
    ASSERT_EQ(res, ans);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

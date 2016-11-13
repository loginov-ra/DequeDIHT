#pragma once
#include <gtest/gtest.h>
#include "Deque.h"
#include <deque>
#include <vector>
#include <algorithm>

using std::deque;
using std::vector;

const int N_RANDOM_OPERATIONS = 100000;
const int ITERATOR_TEST_DEQUE_SIZE = 100000;
const int N_ITERATOR_TESTS = 300;
const int MAX_STEP = 5;

struct Point
{
    int x;
    int y;

    Point(int new_x = 0, int new_y = 0):
        x(new_x),
        y(new_y)
    
    {}

    bool operator == (const Point& that) const
    {
        return (x == that.x && y == that.y);
    }

    bool operator != (const Point& that) const
    {
        return !(*this == that);
    }

    double distanceToZero()
    {
        int sqr_res = x * x + y * y;
        return sqrt(sqr_res);
    };
};

class PointDequeTest : public testing::Test
{
protected:
    Deque<Point> own_deque;
    deque<Point> std_deque;

    void SetUp() {}
    void TearDown() {}

    testing::AssertionResult areEqual() const
    {
        if (own_deque.size() != std_deque.size())
            return testing::AssertionFailure() << "Deques have different size\n";

        for (size_t i = 0; i < own_deque.size(); i++)
        {
            if (own_deque[i]  != std_deque[i])
                return testing::AssertionFailure() << "Deques differ in position " << i << "\n";
        }

        return testing::AssertionSuccess();
    }

    void makeOperation(bool push, bool back, Point element)
    {
        if (push)
        {
            if (back)
            {
                std_deque.push_back(element);
                own_deque.push_back(element);
            }
            else
            {
                std_deque.push_front(element);
                own_deque.push_front(element);
            }
        }
        else
        {
            if (own_deque.size() == 0)
                return;

            if (back)
            {
                std_deque.pop_back();
                own_deque.pop_back();
            }
            else
            {
                std_deque.pop_front();
                own_deque.pop_front();
            }
        }
    }
};

class IteratorDequeTest : public PointDequeTest
{
protected:
    void SetUp()
    {
        for (int i = 0; i < ITERATOR_TEST_DEQUE_SIZE; i++)
        {
            Point pt(rand(), rand());
            std_deque.push_back(pt);
            own_deque.push_back(pt);
        }
    }

    void TearDown() {}

    template <class Iterator>
    vector<Point> getRangeResult(Iterator start, Iterator finish, int step)
    {
        vector<Point> res;
        for (Iterator it = start; it < finish; it++)
        {
            res.push_back(*it);
        }
        return res;
    }

    template <class StdIterator, class OwnIterator>
    testing::AssertionResult checkRange(OwnIterator own_start, OwnIterator own_finish,
                                        StdIterator std_start, StdIterator std_finish, int step)
    {
        vector<Point> own_res = getRangeResult(own_start, own_finish, step);
        vector<Point> std_res = getRangeResult(std_start, std_finish, step);

        if (own_res.size() != std_res.size())
            return testing::AssertionFailure() << "Range sizes are different\n";

        for (size_t i = 0; i < own_res.size(); i++)
        {
            if (own_res[i] != std_res[i])
                return testing::AssertionFailure() << "Range differ in position " << i << "\n";
        }

        return testing::AssertionSuccess();
    }
};

TEST_F(PointDequeTest, RandomOperationsTest)
{
    for (int test_i = 0; test_i < N_RANDOM_OPERATIONS; test_i++)
    {
        makeOperation(rand() % 2, rand() % 2, Point(rand(), rand()));
        ASSERT_TRUE(areEqual()) << "Random test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, Iterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; test_i++)
    {
        ASSERT_TRUE(checkRange(own_deque.begin(), own_deque.end(),
                               std_deque.begin(), std_deque.end(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ReverseIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; test_i++)
    {
        ASSERT_TRUE(checkRange(own_deque.rbegin(), own_deque.rend(),
                               std_deque.rbegin(), std_deque.rend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ConstIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; test_i++)
    {
        ASSERT_TRUE(checkRange(own_deque.cbegin(), own_deque.cend(),
                               std_deque.cbegin(), std_deque.cend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ConstReverseIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; test_i++)
    {
        ASSERT_TRUE(checkRange(own_deque.crbegin(), own_deque.crend(),
                               std_deque.crbegin(), std_deque.crend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(PointDequeTest, AssignationTest)
{
    for (int i = 0; i < N_RANDOM_OPERATIONS; i++)
        makeOperation(true, rand() % 2, Point(rand(), rand()));

    for (int i = 0; i < own_deque.size(); i++)
    {
        *(own_deque.begin() + i) = Point(i, (own_deque.begin() + i)->y);
        *(std_deque.begin() + i) = Point(i, (std_deque.begin() + i)->y);
    }

    ASSERT_TRUE(areEqual());
}

TEST_F(PointDequeTest, AlgorithmOperationsTest)
{
    for (int i = 0; i < N_RANDOM_OPERATIONS; i++)
    {
        Point pt(rand(), rand());
        std_deque.push_front(pt);
        own_deque.push_back(pt);
    }

    std::reverse(own_deque.begin(), own_deque.end());

    ASSERT_TRUE(areEqual());
}
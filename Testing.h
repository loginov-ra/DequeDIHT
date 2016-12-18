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

    Point(int newX = 0, int newY = 0):
        x(newX),
        y(newY)
    
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
        int sqrRes = x * x + y * y;
        return sqrt(sqrRes);
    };
};

class PointDequeTest : public testing::Test
{
protected:
    Deque<Point> ownDeque;
    deque<Point> stdDeque;

    void SetUp() {}
    void TearDown() {}

    testing::AssertionResult areEqual() const
    {
        if (ownDeque.size() != stdDeque.size())
            return testing::AssertionFailure() << "Deques have different size\n";

        for (size_t i = 0; i < ownDeque.size(); ++i)
        {
            if (ownDeque[i]  != stdDeque[i])
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
                stdDeque.push_back(element);
                ownDeque.push_back(element);
            }
            else
            {
                stdDeque.push_front(element);
                ownDeque.push_front(element);
            }
        }
        else
        {
            if (ownDeque.size() == 0)
                return;

            if (back)
            {
                stdDeque.pop_back();
                ownDeque.pop_back();
            }
            else
            {
                stdDeque.pop_front();
                ownDeque.pop_front();
            }
        }
    }
};

class IteratorDequeTest : public PointDequeTest
{
protected:
    void SetUp()
    {
        for (int i = 0; i < ITERATOR_TEST_DEQUE_SIZE; ++i)
        {
            Point pt(rand(), rand());
            stdDeque.push_back(pt);
            ownDeque.push_back(pt);
        }
    }

    void TearDown() {}

    template <class Iterator>
    vector<Point> getRangeResult(Iterator start, Iterator finish, int step)
    {
        vector<Point> res;
        for (Iterator it = start; it < finish; ++it)
        {
            res.push_back(*it);
        }
        return res;
    }

    template <class StdIterator, class OwnIterator>
    testing::AssertionResult checkRange(OwnIterator ownStart, OwnIterator ownFinish,
                                        StdIterator stdStart, StdIterator stdFinish, int step)
    {
        vector<Point> ownRes = getRangeResult(ownStart, ownFinish, step);
        vector<Point> stdRes = getRangeResult(stdStart, stdFinish, step);

        if (ownRes.size() != stdRes.size())
            return testing::AssertionFailure() << "Range sizes are different\n";

        for (size_t i = 0; i < ownRes.size(); ++i)
        {
            if (ownRes[i] != stdRes[i])
                return testing::AssertionFailure() << "Range differ in position " << i << "\n";
        }

        return testing::AssertionSuccess();
    }
};

TEST_F(PointDequeTest, RandomOperationsTest)
{
    for (int test_i = 0; test_i < N_RANDOM_OPERATIONS; ++test_i)
    {
        makeOperation(rand() % 2, rand() % 2, Point(rand(), rand()));
        ASSERT_TRUE(areEqual()) << "Random test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, Iterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; ++test_i)
    {
        ASSERT_TRUE(checkRange(ownDeque.begin(), ownDeque.end(),
                               stdDeque.begin(), stdDeque.end(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ReverseIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; ++test_i)
    {
        ASSERT_TRUE(checkRange(ownDeque.rbegin(), ownDeque.rend(),
                               stdDeque.rbegin(), stdDeque.rend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ConstIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; ++test_i)
    {
        ASSERT_TRUE(checkRange(ownDeque.cbegin(), ownDeque.cend(),
                               stdDeque.cbegin(), stdDeque.cend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(IteratorDequeTest, ConstReverseIterator)
{
    for (int test_i = 0; test_i < N_ITERATOR_TESTS; ++test_i)
    {
        ASSERT_TRUE(checkRange(ownDeque.crbegin(), ownDeque.crend(),
                               stdDeque.crbegin(), stdDeque.crend(), rand() % (MAX_STEP - 1) + 1)) << "Range test failed in test " << test_i << "\n";
    }
}

TEST_F(PointDequeTest, AssignationTest)
{
    for (int i = 0; i < N_RANDOM_OPERATIONS; ++i)
        makeOperation(true, rand() % 2, Point(rand(), rand()));

    for (int i = 0; i < ownDeque.size(); ++i)
    {
        *(ownDeque.begin() + i) = Point(i, (ownDeque.begin() + i)->y);
        *(stdDeque.begin() + i) = Point(i, (stdDeque.begin() + i)->y);
    }

    ASSERT_TRUE(areEqual());
}

TEST_F(PointDequeTest, AlgorithmOperationsTest)
{
    for (int i = 0; i < N_RANDOM_OPERATIONS; ++i)
    {
        Point pt(rand(), rand());
        stdDeque.push_front(pt);
        ownDeque.push_back(pt);
    }   

    std::reverse(ownDeque.begin(), ownDeque.end());

    ASSERT_TRUE(areEqual());    
}
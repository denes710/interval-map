#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <interval_map.hpp>

// ---------------------------------------------------------------------------------------------------------------------
class CTestKeyType
{
    public:
        CTestKeyType(unsigned p_val)
            : m_val(p_val)
        {}

        bool operator==(const CTestKeyType&) const = delete;
        bool operator<(const CTestKeyType& p_other) const
        { return m_val < p_other.m_val; }

    private:
        friend std::ostream& operator<<(std::ostream&, const CTestKeyType&);
        unsigned m_val;
};

std::ostream& operator<<(std::ostream& p_os, const CTestKeyType& p_value)
{
    p_os << p_value.m_val;
    return p_os;
}

// ---------------------------------------------------------------------------------------------------------------------
class CTestValueType
{
    public:
        CTestValueType(char p_val)
            : m_val(p_val)
        {}

        bool operator<(const CTestValueType&) const = delete;
        bool operator==(const CTestValueType& p_other) const
        { return m_val == p_other.m_val; }

    private:
        friend std::ostream& operator<<(std::ostream&, const CTestValueType&);
        char m_val;
};

std::ostream& operator<<(std::ostream& p_os, const CTestValueType& p_value)
{
    p_os << p_value.m_val;
    return p_os;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(EmptyRange)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(3, 3, 'B');
    BOOST_CHECK_EQUAL(m.count(3), 0);
    BOOST_CHECK_EQUAL(m.size(), 0);

    m.assign(3, 2, 'B');
    BOOST_CHECK_EQUAL(m.count(2), 0);
    BOOST_CHECK_EQUAL(m.count(3), 0);
    BOOST_CHECK_EQUAL(m.size(), 0);
}


// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TrivialRange)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 10, 'B');
    BOOST_CHECK_EQUAL(m[0], 'A');
    for (int i = 1; i < 10; i++)
    {
        BOOST_CHECK_EQUAL(m[i], 'B');
    }
    BOOST_CHECK_EQUAL(m[10], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TrivialTwoRange)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 3, 'B');
    m.assign(6, 8, 'C');
    BOOST_CHECK_EQUAL(m.size(), 4);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'A');
    BOOST_CHECK_EQUAL(m[4], 'A');
    BOOST_CHECK_EQUAL(m[5], 'A');
    BOOST_CHECK_EQUAL(m[6], 'C');
    BOOST_CHECK_EQUAL(m[7], 'C');
    BOOST_CHECK_EQUAL(m[8], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverwriteLowest)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(0, 10000, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'B');
    BOOST_CHECK_EQUAL(m[9999], 'B');
    BOOST_CHECK_EQUAL(m[10000], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Merge)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(0, 10, 'B');
    m.assign(10, 20, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'B');
    BOOST_CHECK_EQUAL(m[10], 'B');
    BOOST_CHECK_EQUAL(m[19], 'B');
    BOOST_CHECK_EQUAL(m[20], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverlappingRangeComplete)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(3, 5, 'B');
    m.assign(1, 6, 'C');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'C');
    BOOST_CHECK_EQUAL(m[2], 'C');
    BOOST_CHECK_EQUAL(m[3], 'C');
    BOOST_CHECK_EQUAL(m[4], 'C');
    BOOST_CHECK_EQUAL(m[5], 'C');
    BOOST_CHECK_EQUAL(m[6], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverlappingRangeInner)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 6, 'C');
    m.assign(3, 5, 'B');
    BOOST_CHECK_EQUAL(m.size(), 4);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'C');
    BOOST_CHECK_EQUAL(m[2], 'C');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'C');
    BOOST_CHECK_EQUAL(m[6], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverlappingRangeSmallToLarge)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(3, 6, 'C');
    BOOST_CHECK_EQUAL(m.size(), 3);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'C');
    BOOST_CHECK_EQUAL(m[4], 'C');
    BOOST_CHECK_EQUAL(m[5], 'C');
    BOOST_CHECK_EQUAL(m[6], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverlappingRangeLargeToSmall)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(3, 6, 'C');
    m.assign(1, 5, 'B');
    BOOST_CHECK_EQUAL(m.size(), 3);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'C');
    BOOST_CHECK_EQUAL(m[6], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(TestingEverything)
{
    interval_map<int, char> m('A');
    m.assign(1, 5, 'C');
    m.assign(2, 3, 'B');
    m.assign(7, 8, 'E');
    m.assign(-2, 0, 'C');
    BOOST_CHECK_EQUAL(m.size(), 8);
    BOOST_CHECK_EQUAL(m[-3], 'A');
    BOOST_CHECK_EQUAL(m[-2], 'C');
    BOOST_CHECK_EQUAL(m[-1], 'C');
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'C');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'C');
    BOOST_CHECK_EQUAL(m[4], 'C');
    BOOST_CHECK_EQUAL(m[5], 'A');
    BOOST_CHECK_EQUAL(m[6], 'A');
    BOOST_CHECK_EQUAL(m[7], 'E');
    BOOST_CHECK_EQUAL(m[8], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ExtendingRangeBegin)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(3, 5, 'B');
    m.assign(1, 4, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ExtendingRangeEnd)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(3, 6, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'B');
    BOOST_CHECK_EQUAL(m[6], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ExtendingRangeBothBeginEnd)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(2, 3, 'B');
    m.assign(1, 5, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(OverwriteEndValueSafety)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(2, 5, 'B');
    m.assign(5, 8, 'C');
    m.assign(4, 5, 'A');
    BOOST_CHECK_EQUAL(m.size(), 4);
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ReusingExistingRangeBothBeginEnd)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(2, 3, 'B');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'B');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(ReusingEnd)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(4, 6, 'A');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'B');
    BOOST_CHECK_EQUAL(m[2], 'B');
    BOOST_CHECK_EQUAL(m[3], 'B');
    BOOST_CHECK_EQUAL(m[4], 'A');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RestoringInitial)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(1, 5, 'A');
    BOOST_CHECK_EQUAL(m.size(), 0);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'A');
    BOOST_CHECK_EQUAL(m[2], 'A');
    BOOST_CHECK_EQUAL(m[3], 'A');
    BOOST_CHECK_EQUAL(m[4], 'A');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RestoringInitial2)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(1, 5, 'B');
    m.assign(0, 7, 'A');
    BOOST_CHECK_EQUAL(m.size(), 0);
    BOOST_CHECK_EQUAL(m[0], 'A');
    BOOST_CHECK_EQUAL(m[1], 'A');
    BOOST_CHECK_EQUAL(m[2], 'A');
    BOOST_CHECK_EQUAL(m[3], 'A');
    BOOST_CHECK_EQUAL(m[4], 'A');
    BOOST_CHECK_EQUAL(m[5], 'A');
}

// ---------------------------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(VeryComplex)
{
    interval_map<CTestKeyType, CTestValueType> m('A');
    m.assign(3, 6, 'B');
    m.assign(2, 5, 'C');
    m.assign(4, 7, 'A');
    BOOST_CHECK_EQUAL(m.size(), 2);
    BOOST_CHECK_EQUAL(m[1], 'A');
    BOOST_CHECK_EQUAL(m[2], 'C');
    BOOST_CHECK_EQUAL(m[3], 'C');
    BOOST_CHECK_EQUAL(m[4], 'A');
    BOOST_CHECK_EQUAL(m[5], 'A');
    BOOST_CHECK_EQUAL(m[6], 'A');
    BOOST_CHECK_EQUAL(m[7], 'A');
}

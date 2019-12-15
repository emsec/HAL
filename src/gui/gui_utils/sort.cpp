#include "gui_utils/sort.h"

#include "def.h"

#include <assert.h>

namespace gui_utility
{

    bool natural_order_compare(const QString& a, const QString& b)
    {
        const QChar* a_it = a.constBegin();
        const QChar* b_it = b.constBegin();
        QString a_num, b_num;
        bool number_mode = false;
        while (!a_it->isNull() && !b_it->isNull())
        {
            // read the entire number of each string since we want to be safe
            // against leading zeros: 1 < 0002
            if (a_it->isNumber() || b_it->isNumber())
            {
                number_mode = true;
                if (a_it->isNumber())
                {
                    a_num += *a_it;
                    ++a_it;
                }
                if (b_it->isNumber())
                {
                    b_num += *b_it;
                    ++b_it;
                }
            }
            else
            {
                if (number_mode)
                {
                    int diff = numeric_string_compare(a_num, b_num);
                    if (diff) // keep looking if both numbers are equal
                    {
                        return diff > 0;
                    }
                    // reset number cache
                    a_num.clear();
                    b_num.clear();
                    number_mode = false;
                    // no iterator increment here, this block just evaluates
                    // the result of number mode
                }
                else if (*a_it != *b_it)
                {
                    return *a_it > *b_it;
                }
                else
                {
                    ++a_it;
                    ++b_it;
                }
            }
        }
        // if strings have equal length and end with numbers, compare the
        // numbers
        if (a_it->isNull() && b_it->isNull() && number_mode)
        {
            int diff = numeric_string_compare(a_num, b_num);
            if (diff) // keep looking if both numbers are equal
            {
                return diff > 0;
            }
        }
        return b_it->isNull(); // is b shorter than a?
    }

    bool lexical_order_compare(const QString& a, const QString& b)
    {
        return a > b;
    }

    int numeric_string_compare(QString a_num, QString b_num)
    {
        // this allows us to compare any length of number, regardless of
        // any integer limits

        // zero-pad to same length (we don't care if the strings themselves
        // contain any leading zeros, this still works)
        if (a_num.size() < b_num.size())
        {
            a_num = a_num.rightJustified(b_num.size(), '0');
        }
        else
        {
            b_num = b_num.rightJustified(a_num.size(), '0');
        }
        return a_num.compare(b_num);
    }

    int compare(sort_mechanism mechanism, QString a, QString b)
    {
        switch(mechanism)
        {
        case lexical:
            return lexical_order_compare(a, b);
        case natural:
            return natural_order_compare(a, b);
        }
        assert(false);
    }


} // namespace gui_utility

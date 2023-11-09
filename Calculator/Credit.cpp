#include "Credit.h"

namespace Calculator
{

Payment::Payment(double value, QDate date, PaymentType type, double percents, double sum_before_payment)
    : type(type)
    , date(date)
    , value(value)
    , percents(percents)
    , sum_before_payment(sum_before_payment)
{}


} // namespace Calculator



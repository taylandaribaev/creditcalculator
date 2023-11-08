#pragma once

#include "Credit.h"
#include "Fwd.h"

namespace Calculator
{

class CreditCalculator
{
public:
    static ResultsPtr calculate(const CreditInfo &info, std::optional<Payments> additional_payments = std::nullopt);

};

} // namespace Calculator

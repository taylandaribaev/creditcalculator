#pragma once

#include <memory>
#include <vector>

namespace Calculator
{

struct Payment;
using Payments = std::vector<Payment>;

struct Results;
using ResultsPtr = std::shared_ptr<Results>;

}


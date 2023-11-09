#include "CreditCalculator.h"

#include <math.h>

namespace Calculator
{

namespace
{

std::vector<Payment> prepare_payments(const CreditInfo & info, std::optional<Payments> additional_payments)
{
    std::vector<Payment> payments;
    for (int i = 0; i < info.credit_term; ++i)
    {
        payments.push_back(Payment(0.0, info.start_date.addMonths(i)));
    }

    if (!additional_payments.has_value())
        return std::move(payments);

    for (const auto payment : additional_payments.value())
    {
        payments.push_back(payment);
    }

    std::sort(payments.begin(), payments.end(), [](const Payment & p1, const Payment & p2){
        if (p1.date != p2.date)
        {
            return p1.date < p2.date;
        }
        else
        {
            return p1.type < p2.type;
        }
    });

    return std::move(payments);
}

}

// Differential payments
namespace DifferencialPayments
{

ResultsPtr calculate(const CreditInfo & info, std::optional<Payments> additional_payments)
{
    ResultsPtr results (new Results);
    results->info = info;
    results->percents_summ = 0;

    auto payments = prepare_payments(info, additional_payments);

    double total_summ = info.total_summ;
    auto previews_date = info.start_date;

    int usual_payment = 0;

    for (Payment payment : payments)
    {
        auto date = payment.date;
        double percents_per_day = total_summ * info.percent / date.daysInYear();
        int days = previews_date.daysTo(date);
        previews_date = date;
        double percents_value = percents_per_day * days;

        switch (payment.type) {
        case Payment::PaymentType::Usual:
            payment.value = (total_summ / (info.credit_term - usual_payment)) + percents_value;
            total_summ -= payment.value;
            usual_payment++;
            break;
        case Payment::PaymentType::ForPayment:
        case Payment::PaymentType::ForTerm:
            total_summ += percents_value - payment.value;
            break;
        }
        payment.percents = percents_value;
        results->payments.push_back(payment);
    }




//    for (int i = 0; i < info.credit_term; ++i)
//    {
//        auto date = info.start_date.addMonths(i);




//        auto payment = (total_summ / (info.credit_term - i)) + percents_value;

//        results->payments.push_back(Payment(payment, date, Payment::PaymentType::Usual, percents_value, total_summ));
//        total_summ -= payment;
//    }

    results->percents_summ = 0;
    for (const auto payment : results->payments)
    {
        results->percents_summ += payment.percents;
    }

    return results;
}

}

/// Fixed payments
namespace FixedPayments
{

double payment_value(double summ, double percent, int credit_term)
{
    double annuitet_month_rate = percent / 12;

    double annuitet_coef = annuitet_month_rate * std::pow(1.0 + annuitet_month_rate, credit_term) / (std::pow(1 + annuitet_month_rate, credit_term) - 1);
    return annuitet_coef * summ;
}

double payment_value(const CreditInfo & info)
{
    return payment_value(info.total_summ, info.percent, info.credit_term);
}

ResultsPtr calculate(const CreditInfo & info, std::optional<Payments> additional_payments)
{
    ResultsPtr results (new Results);
    results->info = info;
    results->percents_summ = 0;

    auto all_payments = prepare_payments(info, additional_payments);

    double sum_before_payment = info.total_summ;

    auto monthly_payment = payment_value(info);

    auto last_months = info.credit_term;

    for (Payment payment : all_payments)
    {
        payment.sum_before_payment = sum_before_payment;
        QDate previews_date = results->payments.size() > 0
                                  ? results->payments.back().date
                                  : info.start_date;
        auto days = previews_date.daysTo(payment.date);

        if (payment.type == Payment::PaymentType::Usual)
        {
            last_months--;
            payment.value = monthly_payment;
        }

        double percents_value = 0;
        if (days == 0)
        {
            percents_value = 0;
        }
        else
        {
            percents_value = sum_before_payment * info.percent / 12;

            if (payment.type != Payment::PaymentType::Usual)
            {
                auto d = payment.date.daysInMonth();
                percents_value = days * percents_value / d;
            }
        }
        payment.percents = percents_value;
        sum_before_payment += percents_value - payment.value;

        if (payment.type == Payment::PaymentType::ForPayment)
        {
            monthly_payment = payment_value(sum_before_payment, info.percent, last_months);
        }

        if (sum_before_payment <= 0)
            break;

        results->payments.push_back(payment);
    }

    results->percents_summ = 0;
    for (const auto payment : results->payments)
    {
        results->percents_summ += payment.percents;
    }

    return results;
}

}


ResultsPtr CreditCalculator::calculate(const CreditInfo & info, std::optional<Payments> additional_payments)
{
    switch (info.type) {
    case CreditInfo::CreditType::Fixed:
        return FixedPayments::calculate(info, additional_payments);
    default:
        return DifferencialPayments::calculate(info, additional_payments);
    }

    return nullptr;
}

} // namespace Calculator

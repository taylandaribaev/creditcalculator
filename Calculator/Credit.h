#pragma once

#include <QtCore/QDate>
#include <QtCore/QJsonObject>
#include <QtCore/QDebug>

#include <optional>

namespace Calculator
{

struct Payment
{
    enum class PaymentType
    {
        /// Usual monthly payment
        Usual = 0,
        /// Payment for reduce term of credit
        ForTerm = 1,
        /// Payment for reduce payment of credit
        ForPayment = 2
    };

    Payment() = default;

    Payment(
        double value,
        QDate date,
        PaymentType type = PaymentType::Usual,
        double percents = 0.0,
        double sum_before_payment = 0.0);

    /// Payment type
    PaymentType type { PaymentType::Usual };

    /// Date of current payment
    QDate date;

    /// Payment
    double value {0.0};

    /// Calculated percents in current payment
    double percents {0.0};

    /// Debt balance at the beginning of the period
    double sum_before_payment {0.0};
};

struct CreditInfo
{
    enum class CreditType
    {
        /// With fixed payments
        Fixed,
        /// With differential payments
        Differencial,
    };

    /// Type of payments
    CreditType type { CreditType::Fixed };

    /// Totall summ of credit
    double total_summ {0.0};

    /// Percent of credit
    double percent {0.0};

    /// Date of start of payments
    QDate start_date;

    /// Term of credit in months
    int credit_term {0};
};

struct Results
{
    /// Information of credit
    CreditInfo info;

    /// All payments
    std::vector<Payment> payments;

    /// Amount of overpayment for credit
    double percents_summ {0.0};
};

} // namespace Calculator

QString uiPrice(double price,
                int decimals = 2,
                QLocale locale = QLocale(QLocale::Russian, QLocale::AnyCountry),
                bool removeZeroDecimalsPart = true);

QDebug operator<<(QDebug debug, const Calculator::Payment & payment);

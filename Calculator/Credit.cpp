#include "Credit.h"

#include <QtCore/QRegularExpression>

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

QString uiPrice(double price, int decimals, QLocale locale, bool removeZeroDecimalsPart)
{
    QString result = locale.toString(price, 'f', decimals);

    if (removeZeroDecimalsPart)
    {
        result.remove(QRegularExpression("[\\.,]0+$"));
    }

    return result;
}

QDebug operator<<(QDebug debug, const Calculator::Payment &payment)
{
    QDebugStateSaver saver(debug);

    QString payment_type = "PaymentType::Usual";
    switch (payment.type) {
    case Calculator::Payment::PaymentType::ForPayment:
        payment_type = "PaymentType::ForPayment";
        break;
    case Calculator::Payment::PaymentType::ForTerm:
        payment_type = "PaymentType::ForTerm";
        break;
    case Calculator::Payment::PaymentType::Usual:
        break;
    }

    debug << "Date:" << payment.date
            << "Before payment:" << uiPrice(payment.sum_before_payment) << "\t"
            << "Value:" << uiPrice(payment.value) << "\t"
            << "Percents:" << uiPrice(payment.percents) << "\t"
            << "Payment type:" << payment_type;

    return debug;
}

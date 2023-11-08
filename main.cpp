#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Calculator/CreditCalculator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/CreditCalc/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    Calculator::CreditInfo info;

    info.total_summ = 5000000.0;
    info.percent = 0.129;
    info.start_date = QDate(2022, 12, 20);
    info.credit_term = 60;

    Calculator::Payments additional_payments;
    for (int i = 1; i < 11; ++i)
    {
        additional_payments.push_back(Calculator::Payment(36490, info.start_date.addMonths(i), Calculator::Payment::PaymentType::ForTerm));
    }

//    additional_payments.push_back(Calculator::Payment(2000000, QDate(2023, 12, 20), Calculator::Payment::PaymentType::ForTerm));
    additional_payments.push_back(Calculator::Payment(400000, QDate(2023, 12, 20), Calculator::Payment::PaymentType::ForPayment));

//    for (int i = 0; i < 10; ++i)
//    {
//        additional_payments.push_back(Calculator::Payment(86490, additional_payments.back().date.addMonths(1), Calculator::Payment::PaymentType::ForTerm));
//    }

    double discount = 0;

    {
        auto results = Calculator::CreditCalculator::calculate(info);

        qDebug() << "Overpayment:" << uiPrice(results->percents_summ) << "Count of usual payments: " << results->payments.size();
        discount = results->percents_summ;
    }

    {
        auto results = Calculator::CreditCalculator::calculate(info, additional_payments);
        int payments_count = 0;
        for (auto payment : results->payments)
        {
            qDebug() << payment;
            if (payment.type == Calculator::Payment::PaymentType::Usual)
            {
                payments_count++;
            }
        }
        discount -= results->percents_summ;
        qDebug() << "Overpayment:" << uiPrice(results->percents_summ) << "Count of usual payments: " << payments_count;
        qDebug() << "Discount after additional payments:" << uiPrice(discount);
    }

    return app.exec();
}

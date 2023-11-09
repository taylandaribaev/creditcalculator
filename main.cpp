#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QRegularExpression>

#include "Calculator/CreditCalculator.h"

QString uiPrice(double price,
                int decimals = 2,
                QLocale locale = QLocale(QLocale::Russian, QLocale::AnyCountry),
                bool removeZeroDecimalsPart = true)
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

    {
        info.type = Calculator::CreditInfo::CreditType::Differencial;
        auto results = Calculator::CreditCalculator::calculate(info);
        if (results) {
            qDebug() << "Overpayment:" << uiPrice(results->percents_summ) << "Count of usual payments: " << results->payments.size();

            for (auto payment : results->payments)
            {
                qDebug() << payment;
            }
        }
    }

    {
        info.type = Calculator::CreditInfo::CreditType::Fixed;
        auto results = Calculator::CreditCalculator::calculate(info);
        if (results) {
            qDebug() << "Overpayment:" << uiPrice(results->percents_summ) << "Count of usual payments: " << results->payments.size();
        }
    }

    return app.exec();
}

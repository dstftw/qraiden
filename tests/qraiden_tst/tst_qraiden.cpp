#include <QtTest>
#include <QCoreApplication>

#include <QRaiden>

class tst_QRaiden : public QObject
{
    Q_OBJECT

public:
    tst_QRaiden();

private Q_SLOTS:
    void encryptDecrypt_data();
    void encryptDecrypt();

private:
    static quint32 key[4];
};

quint32 tst_QRaiden::key[4] = { 0x13859889, 0xef54ef32, 0x939039f4, 0x12ac340f };

tst_QRaiden::tst_QRaiden()
{
}

void tst_QRaiden::encryptDecrypt_data()
{
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("null string") << QString().toUtf8();
    QTest::newRow("empty string") << QString("").toUtf8();
    QTest::newRow("test") << QString("test").toUtf8();
    QTest::newRow("testtest") << QString("testtest").toUtf8();
    QTest::newRow("\u0000 ") << QString("\u0000").toUtf8();
    QTest::newRow("\u4242") << QString("\u4242").toUtf8();
    QTest::newRow("\uffff") << QString("\uffff").toUtf8();
    QTest::newRow("test\u4242") << QString("test\u4242").toUtf8();
}

void tst_QRaiden::encryptDecrypt()
{
    QFETCH(QByteArray, data);

    int expectedLen = ((data.size() / 8) + 1) * 8;

    const QByteArray encrypted = QRaiden::encrypt(data, key);
    QCOMPARE(encrypted.size(), expectedLen);

    const QByteArray decrypted = QRaiden::decrypt(encrypted, key);

    QCOMPARE(data, decrypted);
}

#include "tst_qraiden.moc"
QTEST_MAIN(tst_QRaiden)

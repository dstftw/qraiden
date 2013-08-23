#ifndef QRAIDEN_H
#define QRAIDEN_H

#include "qraiden_global.h"

class QRAIDEN_EXPORT QRaiden
{
    QRaiden();
    Q_DISABLE_COPY(QRaiden)
public:
    static QByteArray encrypt(const QByteArray &inputData, const quint32 (&key)[4]);
    static QByteArray encrypt(const QString &inputText, const quint32 (&key)[4]);

    static QByteArray decrypt(const QByteArray &cipherData, const quint32 (&key)[4]);
    static QString decryptAsString(const QByteArray &cipherData, const quint32 (&key)[4]);

private:
    static void encode(const quint32 key[4], const quint32 data[2], quint32 result[2]);
    static void decode(const quint32 key[4], const quint32 data[2], quint32 result[2]);

    static const quint8 block = 8;
};

#endif // QRAIDEN_H

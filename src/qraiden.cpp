#include <QByteArray>
#include <QString>

#include "qraiden.h"

QByteArray QRaiden::encrypt(const QByteArray &inputData, const quint32 (&key)[4])
{
    QByteArray dst;

    quint32 data[2], encoded[2];

    quint32 srcLen = inputData.size();
    quint32 len = srcLen;

    quint8 padding = 0x0;
    if (len % block == 0) {
        len += block;
        padding = block;
    } else {
        padding = block - len % block;
    }

    for (quint32 i = 0; i < len; i += block) {
        data[0] = data[1] = 0;
        for (quint8 p = 0; p < 2; ++p) {
            for (quint8 j = 0; j < 4; ++j) {
                quint32 idx = i + j + (p * 4);
                data[p] |= (idx < srcLen ? static_cast<quint8>(inputData.at(idx)) : padding) << (j * block);
            }
        }

        encode(key, data, encoded);

        for (quint8 p = 0; p < 2; ++p) {
            for (quint8 j = 0; j < 4; ++j)
                dst.append(static_cast<quint8>(encoded[p] >> (j * block) & 0xff));
        }
    }

    return dst;
}

QByteArray QRaiden::encrypt(const QString &inputText, const quint32 (&key)[4])
{
    return encrypt(inputText.toUtf8(), key);
}

QByteArray QRaiden::decrypt(const QByteArray &cipherData, const quint32 (&key)[4])
{
    quint32 srcLen = cipherData.size();

    if (srcLen < block || srcLen % block > 0) {
        qWarning("QRaiden::decrypt(): invalid cipher data");
        return QByteArray();
    }

    QByteArray dst;
    quint32 data[2], decoded[2];


    for (quint32 i = 0; i < srcLen; i += block) {
        data[0] = data[1] = 0;
        for (quint8 p = 0; p < 2; p++) {
            for (quint8 j = 0; j < 4; j++) {
                data[p] |= static_cast<quint8>(cipherData.at(i + j + (p * 4))) << (j * block);
            }
        }

        decode(key, data, decoded);

        for (int p = 0; p < 2; p++) {
            for (int j = 0; j < 4; j++)
                dst.append(static_cast<quint8>((decoded[p] >> (j * block)) & 0xff));
        }
    }

    quint8 padding = dst.at(srcLen - 1);

    if (padding > block || padding < 0x1) {
        qWarning("QRaiden::decrypt(): invalid padding value 0x%X", padding);
        return QByteArray();
    }

    for (quint32 i = srcLen - padding; i < srcLen; ++i) {
        if (dst.at(i) != padding) {
            qWarning("QRaiden::decrypt(): invalid padding, expected %d padding bytes of value 0x%X", padding, padding);
            return QByteArray();
        }
    }

    dst.chop(padding);
    return dst;
}

QString QRaiden::decryptAsString(const QByteArray &cipherData, const quint32 (&key)[4])
{
    return QString::fromUtf8(decrypt(cipherData, key));
}

// raiden.c

/*
*	This is the Raiden cipher source code.
*	This file includes the encoding and decoding routines
*
*	Developed by Javier Polimón and Julio César Hernández
*	Last update: 15th of October, 2006.
*/

/*
 *	This is the encoding routine.
 *	It receives the plain text in 'data' and puts
 *	the cipher text in 'result'.
 *
*/
void QRaiden::encode(const quint32 key[], const quint32 data[], quint32 result[])
{
    quint32 b0 = data[0], b1 = data[1], k[4] = { key[0], key[1], key[2], key[3] }, sk;
    for (qint8 i = 0; i < 16; ++i) {
        sk = k[i%4] = ((k[0]+k[1])+((k[2]+k[3])^(k[0]<<k[2])));
        b0 += ((sk+b1)<<9)^((sk-b1)^((sk+b1)>>14));
        b1 += ((sk+b0)<<9)^((sk-b0)^((sk+b0)>>14));
    }
    result[0] = b0;
    result[1] = b1;
}

/*
*	This is the decoding routine.
*	It receives the ciphertext as the 'data' parameter, and puts the
*	plain text in the 'result' one.
*
*/
void QRaiden::decode(const quint32 key[], const quint32 data[], quint32 result[])
{
    quint32 b0 = data[0], b1=data[1], k[4] = { key[0], key[1], key[2], key[3] }, subkeys[16];
    for (qint8 i = 0; i < 16; ++i) {
        //Subkeys are calculated
        k[i%4] = ((k[0]+k[1])+((k[2]+k[3])^(k[0]<<k[2])));
        subkeys[i] = k[i%4];
    }
    for (qint8 i = 15; i >= 0; --i) {
        //Process is applied in the inverse order
        b1 -= ((subkeys[i]+b0)<<9)^((subkeys[i]-b0)^((subkeys[i]+b0)>>14));
        b0 -= ((subkeys[i]+b1)<<9)^((subkeys[i]-b1)^((subkeys[i]+b1)>>14));
    }
    result[0] = b0;
    result[1] = b1;
}

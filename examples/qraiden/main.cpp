#include <QSettings>
#include <QRaiden>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    quint32 key[4] = { 0x13859889, 0xef54ef32, 0x939039f4, 0x12ac340f };

    const QString secret("secret_data");

    {
        QSettings settings("QRaiden", "QRaiden");
        settings.setValue("secret", QRaiden::encrypt(secret, key));
    }

    {
        QSettings settings("QRaiden", "QRaiden");
        const QString decryptedSecret = QRaiden::decryptAsString(settings.value("secret").toByteArray(), key);
        settings.clear();
        Q_ASSERT(secret == decryptedSecret);
    }

    return 0;
}

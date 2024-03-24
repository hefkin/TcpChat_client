#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QDataStream>
#include <QDateTime>
#include <QHostAddress>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include <QFileDialog>
#include <QBuffer>
#include <QImage>
#include <QPixmap>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *connectButton;
    QTextEdit *logTextEdit;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
    QPushButton *sendImgButton;

    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;

    QTcpSocket *socket;
    QByteArray Data;
    QString filePath;
    void SendToServer(QString str);
    void SendImgToServer(QString str);
    void connectToS(QString ip, int port);
    void printTE(QString str);
    void printFromServer(QString str);
    void openFile();
    quint64 nextBlockSize;

public slots:
    void slotReadyRead();
    void disconnectFromServer();

private slots:
    void ClickedSlot();
    void ClickedImgSlot();
    void ToggleSlot(bool checked);
};

#endif // WIDGET_H

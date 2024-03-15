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

    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;

    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);
    void connectToS(QString ip, int port);
    void printTE(QString str);
    void printFromServer(QString str);

public slots:
    void slotReadyRead();
    void disconnectFromServer();

private slots:
    void ClickedSlot();
    void ToggleSlot(bool checked);
};

#endif // WIDGET_H

#ifndef THREADS_H
#define THREADS_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QDebug>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QString &string, unsigned int num) : m_string(&string), m_num(num) {}
protected:
    void run() override {
        m_string->append(QString::number(m_num));
        qDebug() << m_string << " from " << this << " m_string address: " << &m_string;
        emit ResultReady();
    }
private:
    QString *m_string;
    int m_num;
signals:
    void ResultReady();
};


class ThreadContainer : public QObject {
    Q_OBJECT
public:
    ThreadContainer(unsigned int numThreads = 2)
    {
        m_thread1 = new WorkerThread(std::ref(m_string), 1);
        m_thread2 = new WorkerThread(std::ref(m_string), 2);

        connect(m_thread1, SIGNAL(ResultReady()), m_thread2, SLOT(start()));
        connect(m_thread2, SIGNAL(ResultReady()), this, SLOT(onAllThreadsCompleted()));
    }

    ~ThreadContainer(){
        m_thread1->quit();
        m_thread1->wait();
        m_thread2->quit();
        m_thread2->wait();
    }

    void StartThreads(){
        m_thread1->start();
    }

    QString GetString(){
        return m_string;
    }

signals:
    void ThreadsCompleted();

public slots:
    void onAllThreadsCompleted(){
        m_thread1->quit();
        m_thread1->wait();
        m_thread2->quit();
        m_thread2->wait();
        emit ThreadsCompleted();
    }

private:
    int m_num;
    QString m_string;

    WorkerThread *m_thread1;
    WorkerThread *m_thread2;
};

#endif // THREADS_H

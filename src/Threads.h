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
        m_threads.reserve(numThreads);
        for(qsizetype i = 0; i < numThreads; ++i){
            m_threads.emplaceBack(new WorkerThread(std::ref(m_string), i + 1));
            if(i > 0 && i != numThreads - 1)
                connect(m_threads.at(i-1), SIGNAL(ResultReady()), m_threads.at(i), SLOT(start()));
        }
        connect(m_threads.back(), SIGNAL(ResultReady()), this, SLOT(onAllThreadsCompleted()));
    }

    ~ThreadContainer(){
        for (auto &thread : m_threads) {
            thread->quit();
            thread->wait();
        }
        qDebug() << m_string << Qt::endl;
        qDeleteAll(m_threads);
    }

    void StartThreads(){
        m_threads.front()->start();
    }

    QString GetString(){
        return m_string;
    }

signals:
    void ThreadsCompleted();

public slots:
    void onAllThreadsCompleted(){
        qDebug() << "threads completed";
        emit ThreadsCompleted();
    }

private:
    QString m_string;

    QList<WorkerThread*> m_threads;
};

#endif // THREADS_H

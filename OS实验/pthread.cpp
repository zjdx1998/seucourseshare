#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 
using namespace std;

#define BUFFER_SIZE 10//缓冲区大小为10
sem_t  Mutex, Empty, Full;//互斥信号量Mutex,资源信号量Empty,Full
int producerPtr = 0, consumerPtr = 0;//插入或删除的元素在缓冲区的序号
int buffer[10];//表示缓冲区

void output() {//输出缓冲区
	cout << "缓冲区:";
	for (int i = 0; i < 10; i++)
		cout << buffer[i] << ' ';
	cout << endl;
}

bool insert(int x) {//插入元素
	if (producerPtr >= consumerPtr&&producerPtr < consumerPtr + 10) {//若缓冲区未满,则可成功插入元素
		buffer[producerPtr++ % 10] = x;
		return true;
	}
	else
		return false;//否则插入失败
}

bool remove() {//移除元素
	if (consumerPtr <= producerPtr) {//若缓冲区非空则可成功移除一个元素
		buffer[consumerPtr++ % 10] = 0;
		return true;
	}
	else
		return false;//否则移除失败
}

void *producer(void *ptr)//生产者线程
{
		srand(2*time(0)-pthread_self());
	do {
		int random = rand() % 100+1;
		usleep(random*1000);//随机等待1~100ms
		sem_wait(&Empty);//若缓冲区已满,阻塞当前生产者线程
		sem_wait(&Mutex);//实现对缓冲区的互斥访问
		cout << "--------------------------------" << endl;
		cout << "Producer " << pthread_self() << ": insert ";
		if (insert(random))
			cout << "successfully\nproducerd " << (producerPtr - 1) % 10 + 1 << endl;
		else
			cout << "failed\n" << endl;
		output();
		cout << "--------------------------------" << endl << endl;
		sem_post(&Mutex);
		sem_post(&Full);
	} while (true);
	return 0;
}

void *consumer(void *ptr)//消费者线程
{
		srand(3*time(0)+pthread_self());
	do {
		int random = rand() % 100 + 1;
		usleep(random*1000);//随机等待1~100ms
		sem_wait(&Full);//若缓冲区已空,阻塞当前消费者线程
		sem_wait(&Mutex);//实现对缓冲区的互斥访问
		cout << "********************************" << endl;
		cout << "Consumer " << pthread_self() << ": remove ";
		if (remove())
			cout << "successfully\nconsumed " << (consumerPtr - 1) % 10 + 1 << endl;
		else
			cout << "failed\n" << endl;
		output();
		cout << "********************************" << endl << endl;
		sem_post(&Mutex);
		sem_post(&Empty);
	} while (true);
	return 0;
}

int main() {
	int SleepTime, NumOfProducer, NumOfConsumer;
	cout << "输入主函数睡眠时间(秒数)" << endl;
	cin >> SleepTime;
	cout << "输入生产者进程数量(1~10)" << endl;
	cin >> NumOfProducer;
	cout << "输入消费者进程数量(1~10)" << endl;
	cin >> NumOfConsumer;

	cout << "初始化" << endl;
	memset(buffer, false, 10);
	output();
	cout << endl;

	sem_init(&Empty,0,BUFFER_SIZE);
	sem_init(&Full,0,0);
	sem_init(&Mutex,1,1);

	pthread_t *pid=new pthread_t[NumOfProducer];
	pthread_t *cid=new pthread_t[NumOfConsumer];
	pthread_attr_t attr;
	pthread_attr_init(&attr);	

	for (unsigned short i = 0; i<NumOfProducer; i++)//创建生产者线程
	{
		pid[i] = i;
		pthread_create(&pid[i], &attr, producer, NULL);
	}
	for (unsigned short i = 0; i<NumOfConsumer; i++)//创建消费者线程
	{
		cid[i] = 10+i;
		pthread_create(&cid[i], &attr, consumer, NULL);
	}
		
	sleep(SleepTime);//主线程睡眠输入时间
	cout<<"End"<<endl;
	return 0;
}

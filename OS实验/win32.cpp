#include<iostream>
#include<Windows.h>
#include<stdlib.h>      
#include<time.h>   
using namespace std;

#define BUFFER_SIZE 10;//缓冲区大小为10
HANDLE Mutex, Empty, Full;//互斥信号量Mutex,资源信号量Empty,Full
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

DWORD WINAPI producer(LPVOID param)//生产者线程
{
	srand(3 * time(0) + GetCurrentThreadId());
	do {
		int random = rand() % 100 + 1;
		Sleep(random);//随机等待1~100ms
		WaitForSingleObject(Empty, INFINITE);//若缓冲区已满,阻塞当前生产者线程
		WaitForSingleObject(Mutex, INFINITE);//实现对缓冲区的互斥访问
		cout << "--------------------------------" << endl;
		cout << "Producer " << GetCurrentThreadId() << ": insert ";
		if (insert(random))
			cout << "successfully\nproducerd " << (producerPtr - 1) % 10 + 1 << endl;
		else
			cout << "failed\n" << endl;
		output();
		cout << "--------------------------------" << endl << endl;
		ReleaseMutex(Mutex);
		ReleaseSemaphore(Full, 1, NULL);
	} while (true);
	return 0;
}

DWORD WINAPI consumer(LPVOID param)//消费者线程
{
	srand(2 * time(0) - GetCurrentThreadId());
	do {
		int random = rand() % 100 + 1;
		Sleep(random);//随机等待1~100ms
		WaitForSingleObject(Full, INFINITE);//若缓冲区已空,阻塞当前消费者线程
		WaitForSingleObject(Mutex, INFINITE);//实现对缓冲区的互斥访问
		cout << "********************************" << endl;
		cout << "Consumer " << GetCurrentThreadId() << ": remove ";
		if (remove())
			cout << "successfully\nconsumed " << (consumerPtr - 1) % 10 + 1 << endl;
		else
			cout << "failed\n" << endl;
		output();
		cout << "********************************" << endl << endl;
		ReleaseMutex(Mutex);
		ReleaseSemaphore(Empty, 1, NULL);
	} while (true);
	return 0;
}

int main() {
	int SleepTime, NumOfProducer, NumOfConsumer;
	cout << "输入主函数睡眠时间(millisecond)" << endl;
	cin >> SleepTime;
	cout << "输入生产者进程数量(1~10)" << endl;
	cin >> NumOfProducer;
	cout << "输入消费者进程数量(1~10)" << endl;
	cin >> NumOfConsumer;

	cout << "初始化" << endl;
	memset(buffer, 0, 10);//将缓冲区初始化为0
	output();
	cout << endl;

	Empty = CreateSemaphore(NULL, 10, 10, NULL);
	Full = CreateSemaphore(NULL, 0, 10, NULL);
	Mutex = CreateMutex(NULL, FALSE, NULL);

	DWORD *PThreadId = new DWORD[NumOfProducer];
	DWORD *CThreadId = new DWORD[NumOfConsumer];
	HANDLE *PThreadHandle = new HANDLE[NumOfProducer];
	HANDLE *CThreadHandle = new HANDLE[NumOfConsumer];

	for (unsigned short i = 0; i < NumOfProducer; i++)//创建生产者线程
	{
		PThreadId[i] = i;
		PThreadHandle[i] = CreateThread(NULL, 0, producer, NULL, 0, &PThreadId[i]);
	}
	for (unsigned short i = 0; i < NumOfConsumer; i++)//创建消费者线程
	{
		CThreadId[i] = i;
		CThreadHandle[i] = CreateThread(NULL, 0, consumer, NULL, 0, &CThreadId[i]);
	}

	Sleep(SleepTime);//主线程睡眠输入时间
	cout << "End" << endl;
	return 0;
}
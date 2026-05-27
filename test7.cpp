// C++ 多线程同步：互斥锁、条件变量、lock_guard、unique_lock 教学示例
// 编译: g++ -std=c++11 -pthread test7.cpp -o test7 && ./test7

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

std::mutex mtx;                     // 全局互斥锁
std::condition_variable cv;         // 条件变量
bool ready = false;                 // 共享条件
int shared_data = 0;                // 共享数据

// ========== 1. std::lock_guard：自动加锁/解锁 (RAII) ==========
// lock_guard 构造时 lock，析构时 unlock，简单高效，但不能手动控制锁的粒度
void demo_lock_guard() {
    std::cout << "\n===== 1. std::lock_guard 示例 =====\n";

    std::thread t1([] {
        // lock_guard 构造时自动加锁，出作用域自动解锁
        std::lock_guard<std::mutex> lock(mtx);
        shared_data = 42;
        std::cout << "线程1: 设置 shared_data = " << shared_data << std::endl;
    }); // 这里 lock 析构，自动解锁

    std::thread t2([] {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "线程2: 读取 shared_data = " << shared_data << std::endl;
    });

    t1.join();
    t2.join();

    // 对比：如果不加锁，两个线程同时访问 shared_data 会导致数据竞争
    std::cout << "lock_guard 总结: 构造即 lock，析构即 unlock，不能中途 unlock/relock\n";
}

// ========== 2. std::unique_lock：更灵活的 RAII 锁 ==========
// unique_lock 可以手动 lock/unlock，支持 deferred 延迟锁，可与条件变量配合
void demo_unique_lock() {
    std::cout << "\n===== 2. std::unique_lock 示例 =====\n";

    // 场景：模拟批量处理，手动控制锁粒度
    std::vector<int> buffer;
    int sum = 0;

    std::thread producer([] {
        for (int i = 0; i < 5; ++i) {
            // 用 unique_lock 的 defer 策略：先构造但不加锁
            std::unique_lock<std::mutex> ul(mtx, std::defer_lock);

            // 在需要的时候手动加锁 —— 只有 push_back 需要保护
            ul.lock();
            shared_data = i * 10;
            std::cout << "生产者: 生产 " << shared_data << std::endl;
            ul.unlock();  // 提前解锁，不等待析构

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    std::thread consumer([] {
        for (int i = 0; i < 5; ++i) {
            std::unique_lock<std::mutex> ul(mtx);
            std::cout << "消费者: 消费 " << shared_data << std::endl;
            // unique_lock 析构时自动解锁
        }
    });

    producer.join();
    consumer.join();

    std::cout << "unique_lock 总结: 比 lock_guard 灵活 —— 支持 defer_lock、"
              << "手动 lock/unlock、移动语义，是条件变量的标配\n";
}

// ========== 3. 条件变量 (condition_variable) ==========
// 一个线程等待某个条件成立，另一个线程通知它
// 必须搭配 std::unique_lock 使用

void demo_condition_variable() {
    std::cout << "\n===== 3. 条件变量 (condition_variable) 示例 =====\n";

    ready = false;

    std::thread waiter([] {
        std::unique_lock<std::mutex> ul(mtx);

        // wait 做了三件事：
        //   1. 解锁 (暂时释放锁，让别的线程能获取)
        //   2. 阻塞等待 notify
        //   3. 被唤醒后重新加锁
        // 第二个参数是谓词 —— 防止"虚假唤醒"(spurious wakeup)
        std::cout << "等待线程: 条件不满足，进入等待...\n";
        cv.wait(ul, [] { return ready; });
        std::cout << "等待线程: 被唤醒，条件已满足! shared_data = " << shared_data << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::thread notifier([] {
        {
            std::lock_guard<std::mutex> lock(mtx);
            shared_data = 99;
            ready = true;
            std::cout << "通知线程: 设置 ready = true, shared_data = " << shared_data << std::endl;
        }
        // 注意：notify 在解锁后调用，唤醒的 waiter 能立刻拿到锁，减少上下文切换
        cv.notify_one();  // 唤醒一个等待的线程
        // 如果用 notify_all() 则唤醒所有等待线程
    });

    waiter.join();
    notifier.join();

    std::cout << "条件变量总结:\n";
    std::cout << "  - wait(lock, pred)：解锁 → 等待 → 被唤醒 → 加锁 → 检查谓词\n";
    std::cout << "  - notify_one()：唤醒一个等待线程\n";
    std::cout << "  - notify_all()：唤醒所有等待线程\n";
    std::cout << "  - 必须配合 std::unique_lock (lock_guard 不够灵活)\n";
    std::cout << "  - 使用谓词避免虚假唤醒\n";
}

// ========== 4. 综合示例：生产者-消费者模型 ==========
// 用条件变量 + unique_lock 实现一个线程安全的队列

std::mutex qmtx;
std::condition_variable qcv;
std::vector<int> queue;
const int MAX_QUEUE = 5;

void producer(int id) {
    for (int i = 0; i < 3; ++i) {
        std::unique_lock<std::mutex> ul(qmtx);

        // 队列满了就等待消费者消费
        qcv.wait(ul, [] { return queue.size() < MAX_QUEUE; });

        int val = id * 100 + i;
        queue.push_back(val);
        std::cout << "  生产者" << id << " 生产: " << val
                  << " (队列大小: " << queue.size() << ")\n";

        ul.unlock();
        qcv.notify_all();  // 通知消费者
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void consumer(int id) {
    for (int i = 0; i < 3; ++i) {
        std::unique_lock<std::mutex> ul(qmtx);

        // 队列空了就等待生产者生产
        qcv.wait(ul, [] { return !queue.empty(); });

        int val = queue.back();
        queue.pop_back();
        std::cout << "  消费者" << id << " 消费: " << val
                  << " (队列大小: " << queue.size() << ")\n";

        ul.unlock();
        qcv.notify_all();  // 通知生产者
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}

void demo_producer_consumer() {
    std::cout << "\n===== 4. 综合示例：生产者-消费者模型 =====\n";
    std::cout << "2个生产者 + 2个消费者，共享一个有限队列\n\n";

    std::thread p1(producer, 1);
    std::thread p2(producer, 2);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    std::cout << "\n所有线程执行完毕，队列为空确认: " << queue.empty() << std::endl;
}

// ========== 主函数 ==========
int main() {
    std::cout << "=== C++ 多线程同步教学示例 ===\n\n";

    demo_lock_guard();
    demo_unique_lock();
    demo_condition_variable();
    demo_producer_consumer();

    std::cout << "\n=== 总结对比 ===\n";
    std::cout << "std::lock_guard   | 简单RAII锁       | 构造lock，析构unlock，不可中途操作\n";
    std::cout << "std::unique_lock  | 灵活RAII锁       | 可手动lock/unlock，支持defer/adopt/try_to_lock\n";
    std::cout << "                  |                  | 配合条件变量必须用它\n";
    std::cout << "condition_variable| 线程间条件通知   | wait/notify_one/notify_all\n";
    std::cout << "                  |                  | 解决「不断轮询」问题，提高效率\n";

    return 0;
}

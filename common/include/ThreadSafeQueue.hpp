#ifndef MINIPROJECT_THREADSAFE_QUEUE_HPP
#define MINIPROJECT_THREADSAFE_QUEUE_HPP

#include <vector>
#include <mutex>
#include <condition_variable>

namespace MiniProject
{
    template <typename T>
    class ThreadSafeQueue
    {
    private:
        const int MAX_SIZE;
        std::vector<T> _vecs;
        int m_int_head;
        int m_int_tail;
        int m_int_size;

    private:
        std::mutex queue_mutex_;
        std::condition_variable read_condion_;
        std::condition_variable write_condion_;

    public:
        ThreadSafeQueue(int max_size = 20) : MAX_SIZE(max_size), _vecs(max_size), m_int_head(0), m_int_tail(0), m_int_size(0){};
        ~ThreadSafeQueue()
        {
            _vecs.clear();
        };

        int size()
        {
            return m_int_size;
        }

        void Enqueue(T val)
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            while (is_full())
            {
                write_condion_.wait(lock);
            }
            _vecs[m_int_tail] = val;
            ++m_int_size;
            set_tail();
            read_condion_.notify_all();
        }

        T Dequeue()
        {
            T val;
            std::unique_lock<std::mutex> lock(queue_mutex_);
            while (is_empty())
            {
                read_condion_.wait(lock);
            }
            val = std::move(_vecs[m_int_head]);
            --m_int_size;
            set_front();
            write_condion_.notify_all();
            return val;
        }

        bool TryEnqueue(T val)
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (is_full())
            {
                read_condion_.notify_all();
                return false;
            }
            _vecs[m_int_tail] = val;
            ++m_int_size;
            set_tail();
            read_condion_.notify_all();
            return true;
        }

        bool TryDequeue(T &val)
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (is_empty())
            {
                write_condion_.notify_all();
                return false;
            }
            val = std::move(_vecs[m_int_head]);
            --m_int_size;
            set_front();
            write_condion_.notify_all();
            return true;
        }

    private:
        void set_front()
        {
            m_int_head = (m_int_head + 1) % MAX_SIZE;
        }

        void set_tail()
        {
            m_int_tail = (m_int_tail + 1) % MAX_SIZE;
        }

        bool is_empty()
        {
            return m_int_size == 0;
        }

        bool is_full()
        {
            return m_int_size == MAX_SIZE;
        }
    };
}

#endif
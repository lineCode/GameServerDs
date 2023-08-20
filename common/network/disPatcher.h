#ifndef MINIPROJECT_DISPATHCER_HPP
#define MINIPROJECT_DISPATHCER_HPP
#include <thread>
#include "../include/ThreadSafeQueue.hpp"
#include <boost/noncopyable.hpp>
#include <mutex>
#include <functional>


namespace MiniProject
{
    typedef std::function<int(void *)> DisPatchFuntion;
    typedef std::function<void()> MessageFunc;

    class DisPatcher : boost::noncopyable
    {
    public:
        static DisPatcher *Instance()
        {
            if (dpInstance_ == nullptr)
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (dpInstance_ == nullptr)
                {
                    dpInstance_ = new DisPatcher();
                }
            }
            return dpInstance_;
        }

        void init(int work_thread, DisPatchFuntion _func);

        void enqueueTask(MessageFunc _func, void *_data);

        ~DisPatcher();

    private:
        DisPatcher();
        DisPatchFuntion _dispatchfunc;
        std::vector<std::shared_ptr<ThreadSafeQueue<MessageFunc>>> m_vec_funcs_;

        bool is_stop_;
        int work_thread_;

        static DisPatcher *dpInstance_;
        static std::mutex mutex_;
    };

}

#endif
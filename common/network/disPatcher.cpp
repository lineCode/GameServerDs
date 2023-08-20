#include "disPatcher.h"

namespace MiniProject
{

    DisPatcher *DisPatcher::dpInstance_ = nullptr;
    std::mutex DisPatcher::mutex_;

    DisPatcher::DisPatcher() : is_stop_(true), work_thread_(0) {}

    DisPatcher::~DisPatcher()
    {
        if (!is_stop_)
        {
            is_stop_ = true;
            for (int i = 0; i < work_thread_; ++i)
            {
                m_vec_funcs_[i]->Enqueue(nullptr);
            }
        }
    }

    void DisPatcher::init(int work_thread, DisPatchFuntion _func)
    {
        if (_func == nullptr)
        {
            return;
        }

        is_stop_ = false;

        m_vec_funcs_.resize(work_thread_, std::make_shared<ThreadSafeQueue<MessageFunc>>());

        work_thread_ = work_thread;
        for (int i = 0; i < work_thread; ++i)
        {
            std::thread([this, i]()
                        {
                            int _idx = i;
                            while (!is_stop_)
                            {
                                MessageFunc _func = m_vec_funcs_[_idx]->Dequeue();

                                if(_func == nullptr)
                                {
                                    continue;
                                }
                                _func();
                            } })
                .detach();
        }
    }

    void DisPatcher::enqueueTask(MessageFunc _func, void *_data)
    {
        if (_dispatchfunc == nullptr)
        {
            _func();
            return;
        }
        int ret = _dispatchfunc(_data) % work_thread_;

        m_vec_funcs_[ret]->Enqueue(_func);
    }

} // namespace MiniProject
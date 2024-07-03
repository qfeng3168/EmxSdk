//
// Created by xiong on 2021/7/30.
//

#ifndef EMX_MediaRingBuffer_HPP
#define EMX_MediaRingBuffer_HPP

#include <string>
#include "core/EmxUtils.hpp"

namespace Emx {

    //!@brief 媒体环形共享缓冲区写类(注意：所有操作都是非线程安全的)
    class MediaRingBufferWriter {
    public:
        /**
         * @brief 创建或关联一个缓冲区
         * @param key [in] 缓冲区的识别码
         * @param size [in] 缓冲区的大小
         * @return ErrCodeE
         */
        ErrCodeE Create(int32_t key, int32_t size = 0);

        /**
         * @brief 销毁或断开缓冲区
         */
        void Destroy() { m_shm.Destroy(); }

        /**
         * @brief 发送数据到缓冲区
         * @param data [in] 数据内存地址
         * @param size [in] 数据大小
         * @param magicId [in] 用于判断数据是否被改写，尽量保证每次push都会变化
         * @retval -1:失败
         * @retval other: 写入地址与缓冲区起始地址的的偏移量
         */
        int32_t Push(char *data, int32_t size, uint32_t magicId);

        /**
         * @brief 从缓冲区获取一块内存
         * @param size [in] 期望获取内存大小
         * @param magicId [in] 用于判断数据是否被改写，尽量保证每次Get都会变化
         * @param data [out] 获取到的内存地址
         * @retval -1:失败
         * @retval other: 获取地址与缓冲区起始地址的的偏移量
         */
        int32_t GetFreeBuffer(int32_t size, uint32_t magicId, char **data);

    private:
        Shm m_shm;
        char *m_buffer;
        int32_t m_bufferSize;
        int32_t m_off = 0;
    };

    //!@brief 媒体环形共享缓冲区读类(注意：所有操作都是非线程安全的)
    class MediaRingBufferReader {
    public:
        /**
         * @brief 创建或关联一个缓冲区
         * @param key [in] 缓冲区的识别码
         * @param size [in] 缓冲区的大小
         * @return ErrCodeE
        */
        ErrCodeE Create(int32_t key, int32_t size);

        /**
         * @brief 关联一个缓冲区
         * @param key [in] 缓冲区的识别码
         * @return ErrCodeE
         */
        void Create(int32_t key){Create(key,0);}

        /**
         * @brief 断开缓冲区
         */
        void Destroy() { m_shm.Destroy(); }

        /**
         * @brief 从缓冲区里读取一块数据
         * @param off [in] 读取地址与缓冲区起始地址的的偏移量
         * @param size [in] 读取大小
         * @param magicId [in] 用于判断数据是否被改写，需要与此数据在Push或GetFreeBuffer时填写的magicId一致
         * @param data [out] 读取到的数据
         * @return ErrCodeE
         */
        ErrCodeE Pop(int32_t off, int32_t size, uint32_t magicId, std::string &data);

        using OnGetBuffer = std::function<ErrCodeE(ErrCodeE e, const uint8_t *buffer, void *usr,
                                                   int32_t off, int32_t size, uint32_t magicId)>;

        /**
         * @brief 将缓冲去中的数据直接送给注册的回调
         * @note 这个操作不会像Pop那样将缓冲区中的数据拷贝到data中，而是直接将缓冲区中的数据提供出来
         * 因此，这个函数并不安全，有可能在cb中使用buffer数据的过程中，buffer内的数据就正在被覆盖。
         * 所以，首先cb中对buffer的处理要留意这种况状的发生，并且在使用完buffer后调用CheckBufferIsDirty来确认
         * 刚刚使用buffer的过程中，buffer内的数据有没有被覆盖。PopDirect的主要用途是可以节省内存的开销
         * @param off [in] 读取地址与缓冲区起始地址的的偏移量
         * @param size [in] 读取大小
         * @param magicId [in] 用于判断数据是否被改写，需要与此数据在Push或GetFreeBuffer时填写的magicId一致
         * @param cb [in] 读取到数据后用于接收数据的回调,此回调是同步回调
         * @param usr [in] 用户数据
         * @return ErrCodeE
         */
        ErrCodeE PopDirect(int32_t off, int32_t size, uint32_t magicId, OnGetBuffer cb, void *usr);

        /**
         * @brief 检查buffer当前是否被覆盖过
         * @param off [in] 读取地址与缓冲区起始地址的的偏移量
         * @param size [in] 读取大小
         * @param magicId [in] 用于判断数据是否被改写，需要与此数据在Push或GetFreeBuffer时填写的magicId一致
         * @return bool
         */
        bool CheckBufferIsDirty(int32_t off, int32_t size, uint32_t magicId);

    private:
        int32_t m_key;
        Shm m_shm;
        char *m_buffer;
        int32_t m_bufferSize;
    };
}

#endif //EMX_MediaRingBuffer_HPP

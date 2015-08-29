#pragma once
#include <string>

namespace minijava
{

class CBaseScannerStream
{
public:
    virtual ~CBaseScannerStream() = default;

    char ReadChar()
    {
        char ret = Peek();
        if (ret == '\n')
        {
            ++m_line;
            m_column = 1;
        }
        else
        {
            ++m_column;
        }
        ++m_position;
        if (m_position + MAX_PEEK_AHEAD >= m_buffer.size())
        {
            MoveBufferAhead();
        }
        return ret;
    }

    inline char Peek() const
    {
        return m_buffer[m_position];
    }

    inline char Peek2() const
    {
        return m_buffer[m_position + 1];
    }

    inline size_t GetLine() const
    {
        return m_line;
    }

    inline size_t GetColumn() const
    {
        return m_column;
    }

    inline size_t GetOffset() const
    {
        return m_position;
    }

protected:
    CBaseScannerStream() = default;
    virtual size_t ReadString(std::string &buffer, size_t bytesLimit) = 0;

    void MoveBufferAhead()
    {
        m_buffer = m_buffer.substr(m_position);
        m_position = 0;
        if (0 == ReadString(m_buffer, BUFFER_SIZE))
        {
            m_buffer.append({'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'});
        }
    }

private:
    static const size_t MAX_PEEK_AHEAD = 1;
    static const size_t BUFFER_SIZE = 64 * 1024 - MAX_PEEK_AHEAD;
    size_t m_position = 0;
    size_t m_line     = 1;
    size_t m_column   = 1;
    std::string m_buffer;
};

}

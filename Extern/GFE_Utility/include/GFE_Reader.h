#pragma once

#include <string>
#include <vector>
#include <mutex>

class GFE_Reader
{
public:
    GFE_Reader();
	virtual ~GFE_Reader() { /*delete m_buffer;*/ ClearBuffer(); }
    explicit GFE_Reader(const std::string& path);
    void SetPath(const std::string& path) { m_filePath = path; }
    const std::string& GetPath() { return m_filePath; }

    //! @attention msg use utf-8 encoding
    inline void AddError(size_t line, const std::string& msg)
    {
        error_mtx.lock();
        errors.emplace_back(line, msg);
        error_mtx.unlock();
    }

    inline void ClearErrors()
    {
        error_mtx.lock();
        errors.clear();
        error_mtx.unlock();
    }

    inline const auto& Errors()
    {
        return errors;
    }

    //! @attention msg use utf-8 encoding
    inline void AddWarning(size_t line, const std::string& msg)
    {
        warning_mtx.lock();
        warnnings.emplace_back(line, msg);
        warning_mtx.unlock();
    }

    inline void ClearWarning()
    {
        warning_mtx.lock();
        warnnings.clear();
        warning_mtx.unlock();
    }

    inline const auto& Warnings()
    {
        return warnnings;
    }

    virtual bool Perform() { return true; };
    bool Perform(const std::string&);

    static std::vector<std::string> GetLines(const std::string&);

    [[deprecated("use AddError instead")]] std::vector<std::pair<std::size_t, std::string>> errors;        // {行号, 错误信息}, 用于替换lastError, 暂时只改了一部分
    [[deprecated("use AddWarning instead")]] std::vector<std::pair<std::size_t, std::string>> warnnings;        // {行号, 警告信息}

protected:
    std::pair<const char*, unsigned long long> BuildBuffer();
    std::pair<const char*, unsigned long long> GetBuffer() { return {m_buffer, m_bufLen}; }
    void ClearBuffer();

    std::string m_filePath;

private:
    char* m_buffer = nullptr;
    unsigned long long m_bufLen = 0;

    std::mutex error_mtx;
    std::mutex warning_mtx;
};


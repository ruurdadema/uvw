#pragma once


#include <utility>
#include <memory>
#include <string>
#include <chrono>
#include <uv.h>
#include "event.hpp"
#include "request.hpp"
#include "util.hpp"
#include "loop.hpp"


namespace uvw {


namespace details {


enum class UVFsType: std::underlying_type_t<uv_fs_type> {
    UNKNOWN = UV_FS_UNKNOWN,
    CUSTOM = UV_FS_CUSTOM,
    OPEN = UV_FS_OPEN,
    CLOSE = UV_FS_CLOSE,
    READ = UV_FS_READ,
    WRITE = UV_FS_WRITE,
    SENDFILE = UV_FS_SENDFILE,
    STAT = UV_FS_STAT,
    LSTAT = UV_FS_LSTAT,
    FSTAT = UV_FS_FSTAT,
    FTRUNCATE = UV_FS_FTRUNCATE,
    UTIME = UV_FS_UTIME,
    FUTIME = UV_FS_FUTIME,
    ACCESS = UV_FS_ACCESS,
    CHMOD = UV_FS_CHMOD,
    FCHMOD = UV_FS_FCHMOD,
    FSYNC = UV_FS_FSYNC,
    FDATASYNC = UV_FS_FDATASYNC,
    UNLINK = UV_FS_UNLINK,
    RMDIR = UV_FS_RMDIR,
    MKDIR = UV_FS_MKDIR,
    MKDTEMP = UV_FS_MKDTEMP,
    RENAME = UV_FS_RENAME,
    SCANDIR = UV_FS_SCANDIR,
    LINK = UV_FS_LINK,
    SYMLINK = UV_FS_SYMLINK,
    READLINK = UV_FS_READLINK,
    CHOWN = UV_FS_CHOWN,
    FCHOWN = UV_FS_FCHOWN,
    REALPATH = UV_FS_REALPATH
};


enum class UVDirentTypeT: std::underlying_type_t<uv_dirent_type_t> {
    UNKNOWN = UV_DIRENT_UNKNOWN,
    FILE = UV_DIRENT_FILE,
    DIR = UV_DIRENT_DIR,
    LINK = UV_DIRENT_LINK,
    FIFO = UV_DIRENT_FIFO,
    SOCKET = UV_DIRENT_SOCKET,
    CHAR = UV_DIRENT_CHAR,
    BLOCK = UV_DIRENT_BLOCK
};


}


/**
 * @brief Default FsEvent event.
 *
 * Available types are:
 *
 * * `FsRequest::Type::UNKNOWN`
 * * `FsRequest::Type::CUSTOM`
 * * `FsRequest::Type::OPEN`
 * * `FsRequest::Type::CLOSE`
 * * `FsRequest::Type::READ`
 * * `FsRequest::Type::WRITE`
 * * `FsRequest::Type::SENDFILE`
 * * `FsRequest::Type::STAT`
 * * `FsRequest::Type::LSTAT`
 * * `FsRequest::Type::FSTAT`
 * * `FsRequest::Type::FTRUNCATE`
 * * `FsRequest::Type::UTIME`
 * * `FsRequest::Type::FUTIME`
 * * `FsRequest::Type::ACCESS`
 * * `FsRequest::Type::CHMOD`
 * * `FsRequest::Type::FCHMOD`
 * * `FsRequest::Type::FSYNC`
 * * `FsRequest::Type::FDATASYNC`
 * * `FsRequest::Type::UNLINK`
 * * `FsRequest::Type::RMDIR`
 * * `FsRequest::Type::MKDIR`
 * * `FsRequest::Type::MKDTEMP`
 * * `FsRequest::Type::RENAME`
 * * `FsRequest::Type::SCANDIR`
 * * `FsRequest::Type::LINK`
 * * `FsRequest::Type::SYMLINK`
 * * `FsRequest::Type::READLINK`
 * * `FsRequest::Type::CHOWN`
 * * `FsRequest::Type::FCHOWN`
 * * `FsRequest::Type::REALPATH`
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 *
 * See the official
 * [documentation](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_type)
 * for further details.
 */
template<details::UVFsType e>
struct FsEvent: Event<FsEvent<e>> {
    FsEvent(const char *path) noexcept: path{path} { }

    const char * path; /*!< The path affecting the request. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::READ`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::READ>
        : Event<FsEvent<details::UVFsType::READ>>
{
    FsEvent(const char *path, std::unique_ptr<const char[]> data, ssize_t size) noexcept
        : path{path}, data{std::move(data)}, size{size}
    { }

    const char * path; /*!< The path affecting the request. */
    std::unique_ptr<const char[]> data; /*!< A bunch of data read from the given path. */
    ssize_t size; /*!< The amount of data read from the given path. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::WRITE`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::WRITE>
        : Event<FsEvent<details::UVFsType::WRITE>>
{
    FsEvent(const char *path, ssize_t size) noexcept
        : path{path}, size{size}
    { }

    const char * path; /*!< The path affecting the request. */
    ssize_t size; /*!< The amount of data written to the given path. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::SENDFILE`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::SENDFILE>
        : Event<FsEvent<details::UVFsType::SENDFILE>>
{
    FsEvent(const char *path, ssize_t size) noexcept
        : path{path}, size{size}
    { }

    const char * path; /*!< The path affecting the request. */
    ssize_t size; /*!< The amount of data transferred. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::STAT`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::STAT>
        : Event<FsEvent<details::UVFsType::STAT>>
{
    FsEvent(const char *path, Stat stat) noexcept
        : path{path}, stat(std::move(stat))
    { }

    const char * path; /*!< The path affecting the request. */
    Stat stat; /*!< An initialized instance of Stat. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::FSTAT`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::FSTAT>
        : Event<FsEvent<details::UVFsType::FSTAT>>
{
    FsEvent(const char *path, Stat stat) noexcept
        : path{path}, stat(std::move(stat))
    { }

    const char * path; /*!< The path affecting the request. */
    Stat stat; /*!< An initialized instance of Stat. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::LSTAT`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::LSTAT>
        : Event<FsEvent<details::UVFsType::LSTAT>>
{
    FsEvent(const char *path, Stat stat) noexcept
        : path{path}, stat(std::move(stat))
    { }

    const char * path; /*!< The path affecting the request. */
    Stat stat; /*!< An initialized instance of Stat. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::SCANDIR`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::SCANDIR>
        : Event<FsEvent<details::UVFsType::SCANDIR>>
{
    FsEvent(const char *path, ssize_t size) noexcept
        : path{path}, size{size}
    { }

    const char * path; /*!< The path affecting the request. */
    ssize_t size; /*!< The number of directory entries selected. */
};


/**
 * @brief FsEvent event specialization for `FsRequest::Type::READLINK`.
 *
 * It will be emitted by FsReq and/or FileReq according with their
 * functionalities.
 */
template<>
struct FsEvent<details::UVFsType::READLINK>
        : Event<FsEvent<details::UVFsType::READLINK>>
{
    explicit FsEvent(const char *path, const char *data, ssize_t size) noexcept
        : path{path}, data{data}, size{size}
    { }

    const char * path; /*!< The path affecting the request. */
    const char * data; /*!< A bunch of data read from the given path. */
    ssize_t size; /*!< The amount of data read from the given path. */
};


/**
 * @brief Base class for FsReq and/or FileReq.
 *
 * Not directly instantiable, should not be used by the users of the library.
 */
template<typename T>
class FsRequest: public Request<T, uv_fs_t> {
protected:
    template<details::UVFsType e>
    static void fsGenericCallback(uv_fs_t *req) {
        auto ptr = Request<T, uv_fs_t>::reserve(reinterpret_cast<uv_req_t*>(req));
        if(req->result < 0) { ptr->publish(ErrorEvent{req->result}); }
        else { ptr->publish(FsEvent<e>{req->path}); }
    }

    template<details::UVFsType e>
    static void fsResultCallback(uv_fs_t *req) {
        auto ptr = Request<T, uv_fs_t>::reserve(reinterpret_cast<uv_req_t*>(req));
        if(req->result < 0) { ptr->publish(ErrorEvent{req->result}); }
        else { ptr->publish(FsEvent<e>{req->path, req->result}); }
    }

    template<details::UVFsType e>
    static void fsStatCallback(uv_fs_t *req) {
        auto ptr = Request<T, uv_fs_t>::reserve(reinterpret_cast<uv_req_t*>(req));
        if(req->result < 0) { ptr->publish(ErrorEvent{req->result}); }
        else { ptr->publish(FsEvent<e>{req->path, req->statbuf}); }
    }

    using Request<T, uv_fs_t>::Request;

    template<typename... Args>
    void cleanupAndInvoke(Args&&... args) {
        uv_fs_req_cleanup(this->get());
        this->invoke(std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
    void cleanupAndInvokeSync(F &&f, Args&&... args) {
        uv_fs_req_cleanup(this->get());
        std::forward<F>(f)(std::forward<Args>(args)..., nullptr);
    }

public:
    using Time = std::chrono::seconds;
    using Type = details::UVFsType;
    using EntryType = details::UVDirentTypeT;
    using Entry = std::pair<EntryType, std::string>;
};


/**
 * @brief The FileReq request.
 *
 * Cross-platform sync and async filesystem operations.<br/>
 * All file operations are run on the threadpool.
 *
 * See the official
 * [documentation](http://docs.libuv.org/en/v1.x/fs.html)
 * for further details.
 */
class FileReq final: public FsRequest<FileReq> {
    static constexpr uv_file BAD_FD = -1;

    static void fsOpenCallback(uv_fs_t *req) {
        auto ptr = reserve(reinterpret_cast<uv_req_t*>(req));

        if(req->result < 0) {
            ptr->publish(ErrorEvent{req->result});
        } else {
            ptr->file = static_cast<uv_file>(req->result);
            ptr->publish(FsEvent<Type::OPEN>{req->path});
        }
    }

    static void fsCloseCallback(uv_fs_t *req) {
        auto ptr = reserve(reinterpret_cast<uv_req_t*>(req));

        if(req->result < 0) {
            ptr->publish(ErrorEvent{req->result});
        } else {
            ptr->file = BAD_FD;
            ptr->publish(FsEvent<Type::CLOSE>{req->path});
        }
    }

    static void fsReadCallback(uv_fs_t *req) {
        auto ptr = reserve(reinterpret_cast<uv_req_t*>(req));
        if(req->result < 0) { ptr->publish(ErrorEvent{req->result}); }
        else { ptr->publish(FsEvent<Type::READ>{req->path, std::move(ptr->data), req->result}); }
    }

    using FsRequest::FsRequest;

public:
    /**
     * @brief Creates a new file request.
     * @param loop A pointer to the loop from which the handle generated.
     * @return A pointer to the newly created request.
     */
    static std::shared_ptr<FileReq> create(std::shared_ptr<Loop> loop) {
        return std::shared_ptr<FileReq>{new FileReq{std::move(loop)}};
    }

    ~FileReq() noexcept {
        uv_fs_req_cleanup(get());
    }

    /**
     * @brief Async [close](http://linux.die.net/man/2/close).
     *
     * Emit a `FsEvent<FileReq::Type::CLOSE>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     */
    void close() {
        cleanupAndInvoke(&uv_fs_close, parent(), get(), file, &fsCloseCallback);
    }

    /**
     * @brief Sync [close](http://linux.die.net/man/2/close).
     * @return True in case of success, false otherwise.
     */
    bool closeSync() {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_close, parent(), req, file);
        if(req->result >= 0) { file = BAD_FD; }
        return !(req->result < 0);
    }

    /**
     * @brief Async [open](http://linux.die.net/man/2/open).
     *
     * Emit a `FsEvent<FileReq::Type::OPEN>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path A valid path name for a file.
     * @param flags Flags, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     */
    void open(std::string path, int flags, int mode) {
        cleanupAndInvoke(&uv_fs_open, parent(), get(), path.data(), flags, mode, &fsOpenCallback);
    }

    /**
     * @brief Sync [open](http://linux.die.net/man/2/open).
     * @param path A valid path name for a file.
     * @param flags Flags, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool openSync(std::string path, int flags, int mode) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_open, parent(), req, path.data(), flags, mode);
        if(req->result >= 0) { file = static_cast<uv_file>(req->result); }
        return !(req->result < 0);
    }

    /**
     * @brief Async [read](http://linux.die.net/man/2/preadv).
     *
     * Emit a `FsEvent<FileReq::Type::READ>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param offset Offset, as described in the official documentation.
     * @param len Length, as described in the official documentation.
     */
    void read(int64_t offset, unsigned int len) {
        data = std::unique_ptr<char[]>{new char[len]};
        buffer = uv_buf_init(data.get(), len);
        uv_buf_t bufs[] = { buffer };
        cleanupAndInvoke(&uv_fs_read, parent(), get(), file, bufs, 1, offset, &fsReadCallback);
    }

    /**
     * @brief Sync [read](http://linux.die.net/man/2/preadv).
     *
     * @param offset Offset, as described in the official documentation.
     * @param len Length, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * A `std::pair` composed as it follows:
     *   * A bunch of data read from the given path.
     *   * The amount of data read from the given path.
     */
    std::pair<bool, std::pair<std::unique_ptr<const char[]>, ssize_t>>
    readSync(int64_t offset, unsigned int len) {
        data = std::unique_ptr<char[]>{new char[len]};
        buffer = uv_buf_init(data.get(), len);
        uv_buf_t bufs[] = { buffer };
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_read, parent(), req, file, bufs, 1, offset);
        return std::make_pair(!(req->result < 0), std::make_pair(std::move(data), req->result));
    }

    /**
     * @brief Async [write](http://linux.die.net/man/2/pwritev).
     *
     * Emit a `FsEvent<FileReq::Type::WRITE>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param data The data to be written.
     * @param len The lenght of the submitted data.
     * @param offset Offset, as described in the official documentation.
     */
    void write(std::unique_ptr<char[]> data, ssize_t len, int64_t offset) {
        this->data = std::move(data);
        uv_buf_t bufs[] = { uv_buf_init(this->data.get(), len) };
        cleanupAndInvoke(&uv_fs_write, parent(), get(), file, bufs, 1, offset, &fsResultCallback<Type::WRITE>);
    }

    /**
     * @brief Sync [write](http://linux.die.net/man/2/pwritev).
     *
     * @param data The data to be written.
     * @param len The lenght of the submitted data.
     * @param offset Offset, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * The amount of data written to the given path.
     */
    std::pair<bool, ssize_t> writeSync(std::unique_ptr<char[]> data, ssize_t len, int64_t offset) {
        this->data = std::move(data);
        uv_buf_t bufs[] = { uv_buf_init(this->data.get(), len) };
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_write, parent(), req, file, bufs, 1, offset);
        return std::make_pair(!(req->result < 0), req->result);
    }

    /**
     * @brief Async [fstat](http://linux.die.net/man/2/fstat).
     *
     * Emit a `FsEvent<FileReq::Type::FSTAT>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     */
    void stat() {
        cleanupAndInvoke(&uv_fs_fstat, parent(), get(), file, &fsStatCallback<Type::FSTAT>);
    }

    /**
     * @brief Sync [fstat](http://linux.die.net/man/2/fstat).
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * An initialized instance of Stat.
     */
    std::pair<bool, Stat> statSync() {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_fstat, parent(), req, file);
        return std::make_pair(!(req->result < 0), req->statbuf);
    }

    /**
     * @brief Async [fsync](http://linux.die.net/man/2/fsync).
     *
     * Emit a `FsEvent<FileReq::Type::FSYNC>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     */
    void sync() {
        cleanupAndInvoke(&uv_fs_fsync, parent(), get(), file, &fsGenericCallback<Type::FSYNC>);
    }

    /**
     * @brief Sync [fsync](http://linux.die.net/man/2/fsync).
     * @return True in case of success, false otherwise.
     */
    bool syncSync() {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_fsync, parent(), req, file);
        return !(req->result < 0);
    }

    /**
     * @brief Async [fdatasync](http://linux.die.net/man/2/fdatasync).
     *
     * Emit a `FsEvent<FileReq::Type::FDATASYNC>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     */
    void datasync() {
        cleanupAndInvoke(&uv_fs_fdatasync, parent(), get(), file, &fsGenericCallback<Type::FDATASYNC>);
    }

    /**
     * @brief Sync [fdatasync](http://linux.die.net/man/2/fdatasync).
     * @return True in case of success, false otherwise.
     */
    bool datasyncSync() {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_fdatasync, parent(), req, file);
        return !(req->result < 0);
    }

    /**
     * @brief Async [ftruncate](http://linux.die.net/man/2/ftruncate).
     *
     * Emit a `FsEvent<FileReq::Type::FTRUNCATE>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param offset Offset, as described in the official documentation.
     */
    void truncate(int64_t offset) {
        cleanupAndInvoke(&uv_fs_ftruncate, parent(), get(), file, offset, &fsGenericCallback<Type::FTRUNCATE>);
    }

    /**
     * @brief Sync [ftruncate](http://linux.die.net/man/2/ftruncate).
     * @param offset Offset, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool truncateSync(int64_t offset) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_ftruncate, parent(), req, file, offset);
        return !(req->result < 0);
    }

    /**
     * @brief Async [sendfile](http://linux.die.net/man/2/sendfile).
     *
     * Emit a `FsEvent<FileReq::Type::SENDFILE>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param out A valid instance of FileHandle.
     * @param offset Offset, as described in the official documentation.
     * @param length Length, as described in the official documentation.
     */
    void sendfile(FileHandle out, int64_t offset, size_t length) {
        cleanupAndInvoke(&uv_fs_sendfile, parent(), get(), out, file, offset, length, &fsResultCallback<Type::SENDFILE>);
    }

    /**
     * @brief Sync [sendfile](http://linux.die.net/man/2/sendfile).
     *
     * @param out A valid instance of FileHandle.
     * @param offset Offset, as described in the official documentation.
     * @param length Length, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * The amount of data transferred.
     */
    std::pair<bool, ssize_t> sendfileSync(FileHandle out, int64_t offset, size_t length) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_sendfile, parent(), req, out, file, offset, length);
        return std::make_pair(!(req->result < 0), req->result);
    }

    /**
     * @brief Async [fchmod](http://linux.die.net/man/2/fchmod).
     *
     * Emit a `FsEvent<FileReq::Type::FCHMOD>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param mode Mode, as described in the official documentation.
     */
    void chmod(int mode) {
        cleanupAndInvoke(&uv_fs_fchmod, parent(), get(), file, mode, &fsGenericCallback<Type::FCHMOD>);
    }

    /**
     * @brief Sync [fchmod](http://linux.die.net/man/2/fchmod).
     * @param mode Mode, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool chmodSync(int mode) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_fchmod, parent(), req, file, mode);
        return !(req->result < 0);
    }

    /**
     * @brief Async [futime](http://linux.die.net/man/2/futime).
     *
     * Emit a `FsEvent<FileReq::Type::FUTIME>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param atime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @param mtime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     */
    void utime(Time atime, Time mtime) {
        cleanupAndInvoke(&uv_fs_futime, parent(), get(), file, atime.count(), mtime.count(), &fsGenericCallback<Type::FUTIME>);
    }

    /**
     * @brief Sync [futime](http://linux.die.net/man/2/futime).
     * @param atime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @param mtime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool utimeSync(Time atime, Time mtime) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_futime, parent(), req, file, atime.count(), mtime.count());
        return !(req->result < 0);
    }

    /**
     * @brief Async [fchown](http://linux.die.net/man/2/fchown).
     *
     * Emit a `FsEvent<FileReq::Type::FCHOWN>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param uid UID, as described in the official documentation.
     * @param gid GID, as described in the official documentation.
     */
    void chown(Uid uid, Gid gid) {
        cleanupAndInvoke(&uv_fs_fchown, parent(), get(), file, uid, gid, &fsGenericCallback<Type::FCHOWN>);
    }

    /**
     * @brief Sync [fchown](http://linux.die.net/man/2/fchown).
     * @param uid UID, as described in the official documentation.
     * @param gid GID, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool chownSync(Uid uid, Gid gid) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_fchown, parent(), req, file, uid, gid);
        return !(req->result < 0);
    }

    /**
     * @brief Cast operator to FileHandle.
     *
     * Cast operator to an internal representation of the underlying file
     * handle.
     *
     * @return A valid instance of FileHandle (the descriptor can be invalid).
     */
    operator FileHandle() const noexcept { return file; }

private:
    std::unique_ptr<char[]> data{nullptr};
    uv_buf_t buffer{};
    uv_file file{BAD_FD};
};


/**
 * @brief The FsReq request.
 *
 * Cross-platform sync and async filesystem operations.<br/>
 * All file operations are run on the threadpool.
 *
 * See the official
 * [documentation](http://docs.libuv.org/en/v1.x/fs.html)
 * for further details.
 */
class FsReq final: public FsRequest<FsReq> {
    static void fsReadlinkCallback(uv_fs_t *req) {
        auto ptr = reserve(reinterpret_cast<uv_req_t*>(req));
        if(req->result < 0) { ptr->publish(ErrorEvent{req->result}); }
        else { ptr->publish(FsEvent<Type::READLINK>{req->path, static_cast<char *>(req->ptr), req->result}); }
    }

    using FsRequest::FsRequest;

public:
    /**
     * @brief Creates a new file request.
     * @param loop A pointer to the loop from which the handle generated.
     * @return A pointer to the newly created request.
     */
    static std::shared_ptr<FsReq> create(std::shared_ptr<Loop> loop) {
        return std::shared_ptr<FsReq>{new FsReq{std::move(loop)}};
    }

    ~FsReq() noexcept {
        uv_fs_req_cleanup(get());
    }

    /**
     * @brief Async [unlink](http://linux.die.net/man/2/unlink).
     *
     * Emit a `FsEvent<FileReq::Type::UNLINK>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void unlink(std::string path) {
        cleanupAndInvoke(&uv_fs_unlink, parent(), get(), path.data(), &fsGenericCallback<Type::UNLINK>);
    }

    /**
     * @brief Sync [unlink](http://linux.die.net/man/2/unlink).
     * @param path Path, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool unlinkSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_unlink, parent(), req, path.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [mkdir](http://linux.die.net/man/2/mkdir).
     *
     * Emit a `FsEvent<FileReq::Type::MKDIR>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     */
    void mkdir(std::string path, int mode) {
        cleanupAndInvoke(&uv_fs_mkdir, parent(), get(), path.data(), mode, &fsGenericCallback<Type::MKDIR>);
    }

    /**
     * @brief Sync [mkdir](http://linux.die.net/man/2/mkdir).
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool mkdirSync(std::string path, int mode) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_mkdir, parent(), req, path.data(), mode);
        return !(req->result < 0);
    }

    /**
     * @brief Async [mktemp](http://linux.die.net/man/3/mkdtemp).
     *
     * Emit a `FsEvent<FileReq::Type::MKDTEMP>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param tpl Template, as described in the official documentation.
     */
    void mkdtemp(std::string tpl) {
        cleanupAndInvoke(&uv_fs_mkdtemp, parent(), get(), tpl.data(), &fsGenericCallback<Type::MKDTEMP>);
    }

    /**
     * @brief Sync [mktemp](http://linux.die.net/man/3/mkdtemp).
     * @param tpl Template, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool mkdtempSync(std::string tpl) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_mkdtemp, parent(), req, tpl.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [rmdir](http://linux.die.net/man/2/rmdir).
     *
     * Emit a `FsEvent<FileReq::Type::RMDIR>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void rmdir(std::string path) {
        cleanupAndInvoke(&uv_fs_rmdir, parent(), get(), path.data(), &fsGenericCallback<Type::RMDIR>);
    }

    /**
     * @brief Sync [rmdir](http://linux.die.net/man/2/rmdir).
     * @param path Path, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool rmdirSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_rmdir, parent(), req, path.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [scandir](http://linux.die.net/man/3/scandir).
     *
     * Emit a `FsEvent<FileReq::Type::SCANDIR>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param flags Flags, as described in the official documentation.
     */
    void scandir(std::string path, int flags) {
        cleanupAndInvoke(&uv_fs_scandir, parent(), get(), path.data(), flags, &fsResultCallback<Type::SCANDIR>);
    }

    /**
     * @brief Sync [scandir](http://linux.die.net/man/3/scandir).
     *
     * @param path Path, as described in the official documentation.
     * @param flags Flags, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * The number of directory entries selected.
     */
    std::pair<bool, ssize_t> scandirSync(std::string path, int flags) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_scandir, parent(), req, path.data(), flags);
        return std::make_pair(!(req->result < 0), req->result);
    }

    /**
     * @brief Gets entries populated with the next directory entry data.
     *
     * Returns instances of Entry, that is an alias for a pair where:
     *
     * * The first parameter indicates the entry type (see below).
     * * The second parameter is a `std::string` that contains the actual value.
     *
     * Available entry types are:
     *
     * * `FsReq::EntryType::UNKNOWN`
     * * `FsReq::EntryType::FILE`
     * * `FsReq::EntryType::DIR`
     * * `FsReq::EntryType::LINK`
     * * `FsReq::EntryType::FIFO`
     * * `FsReq::EntryType::SOCKET`
     * * `FsReq::EntryType::CHAR`
     * * `FsReq::EntryType::BLOCK`
     *
     * See the official
     * [documentation](http://docs.libuv.org/en/v1.x/fs.html#c.uv_dirent_t)
     * for further details.
     *
     * @return A pair where:
     *
     * * The first parameter is a boolean value that indicates if the current
     * entry is still valid.
     * * The second parameter is an instance of `Entry` (see above).
     */
    std::pair<bool, Entry> scandirNext() {
        uv_dirent_t dirent;
        std::pair<bool, Entry> ret{false, { EntryType::UNKNOWN, "" }};
        auto res = uv_fs_scandir_next(get(), &dirent);

        if(UV_EOF != res) {
            ret.second.first = static_cast<EntryType>(dirent.type);
            ret.second.second = dirent.name;
            ret.first = true;
        }

        return ret;
    }

    /**
     * @brief Async [stat](http://linux.die.net/man/2/stat).
     *
     * Emit a `FsEvent<FileReq::Type::STAT>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void stat(std::string path) {
        cleanupAndInvoke(&uv_fs_stat, parent(), get(), path.data(), &fsStatCallback<Type::STAT>);
    }

    /**
     * @brief Sync [stat](http://linux.die.net/man/2/stat).
     *
     * @param path Path, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * An initialized instance of Stat.
     */
    std::pair<bool, Stat> statSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_stat, parent(), req, path.data());
        return std::make_pair(!(req->result < 0), req->statbuf);
    }

    /**
     * @brief Async [lstat](http://linux.die.net/man/2/lstat).
     *
     * Emit a `FsEvent<FileReq::Type::LSTAT>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void lstat(std::string path) {
        cleanupAndInvoke(&uv_fs_lstat, parent(), get(), path.data(), &fsStatCallback<Type::LSTAT>);
    }

    /**
     * @brief Sync [lstat](http://linux.die.net/man/2/lstat).
     *
     * @param path Path, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * An initialized instance of Stat.
     */
    std::pair<bool, Stat> lstatSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_lstat, parent(), req, path.data());
        return std::make_pair(!(req->result < 0), req->statbuf);
    }

    /**
     * @brief Async [rename](http://linux.die.net/man/2/rename).
     *
     * Emit a `FsEvent<FileReq::Type::RENAME>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     */
    void rename(std::string old, std::string path) {
        cleanupAndInvoke(&uv_fs_rename, parent(), get(), old.data(), path.data(), &fsGenericCallback<Type::RENAME>);
    }

    /**
     * @brief Sync [rename](http://linux.die.net/man/2/rename).
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool renameSync(std::string old, std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_rename, parent(), req, old.data(), path.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [access](http://linux.die.net/man/2/access).
     *
     * Emit a `FsEvent<FileReq::Type::ACCESS>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     */
    void access(std::string path, int mode) {
        cleanupAndInvoke(&uv_fs_access, parent(), get(), path.data(), mode, &fsGenericCallback<Type::ACCESS>);
    }

    /**
     * @brief Sync [access](http://linux.die.net/man/2/access).
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool accessSync(std::string path, int mode) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_access, parent(), req, path.data(), mode);
        return !(req->result < 0);
    }

    /**
     * @brief Async [chmod](http://linux.die.net/man/2/chmod).
     *
     * Emit a `FsEvent<FileReq::Type::CHMOD>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     */
    void chmod(std::string path, int mode) {
        cleanupAndInvoke(&uv_fs_chmod, parent(), get(), path.data(), mode, &fsGenericCallback<Type::CHMOD>);
    }

    /**
     * @brief Sync [chmod](http://linux.die.net/man/2/chmod).
     * @param path Path, as described in the official documentation.
     * @param mode Mode, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool chmodSync(std::string path, int mode) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_chmod, parent(), req, path.data(), mode);
        return !(req->result < 0);
    }

    /**
     * @brief Async [utime](http://linux.die.net/man/2/utime).
     *
     * Emit a `FsEvent<FileReq::Type::UTIME>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param atime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @param mtime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     */
    void utime(std::string path, Time atime, Time mtime) {
        cleanupAndInvoke(&uv_fs_utime, parent(), get(), path.data(), atime.count(), mtime.count(), &fsGenericCallback<Type::UTIME>);
    }

    /**
     * @brief Sync [utime](http://linux.die.net/man/2/utime).
     * @param path Path, as described in the official documentation.
     * @param atime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @param mtime `std::chrono::seconds`, having the same meaning as described
     * in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool utimeSync(std::string path, Time atime, Time mtime) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_utime, parent(), req, path.data(), atime.count(), mtime.count());
        return !(req->result < 0);
    }

    /**
     * @brief Async [link](http://linux.die.net/man/2/link).
     *
     * Emit a `FsEvent<FileReq::Type::LINK>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     */
    void link(std::string old, std::string path) {
        cleanupAndInvoke(&uv_fs_link, parent(), get(), old.data(), path.data(), &fsGenericCallback<Type::LINK>);
    }

    /**
     * @brief Sync [link](http://linux.die.net/man/2/link).
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool linkSync(std::string old, std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_link, parent(), req, old.data(), path.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [symlink](http://linux.die.net/man/2/symlink).
     *
     * Emit a `FsEvent<FileReq::Type::SYMLINK>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     * @param flags Flags, as described in the official documentation.
     */
    void symlink(std::string old, std::string path, int flags) {
        cleanupAndInvoke(&uv_fs_symlink, parent(), get(), old.data(), path.data(), flags, &fsGenericCallback<Type::SYMLINK>);
    }

    /**
     * @brief Sync [symlink](http://linux.die.net/man/2/symlink).
     * @param old Old path, as described in the official documentation.
     * @param path New path, as described in the official documentation.
     * @param flags Flags, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool symlinkSync(std::string old, std::string path, int flags) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_symlink, parent(), req, old.data(), path.data(), flags);
        return !(req->result < 0);
    }

    /**
     * @brief Async [readlink](http://linux.die.net/man/2/readlink).
     *
     * Emit a `FsEvent<FileReq::Type::READLINK>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void readlink(std::string path) {
        cleanupAndInvoke(&uv_fs_readlink, parent(), get(), path.data(), &fsReadlinkCallback);
    }

    /**
     * @brief Sync [readlink](http://linux.die.net/man/2/readlink).
     *
     * @param path Path, as described in the official documentation.
     *
     * @return A `std::pair` composed as it follows:
     * * A boolean value that is true in case of success, false otherwise.
     * * A `std::pair` composed as it follows:
     *   * A bunch of data read from the given path.
     *   * The amount of data read from the given path.
     */
    std::pair<bool, std::pair<const char *, ssize_t>>
    readlinkSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_readlink, parent(), req, path.data());
        return std::make_pair(!(req->result < 0), std::make_pair(static_cast<char *>(req->ptr), req->result));
    }

    /**
     * @brief Async [realpath](http://linux.die.net/man/3/realpath).
     *
     * Emit a `FsEvent<FileReq::Type::REALPATH>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     */
    void realpath(std::string path) {
        cleanupAndInvoke(&uv_fs_realpath, parent(), get(), path.data(), &fsGenericCallback<Type::REALPATH>);
    }

    /**
     * @brief Sync [realpath](http://linux.die.net/man/3/realpath).
     * @param path Path, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool realpathSync(std::string path) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_realpath, parent(), req, path.data());
        return !(req->result < 0);
    }

    /**
     * @brief Async [chown](http://linux.die.net/man/2/chown).
     *
     * Emit a `FsEvent<FileReq::Type::CHOWN>` event when completed.<br/>
     * Emit an ErrorEvent event in case of errors.
     *
     * @param path Path, as described in the official documentation.
     * @param uid UID, as described in the official documentation.
     * @param gid GID, as described in the official documentation.
     */
    void chown(std::string path, Uid uid, Gid gid) {
        cleanupAndInvoke(&uv_fs_chown, parent(), get(), path.data(), uid, gid, &fsGenericCallback<Type::CHOWN>);
    }

    /**
     * @brief Sync [chown](http://linux.die.net/man/2/chown).
     * @param path Path, as described in the official documentation.
     * @param uid UID, as described in the official documentation.
     * @param gid GID, as described in the official documentation.
     * @return True in case of success, false otherwise.
     */
    bool chownSync(std::string path, Uid uid, Gid gid) {
        auto req = get();
        cleanupAndInvokeSync(&uv_fs_chown, parent(), req, path.data(), uid, gid);
        return !(req->result < 0);
    }
};


}

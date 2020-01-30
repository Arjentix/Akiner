#include "executor.h"

#include <unordered_set>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

using std::vector;
using std::unordered_set;
using std::string;
using std::future;
using std::async;
using std::ifstream;
using std::distance;
using std::next;
using std::advance;
using std::invalid_argument;

struct FilePathHasher;
struct FilePathComparator;

using FileSet = unordered_set<string, FilePathHasher, FilePathComparator>;

string cutFileExtension(const string& filePath)
{
    return filePath.substr(0, filePath.find_last_of('.'));
}

string cutFilePath(const string& filePath)
{
    char pathDelimiter = '/';

#ifdef __WIN32__
    pathDelimiter = '\\';
#endif

    return filePath.substr(filePath.find_last_of(pathDelimiter));
}
struct FilePathHasher
{
    size_t operator()(const string& filePath) const
    {
        return std::hash<string>()(cutFilePath(cutFileExtension(filePath)));
    }
};

struct FilePathComparator
{
    bool operator()(const string& lhs, const string& rhs) const
    {
        return cutFilePath(cutFileExtension(lhs)) == cutFilePath(cutFileExtension(rhs));
    }
};

FileSet getAllFiles(const string& directory)
{
    if (!fs::is_directory(directory)) {
        throw invalid_argument(directory + " не является директорией");
    }
    FileSet allFiles;
    for (const auto& entry : fs::directory_iterator(directory)) {
        allFiles.insert(entry.path());
    }

    return allFiles;
}

FileSet getAllFiles(const vector<string>& searchFolders)
{
    FileSet allFiles;
    for (const string& dir : searchFolders) {
        FileSet currentDirectoryFiles = getAllFiles(dir);
        allFiles.insert(currentDirectoryFiles.begin(), currentDirectoryFiles.end());
    }

    return allFiles;
}

unsigned int getThreadNum()
{
    unsigned int threadNum = std::thread::hardware_concurrency();

    // If can't detect then use 4 threads
    if (threadNum == 0) {
        threadNum = 4;
    }

    return threadNum;
}

void Executor::removeFiles(
    const string& sourceFolder,
    const vector<string>& searchFolders
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);
    unsigned int threadNum = getThreadNum();
    unsigned int filesToThread = sourceFiles.size() / threadNum;

    vector<future<void>> futures;
    for (
        auto it = sourceFiles.begin();
        it != sourceFiles.end() && distance(it, sourceFiles.end()) > filesToThread;
        advance(it, filesToThread)
    ) {
        auto last = (
            distance(it, sourceFiles.end()) > 2 * filesToThread ? next(it, filesToThread) : sourceFiles.end()
        );
        futures.push_back(
            async(
                [&filesToSearch] (FileSet::iterator begin, FileSet::iterator end) {
                    for (; begin != end; ++begin) {
                        if (filesToSearch.count(*begin)) {
                            remove(filesToSearch.find(*begin)->c_str());
                        }
                    }
                },
                it,
                last
            )
        );
    }
}

void Executor::moveFiles(
    const std::string& sourceFolder,
    const std::vector<std::string>&  searchFolders,
    const std::string& destFolder
)
{
}

void Executor::copyFiles(
    const std::string& sourceFolder,
    const std::vector<std::string>&  searchFolders,
    const std::string& destFolder
)
{
}

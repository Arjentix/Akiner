/**
 * Akiner - crossplatform Qt-based app which helps you to delete, move or copy akin files.
 * Copyright (C) 2020  Polyakov Daniil
 * Email address: arjentix@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "executor.h"

#include <unordered_set>
#include <future>
#include <filesystem>
#include <iostream>

#ifdef __unix__
#define PATH_DELIMITER '/'
#endif

#ifdef __WIN32__
#define PATH_DELIMITER '\\'
#endif

namespace fs = std::filesystem;

using std::vector;
using std::unordered_set;
using std::future;
using std::async;
using std::wstring;
using std::ifstream;
using std::distance;
using std::next;
using std::advance;
using std::invalid_argument;

// Local structures and functions declaration

struct FilePathHasher
{
    size_t operator()(const fs::path& filePath) const;
};

struct FilePathComparator
{
    bool operator()(const fs::path& lhs, const fs::path& rhs) const;
};

using FileSet = unordered_set<fs::path, FilePathHasher, FilePathComparator>;

/**
 * pathToSysString() - converts std::filesystem::path to std::string or
 * std::wstring, depending on OS. Used for better compatibility with Russian
 * language.
 */
SysString pathToSysString(const fs::path& path)
{
#ifdef __unix__
    return path.string();
#endif

#ifdef __WIN32__
    return path.wstring();
#endif
}

template <typename Function>
void runFunctionMultiThreaded(const FileSet& sourceFiles, Function&& function);

fs::path cutFileExtension(const fs::path& filePath);
fs::path cutFilePath(const fs::path& filePath);

FileSet getAllFiles(const fs::path& directory);
FileSet getAllFiles(const vector<fs::path>& searchFolders);

unsigned int getThreadNum();

// Member methods definition

void Executor::removeFiles(
    const fs::path& sourceFolder,
    const vector<fs::path>& searchFolders
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);

    runFunctionMultiThreaded(
        sourceFiles,
        [&filesToSearch] (FileSet::const_iterator begin, FileSet::const_iterator end) {
            for (; begin != end; ++begin) {
                if (filesToSearch.count(*begin)) {
                    fs::remove(*filesToSearch.find(*begin));
                }
            }
        }
    );
}

void Executor::moveFiles(
    const fs::path& sourceFolder,
    const std::vector<fs::path>&  searchFolders,
    const fs::path& destFolder
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);

    runFunctionMultiThreaded(
        sourceFiles,
        [&filesToSearch, &destFolder] (FileSet::const_iterator begin, FileSet::const_iterator end) {
            for (; begin != end; ++begin) {
                if (filesToSearch.count(*begin)) {
                    fs::path fileName = *filesToSearch.find(*begin);
                    fs::rename(
                        fileName,
                        pathToSysString(destFolder) +
                        SysString(1, PATH_DELIMITER) +
                        pathToSysString(cutFilePath(fileName))
                    );
                }
            }
        }
    );
}

void Executor::copyFiles(
    const fs::path& sourceFolder,
    const std::vector<fs::path>&  searchFolders,
    const fs::path& destFolder
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);

    runFunctionMultiThreaded(
        sourceFiles,
        [&filesToSearch, &destFolder] (FileSet::const_iterator begin, FileSet::const_iterator end) {
            for (; begin != end; ++begin) {
                if (filesToSearch.count(*begin)) {
                    fs::path fileName = *filesToSearch.find(*begin);
                    fs::copy_file(
                        fileName,
                        pathToSysString(destFolder) +
                        SysString(1, PATH_DELIMITER) +
                        pathToSysString(cutFilePath(fileName))
                    );
                }
            }
        }
    );
}

fs::path cutFileExtension(const fs::path& filePath)
{
    SysString sysStr = pathToSysString(filePath);
    return sysStr.substr(0, sysStr.find_last_of('.'));
}

fs::path cutFilePath(const fs::path& filePath)
{
    SysString sysStr = pathToSysString(filePath);
    return sysStr.substr(sysStr.find_last_of(PATH_DELIMITER) + 1);
}

size_t FilePathHasher::operator()(const fs::path& filePath) const
{
    return std::hash<SysString>()(pathToSysString(cutFilePath(cutFileExtension(filePath))));
}

bool FilePathComparator::operator()(const fs::path& lhs, const fs::path& rhs) const
{
    return cutFilePath(cutFileExtension(lhs)) == cutFilePath(cutFileExtension(rhs));
}

FileSet getAllFiles(const fs::path& directory)
{
    std::error_code err;
    if (!fs::is_directory(directory, err)) {
        throw invalid_argument(
            directory.string() +
            " не является директорией: " +
            err.message()
       );
    }

    FileSet allFiles;
    for (const auto& entry : fs::directory_iterator(directory)) {
        allFiles.insert(entry.path());
    }

    return allFiles;
}

FileSet getAllFiles(const vector<fs::path>& searchFolders)
{
    FileSet allFiles;
    for (const fs::path& dir : searchFolders) {
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

template <typename Function>
void runFunctionMultiThreaded(const FileSet& sourceFiles, Function&& function)
{
    unsigned int threadNum = getThreadNum();
    unsigned int filesToThread = sourceFiles.size() / threadNum;
    if (filesToThread == 0) {
        filesToThread = 1;
    }

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
            async(function, it, last)
        );
    }
}


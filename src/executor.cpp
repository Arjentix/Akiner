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
#include <filesystem>
#include <future>
#include <iostream>

#ifdef __unix__
#define PATH_DELIMITER '/'
#endif

#ifdef __WIN32__
#define PATH_DELIMITER '\\';
#endif

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

// Local structures and functions declaration

struct FilePathHasher
{
    size_t operator()(const string& filePath) const;
};

struct FilePathComparator
{
    bool operator()(const string& lhs, const string& rhs) const;
};

using FileSet = unordered_set<string, FilePathHasher, FilePathComparator>;

template <typename Function>
void runFunctionMultiThreaded(const FileSet& sourceFiles, Function&& function);

string cutFileExtension(const string& filePath);
string cutFilePath(const string& filePath);

FileSet getAllFiles(const string& directory);
FileSet getAllFiles(const vector<string>& searchFolders);

unsigned int getThreadNum();

// Member methods definition

void Executor::removeFiles(
    const string& sourceFolder,
    const vector<string>& searchFolders
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
    const std::string& sourceFolder,
    const std::vector<std::string>&  searchFolders,
    const std::string& destFolder
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);

    runFunctionMultiThreaded(
        sourceFiles,
        [&filesToSearch, &destFolder] (FileSet::const_iterator begin, FileSet::const_iterator end) {
            for (; begin != end; ++begin) {
                if (filesToSearch.count(*begin)) {
                    string fileName = *filesToSearch.find(*begin);
                    fs::rename(fileName, destFolder + PATH_DELIMITER + cutFilePath(fileName));
                }
            }
        }
    );
}

void Executor::copyFiles(
    const std::string& sourceFolder,
    const std::vector<std::string>&  searchFolders,
    const std::string& destFolder
)
{
    FileSet sourceFiles = getAllFiles(sourceFolder);
    FileSet filesToSearch = getAllFiles(searchFolders);

    runFunctionMultiThreaded(
        sourceFiles,
        [&filesToSearch, &destFolder] (FileSet::const_iterator begin, FileSet::const_iterator end) {
            for (; begin != end; ++begin) {
                if (filesToSearch.count(*begin)) {
                    string fileName = *filesToSearch.find(*begin);
                    std::cout << "Copying '" << fileName << "' to '" << destFolder + PATH_DELIMITER + cutFilePath(fileName) << std::endl;
                    fs::copy_file(fileName, destFolder + PATH_DELIMITER + cutFilePath(fileName));
                }
            }
        }
    );
}

string cutFileExtension(const string& filePath)
{
    return filePath.substr(0, filePath.find_last_of('.'));
}

string cutFilePath(const string& filePath)
{
    return filePath.substr(filePath.find_last_of(PATH_DELIMITER) + 1);
}

size_t FilePathHasher::operator()(const string& filePath) const
{
    return std::hash<string>()(cutFilePath(cutFileExtension(filePath)));
}

bool FilePathComparator::operator()(const string& lhs, const string& rhs) const
{
    return cutFilePath(cutFileExtension(lhs)) == cutFilePath(cutFileExtension(rhs));
}

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

template <typename Function>
void runFunctionMultiThreaded(const FileSet& sourceFiles, Function&& function)
{
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
            async(function, it, last)
        );
    }
}


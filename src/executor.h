#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <set>
#include <string>

/**
 * @brief The Executor class executes commands with files.
 */

class Executor
{
public:
    static void removeFiles(
        const std::string& sourceFolder,
        const std::vector<std::string>&  searchFolders
    );

    static void moveFiles(
        const std::string& sourceFolder,
        const std::vector<std::string>&  searchFolders,
        const std::string& destFolder
    );

    static void copyFiles(
        const std::string& sourceFolder,
        const std::vector<std::string>&  searchFolders,
        const std::string& destFolder
    );
};

#endif // EXECUTOR_H

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

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <vector>
#include <set>
#include <filesystem>

/**
 * SysString - system specific string needed to support files with Russian
 * language in the name.
 */

#ifdef __unix__
using SysString = std::string;
#endif

#ifdef __WIN32__
using SysString = std::wstring;
#endif

/**
 * The Executor class executes commands with files.
 */

class Executor
{
public:
    static void removeFiles(
        const std::filesystem::path& sourceFolder,
        const std::vector<std::filesystem::path>&  searchFolders
    );

    static void moveFiles(
        const std::filesystem::path& sourceFolder,
        const std::vector<std::filesystem::path>& searchFolders,
        const std::filesystem::path& destFolder
    );

    static void copyFiles(
        const std::filesystem::path& sourceFolder,
        const std::vector<std::filesystem::path>&  searchFolders,
        const std::filesystem::path& destFolder
    );
};

#endif // EXECUTOR_H

/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2024, Captain Yoshi
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Captain Yoshi
   Desc: Provide helpers for using the filesystem io
*/

#pragma once

#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

#include <ros/ros.h>
#include <ros/package.h>

#include <moveit_serialization/ryml/ryml.h>

std::string getDateStr()
{
    boost::posix_time::microsec_clock clock;

    return to_simple_string(clock.local_time());
}

bool isPrefix(const std::string& lhs, const std::string& rhs)
{
    return std::equal(lhs.begin(), lhs.begin() + std::min(lhs.size(), rhs.size()), rhs.begin());
}

bool isSuffix(const std::string& lhs, const std::string& rhs)
{
    return std::equal(lhs.rbegin(), lhs.rbegin() + std::min(lhs.size(), rhs.size()), rhs.rbegin());
}

bool isExtension(const std::string& path_string, const std::string& extension)
{
    boost::filesystem::path path(path_string);
    const std::string last = boost::filesystem::extension(path);
    return isSuffix(extension, last);
}

std::string getEnvironmentPath(const std::string& env)
{
    const char* home = std::getenv(env.c_str());
    if (home == nullptr)
        return "";

    return home;
}

boost::filesystem::path expandHome(const boost::filesystem::path& in)
{
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        ROS_WARN_STREAM("HOME Environment variable is not set! Cannot resolve ~ in path.");
        return in;
    }

    boost::filesystem::path out;
    for (const auto& p : in)
        out /= (p.string() == "~") ? std::string(home) + "/" : p;

    return out;
}

boost::filesystem::path expandSymlinks(const boost::filesystem::path& in)
{
    // Check if the path has a symlink before expansion to avoid error.
    boost::filesystem::path out;
    for (const auto& p : in) {
        auto tmp = out / p;
        if (boost::filesystem::is_symlink(tmp))
            return boost::filesystem::canonical(in);
    }

    return in;
}

boost::filesystem::path expandROSHome(const boost::filesystem::path& in)
{
    auto parent = in.parent_path().string();

    //
    if (parent.empty())
        parent = getEnvironmentPath("ROS_HOME");

    // Set parent as default ROS default home path
    if (parent.empty()) {
        parent = getEnvironmentPath("HOME");
        parent = parent + "/.ros";
    } else if (parent[0] != '/') {
        std::string tmp = parent;
        parent = getEnvironmentPath("HOME");
        parent = parent + "/.ros";
        parent = parent + "/" + tmp;
    }

    if (!parent.empty() && parent.back() != '/')
        parent = parent + '/';

    return parent + in.filename().string();
}

boost::filesystem::path expandPath(const boost::filesystem::path& in)
{
    boost::filesystem::path out = in;
    out = expandHome(out);
    out = expandSymlinks(out);
    out = expandROSHome(out);

    return boost::filesystem::absolute(out);
}

ryml::substr loadFileToString(const std::string& path, ryml::Tree& tree)
{
    std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type size = ifs.tellg();
    ryml::substr bytes = tree.alloc_arena(size);

    ifs.seekg(0, std::ios::beg);
    ifs.read(bytes.data(), bytes.size());

    return bytes;
}

const std::string resolvePackage(const std::string& path)
{
    if (path.empty())
        return "";

    const std::string prefix = "package://";

    boost::filesystem::path file;
    if (isPrefix(prefix, path)) {
        boost::filesystem::path subpath(path.substr(prefix.length(), path.length() - 1));
        const std::string package_name = (*subpath.begin()).string();

        const std::string package = ros::package::getPath(package_name);
        if (package.empty()) {
            // rospack above already complains on the command line
            // ROS_WARN("Package `%s` does not exist.", package_name.c_str());
            return "";
        }

        file = package;
        for (auto it = ++subpath.begin(); it != subpath.end(); ++it)
            file /= *it;
    } else
        file = path;

    return file.string();
}

const std::string resolvePath(const std::string& path)
{
    boost::filesystem::path file = resolvePackage(path);
    file = expandPath(file);

    if (!boost::filesystem::exists(file)) {
        ROS_WARN("File `%s` does not exist.", path.c_str());
        return "";
    }

    return boost::filesystem::canonical(boost::filesystem::absolute(file)).string();
}

ryml::substr loadFileToYAML(const std::string& path, ryml::NodeRef& node)
{
    ryml::substr substr;
    const std::string full_path = resolvePath(path);
    if (full_path.empty()) {
        ROS_ERROR("Failed to resolve file path `%s`.", path.c_str());
        return substr;
    }

    if (!isExtension(full_path, "yml") && !isExtension(full_path, "yaml")) {
        ROS_ERROR("YAML wrong extension for path `%s`.", full_path.c_str());
        return substr;
    }

    try {
        substr = ::loadFileToString(full_path, *node.tree());

        ryml::parse_in_place(ryml::to_csubstr(full_path), substr, node);

        // resolve yaml references
        node.tree()->resolve();
    }
    catch (moveit_serialization::yaml_error& e) {
        std::cout << e.what() << std::endl;

        return substr;
    }
    return substr;
}

std::string createFile(std::ofstream& out, const std::string& file)
{
    // resolve ros packages + path expansion
    boost::filesystem::path path = resolvePackage(file);
    path = expandPath(path);

    // create directories
    auto parent = path.parent_path().string();

    if (!parent.empty())
        boost::filesystem::create_directories(parent);

    out.open(path.string(), std::ofstream::out | std::ofstream::app);

    return path.string();
}

/**
 * @file coverage_database.cpp
 * @brief Implementation of CoverageDatabase class
 * 
 * This file contains the implementation of the CoverageDatabase class
 * which manages all the coverage data structures and provides methods
 * for querying and manipulating coverage information.
 * 
 * @author FunctionalCoverageParsers Library
 * @version 1.0
 * @date 2025
 */

#include "../include/coverage_types.h"
#include "../include/functional_coverage_parser.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace coverage_parser {

// Constructor
CoverageDatabase::CoverageDatabase() {
    reset();
}

// Query methods
CoverageGroup* CoverageDatabase::find_coverage_group(const std::string& name) {
    auto it = groups_table.find(name);
    return (it != groups_table.end()) ? it->second.get() : nullptr;
}

const CoverageGroup* CoverageDatabase::find_coverage_group(const std::string& name) const {
    auto it = groups_table.find(name);
    return (it != groups_table.end()) ? it->second.get() : nullptr;
}

HierarchyInstance* CoverageDatabase::find_hierarchy_instance(const std::string& path) {
    auto it = hierarchy_table.find(path);
    return (it != hierarchy_table.end()) ? it->second.get() : nullptr;
}

const HierarchyInstance* CoverageDatabase::find_hierarchy_instance(const std::string& path) const {
    auto it = hierarchy_table.find(path);
    return (it != hierarchy_table.end()) ? it->second.get() : nullptr;
}

ModuleDefinition* CoverageDatabase::find_module_definition(const std::string& name) {
    auto it = modules_table.find(name);
    return (it != modules_table.end()) ? it->second.get() : nullptr;
}

const ModuleDefinition* CoverageDatabase::find_module_definition(const std::string& name) const {
    auto it = modules_table.find(name);
    return (it != modules_table.end()) ? it->second.get() : nullptr;
}

AssertCoverage* CoverageDatabase::find_assert_coverage(const std::string& name) {
    auto it = asserts_table.find(name);
    return (it != asserts_table.end()) ? it->second.get() : nullptr;
}

const AssertCoverage* CoverageDatabase::find_assert_coverage(const std::string& name) const {
    auto it = asserts_table.find(name);
    return (it != asserts_table.end()) ? it->second.get() : nullptr;
}

// Data manipulation methods
void CoverageDatabase::add_coverage_group(std::unique_ptr<CoverageGroup> group) {
    if (group && !group->name.empty()) {
        groups_table[group->name] = std::move(group);
        update_timestamp();
    }
}

void CoverageDatabase::add_hierarchy_instance(std::unique_ptr<HierarchyInstance> instance) {
    if (instance && !instance->instance_path.empty()) {
        hierarchy_table[instance->instance_path] = std::move(instance);
        update_timestamp();
    }
}

void CoverageDatabase::add_module_definition(std::unique_ptr<ModuleDefinition> module) {
    if (module && !module->module_name.empty()) {
        modules_table[module->module_name] = std::move(module);
        update_timestamp();
    }
}

void CoverageDatabase::add_assert_coverage(std::unique_ptr<AssertCoverage> assert_cov) {
    if (assert_cov && !assert_cov->assert_name.empty()) {
        asserts_table[assert_cov->assert_name] = std::move(assert_cov);
        update_timestamp();
    }
}

// Utility methods
void CoverageDatabase::reset() {
    dashboard_data.reset();
    groups_table.clear();
    hierarchy_table.clear();
    modules_table.clear();
    asserts_table.clear();
    is_valid = false;
    update_timestamp();
}

bool CoverageDatabase::validate() const {
    // Basic validation checks
    if (groups_table.empty() && hierarchy_table.empty() && 
        modules_table.empty() && asserts_table.empty()) {
        return false; // Empty database is not valid
    }
    
    // Check if all groups have valid metrics
    for (const auto& [name, group] : groups_table) {
        if (!group || group->name.empty()) {
            return false;
        }
        if (group->coverage.expected == 0 && group->coverage.covered > 0) {
            return false; // Invalid metrics
        }
    }
    
    // Check if all hierarchy instances have valid paths
    for (const auto& [path, instance] : hierarchy_table) {
        if (!instance || instance->instance_path.empty()) {
            return false;
        }
    }
    
    // Check if all modules have valid definitions
    for (const auto& [name, module] : modules_table) {
        if (!module || module->module_name.empty()) {
            return false;
        }
    }
    
    // Check if all assertions have valid coverage
    for (const auto& [name, assert_cov] : asserts_table) {
        if (!assert_cov || assert_cov->assert_name.empty()) {
            return false;
        }
    }
    
    return true;
}

double CoverageDatabase::calculate_overall_score() const {
    if (groups_table.empty()) {
        return 0.0;
    }
    
    std::uint32_t total_covered = 0;
    std::uint32_t total_expected = 0;
    
    for (const auto& [name, group] : groups_table) {
        if (group) {
            total_covered += group->coverage.covered;
            total_expected += group->coverage.expected;
        }
    }
    
    return (total_expected > 0) ? (100.0 * total_covered / total_expected) : 0.0;
}

std::vector<CoverageGroup*> CoverageDatabase::get_groups_by_pattern(const std::string& pattern) const {
    std::vector<CoverageGroup*> result;
    
    for (const auto& [name, group] : groups_table) {
        if (group && name.find(pattern) != std::string::npos) {
            result.push_back(group.get());
        }
    }
    
    return result;
}

std::vector<CoverageGroup*> CoverageDatabase::get_uncovered_groups() const {
    std::vector<CoverageGroup*> result;
    
    for (const auto& [name, group] : groups_table) {
        if (group && group->coverage.covered == 0) {
            result.push_back(group.get());
        }
    }
    
    return result;
}

std::unique_ptr<CoverageStatistics> CoverageDatabase::generate_statistics() const {
    auto stats = std::make_unique<CoverageStatistics>();
    
    // Calculate overall coverage metrics
    std::uint32_t total_covered = 0;
    std::uint32_t total_expected = 0;
    
    for (const auto& [name, group] : groups_table) {
        if (group) {
            total_covered += group->coverage.covered;
            total_expected += group->coverage.expected;
            
            if (group->coverage.covered == 0) {
                stats->num_zero_coverage_groups++;
            } else if (group->coverage.covered == group->coverage.expected) {
                stats->num_full_coverage_groups++;
            }
        }
    }
    
    stats->overall_coverage_score = (total_expected > 0) ? (100.0 * total_covered / total_expected) : 0.0;
    stats->total_coverage_points = total_expected;
    stats->covered_points = total_covered;
    
    return stats;
}

// Utility functions
std::string parser_result_to_string(ParserResult result) {
    switch (result) {
        case ParserResult::SUCCESS:
            return "SUCCESS";
        case ParserResult::ERROR_FILE_NOT_FOUND:
            return "ERROR_FILE_NOT_FOUND";
        case ParserResult::ERROR_INVALID_FORMAT:
            return "ERROR_INVALID_FORMAT";
        case ParserResult::ERROR_MEMORY_ALLOCATION:
            return "ERROR_MEMORY_ALLOCATION";
        case ParserResult::ERROR_INVALID_PARAMETER:
            return "ERROR_INVALID_PARAMETER";
        case ParserResult::ERROR_PARSE_FAILED:
            return "ERROR_PARSE_FAILED";
        default:
            return "UNKNOWN_ERROR";
    }
}

std::ostream& operator<<(std::ostream& os, const CoverageMetrics& metrics) {
    os << "Coverage: " << metrics.covered << "/" << metrics.expected 
       << " (" << std::fixed << std::setprecision(2) << metrics.score << "%)";
    return os;
}

std::ostream& operator<<(std::ostream& os, const CoverageGroup& group) {
    os << "Group: " << group.name << " - " << group.coverage;
    return os;
}

// HierarchyInstance utility methods implementation
void HierarchyInstance::calculate_depth_level() {
    depth_level = 0;
    for (char c : instance_path) {
        if (c == '.') {
            depth_level++;
        }
    }
}

void HierarchyInstance::extract_module_name() {
    if (instance_path.empty()) {
        module_name = "";
        return;
    }
    
    // Find the last component of the path (after the last '.')
    size_t last_dot = instance_path.find_last_of('.');
    if (last_dot != std::string::npos) {
        module_name = instance_path.substr(last_dot + 1);
    } else {
        module_name = instance_path; // No dots, entire path is module name
    }
}

std::string HierarchyInstance::get_parent_path() const {
    size_t last_dot = instance_path.find_last_of('.');
    if (last_dot != std::string::npos) {
        return instance_path.substr(0, last_dot);
    }
    return ""; // No parent (root level)
}

std::vector<std::string> HierarchyInstance::get_path_components() const {
    std::vector<std::string> components;
    std::stringstream ss(instance_path);
    std::string component;
    
    while (std::getline(ss, component, '.')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    
    return components;
}

} // namespace coverage_parser
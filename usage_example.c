// Example usage of FunctionalCoverageParsers DLL
#include "functional_coverage_parser_dll.h"

int main() {
    // Create database
    void* db = create_coverage_database();
    
    // Create parsers
    void* dashboard_parser = create_dashboard_parser();
    void* groups_parser = create_groups_parser();
    
    // Parse files
    int result = parse_coverage_file(dashboard_parser, "dashboard.txt", db);
    if (result == 0) {
        printf("Dashboard parsed successfully\n");
    }
    
    result = parse_coverage_file(groups_parser, "groups.txt", db);
    if (result == 0) {
        printf("Groups parsed successfully\n");
    }
    
    // Get statistics
    double score = calculate_overall_score(db);
    printf("Overall coverage: %.2f%%\n", score);
    
    int num_groups = get_num_groups(db);
    printf("Number of groups: %d\n", num_groups);
    
    // Export results
    export_coverage_to_xml(db, "coverage_report.xml");
    export_coverage_to_json(db, "coverage_report.json");
    
    // Cleanup
    destroy_parser(dashboard_parser);
    destroy_parser(groups_parser);
    destroy_coverage_database(db);
    cleanup_library();
    
    return 0;
}

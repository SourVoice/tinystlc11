#include "test.h"
#include "include/algorithm_test.h"
#include "include/algorithm_performance.h"
#include "include/vector_test.h"
#include "include/deque_test.h"
#include "include/queue_test.h"
#include "include/stack_test.h"
#include "include/list_test.h"
#include "include/string_test.h"
#include "include/map_test.h"
#include "include/set_test.h"

int main() {

    using namespace MySTL::test;
    std::cout.sync_with_stdio(false);

    RUN_ALL_TESTS();
    algorithm_performance::algorithm_performance_test();
    vector_test::vector_test();
    deque_test::deque_test();
    queue_test::queue_test();
    queue_test::priority_queue_test();
    stack_test::stack_test();
    list_test::list_test();
    string_test::string_test();
    map_test::map_test();
    map_test::multimap_test();
    set_test::set_test();
    set_test::multiset_test();
}
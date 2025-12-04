#include "Boat.h"
#include <cstdlib>
#include "Human.h"

Boat::Boat(int numAdults, int numChildren)
    : adults_island_(numAdults),
      children_island_(numChildren),
      travel_to_mainland_count_(0),
      travel_to_island_count_(0),
      boats_two_children_(0),
      boats_child_adult_(0),
      boats_one_person_(0),
      times_adult_driver_(0),
      times_child_driver_(0),
      peopleleft(numAdults + numChildren)
{
}



void Boat::print_summary() {
    std::cout << "\nSummary of Events\n";
    std::cout << "Boat traveled to the mainland: " << travel_to_mainland_count_ << "\n";
    std::cout << "Boat returned to the island: " << travel_to_island_count_ << "\n";
    std::cout << "Boats with 2 children: " << boats_two_children_ << "\n";
    std::cout << "Boats with 1 child and 1 adult: " << boats_child_adult_ << "\n";
    std::cout << "Boats with only 1 person (child or adult): " << boats_one_person_ << "\n";
    std::cout << "Times adults where the driver: " << times_adult_driver_ << "\n";
    std::cout << "Times children where the driver: " << times_child_driver_ << "\n";
}

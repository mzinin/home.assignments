#include <order_counter.hpp>


void OrderCounter::add(const Record& record)
{
    switch (record.operation)
    {
        case Operation::INSERT:
            {
                auto& orders = activeOrders_[record.symbol];
                orders.insert(record.orderID);
                numberOfOrders_[record.symbol] = static_cast<uint32_t>(orders.size());
            }
            break;

        case Operation::CANCEL:
            {
                auto it = activeOrders_.find(record.symbol);
                if (it == activeOrders_.end())
                {
                    break;
                }

                auto& orders = it->second;
                orders.erase(record.orderID);
                if (orders.empty())
                {
                    activeOrders_.erase(record.symbol);
                    numberOfOrders_.erase(record.symbol);
                }
                else
                {
                    numberOfOrders_[record.symbol] = static_cast<uint32_t>(orders.size());
                }
            }
            break;

        default:
            break;
    }
}

std::unordered_map<Symbol, uint32_t> OrderCounter::orderCounts() const
{
    return numberOfOrders_;
}

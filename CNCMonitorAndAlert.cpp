
#include <iostream>
#include <memory>
#include <string>

#define THRESHOLD_TEMP 35
#define THRESHOLD_DURATION_IN_HOURS 6
#define HOUR_IN_MINUTES 60



/**
 * Class implements the interface to provide the monitored data.
 */
class CNCMonitoredData
{
    public:
    int GetTemperatureData()
    {
        int input;
        std::cout << "Enter Temperature data"<<std::endl;
        std::cin >> input;
        return input;
    }

    float GetPartDimensionData()
    {
        float input;
        std::cout << "Enter Dimension data"<<std::endl;
        std::cin >> input;
        return input;
    }

    int GetDurationOfOperation()
    {
        int input;
        std::cout << "Enter Duration of hours data"<<std::endl;
        std::cin >> input;
        return input * HOUR_IN_MINUTES;
    }
};

class AlertStrategy
{
public:
    virtual ~AlertStrategy() = default;
    virtual std::string performAlertFunction() const = 0;
};

/**
  This class delegates some work to the Strategy object instead of
  implementing multiple versions of the algorithm on its own.
 */

class DelegateAlertStrategy
{
private:
    std::unique_ptr<AlertStrategy> alertstrategy_;
public:

    explicit DelegateAlertStrategy(std::unique_ptr<AlertStrategy> &&strategy = {}) : alertstrategy_(std::move(strategy))
    {

    }

    
    /**
     * Sets strategy based on the action that needs to be performed.
     */
    void set_strategy(std::unique_ptr<AlertStrategy> &&strategy)
    {
        alertstrategy_ = std::move(strategy);
    }
    /**
     * Generic method which calls specific strategy to perform intentional function
     */
    
    void DelegateStrategy() const
    {
        if (alertstrategy_) 
        {
            std::string result = alertstrategy_->performAlertFunction();
            std::cout << result << "\n";
        } else 
        {
            std::cout << "Strategy not set\n";
        }
    }
};


/**
 * Concrete Strategies implement the algorithm while following the base Strategy
 * interface. The interface makes them interchangeable in the Context.
 */
class DefaultAlertStrategy : public AlertStrategy
{
public:
    std::string performAlertFunction() const override
    {
        std::string result("Default strategy");
        return result;
    }
};

class TemperatureAlertStrategy : public AlertStrategy
{
    std::string performAlertFunction() const override
    {
        std::string result("Temperature Normal");
        CNCMonitoredData obj;
        int monitoredTempData = obj.GetTemperatureData();
        if(monitoredTempData > THRESHOLD_TEMP)
        {
            result = "Environment Temperature too high";
        }

        return result;
    }
};

const float referencevalue = 1.85f;

class PartDimensionAlertStrategy : public AlertStrategy
{
    std::string performAlertFunction() const override
    {
        CNCMonitoredData obj;
        std::string result("Dimension alert");
        float monitoredDimValue = obj.GetPartDimensionData();
        if((monitoredDimValue - referencevalue) > 0.05)
        {
            result = "Dimension of the part more than threshold value";
        }
        return result;
    }
};

class DurationOfOperationAlertStrategy : public AlertStrategy
{
    std::string performAlertFunction() const override
    {
        int convertohours;
        std::string result("Normal operation hours");
        CNCMonitoredData obj;
        int monitoredDurationMinutes = obj.GetDurationOfOperation();
        if(monitoredDurationMinutes)
        {
            convertohours = monitoredDurationMinutes/HOUR_IN_MINUTES;
            if(convertohours > THRESHOLD_DURATION_IN_HOURS)
            {
                result = "Operation hours more than threshold hours (6)";
            }
        }
        else
        {
            result = "Invalid minute info received";
        }
    
        return result;
    }
};


/**
 * This function shall call and set the appropriate strategy to process alerts
 * and accordingly inform via selected strategy
 */

void ProcessCNCMonitoredData()
{
    DelegateAlertStrategy context(std::make_unique<DefaultAlertStrategy>());
    std::cout << "Default strategy to display self-status-code.\n";
    context.DelegateStrategy();
    std::cout << "\n";
    std::cout << "Call to temperature strategy to display temperature Alert.\n";
    context.set_strategy(std::make_unique<TemperatureAlertStrategy>());
    context.DelegateStrategy();
    std::cout << "\n";
    std::cout << "Call to part-dimension strategy to display Dimension Alert.\n";
    context.set_strategy(std::make_unique<PartDimensionAlertStrategy>());
    context.DelegateStrategy();
    std::cout << "\n";
    std::cout << "Call to Duration of operation strategy to display over duration alert Alert.\n";
    context.set_strategy(std::make_unique<DurationOfOperationAlertStrategy>());
    context.DelegateStrategy();
}

int main()
{
    ProcessCNCMonitoredData();
    return 0;
}
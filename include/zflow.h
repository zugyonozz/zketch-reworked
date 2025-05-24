// zflow.h
#pragma once
#include <functional>

class zFlow {
private:
    std::function<bool()> condition;
    std::function<void()> funcYes;
    std::function<void()> funcNo;

public:
    // Default constructor
    zFlow() = default;
    
    // Construct from condition and two functions
    template <typename Cond, typename YesFunc, typename NoFunc>
    zFlow(Cond&& cond, YesFunc&& yes, NoFunc&& no = [](){}) :
        condition(std::forward<Cond>(cond)),
        funcYes(std::forward<YesFunc>(yes)),
        funcNo(std::forward<NoFunc>(no))
    {}
    
	// Construct from std::pair structure
    zFlow(const std::pair<std::function<bool()>, 
          std::pair<std::function<void()>, std::function<void()>>>& flow);
    
    // Helper function to create from condition and two functions
    template <typename Cond, typename YesFunc, typename NoFunc>
    static zFlow create(Cond&& cond, YesFunc&& yes, NoFunc&& no) {
        return zFlow(std::forward<Cond>(cond),
                    std::forward<YesFunc>(yes),
                    std::forward<NoFunc>(no));
    }
    
    // Helper function to create with arguments for yes and no functions
    template <typename Cond, typename YesFunc, typename... YesArgs,
              typename NoFunc, typename... NoArgs>
    static zFlow createWithArgs(
        Cond&& cond,
        YesFunc&& yes, std::tuple<YesArgs...> yesArgs,
        NoFunc&& no, std::tuple<NoArgs...> noArgs
    ) {
        auto boundYes = [yes, yesArgs]() { std::apply(yes, yesArgs); };
        auto boundNo = [no, noArgs]() { std::apply(no, noArgs); };
        
        return zFlow(std::forward<Cond>(cond), boundYes, boundNo);
    }
    
    // Operator to execute the flow
    void operator()() const;
    // Execute method as an alternative to operator()
    void execute() const;
    // Fluent setter methods
    zFlow& setCondition(std::function<bool()> cond);
    zFlow& setYesAction(std::function<void()> action);
    zFlow& setNoAction(std::function<void()> action);
};

// Helper function to create a zFlow from a pair structure
zFlow makeFlow(
	const std::pair<std::function<bool()>, 
	std::pair<std::function<void()>, 
	std::function<void()>>>& flowPair
);
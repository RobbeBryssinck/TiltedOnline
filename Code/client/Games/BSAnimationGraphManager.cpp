#include <TiltedOnlinePCH.h>

#include <BSAnimationGraphManager.h>
#include <Havok/BShkbAnimationGraph.h>
#include <Havok/BShkbHkxDB.h>
#include <Havok/hkbBehaviorGraph.h>

#include <map>

void BSAnimationGraphManager::DumpAnimationVariables()
{
    if (animationGraphIndex < animationGraphs.size)
    {
        const auto pGraph = animationGraphs.Get(animationGraphIndex);
        if(pGraph)
        {
            const auto pDb = pGraph->hkxDB;
            const auto pBuckets = pDb->animationVariables.buckets;
            const auto pVariableSet = pGraph->behaviorGraph->animationVariables;

            if(pBuckets && pVariableSet)
            {
                std::map<uint32_t, std::tuple<const char*, uint32_t> > variables;

                for(decltype(pDb->animationVariables.bucketCount) i = 0; i < pDb->animationVariables.bucketCount; ++i)
                {
                    auto pBucket = &pBuckets[i];
                    if (!pBucket->next)
                        continue;

                    while(pBucket != pDb->animationVariables.end)
                    {
                        const auto variableIndex = pBucket->value;
                        if(pVariableSet->size > static_cast<uint32_t>(variableIndex))
                        {
                            const auto value = pVariableSet->data[variableIndex];

                            variables[variableIndex] = std::make_tuple(pBucket->key.AsAscii(), value);
                        }

                        pBucket = pBucket->next;
                    }
                }

                for(auto& [id, name] : variables)
                {
                    auto val = std::get<1>(name);
                    std::cout << "k" << std::get<0>(name) << " = " << id << "," << std::endl;

                    //spdlog::info("{} {} f: {}, i: {}", var.first, std::get<0>(var.second), *(float*)&val, *(int32_t*)&val);
                }
            }
        }
    }
}

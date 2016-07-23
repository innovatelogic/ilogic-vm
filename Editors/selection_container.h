#pragma once

#include <vector>
#include <map>

namespace oes
{
    namespace editors
    {

        template<class T_CLASS>
        class SelectionContainer final
        {
            using TMapContainer = std::map<T_CLASS*, std::vector<T_CLASS*>>;

        public:
            //----------------------------------------------------------------------------------------------
            SelectionContainer()
            {

            }

            //----------------------------------------------------------------------------------------------
            ~SelectionContainer()
            {
                Empty();
            }

            //----------------------------------------------------------------------------------------------
            bool AddItem(T_CLASS *key, T_CLASS *item)
            {
                bool bResult = false;
                if (key && item)
                {
                    //case: no key
                    auto iter = m_container.find(key);
                    if (iter == m_container.end())
                    {
                        m_container[key] = { item };
                        bResult = true;
                    }
                    else
                    {
                        auto &refVec = iter->second;

                        // key already present try to push item
                        auto iterElem = std::find(refVec.begin(), refVec.end(), item);
                        if (iterElem == refVec.end())
                        {
                            refVec.push_back(item);
                            bResult = true;
                        }
                    }
                }

                return bResult;
            }

            //----------------------------------------------------------------------------------------------
            bool RemoveItem(T_CLASS *key, T_CLASS *item = nullptr)
            {
                bool bResult = false;
                if (key)
                {
                    auto iter = m_container.find(key);
                    if (iter != m_container.end())
                    {
                        if (item)
                        {
                            auto &refVec = iter->second;
                            auto itemFind = std::find(refVec.begin(), refVec.end(), item);
                            if (itemFind != refVec.end())
                            {
                                refVec.erase(itemFind);

                                if (refVec.empty()) { // empty size release all record
                                    m_container.erase(iter);
                                }

                                bResult = true;
                            }
                        }
                        else
                        {
                            m_container.erase(iter);
                            bResult = true;
                        }
                    }
                }
                return bResult;
            }

            //----------------------------------------------------------------------------------------------
            size_t Size()
            {
                size_t out = 0;
                for (auto iter = m_container.begin(); iter != m_container.end(); ++iter)
                {
                    out += iter->second.size();
                }
                return out;
            }

            //----------------------------------------------------------------------------------------------
            void Empty()
            {
                m_container.clear();
            }

            //----------------------------------------------------------------------------------------------
            std::vector<T_CLASS*> Keys()
            {
                std::vector<T_CLASS*> out;
                for each (auto item in m_container)
                {
                    T_CLASS *key = item.first;
                    out.push_back(key);
                }
                return out;
            }

            //----------------------------------------------------------------------------------------------
            void GetAllElements(std::vector<T_CLASS*> &out)
            {
                out.resize(Size()); 
                size_t idx = 0;

                for each(auto item in m_container)
                {
                    for each (auto *elem in item.second)
                    {
                        out[idx++] = elem;
                    }
                }
            }

        private:
            TMapContainer m_container;
        };
    }
}

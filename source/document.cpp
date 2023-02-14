#include "document.hpp"
#include "util.hpp"
#include <stdexcept>

Document::Document(Memory mode, const std::string& data)
{
    m_mode = mode;
    if (m_mode == Memory::Structured)
    {
        try
        {
            // see if this works, or if the variant has to be accessed by a method
            m_data = util::build_structure(data); // TODO: implement util::build_structure
        }
        catch (...)
        {
            // add catch wrapper to error message
            throw std::runtime_error("Error constructing Document, error messages not yet implemented");
        }
    }
    if (m_mode == Memory::Parsed)
    {
        // possibly add syntax check here controlled by arg or global macro define
        m_data = data;
    }

    // TODO: figure out how Temporary works and implement
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     Node.cpp (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Node.h"
#include "InputPort.h"
#include "ModelTransformer.h"

// utilities
#include "IArchivable.h"

// stl
#include <unordered_set>

namespace emll
{
/// <summary> model namespace </summary>
namespace model
{
    Node::Node(const std::vector<InputPortBase*>& inputs, const std::vector<OutputPortBase*>& outputs)
        : _id(NodeId()), _inputs(inputs), _outputs(outputs){};

    void Node::AddInputPort(InputPortBase* input)
    {
        _inputs.push_back(input);
    }

    InputPortBase* Node::GetInputPort(const std::string& portName)
    {
        for (auto port : _inputs)
        {
            if (port->GetName() == portName)
            {
                return port;
            }
        }
        return nullptr;
    }

    OutputPortBase* Node::GetOutputPort(const std::string& portName)
    {
        for (auto port : _outputs)
        {
            if (port->GetName() == portName)
            {
                return port;
            }
        }
        return nullptr;
    }

    std::vector<const Node*> Node::GetParentNodes() const
    {
        std::unordered_set<const Node*> nodes;
        for (const auto& port : _inputs)
        {
            for (const auto& node : port->GetParentNodes())
            {
                nodes.insert(node);
            }
        }
        return std::vector<const Node*>{ nodes.begin(), nodes.end() };
    }

    void Node::AddDependent(const Node* dependent) const
    {
        _dependentNodes.push_back(dependent);
    }

    void Node::RegisterDependencies() const
    {
        for (const auto& input : _inputs)
        {
            for (const auto& node : input->GetParentNodes())
            {
                node->AddDependent(this);
            }
            for (const auto& range : input->GetInputElements().GetRanges())
            {
                range.ReferencedPort()->ReferencePort();
            }
        }
    }

    void Node::InvokeCopy(ModelTransformer& transformer) const
    {
        Copy(transformer);
    }

    bool Node::InvokeRefine(ModelTransformer& transformer) const
    {
        return Refine(transformer);
    }

    // Default implementation of Refine just copies and returns false
    bool Node::Refine(ModelTransformer& transformer) const
    {
        Copy(transformer);
        return false;
    }

    void Node::WriteToArchive(utilities::Archiver& archiver) const
    {
        archiver["id"] << _id;
    }

    void Node::ReadFromArchive(utilities::Unarchiver& archiver)
    {
        NodeId oldId;
        archiver["id"] >> oldId;
        _id = oldId;
        auto& context = archiver.GetContext();
        ModelSerializationContext& newContext = dynamic_cast<ModelSerializationContext&>(context);
        newContext.MapNode(oldId, this);
    }
}
}

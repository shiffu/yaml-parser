#pragma once

#include <string>
#include <vector>
#include <iostream>

struct Node
{
    Node() = default;
    virtual ~Node() = default;

    virtual Node& operator[](size_t size) = 0;
    virtual const Node& operator[](size_t size) const = 0;

    virtual Node& operator[](const std::string& key) = 0;
    virtual const Node& operator[](const std::string& key) const = 0;
};

struct Document
{
    Document() = default;
    ~Document() = default;

    std::vector<Node> nodes;
};

struct SeqNode : public Node
{
    SeqNode() = default;
    virtual ~SeqNode() = default;

    virtual Node& operator[](size_t index) override;
    virtual const Node& operator[](size_t index) const override;

    virtual Node& operator[](const std::string& key) override;
    virtual const Node& operator[](const std::string& key) const override;

// private:
    std::vector<Node*> data{};
};

struct ValueNode : public Node
{
    ValueNode() = default;
    ValueNode(const char* str) : value(str) {}
    virtual ~ValueNode() = default;

    virtual Node& operator[](size_t index) override;
    virtual const Node& operator[](size_t index) const override;

    virtual Node& operator[](const std::string& key) override;
    virtual const Node& operator[](const std::string& key) const override;

    std::string value{};
};


Node& SeqNode::operator[](size_t index)
{
    return *data[index];
}

const Node& SeqNode::operator[](size_t index) const
{
    return *data[index];
}

Node& SeqNode::operator[](const std::string& key)
{
    std::cerr << "Not implemented for sequence node" << std::endl;
}

const Node& SeqNode::operator[](const std::string& key) const
{
    std::cerr << "Not implemented for sequence node" << std::endl;
}

// ////////////////////////////////////////////////////////////////////////

Node& ValueNode::operator[](size_t index)
{
    std::cerr << "Not implemented for ValueNode" << std::endl;
}

const Node& ValueNode::operator[](size_t index) const
{
    std::cerr << "Not implemented for ValueNode" << std::endl;
}

Node& ValueNode::operator[](const std::string& key)
{
    std::cerr << "Not implemented for ValueNode" << std::endl;
}

const Node& ValueNode::operator[](const std::string& key) const
{
    std::cerr << "Not implemented for ValueNode" << std::endl;
}

#include <bits/stdc++.h>

template<typename Value>
class SingleLinkedList {
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... v)
                : next(nullptr)
                , val(std::forward<Args>(v)...)
        {}

        std::shared_ptr<Node> next;
        Value val;
    };

public:

    class ListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<value_type>;
        using reference = value_type&;

        ListIterator() = default;

        ListIterator(std::shared_ptr<Node> ptr)
                : base_(std::move(ptr))
        {}

        reference operator*() {
            return base_->val;
        }

        reference operator*() const {
            return base_->val;
        }

        pointer operator->() {
            return base_->val;
        }

        std::shared_ptr<const Node> operator->() const {
            return base_;
        }

        ListIterator operator++() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            return base_ = base_->next;
        }

        ListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ListIterator");
            }
            Node *buf = base_;
            base_ = base_->next;
            return buf;
        }

        bool operator==(const ListIterator&) const = default;
        bool operator!=(const ListIterator&) const = default;

    private:
        std::shared_ptr<Node> base_;
    };

    class ConstListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Value;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::shared_ptr<value_type>;
        using reference = const value_type&;

        ConstListIterator(std::shared_ptr<Node> ptr)
                : base_(std::move(ptr))
        {}

        reference operator*() const {
            return *base_;
        }

        pointer operator->() {
            return base_;
        }

        const pointer operator->() const {
            return base_;
        }

        ConstListIterator operator++() {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            return base_ = base_->next;
        }

        ConstListIterator operator++(int /*fucking_unused*/) {
            if (base_ == nullptr) {
                throw std::runtime_error("Attempt to dereference null ConstListIterator");
            }
            Node *buf = base_;
            base_ = base_->next;
            return buf;
        }

        bool operator==(const ConstListIterator&) const = default;
        bool operator!=(const ConstListIterator&) const = default;

    private:
        std::shared_ptr<const Node> base_;
    };

    SingleLinkedList() = default;

    SingleLinkedList(const SingleLinkedList<Value> &other) {
        std::for_each(other.begin(), other.end(), [&] (const Value &val) {
            PushBack(val);
        });
    }

    SingleLinkedList(SingleLinkedList &&other) {
        first_ = other.first_;
        last_ = other.last_;
        other.first_ = nullptr;
        other.last_ = nullptr;
    }

    template<typename Iterator>
    SingleLinkedList(Iterator beg, Iterator end) {
        std::for_each(beg, end, [&] (auto x) {
            PushBack(x);
        });
    }


    template<typename T>
    void PushBack(T&& val) {
        if (last_ != nullptr) {
            last_->next = std::make_shared<Node>(std::forward<T>(val));
            last_ = last_->next;
        } else {
            last_ = first_ = std::make_shared<Node>(std::forward<T>(val));
        }
    }

    template<typename... Args>
    void EmplaceBack(Args&&... val) {
        last_->next = std::make_shared<Node>(std::forward(val)...);
        last_ = last_->next;
    }

    ConstListIterator begin() const {
        return first_;
    }

    ConstListIterator end() const {
        return ConstListIterator();
    }

    ListIterator begin() {
        return first_;
    }

    ListIterator end() {
        return ListIterator();
    }

    ConstListIterator cbegin() const {
        return first_;
    }

    ConstListIterator cend() const {
        return ConstListIterator();
    }

private:
    std::shared_ptr<Node> first_, last_;
};
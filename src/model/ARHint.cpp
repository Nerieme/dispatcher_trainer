#include "dispatcher/model/ARHint.hpp"
#include <stdexcept>

namespace dispatcher {

ARHint::ARHint(std::string id, std::string text, std::string anchorId, int priority)
    : id_(std::move(id)), text_(std::move(text)),
      anchorId_(std::move(anchorId)), priority_(priority) {
    if (id_.empty())       throw std::invalid_argument("ARHint: id must not be empty");
    if (text_.empty())     throw std::invalid_argument("ARHint: text must not be empty");
    if (anchorId_.empty()) throw std::invalid_argument("ARHint: anchorId must not be empty");
    if (priority_ < 0)     throw std::invalid_argument("ARHint: priority must be >= 0");
}

std::string ARHint::id() const { return id_; }
std::string ARHint::kind() const { return "ARHint"; }
std::string ARHint::status() const { return "Normal"; }

std::string ARHint::summary() const {
    return "[ARHint " + id_ + "] \"" + text_ + "\" -> anchor=" + anchorId_;
}

VisualDescriptor ARHint::toVisualDescriptor() const {
    VisualDescriptor d;
    d.id         = id_;
    d.visualKind = "ar-hint";
    d.label      = text_;
    d.color      = "cyan";
    d.priority   = priority_;
    d.anchorId   = anchorId_;
    return d;
}

const std::string& ARHint::anchorId() const { return anchorId_; }
int ARHint::priority() const { return priority_; }

} // namespace dispatcher
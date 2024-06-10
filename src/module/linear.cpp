#include "module/linear.h"

#include "threading/scheduler.h"

namespace dllm::module {
LinearImpl::LinearImpl(const Scheduler& scheduler, const Options& options) {
  std::shared_ptr<compute::Linear::State> state;
  compute::Linear::init(scheduler, state, options);
  register_state("LinearState", state);
  state_ = state;
}

void LinearImpl::forward(const Scheduler& scheduler, Tensor& output,
                         const ReadOnlyTensor& input) const {
  compute::Linear::forward(scheduler, state(), output, input);
}

void LinearImpl::backward(const Scheduler& scheduler, Tensor& grad_input,
                          const ReadOnlyTensor& grad_output) const {
  compute::Linear::backwardInput(scheduler, state(), grad_input, grad_output);
  compute::Linear::backwardParameter(scheduler, state(), grad_output);
}

void LinearImpl::backwardParameter(const Scheduler& scheduler,
                                   const ReadOnlyTensor& grad_output) const {
  compute::Linear::backwardParameter(scheduler, state(), grad_output);
}

void LinearImpl::backwardInput(const Scheduler& scheduler, Tensor& grad_input,
                               const ReadOnlyTensor& grad_output) const {
  compute::Linear::backwardInput(scheduler, state(), grad_input, grad_output);
}

std::shared_ptr<compute::Linear::State> LinearImpl::state() const {
  return state_.lock();
}
}  // namespace dllm::module

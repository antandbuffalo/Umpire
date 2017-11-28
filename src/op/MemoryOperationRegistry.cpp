#include "umpire/config.hpp"

#include "umpire/op/MemoryOperationRegistry.hpp"

#include "umpire/op/HostCopyOperation.hpp"

#if defined(ENABLE_CUDA)
#include "umpire/op/CudaCopyFromOperation.hpp"
#include "umpire/op/CudaCopyToOperation.hpp"
#include "umpire/op/CudaCopyOperation.hpp"
#endif


#include "umpire/util/Macros.hpp"


namespace umpire {
namespace op {

MemoryOperationRegistry*
MemoryOperationRegistry::s_memory_operation_registry_instance = nullptr;

MemoryOperationRegistry&
MemoryOperationRegistry::getInstance()
{
  if (!s_memory_operation_registry_instance) {
    s_memory_operation_registry_instance = new MemoryOperationRegistry();
  }

  return *s_memory_operation_registry_instance;
}

MemoryOperationRegistry::MemoryOperationRegistry()
{
  registerOperation(
      "COPY",
      std::make_pair(Platform::cpu, Platform::cpu),
      std::make_shared<HostCopyOperation>());

#if defined(ENABLE_CUDA)
  registerOperation(
      "COPY",
      std::make_pair(Platform::cpu, Platform::cuda),
      std::make_shared<CudaCopyToOperation>());

  registerOperation(
      "COPY",
      std::make_pair(Platform::cuda, Platform::cpu),
      std::make_shared<CudaCopyFromOperation>());

  registerOperation(
      "COPY",
      std::make_pair(Platform::cuda, Platform::cuda),
      std::make_shared<CudaCopyOperation>());
#endif
}

void
MemoryOperationRegistry::registerOperation(
    const std::string& name,
    std::pair<Platform, Platform> platforms,
    std::shared_ptr<MemoryOperation>&& operation)
{
  auto operations = m_operators.find(name);

  if (operations == m_operators.end()) {
    operations = m_operators.insert(
        std::make_pair(name,
          std::unordered_map<std::pair<Platform, Platform>,
          std::shared_ptr<MemoryOperation>, pair_hash >())).first;
  }

  operations->second.insert(std::make_pair(platforms, operation));
}

std::shared_ptr<umpire::op::MemoryOperation>
MemoryOperationRegistry::find(
    const std::string& name,
    std::shared_ptr<strategy::AllocationStrategy>& src_allocator,
    std::shared_ptr<strategy::AllocationStrategy>& dst_allocator)
{
  auto platforms = std::make_pair(
      src_allocator->getPlatform(),
      dst_allocator->getPlatform());

  auto operations = m_operators.find(name);

  if (operations == m_operators.end()) {
    UMPIRE_ERROR("Cannot find operator " << name); 
  }

  auto op = operations->second.find(platforms);

  if (op == operations->second.end()) {
    UMPIRE_ERROR("Cannot find operator" << name << " for platforms " << static_cast<int>(platforms.first) << ", " << static_cast<int>(platforms.second));
  }

  return op->second;
}



} // end of namespace op
} // end of namespace umpire

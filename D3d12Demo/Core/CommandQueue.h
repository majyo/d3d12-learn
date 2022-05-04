#pragma once

#include <d3d12.h>
#include <wrl.h>

#include <cstring>
#include <queue>

class CommandQueue
{
public:
	CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	virtual ~CommandQueue();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> GetCommandList();
	uint64_t ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList);
	uint64_t Signal();
	bool IsFenceComplete(uint64_t fenceValue);
	void WaitForFenceValue(uint64_t fenceValue);
	void Flush();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;

protected:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator);

private:
	struct CommandAllocatorEntry
	{
		uint64_t fenceValue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	};

	using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
	using CommandListQueue = std::queue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>>;

	D3D12_COMMAND_LIST_TYPE m_CommandListType;
	Microsoft::WRL::ComPtr<ID3D12Device2> m_D3d12Device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_D3d12CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_D3d12Fence;
	HANDLE m_FenceEvent;
	uint64_t m_FenceValue;
	CommandAllocatorQueue m_CommandAllocatorQueue;
	CommandListQueue m_CommandListQueue;
};


#pragma once

#include "ResourceFactory.h"
#include "Graphite/RHI/GraphicsContext.h"


namespace Graphite
{
	
	/*
	* Constant buffers are a bit unique compared to other resources in that they are more of a CPU resource than GPU resource.
	* A constant buffer is effectively a wrapper around an upload buffer that automatically handles synchronization.
	* They are designed to be continually update from the CPU each frame.
	*
	* They are also templated to be simpler to work with as they contain an array of structures.
	* Being templated is also quite important to implement the CPU-side 'staging' buffer which allows for CPU read-write usage,
	* unlike upload buffers which are write-only.
	*/
	template <typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		ConstantBuffer(uint32_t elementCount)
		{
			// Allocate underlying buffer
			m_Buffer = ResourceFactory::Get().CreateUploadBuffer(elementCount, GraphicsContext::GetBackBufferCount(), sizeof(T), s_ConstantBufferAlignmentInBytes);

			m_StagingBuffer.resize(elementCount);

			// Dirty flags keep track of which elements have changed since last frame
			m_DirtyFlags.resize((elementCount + s_DirtyFlagBlockWidth - 1) / s_DirtyFlagBlockWidth);
			m_DirtyCounters = std::vector<uint8_t>(elementCount, static_cast<uint8_t>(GraphicsContext::GetBackBufferCount()));

			SetAllDirty();
		}
		ConstantBuffer(std::initializer_list<T> elements)
			: ConstantBuffer(static_cast<uint32_t>(elements.size()))
		{
			m_StagingBuffer = elements;
		}
		~ConstantBuffer() = default;

		DELETE_COPY(ConstantBuffer)
		DEFAULT_MOVE(ConstantBuffer)

		void SetElement(uint32_t element, const T& data)
		{
			m_StagingBuffer.at(element) = data;
			SetDirty(element);
		}
		const T& GetElement(uint32_t element) const
		{
			return m_StagingBuffer.at(element);
		}

		// Call prior to rendering each frame to copy the latest data to the GPU
		void CommitDirtyElements(const GraphicsContext* graphicsContext)
		{
			const uint32_t currentBackBuffer = graphicsContext->GetCurrentBackBuffer();

			for (size_t j = 0; j < m_DirtyFlags.size(); j++)
			{
				if (size_t count = m_DirtyFlags.at(j).count())
				{
					// Copy required for algorithm - bits are cleared each iteration to find the new rightmost set bit
					auto flags = m_DirtyFlags.at(j);
					// Iterate over all set bits in the bitset
					for (size_t i = 0; i < count; i++)
					{
						int idx = std::countr_zero(flags.to_ullong());
						const uint32_t element = static_cast<uint32_t>(j * s_DirtyFlagBlockWidth) + idx;

						flags.set(idx, false);
						if (--m_DirtyCounters.at(element) == 0)
						{
							m_DirtyFlags.at(j).set(idx, false);
						}

						m_Buffer->CopyElement(element, currentBackBuffer, &m_StagingBuffer.at(element), sizeof(T));
					}
				}
			}
		}

		const UploadBuffer* GetBuffer() const { return m_Buffer.get(); }

	private:
		void SetDirty(uint32_t index, bool val = true)
		{
			m_DirtyFlags.at(index / s_DirtyFlagBlockWidth).set(index % s_DirtyFlagBlockWidth, val);
		}
		void SetAllDirty()
		{
			for (size_t j = 0; j < m_DirtyFlags.size(); j++)
			{
				auto& flags = m_DirtyFlags.at(j);
				const int shift = static_cast<int>((j + 1) * s_DirtyFlagBlockWidth) - static_cast<int>(m_StagingBuffer.size());
				flags |= (~(0ull)) >> (shift < 0 ? 0 : shift);
			}
			std::ranges::fill(m_DirtyCounters, static_cast<uint8_t>(GraphicsContext::GetBackBufferCount()));
		}

	private:
		inline static constexpr uint32_t s_ConstantBufferAlignmentInBytes = 256;
		inline static constexpr uint32_t s_DirtyFlagBlockWidth = 64;
	private:
		// The underlying buffer supporting this structure
		std::unique_ptr<UploadBuffer> m_Buffer;
		// CPU read-write staging buffer that can be manipulated at all times
		std::vector<T> m_StagingBuffer;

		// Dirty flags keep track of which elements have changed, to save copying the entire staging buffer every frame
		std::vector<std::bitset<s_DirtyFlagBlockWidth>> m_DirtyFlags;
		// Need to also count how many frames an element has been dirty for
		std::vector<uint8_t> m_DirtyCounters;
	};
}

/// @file event_source.hpp
#ifndef GRAVIS24_EVENT_SOURCE_HPP
#define GRAVIS24_EVENT_SOURCE_HPP


namespace gravis24
{

	class EventListener;

	/// Источник событий, вызывает post(event, *this) при возникновении события для каждого из подписчиков.
	class EventSource
	{
	public:
		virtual ~EventSource() = default;
		
		/// Подписать слушателя на источник событий для получения уведомлений.
		virtual void subscribe(EventListener&) = 0;
		
		/// Убрать слушателя из числа подписчиков на источник событий.
		virtual void unsubscribe(EventListener&) = 0;
		
		/// Проверить, подписан ли слушатель на источник событий.
		[[nodiscard]] virtual bool isSubscribed(EventListener&) const noexcept = 0;
	};
	
	
	/// Вспомогательный объект-синглтон: несуществующий источник событий.
	[[nodiscard]] inline auto nullEventSource() noexcept
		-> EventSource&
	{
		class NullEventSource final: public EventSource
		{
		public:
			void subscribe(EventListener&) override {}
			void unsubscribe(EventListener&) override {}
			bool isSubscribed(EventListener&) const noexcept override { return false; }
		};
		
		static NullEventSource object;
		return object;
	}
	
}

#endif//GRAVIS24_EVENT_SOURCE_HPP

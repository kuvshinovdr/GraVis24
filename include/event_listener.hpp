/// @file event_listener.hpp
#ifndef GRAVIS24_EVENT_LISTENER_HPP
#define GRAVIS24_EVENT_LISTENER_HPP


namespace gravis24
{
	
	class Event;
	class EventSource;
	
	/// "Слушатель", способный получать события.
	class EventListener
	{
	public:
		virtual ~EventListener() = default;
	
		/// Вызывается, когда данный слушатель подписывается на источник событий. 
		virtual void subscribed(EventSource&) {}
		
		/// Вызывается, когда данный слушатель удаляется из списка подписчиков на источник событий.
		virtual void unsubscribed(EventSource&) {}
		
		/// Вызывается источником событий (sender) при возникновении события.
		/// Одно и то же событие может быть передано многим слушателям, поэтому владение event не передаётся.
		/// При необходимости следует скопировать event, 
		/// поскольку его существование после вызова post не гарантируется.
		virtual void post(Event const& event, EventSource& sender) = 0;
	};
	
}

#endif//GRAVIS24_EVENT_LISTENER_HPP

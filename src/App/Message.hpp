#ifndef AIDP_APP_MESSAGE_HPP_
#define AIDP_APP_MESSAGE_HPP_

#include <adbase/Utility.hpp>
#include <adbase/Logging.hpp>
#include <adbase/Metrics.hpp>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <list>
#include <limits.h>
#include "AdbaseConfig.hpp"

namespace app {
// message queue
typedef struct MessageItem {
    int partId;
	int mask; // 0x00 normal, 0x01 stop
    uint64_t offset;
    adbase::Buffer message;
} MessageItem;

typedef adbase::Queue<MessageItem> MessageQueue;
typedef std::list<std::pair<std::string, std::string>> MessageToLua;


class Message {
public:
	Message(AdbaseConfig* configure);
	~Message();
	void start();
	void stop();
	void reload();
	void call(void *data);
	void saveMessage();
	void loadMessage();
	MessageToLua getMessage();
	bool push(MessageItem& item);
    static void deleteThread(std::thread *t);

private:
	typedef std::unique_ptr<std::thread, decltype(&Message::deleteThread)> ThreadPtr;
    typedef std::vector<ThreadPtr> ThreadPool;
    ThreadPool Threads;

	AdbaseConfig *_configure;
	MessageQueue _queue;
	mutable std::mutex _mut;
	std::unordered_map<std::string, MessageItem> _luaMessages;

	void serialize(adbase::Buffer& buffer, MessageItem& item);
	void callMessage();
	void addLuaMessage(MessageItem& item);
	const std::string convertKey(MessageItem& item);

};
}
#endif

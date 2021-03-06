//
// Created by colilo on 16-11-15.
//

#ifndef AMQP_TOOL_PROCUDER_H
#define AMQP_TOOL_PROCUDER_H

#include <string>
#include <vector>
#include "ProducerConsumerBase.h"
#include "Stats.h"

class Producer : ProducerConsumerBase {
private:
    Channel _channel;
    string _exchangeName;
    string _id;
    bool _randomRoutingKey;
    bool _mandatory;
    bool _immediate;
    bool _persistent;
    int _txSize;
    int _msgLimit;
    long _timeLimit;
    float _rateLimit;
    Stats _stats;

    char *_message;

    Semaphore _confirmPool;
    SortedSet <Long> unconfirmedSet =
            Collections.synchronizedSortedSet(new TreeSet<Long>());

public:
    Producer(Channel channel, Parameter param, Stats stats) {

        _channel = channel;
        _exchangeName = param.getExchangeName();
        _id = param.getRoutingKey();
        _randomRoutingKey = param.getRandomRoutingKey();
        _mandatory = flags.has("mandatory");
        _immediate = flags.count("immediate");
        _persistent = flags.count("persistent");
        _txSize = txSize;
        _rateLimit = rateLimit;
        _msgLimit = msgLimit;
        _timeLimit = 1000L * timeLimit;
        _message = new char[minMsgSize];
        if (confirm > 0) {
            _confirmPool = new Semaphore((int) confirm);
        }
        _stats = stats;
    }


    void handleReturn(int replyCode, std::string replyText, std::string exchange, std::string routingKey,
                      AMQP.BasicProperties properties, std::string body);


    void handleAck(long seqNo, bool multiple);

    void handleNack(long seqNo, bool multiple);

    void handleAckNack(long seqNo, bool multiple, bool nack);

    void run();

    void publish(std::string msg);


    std::string createMessage(int sequenceNumber);

};


#endif //AMQP_TOOL_PROCUDER_H

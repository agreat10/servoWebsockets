#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <stack>
#include <atomic>
#include <SFML/Network.hpp>

class ClientHttpThread
{
public:
    ClientHttpThread(const std::string& address, int port)
    {;
        http_.setHost(address, port);
        request_.setMethod(sf::Http::Request::Put);
        request_.setUri("/");
        request_.setHttpVersion(1, 1);
        std::thread t1(&ClientHttpThread::SendPackage, this);
        t1.detach();
    }
    ~ClientHttpThread()
    {
        threadEnableFlag_ = false;
    }
    void SendMessage(const std::string& inputMessage)
    {
        mutex_.lock();
        buffer_.push(inputMessage);
        mutex_.unlock();
    }
    std::string GetLastResponce()
    {
        mutex_.lock();
        std::string tmp = lastResponce_;
        mutex_.unlock();
        return tmp;
    }
private:
    void SendPackage()
    {
        while (threadEnableFlag_)
        {
            if (buffer_.size() > 0)
            {
                mutex_.lock();
                request_.setBody(buffer_.top());
                sf::Http::Response response = http_.sendRequest(request_);
                std::cout<<"SEND => "<<buffer_.top()<<std::endl;
                lastResponce_ = response.getBody();
                buffer_.pop();
                if (buffer_.size() > 30)
                {
                    buffer_ = std::stack<std::string>();
                }
                mutex_.unlock();
            }
        }
    }
private:
    std::thread thread_{};
    std::stack<std::string> buffer_{};
    std::mutex mutex_{};
    sf::Http::Request request_{};
    sf::Http http_{};
    std::atomic_bool threadEnableFlag_{true};
    std::string lastResponce_{};
};
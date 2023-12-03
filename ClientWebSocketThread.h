//#pragma once

#include <libwebsockets.h>
#include <iostream>
#include <string>
#include <stack>
#include <mutex>


struct lws *web_socket = nullptr;

void SendMessage(std::string message);

static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            web_socket = wsi;
            printf("Connection established\n");
            SendMessage("servoRotation");
            break;
        case LWS_CALLBACK_RECEIVE:
            lws_write(wsi, (unsigned char *)in, len, LWS_WRITE_TEXT);
            break;
        case LWS_CALLBACK_CLOSED:
            web_socket = nullptr;
            printf("Connection closed\n");
            break;
        default:
            break;
    }
    return 0;
}

void send_message(const char *message) {
    if (web_socket) {
        size_t message_length = strlen(message);
        unsigned char *buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING + message_length +
                                               LWS_SEND_BUFFER_POST_PADDING];
        memcpy(buf + LWS_SEND_BUFFER_PRE_PADDING, message, message_length);
        lws_write(web_socket, buf + LWS_SEND_BUFFER_PRE_PADDING, message_length, LWS_WRITE_TEXT);
        delete[] buf;
    }
}

static struct lws_protocols protocols[] = {
        {
                "echo-protocol",
                callback_echo,
                      0,
                         512,
        },
        { NULL, NULL, 0, 0 }
};

bool running_ = true;

struct lws_context *context;
struct lws_context_creation_info info;
struct buffer_
{
    std::mutex mutex;
    std::stack<std::string> data;
};

buffer_ buffer;

void SendMessage(std::string message)
{
    buffer.mutex.lock();
    buffer.data.push(message);
    buffer.mutex.unlock();
}

void run()
{
    while (running_)
    {
        lws_service(context, -1);
        if (!buffer.data.empty())
        {
            buffer.mutex.lock();
            send_message(buffer.data.top().c_str());
            std::cout<<"SEND => "<<buffer.data.top()<<std::endl;
            buffer.data.pop();
            buffer.mutex.unlock();
        }
    }
}

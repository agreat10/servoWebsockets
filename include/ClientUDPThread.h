#ifndef SOCKET_UDP_THREAD_H
#define SOCKET_UDP_THREAD_H

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <list>

#define MAX_PACKET_DATA_SIZE 512 // максимальный размер данных отправляемого пакета

// направление обмена данными
enum class SocketDirection {Both/*получать и отправлять*/, Send/*только отправлять*/, Recieve/*только получать*/};

// хранит инфу об отправляемом пакете данных
struct UDPPacketInfo {
    bool IsEmpty = true; // является пустым и не инициализированным
    sf::Time Created; // время когда пакет данных был сформирован
    // std::byte
    char Data[MAX_PACKET_DATA_SIZE]; // отправляемые данные
    std::size_t SizeData; // актуальная длина массива данных
};

// объект создающий отдельный поток осуществляющий передачу данных на сервер через сокеты
class ClientUDPThread {
  private:
      // поток отправляющий и принимающий пакеты
      sf::Thread mThread;
      // является ли поток остановленным
      bool mIsStopped;
      // пакеты данных на отправку
      std::list<UDPPacketInfo> mSendingPackets;
      // принятые пакеты данных
      std::list<UDPPacketInfo> mRecievedPackets;
      // номер порта соединения
      unsigned short mPortNum;
      // айпи адрес сервера
      sf::IpAddress mServerIP;
      // часы для измерения времени запросов и т.д.
      sf::Clock mClock;
      // может ли данный сокет принимать и/или получать данные
      SocketDirection mDirection;
      // подсоединились к серверу
      bool mConnected;
      // мьтех для синхронизации потоков (чтобы запретить потокам одновременное изменение переменных. В один момент времени только один поток может вносить изменения)
      sf::Mutex mMutex;
  private:
      // точка входа потока
      void ThreadFunc();
      // добавить пакет на отправку в очередь
      void AddSendingPacket(UDPPacketInfo &InNewPacket);
      // добавить полученный пакет в список
      void AddRecievedPacket(UDPPacketInfo &InNewPacket);
      // получить самый старый пакет для отправки и вынуть его из очереди
      bool PopSendingPacket(UDPPacketInfo &OutPacket);
      // получить самый старый полученный пакет и вынуть его из очереди
      bool PopRecievedPacket(UDPPacketInfo &OutPacket);
      // отправляем самый старый пакет
      void SendFirstPacket(sf::UdpSocket &InSocUDP);
      // получить пакет
      void RecievePacket(sf::UdpSocket &InSocUDP);
  public:
      // конструктор
      ClientUDPThread(unsigned short InPortNum, sf::IpAddress InServerIP, SocketDirection InDirection);
      // деструктор - высвобождаем ресурсы и останавливаем поток
      ~ClientUDPThread();
      // остановить поток
      void StopThread();
      // отправить сообщение серверу
      void SendMessage(std::string InMessage);
      // есть ли сообщения для прочтения от сервера
      bool HasMessages();
      // прочитать последнее полученное с сервера сообщение
      void PopRecievedMessage(void *OutText, size_t InSize);  
};

#endif
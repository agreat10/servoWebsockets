// Реализация класс ClientUDPThread
// используется для удобства отправки запросов из основного потока не блокируя его
// отправка и получение пакетов происходят в отдельном потоке
// таким образом, интерфейс не будет подвисать во время отправки или получения больших объемов данных

#include <include/ClientUDPThread.h> // подключаем описание класса потока клиента UDP
#include <cstring>
#include <iostream>

// конструктор
ClientUDPThread::ClientUDPThread(unsigned short InPortNum, sf::IpAddress InServerIP, SocketDirection InDirection)
  : mThread(&ClientUDPThread::ThreadFunc, this) {
    mClock.restart();
    mConnected = false;
    mDirection = InDirection;
    mPortNum = InPortNum;
    mServerIP = InServerIP;
    mIsStopped = false;
    mThread.launch();
    std::cout << "UDPThread started" << "\n";
}

// деструктор
ClientUDPThread::~ClientUDPThread() {
  // todo: освободить сокет и остановить поток
}

// точка входа потока
void ClientUDPThread::ThreadFunc() {
  sf::UdpSocket socUDP;
  socUDP.setBlocking(false);
  while(!mIsStopped) {
    // если поток для чтения данных, то читаем их из сокета
    if(mDirection == SocketDirection::Both || mDirection == SocketDirection::Recieve) {
      // получить пакет
      if(mConnected)
        RecievePacket(socUDP);
    }
    // если поток для отправки данных, то отправляем накопившееся пакеты
    if(mDirection == SocketDirection::Both || mDirection == SocketDirection::Send) {
      if(mSendingPackets.size() > 0) {
        // отправляем самый старый пакет
        SendFirstPacket(socUDP);
      }
    }
  }
}

// остановить поток
void ClientUDPThread::StopThread() {
  mIsStopped = true; // синхронизация не требуется, так как изменение только из главного потока
  // mThread.Terminate(); -- насильное завершение потока
}

// добавить пакет на отправку в очередь
void ClientUDPThread::AddSendingPacket(UDPPacketInfo &InNewPacket) {
  mMutex.lock();
  try {
    mSendingPackets.push_front(InNewPacket);
  } 
  catch (...){}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
}

// добавить полученный пакет в список
void ClientUDPThread::AddRecievedPacket(UDPPacketInfo &InNewPacket){
  mMutex.lock();
  try {
    mRecievedPackets.push_front(InNewPacket);
  } 
  catch (...){}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
}

// получить самый старый пакет для отправки и вынуть его из очереди
bool ClientUDPThread::PopSendingPacket(UDPPacketInfo &OutPacket) {
  OutPacket.IsEmpty = true;
  mMutex.lock();
  try {
    OutPacket = mSendingPackets.back();
    mSendingPackets.pop_back();
  } 
  catch (...) {}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
  return !OutPacket.IsEmpty;
}

// получить самый старый полученный пакет и вынуть его из очереди
bool ClientUDPThread::PopRecievedPacket(UDPPacketInfo &OutPacket) {
  OutPacket.IsEmpty = true;
  mMutex.lock();
  try {
    if(mRecievedPackets.size() > 0) {
      OutPacket = mRecievedPackets.back();
      mRecievedPackets.pop_back();
    }
  } 
  catch (...){}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
  return !OutPacket.IsEmpty;
}

// отправляем самый старый пакет
void ClientUDPThread::SendFirstPacket(sf::UdpSocket &InSocUDP) {
  if(mDirection == SocketDirection::Both || mDirection == SocketDirection::Send) {
    UDPPacketInfo packet;
    if(PopSendingPacket(packet)) { // если пакет не ушел, то не повторяем его
      if(InSocUDP.send(packet.Data, packet.SizeData, mServerIP, mPortNum) != sf::Socket::Done) {
        std::cout << "error send packet " << packet.Data << "\n";
        mConnected = false; // не мониторим поступающие пакеты, если не удалось отправить
      } else {
        mConnected = true;
      }
    }
  } else {
    std::cout << "error: does not support send operation" <<  "\n"; // не поддерживает операцию отправки
  }
}

// получить пакет
void ClientUDPThread::RecievePacket(sf::UdpSocket &InSocUDP) { 
  std::size_t PacketReceived;
  UDPPacketInfo NewPack;
  NewPack.Created = mClock.getElapsedTime(); 
  sf::IpAddress sender;
  unsigned short senderPort;
  sf::Socket::Status result;
  result = InSocUDP.receive(NewPack.Data, sizeof(NewPack.Data), PacketReceived, sender, senderPort);
  if ((result == sf::Socket::Done) && (PacketReceived > 0)) { // проверяем чтобы и пакет успешно приняли и чтобы не пустой был
      NewPack.SizeData = PacketReceived;
      NewPack.Data[(int)PacketReceived] = 0; // добавляем конец строки
      NewPack.IsEmpty = false;
      AddRecievedPacket(NewPack);
      // --------------- отладка
      std::string sReceived = std::to_string(PacketReceived);
      std::cout << "recieved " << sReceived << "\n";
      // --------------- отладка
  } else if (result == sf::Socket::NotReady) {
    // --------------- отладка
  //  std::cout << "error recieved: NotReady" << "\n";
    // --------------- отладка
  } else if (result == sf::Socket::Partial) {
    // todo: в принципе можно пытаться обработать частично полученные пакеты
    // --------------- отладка
  //  std::cout << "error recieved: Partial" << "\n";
    // --------------- отладка
  } else if (result == sf::Socket::Disconnected) {
    mConnected = false;
    // --------------- отладка
  //  std::cout << "error recieved: Disconnected" << "\n";
    // --------------- отладка
  } else if (result == sf::Socket::Error) {
    mConnected = false;
    // --------------- отладка
  //  std::cout << "error recieved: Error" << "\n";
    // --------------- отладка
  }
 // sf::sleep(sf::seconds(2.0f));
}

// отправить сообщение серверу (todo: добавить разрезание текста на пакеты)
void ClientUDPThread::SendMessage(std::string InMessage) {
  UDPPacketInfo NewPack;
  NewPack.Created = mClock.getElapsedTime();
  const char *cInMessage = InMessage.c_str();
  if(cInMessage != nullptr) {
    int len = strlen(cInMessage);
    if(len >= MAX_PACKET_DATA_SIZE) len = MAX_PACKET_DATA_SIZE - 1;
    if(len > 0) {
      memcpy(NewPack.Data, cInMessage, len + 1/*длина строки + 0 как окончание строки*/);       // копируем строку str1  и сохраняем в str2
      NewPack.SizeData = len;
      NewPack.IsEmpty = false;
      AddSendingPacket(NewPack);
      std::cout << "add UDP Packet " << InMessage << "\n";
    }
  }
}

// есть ли сообщения для прочтения от сервера
bool ClientUDPThread::HasMessages() {
  bool hasMes = false;
  mMutex.lock();
  try {
    hasMes = mRecievedPackets.size() > 0;
  }
  catch (...){}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
  return hasMes;
}

// прочитать последнее полученное с сервера сообщение
void ClientUDPThread::PopRecievedMessage(void *OutText, size_t InSize) {
  mMutex.lock();
  try {
    UDPPacketInfo Packet;
    if(PopRecievedPacket(Packet)) {
      int len = Packet.SizeData;
      if(len > InSize - 1) len = InSize - 1;
      memcpy(OutText, Packet.Data, len + 1/*длина строки + 0 как окончание строки*/);
    }
  }
  catch (...){}// если вдруг в коде будет ошибка, то не произойдет вечной блокировки данных
  mMutex.unlock();
} 
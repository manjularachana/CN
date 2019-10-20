#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("FirstScriptExample");
int main(int argc,char*argv[]){
  Time::SetResolution(Time::NS);
  LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
  NodeContainer nodes;
  nodes.Create(2);
  NodeContainer nodes2;
  nodes2.Create(2);
  NodeContainer nodes1;
  nodes1.Add(nodes.Get(1));
  nodes1.Add(nodes2.Get(0));
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate",StringValue("8Mbps"));
  pointToPoint.SetChannelAttribute("Delay",StringValue("3ms"));
  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute("DataRate",StringValue("9Mbps"));
  pointToPoint1.SetChannelAttribute("Delay",StringValue("2ms"));
  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute("DataRate",StringValue("7Mbps"));
  pointToPoint2.SetChannelAttribute("Delay",StringValue("1ms"));
  NetDeviceContainer dev,dev1,dev2;
  dev=pointToPoint.Install(nodes);
  dev2=pointToPoint2.Install(nodes2);
  dev1=pointToPoint1.Install(nodes1);
  InternetStackHelper stack;
  stack.Install(nodes);
  stack.Install(nodes2);
  Ipv4AddressHelper add,add1,add2;
  add.SetBase("10.1.1.0","255.255.255.0");
  add1.SetBase("10.1.3.0","255.255.255.0");
  add2.SetBase("10.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer inter=add.Assign(dev);
  Ipv4InterfaceContainer inter1=add1.Assign(dev1);
  Ipv4InterfaceContainer inter2=add2.Assign(dev2);
  UdpEchoServerHelper echoServer(9);
  ApplicationContainer serverApps=echoServer.Install(nodes.Get(1));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));
  UdpEchoClientHelper echoClient(inter.GetAddress(1),9);
  echoClient.SetAttribute("MaxPackets",UintegerValue(1));
  echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize",UintegerValue(1024));
  ApplicationContainer clientApps=echoClient.Install(nodes.Get(0));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10.0));
  UdpEchoServerHelper echoServer1(10);
  ApplicationContainer serverApps2=echoServer1.Install(nodes2.Get(0));
  serverApps2.Start(Seconds(1.0));
  serverApps2.Stop(Seconds(10.0));
  UdpEchoClientHelper echoClient1(inter2.GetAddress(0),10);
  echoClient1.SetAttribute("MaxPackets",UintegerValue(1));
  echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoClient1.SetAttribute("PacketSize",UintegerValue(1024));

  ApplicationContainer clientApps2=echoClient1.Install(nodes2.Get(1));
  clientApps2.Start(Seconds(3.0));
  clientApps2.Stop(Seconds(10.0));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}

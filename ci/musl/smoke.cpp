#include <tinyxml2.h>
#include <cstdio>
int main() {
  tinyxml2::XMLDocument doc;
  if (doc.Parse("<robot><joint count='7'/></robot>")!=tinyxml2::XML_SUCCESS) return 1;
  auto joint=doc.FirstChildElement("robot")->FirstChildElement("joint");
  if (!joint || joint->IntAttribute("count")!=7) return 2;
  std::puts("PASS: TinyXML2 XML parsing");
}

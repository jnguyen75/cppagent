/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include "xml_printer.hpp"

/* XmlPrinter public methods */
XmlPrinter::XmlPrinter(std::string xmlPath, std::ostringstream * xmlStream)
{
  mXmlStream = xmlStream;
  try
  {
    mParser = new xmlpp::DomParser;

    // Set to false now because XML does not contain DTD
    mParser->set_validate(false);
    // We just want the text to be resolved/unescaped automatically.
    mParser->set_substitute_entities();
    mParser->parse_file(xmlPath);
  }
  catch (std::exception & e)
  {
    std::cout << "XML Exception: " << e.what() << std::endl;
  }
}

XmlPrinter::~XmlPrinter()
{
  delete mParser;
}

xmlpp::Node * XmlPrinter::getRootNode()
{
  return mParser->get_document()->get_root_node();
}

void XmlPrinter::printNode(const xmlpp::Node* node, unsigned int indentation)
{
  // Make sure the parser is available
  if (mParser)
  {
    // Constant node data determined for each node
    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
    const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
    const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

    // Ignore empty whitespace
    if (nodeText && nodeText->is_white_space())
      return;
    
    Glib::ustring nodename = node->get_name();
    
    // Node name tag: i.e. "<tag"
    // Leave ">" out in case there are other attributes
    if (!nodeText && !nodeComment && !nodename.empty())
    {
      printIndentation(indentation);
      *mXmlStream << "<" << nodename;
    }
    
    // Right now, nothing is being done for conditions: if nodeContent/nodeComment
    if (nodeText)
    {
      printIndentation(indentation);
      *mXmlStream << nodeText->get_content() << std::endl;
    }
    else if (const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
    {
      // Print attributes for the element
      // i.e. ...attribute1="value1" attribute2="value2"...
      const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
      for (xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
      {
        const xmlpp::Attribute* attribute = *iter;
        *mXmlStream << " " << attribute->get_name() << "=\"" << attribute->get_value() << "\"";
      }
      
      *mXmlStream << ">" << std::endl;
    }
    
    // If node does NOT have content, then it may have children, so perform print on children
    if (!nodeContent)
    {
      // Recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
      {
        printNode(*iter, indentation + 2);
      }
    }
    
    // Close off xml tag, i.e. </tag>
    if (!nodeText && !nodeComment && !nodename.empty())
    {
      printIndentation(indentation);
      *mXmlStream << "</" << nodename << ">" << std::endl;
    }
  }
}

void XmlPrinter::printPath(std::string path)
{
  // TODO, using xml::find
  xmlpp::NodeSet results = getRootNode()->find("//MTConnectStreams");
  
  std::cout << results.size();
  
  //printNode(getRootNode(), 0);
  
  for (int i=0; i<results.size(); i++)
  {
    printNode(results[i], 0);
  }
}

/* XmlPrinter Protected Methods */
void XmlPrinter::printIndentation(unsigned int indentation)
{
  for(unsigned int i = 0; i < indentation; ++i)
  {
    *mXmlStream << " ";
  }
}



package com.konka.passport.aidl;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class ServiceXmlParseHandler extends DefaultHandler {

	public final static int XMLPARSE_CNTV4 = 1; // CNTV4

	private int BusinessType;
	private StringBuffer buf;

	private boolean isSuccess;
	private String License;

	public ServiceXmlParseHandler(int iBusinessType) {
		BusinessType = iBusinessType;
		buf = new StringBuffer();
		isSuccess = false;
		License = null;
	}

	@Override
	public void startDocument() throws SAXException {
		isSuccess = false;
		License = null;
	}

	@Override
	public void endDocument() throws SAXException {
	}

	@Override
	public void startElement(String namespaceURI, String localName,
			String qName, Attributes attr) throws SAXException {
	}

	@Override
	public void endElement(String namespaceURI, String localName, String qName)
			throws SAXException {
		String str = buf.toString();
		buf.delete(0, buf.length());

		if (localName.equals("successful")) {
			if (str.equalsIgnoreCase("yes")) {
				isSuccess = true;
			}
			return;
		}

		if (!isSuccess) {
			return;
		}

		switch (BusinessType) {
		case XMLPARSE_CNTV4:
			if (localName.equals("license")) {
				License = str;
			}
			break;
		}
	}

	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		buf.append(ch, start, length);
	}

	public String GetCNTV4License() {
		return License;
	}
}

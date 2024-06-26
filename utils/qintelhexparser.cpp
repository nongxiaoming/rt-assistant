
#include "qintelhexparser.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QIntelHexParser::QIntelHexParser() {
}

QIntelHexParser::QIntelHexParser(const QString &filename) {
    mEndOfFile=false;
    parseFile(filename);
}

void QIntelHexParser::parseFile(QIODevice &device) {
    int lineCounter=0;
    QTextStream ts(&device);

    mEndOfFile=false;
    mSegments.clear();

    while(!ts.atEnd()) {
        QString line = ts.readLine().trimmed();
        lineCounter++;

        if(mEndOfFile) {
            qDebug("QIntelHexParser::parseFile: Warning line beyond end of file record");
            continue;
        }

        if(line.length()<10) {
            qDebug("QIntelHexParser::parseFile: Invalid line too short");
            continue;
        }
        if(line.at(0)!=':') {
            qDebug("QIntelHexParser::parseFile: Missing start charctar");
            continue;
        }

        quint8 bytesCount = parseHexByte(line.at(1),line.at(2));
        quint16 address = parseHexWord(line.at(3),line.at(4),line.at(5),line.at(6));
        quint8 recordType = parseHexByte(line.at(7),line.at(8));
        QByteArray data(bytesCount,0);
        if(bytesCount>0) {
            for(int i=0;i<bytesCount;i++) {
                data[i]=parseHexByte(line.at(9+i*2),line.at(9+i*2+1));
            }
        }

        quint16 calcLength = 11 + bytesCount*2;
        if(calcLength != line.length()) {
            qDebug("QIntelHexParser::parseFile: String length mismatch");
            continue;
        }

        quint8 readChecksum = parseHexByte(line.at(calcLength-2),line.at(calcLength-1));
        quint8 calcCheckSum = 0;

        for(int i=1;i<line.length()-2;i+=2) {
            calcCheckSum += parseHexByte(line.at(i),line.at(i+1));
        }

        calcCheckSum = ~calcCheckSum+1;

        if(calcCheckSum != readChecksum) {
            qDebug("QIntelHexParser::parseFile: Checksum verification failed calc:%02X readed:%02X",calcCheckSum,readChecksum);
            continue;
        }

        handleRecord(recordType,address,data,lineCounter);
    }

    qDebug("QIntelHexParser::parseFile: parsed %d lines with %d memory segments",lineCounter,mSegments.size());
}

void QIntelHexParser::parseFile(const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        parseFile(filename);
    } else {
        qDebug("QIntelHexParser::parseFile: Can't open file %s",file.errorString().toLatin1().constData());
    }
}

void QIntelHexParser::parseTxtFile(QIODevice &device) {
    int lineCounter=0;
    QTextStream ts(&device);

    mEndOfFile=false;
    mSegments.clear();

    while(!ts.atEnd()) {
        QString line = ts.readLine().trimmed();
        lineCounter++;

        if(mEndOfFile) {
            qDebug("QIntelHexParser::parseFile: Warning line beyond end of file record");
            continue;
        }

        if(line.length()<3) {
            qDebug("QIntelHexParser::parseFile: Invalid line too short");
            continue;
        }

        if(line.at(0)=='@') {
            bool ok = 0;
            int address = line.mid(1).toInt(&ok,16);
             mSegments.append(QIntelHexMemSegment(address));
        } else if(line.at(0)=='q') {

        } else {
            line = line.replace(" ","");
            mSegments.last().memory.append(QByteArray::fromHex(line.toLatin1()));
            qDebug() << line;
        }
    }

    qDebug("QIntelHexParser::parseFile: parsed %d lines with %d memory segments",lineCounter,mSegments.size());
}

int QIntelHexParser::sumTotalMemory() const {
    int mem=0;
    for(int i=0;i<mSegments.count();i++) mem += mSegments.at(i).memory.size();
    return mem;
}

void QIntelHexParser::handleRecord(quint8 recordType, quint16 address, QByteArray payload, int lineNum) {
    switch(recordType) {
    case 0x00:
        if(mSegments.size()==0 || mSegments.last().lastAddress()!=address) {
            qDebug("QIntelHexParser::parseFile: New mwmory segment (%04X) at line %d",address,lineNum);
            mSegments.append(QIntelHexMemSegment(address));
        }
        mSegments.last().memory.append(payload);
        break;
    case 0x01:
        mEndOfFile=true;
        break;
    default:
        qDebug("QIntelHexParser::parseFile: Unkonow record type %02X at line %d",recordType,lineNum);
        break;
    }
}

quint8 QIntelHexParser::decodeHexChar(QChar hex) {
    if(hex>='0'&&hex<='9') return hex.toLatin1()-'0';
    else if(hex>='A'&&hex<='F') return hex.toLatin1()-'A'+10;
    else if(hex>='a'&&hex<='f') return hex.toLatin1()-'a'+10;
    else return 0xFF;
}

quint8 QIntelHexParser::parseHexByte(QChar hexH, QChar hexL) {
    return (decodeHexChar(hexH)<<4) + decodeHexChar(hexL);
}

quint16 QIntelHexParser::parseHexWord(QChar hexHH, QChar hexHL,QChar hexLH, QChar hexLL) {
    return (parseHexByte(hexHH,hexHL)<<8) + parseHexByte(hexLH,hexLL);
}


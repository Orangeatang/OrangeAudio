
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAELogger.h"


//////////////////////////////////////////////////////////////////////////
/// Statics
//////////////////////////////////////////////////////////////////////////

static char*    g_logFilenameFormat     = "OAElog_%Y-%m-%d_%H-%M-%S.txt";
COAELogger*     COAELogger::m_instance  = nullptr;
std::ofstream   COAELogger::m_logFile;


//////////////////////////////////////////////////////////////////////////
/// Global Functions
//////////////////////////////////////////////////////////////////////////

void CreateLogFile( std::ofstream& aFile )
{
    tm      buffer;
    char    fileName[35];
    time_t  now = time(0);
    
    // format the filename
    localtime_s( &buffer, &now );
    strftime( fileName, sizeof(fileName), g_logFilenameFormat, &buffer );

    // open the file
    aFile.open( fileName, std::ios::out | std::ios::app );
}

//////////////////////////////////////////////////////////////////////////

void GetTimeAsString( std::string& aCurrentTime )
{
    time_t  now = time( 0 );
    tm      buffer;
    char    timeString[80];

    localtime_s( &buffer, &now );
    strftime( timeString, sizeof(timeString), "%Y-%m-%d.%X", &buffer );

    aCurrentTime = timeString;
}


//////////////////////////////////////////////////////////////////////////
/// COAELogger
//////////////////////////////////////////////////////////////////////////

COAELogger::~COAELogger()
{
    if( m_logFile.is_open() )
    {
        m_logFile.close();
    }
}

//////////////////////////////////////////////////////////////////////////

COAELogger* COAELogger::GetInstance()
{
    if( m_instance == nullptr )
    {
        m_instance = new COAELogger();
        CreateLogFile( m_logFile );
    }

    return m_instance;
}

//////////////////////////////////////////////////////////////////////////

void COAELogger::Destroy()
{
    if( m_instance == nullptr )
    {
        return;
    }

    delete m_instance;
    m_instance = nullptr;
}
 
//////////////////////////////////////////////////////////////////////////

void COAELogger::LogMessage( ELogMesageType aMessageType, std::string& aMessage )
{
    if (!m_logFile.is_open())
    {
        return;
    }

    std::string currentTime;
    GetTimeAsString(currentTime);

    m_logFile << GetMessageTypeString(aMessageType) << "\t" << currentTime.c_str() << "\t" << aMessage.c_str() << "\n";
}

//////////////////////////////////////////////////////////////////////////

void COAELogger::LogMessage( ELogMesageType aMessageType, const char* aFormat, ... )
{
    if (!m_logFile.is_open())
    {
        return;
    }

    char*           outputMessage = nullptr;
    int             length = 0;
    std::string     currentTime;
    va_list         arguments;

    GetTimeAsString(currentTime);

    va_start(arguments, aFormat);

    length = _vscprintf(aFormat, arguments) + 1;
    outputMessage = new char[length];
    vsprintf_s(outputMessage, length, aFormat, arguments);

    m_logFile << GetMessageTypeString(aMessageType) << "\t" << currentTime.c_str() << "\t" << outputMessage << "\n";

    va_end(arguments);

    delete[] outputMessage;
}

//////////////////////////////////////////////////////////////////////////

COAELogger::COAELogger()
{
    m_messageTypeToString[ELogMesageType::ELogMessageType_Info]     = "[INFO]";
    m_messageTypeToString[ELogMesageType::ELogMessageType_Warning]  = "[WARNING]";
    m_messageTypeToString[ELogMesageType::ELogMessageType_Error]    = "[ERROR]";
}

//////////////////////////////////////////////////////////////////////////

const char* COAELogger::GetMessageTypeString( ELogMesageType aMessageType ) const
{
    auto messageString = m_messageTypeToString.find( aMessageType );
    if( messageString != m_messageTypeToString.end() )
    {
        return messageString->second;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
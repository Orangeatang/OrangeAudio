
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEWavFile.h"
#include "OAELogger.h"


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

static const OAUInt32 g_riffChunk	= 'FFIR';
static const OAUInt32 g_dataChunk	= 'atad';
static const OAUInt32 g_formatChunk	= ' tmf';
static const OAUInt32 g_waveChunk	= 'EVAW';


//////////////////////////////////////////////////////////////////////////
/// COAEWavFile
//////////////////////////////////////////////////////////////////////////

COAEWavFile::COAEWavFile( const OASourceId& anId ) : IOAEFile( anId )
{
}

//////////////////////////////////////////////////////////////////////////

COAEWavFile::~COAEWavFile()
{
}

//////////////////////////////////////////////////////////////////////////

bool COAEWavFile::Load( const std::string& aFilePath )
{
	// make sure we're not already processing a file
	if( m_fileStream.is_open() || m_filePath.size() != 0 )
	{
		OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "OAEWavFile::LoadFile - file is already open: %s", m_filePath.c_str() );
		return false;
	}

	// open the file. we need the binary flag to prevent reads from mistakenly detecting eof flags
	m_filePath = aFilePath;
	m_fileStream.open( aFilePath.c_str(), std::ios::in | std::ios::binary );
	if( !m_fileStream.is_open() )
	{
		return false;
	}

	// locate the RIFF chunk
	OAUInt32 chunkSize, chunkPosition, fileType;
	if( !LocateChunk(g_riffChunk, chunkSize, chunkPosition) )
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "OAEWavFile::LoadFile - unable to locate RIFF chunk: %s", m_filePath.c_str() );
        m_fileStream.close();
        return false;
    }

    // read in the file type 
    if( !ReadChunk( &fileType, chunkSize, chunkPosition ) )
    {
        m_fileStream.close();
        return false;
    }

    // make sure it's a .wav
    if( fileType != g_waveChunk )
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "OAEWavFile::LoadFile - this isn't a .wav file: %s", m_filePath.c_str() );
        m_fileStream.close();
        return false;
    }

    // locate and load in the wave format structure
    if( !LocateChunk(g_formatChunk, chunkSize, chunkPosition) )
    {
        m_fileStream.close();
        return false;
    }

    if( !ReadChunk(&m_wavFormat, chunkSize, chunkPosition) )
    {
        m_fileStream.close();
        return false;
    }

    // locate and load the data chunk
    if( !LocateChunk(g_dataChunk, chunkSize, chunkPosition) )
    {
        m_fileStream.close();
        return false;
    }

    m_dataBuffer = new OAUInt8[chunkSize];
    if( !ReadChunk(m_dataBuffer, chunkSize, chunkPosition) )
    {
        m_fileStream.close();
        return false;
    }

    InitializeXAudioBuffer( chunkSize );
	m_fileStream.close();
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool COAEWavFile::LocateChunk( const OAUInt32 aRiffChunkType, OAUInt32& aChunkSize, OAUInt32& aChunkPosition )
{
	if( !m_fileStream.is_open() )
	{
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAEWavFile::LocateChunk - File handle is invalid: %s", m_filePath.c_str() );
		return false;
	}

	// seek to the very start of the file
	m_fileStream.seekg( 0, std::fstream::beg );

	OAUInt32    type = 0, size = 0, fileType = 0, bytesRead = 0, offset = 0;
	bool		continueSearching = true;
	bool		success			  = false;

	// loop until we hit the end of the file, or find the chunk
	while( continueSearching )
	{
		// read in the next chunk type
		m_fileStream.read( (char*)&type, sizeof(OAUInt32) );
		if( !m_fileStream )
		{
			continueSearching = false;
			continue;
		}

		// read in the next chunk size
		m_fileStream.read( (char*)&size, sizeof(OAUInt32) );
		if( !m_fileStream )
		{
			continueSearching = false;
			continue;
		}

		switch( type )
		{
		// if we hit the RIFF chunk we need to read in the file type as well
		case g_riffChunk:
			size = 4;
			m_fileStream.read( (char*)&fileType, sizeof(OAUInt32) );
			if( !m_fileStream )
			{
				continueSearching = false;
				continue;
			}
			break;

        // otherwise set the file point to past the chunk's data
		default:
            m_fileStream.seekg( size, std::fstream::cur );
            if( !m_fileStream )
            {
                continueSearching = false;
                continue;
            }
			break;
		}

        // increment the offset by the size and type of the chunk
        offset += sizeof(OAUInt32) * 2;

        // if we've found the chunk, stop searching
        if( type == aRiffChunkType )
        {
            aChunkSize          = size;
            aChunkPosition      = offset;
            success             = true;
            continueSearching   = false;
            continue;
        }

        // otherwise add the size of the chunk to the offset and continue searching
        offset += size;
	}
		
    return success;
}

//////////////////////////////////////////////////////////////////////////

bool COAEWavFile::ReadChunk( void* aBuffer, OAUInt32 aBufferSize, OAUInt32 aBufferOffset )
{
    if( !m_fileStream.is_open() )
    {
        OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "COAEWavFile::ReadChunk - File handle is invalid: %s", m_filePath.c_str() );
        return false;
    }

    // seek to the position of the chunk data
    m_fileStream.seekg( aBufferOffset, std::fstream::beg );
    if( !m_fileStream )
    {
        return false;
    }

    // read the data
    m_fileStream.read( (char*)aBuffer, aBufferSize );
    if( !m_fileStream )
    {
        std::streamsize read = m_fileStream.gcount();
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

void COAEWavFile::InitializeXAudioBuffer( OAUInt32 aDataSize )
{
    m_xaudioBuffer.AudioBytes = aDataSize;
    m_xaudioBuffer.pAudioData = m_dataBuffer;
    m_xaudioBuffer.Flags      = XAUDIO2_END_OF_STREAM;
}

//////////////////////////////////////////////////////////////////////////
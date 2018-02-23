
//////////////////////////////////////////////////////////////////////////
/// Includes
//////////////////////////////////////////////////////////////////////////

#include "OAEPrecompiled.h"
#include "OAEWavFile.h"
#include "OAELogger.h"


//////////////////////////////////////////////////////////////////////////
/// Globals
//////////////////////////////////////////////////////////////////////////

static const OAInt32 g_riffChunk	= 'FFIR';
static const OAInt32 g_dataChunk	= 'atad';
static const OAInt32 g_formatChunk	= 'tmf';
static const OAInt32 g_waveChunk	= 'EVAW';


//////////////////////////////////////////////////////////////////////////
/// COAEWavFile
//////////////////////////////////////////////////////////////////////////

COAEWavFile::COAEWavFile( OAUInt64 anId ) : IOAEAudioFile( anId )
{
}

//////////////////////////////////////////////////////////////////////////

COAEWavFile::~COAEWavFile()
{
}

//////////////////////////////////////////////////////////////////////////

bool COAEWavFile::LoadFile( const std::string& aFilePath )
{
	// make sure we're not already processing a file
	if( m_fileStream.is_open() )
	{
		OAELog->LogMessage( ELogMesageType::ELogMessageType_Error, "OAEWavFile::LoadFile - file is already open: %s", m_filePath.c_str() );
		return false;
	}

	// open the file
	m_filePath = aFilePath;
	m_fileStream.open( aFilePath.c_str(), std::fstream::in );
	if( !m_fileStream.is_open() )
	{
		return false;
	}

	// locate the RIFF chunk
	OAInt32 chunkSize, chunkPosition;
	LocateChunk( g_riffChunk, chunkSize, chunkPosition );

	m_fileStream.close();
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool COAEWavFile::LocateChunk( const OAInt32 aRiffChunkType, OAInt32& aChunkSize, OAInt32& aChunkPosition )
{
	if( !m_fileStream.is_open() )
	{
		return false;
	}

	// seek to the very start of the file
	m_fileStream.seekg( 0, std::fstream::beg );

	OAInt32	    type = 0, size = 0, fileType = 0, bytesRead = 0, offset = 0;
	bool		continueSearching = true;
	bool		success			  = false;

	// loop until we hit the end of the file, or find the chunk
	while( continueSearching )
	{
		// read in the next chunk type
		m_fileStream.read( (char*)&type, sizeof(OAInt32) );
		if( m_fileStream.fail() )
		{
			continueSearching = false;
			continue;
		}

		// read in the next chunk size
		m_fileStream.read( (char*)&size, sizeof(OAInt32) );
		if( m_fileStream.fail() )
		{
			continueSearching = false;
			continue;
		}

		switch( type )
		{
		// if we hit the RIFF chunk we need to read in the file type as well
		case g_riffChunk:
			size = 4;
			m_fileStream.read( (char*)&fileType, sizeof(OAInt32) );
			if( m_fileStream.fail() )
			{
				continueSearching = false;
				continue;
			}
			break;

        // otherwise set the file point to past the chunk's data
		default:
            m_fileStream.seekg( size, std::fstream::cur );
            if( m_fileStream.fail() )
            {
                continueSearching = false;
                continue;
            }
			break;
		}

        // increment the offset by the size and type of the chunk
        offset = sizeof(OAInt32) * 2;

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

bool COAEWavFile::ReadChunk( void* aBuffer, OAInt32 aBufferSize, OAInt32 aBufferOffset )
{
    return true;
}

//////////////////////////////////////////////////////////////////////////
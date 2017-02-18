#include "http_request.h"




bool  HttpRequest::read()  //read the request
{
	if(m_read_idx>=READ_BUFFER_SIZE)	return false;
	int byte_read = 0;
	while(1)
	{
		byte_read = recv(m_sockfd,m_read_buf+m_read_idx,
				READ_BUFFER_SIZE-m_read_idx,0);
		if(byte_read == -1)
		{
			if(errno == EAGAIN || EWOULDBLOCK)
			{
				break;
			}
			return false;
		}
		else if(byte_read == 0)
		{
			return false;
		}
		m_read_idx +=byte_read;
	}
	return true;
}

void HttpRequest::init()
{
  	 m_check_state = CHECK_STATE_REQUESTLINE;
    //m_linger = false;
    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    //m_write_idx = 0;
    memset( m_read_buf, '\0', READ_BUFFER_SIZE );
    //memset( m_write_buf, '\0', WRITE_BUFFER_SIZE );
    memset( m_real_file, '\0', FILENAME_LEN );


	
}



HttpRequest::HTTP_CODE HttpRequest::parse_read()
{
	LINE_STATUS line_status = LINE_OK;
	HTTP_CODE ret = NO_REQUEST;
	char *text = 0;

	while((m_check_state==CHECK_STATE_CONTENT)&&(line_status==LINE_OK)||((line_status=parse_line() )==LINE_OK) )
	{
		text = get_line();
		m_start_line = m_checked_idx; //记录下一行起始位置
		std::cout<<"got 1 http line:"<<text<<std::endl;

		switch(m_check_state)
		{
			case CHECK_STATE_REQUESTLINE:
				ret = parse_request_line(text);
				if(ret == BAD_REQUEST)
				{
					return BAD_REQUEST;
				}
				break;
			case CHECK_STATE_HEADER:
				ret = parse_headers(text);
				if(ret == BAD_REQUEST)
				{
					return BAD_REQUEST;
				}
				else if(ret == GET_REQUEST)
				{
					return do_request();
				}
			case CHECK_STATE_CONTENT:
				ret = parse_content(text);
				if(ret == GET_REQUEST)
				{
					return do_request();
				}
				line_status = LINE_OPEN;
				break;
			default:
			{
				return INTERNAL_ERROR;
			}
		}

	}
}
	 

HttpRequest::HTTP_CODE HttpRequest::parse_request_line( char* text ) //解析行
{
	 m_url = strpbrk( text, " \t" );
    if ( ! m_url )
    {
        return BAD_REQUEST;
    }
    *m_url++ = '\0';

    char* method = text;
    if ( strcasecmp( method, "GET" ) == 0 )
    {
        m_method = GET;
    }
    else
    {
        return BAD_REQUEST;
    }

    m_url += strspn( m_url, " \t" );
    m_version = strpbrk( m_url, " \t" );
    if ( ! m_version )
    {
        return BAD_REQUEST;
    }
    *m_version++ = '\0';
    m_version += strspn( m_version, " \t" );
    if ( strcasecmp( m_version, "HTTP/1.1" ) != 0 )
    {
        return BAD_REQUEST;
    }

    if ( strncasecmp( m_url, "http://", 7 ) == 0 )
    {
        m_url += 7;
        m_url = strchr( m_url, '/' );
    }

    if ( ! m_url || m_url[ 0 ] != '/' )
    {
        return BAD_REQUEST;
    }

    m_check_state = CHECK_STATE_HEADER;
    return NO_REQUEST;

}
   

HttpRequest::HTTP_CODE HttpRequest::parse_headers( char* text )		//解析头
{
	if( text[ 0 ] == '\0' )
    {
        if ( m_method == HEAD )
        {
            return GET_REQUEST;
        }

        if ( m_content_length != 0 )
        {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }

        return GET_REQUEST;
    }
    else if ( strncasecmp( text, "Connection:", 11 ) == 0 )
    {
        text += 11;
        text += strspn( text, " \t" );
        if ( strcasecmp( text, "keep-alive" ) == 0 )
        {
            m_linger = true;
        }
    }
    else if ( strncasecmp( text, "Content-Length:", 15 ) == 0 )
    {
        text += 15;
        text += strspn( text, " \t" );
        m_content_length = atol( text );
    }
    else if ( strncasecmp( text, "Host:", 5 ) == 0 )
    {
        text += 5;
        text += strspn( text, " \t" );
        m_host = text;
    }
    else
    {
        printf( "oop! unknow header %s\n", text );
    }

    return NO_REQUEST;


}
 
HttpRequest::HTTP_CODE HttpRequest::parse_content( char* text )		//解析消息体
{
	if ( m_read_idx >= ( m_content_length + m_checked_idx ) )
    {
        text[ m_content_length ] = '\0';
        return GET_REQUEST;
    }

    return NO_REQUEST;

}
   

HttpRequest::HTTP_CODE HttpRequest::do_request()
{

}
    
HttpRequest::LINE_STATUS HttpRequest::parse_line()
{
	 char temp;
    for ( ; m_checked_idx < m_read_idx; ++m_checked_idx )
    {
        temp = m_read_buf[ m_checked_idx ];
        if ( temp == '\r' )
        {
            if ( ( m_checked_idx + 1 ) == m_read_idx )
            {
                return LINE_OPEN;
            }
            else if ( m_read_buf[ m_checked_idx + 1 ] == '\n' )
            {
                m_read_buf[ m_checked_idx++ ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }

            return LINE_BAD;
        }
        else if( temp == '\n' )
        {
            if( ( m_checked_idx > 1 ) && ( m_read_buf[ m_checked_idx - 1 ] == '\r' ) )
            {
                m_read_buf[ m_checked_idx-1 ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }

    return LINE_OPEN;

}




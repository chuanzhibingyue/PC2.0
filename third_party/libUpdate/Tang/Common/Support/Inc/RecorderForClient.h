#pragma once

#ifdef RECORDERSTARTINFODLL_EXPORTS
#define RECORDERSTARTINFOPORT extern "C" __declspec(dllexport)
#else
#define RECORDERSTARTINFOPORT extern "C" __declspec(dllimport)
#endif

#define SERVICEINFO_COUNT   590               // the service information count

namespace clt
{
	template<typename Ty>
	class StablePtr
	{	// wrap an object pointer to ensure destruction
	public:
		StablePtr(Ty *ptr = 0) throw()
			:m_ptr(0)
		{	// construct from object pointer
			reset(ptr);
		}

		StablePtr(const StablePtr<Ty>& right) throw()
			:m_ptr(right.m_ptr)
		{	
		}

		template<class Other>
		StablePtr(const StablePtr<Other>& right) throw()
			: m_ptr(right.m_ptr)
		{	// construct by assuming pointer from _Right
		}

		StablePtr& operator=( const Ty * ptr) throw()
		{	// construct from object pointer
			reset(ptr);
			return *this;
		}

		template<class Other>
		StablePtr& operator=( const Other * ptr) throw()
		{	// construct from object pointer
			reset(ptr);
			return *this;
		}

		StablePtr<Ty>& operator=( const StablePtr<Ty>& right ) throw()
		{	// assign compatible _Right (assume pointer)
			m_ptr = right.m_ptr;
			return (*this);
		}

		template<class Other>
		StablePtr<Ty>& operator=( const StablePtr<Other>& right ) throw()
		{
			m_ptr = right.m_ptr;
			return (*this);
		}

		~StablePtr()
		{	// destroy the object
			m_ptr = 0;
		}

		Ty& operator*() const throw()
		{	
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			return (*pItem);
		}

		Ty *operator->() const throw()
		{	// return pointer to class object
			return (&**this);
		}

		const Ty* Value() const throw()
		{
			return (&**this);
		}

//  		Ty* Get() const throw()
//  		{
//  			return (&**this);
//  		}

		BOOL operator==( const Ty * ptr) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return TRUE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return TRUE;
			}
			return FALSE;
		}

		template<class Other>
		BOOL operator==( const Other * ptr ) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return TRUE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return TRUE;
			}
			return FALSE;
		}

		template<DWORD>
		BOOL operator==(DWORD ptr) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return TRUE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return TRUE;
			}
			return FALSE;
		}

		BOOL operator==(const StablePtr<Ty>& right) const throw()
		{
			return right.m_ptr == m_ptr;
		}

		BOOL operator!=(const Ty * ptr) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return FALSE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return FALSE;
			}
			return TRUE;
		}

		template<class Other>
		BOOL operator!=(const Other * ptr) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return FALSE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return FALSE;
			}
			return TRUE;
		}

		template<DWORD>
		BOOL operator!=(DWORD ptr) const throw()
		{
			if ( 0 == m_ptr && 0 == ptr)
			{
				return FALSE;
			}
			Ty * pItem = (Ty*)(m_ptr + GetBaseAddress());
			if ( pItem == ptr )
			{
				return FALSE;
			}
			return TRUE;
		}

		BOOL operator!=(const StablePtr<Ty>& right) const throw()
		{
			return right.m_ptr != m_ptr;
		}

		void Destroy()
		{
			Release<Ty>();
		}

	private:
		template<typename Other>
		void Release()
		{
			if ( 0 != m_ptr )
			{
				Other * pItem = (Other*)((DWORD)m_ptr + GetBaseAddress());
				delete  pItem;
				m_ptr = 0;
			}
		}
		template<> void Release<int>()
		{
			if ( 0 != m_ptr )
			{
				int * pItem = (int*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<UINT16>()
		{
			if ( 0 != m_ptr )
			{
				UINT16 * pItem = (UINT16*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<UINT32>()
		{
			if ( 0 != m_ptr )
			{
				UINT32 * pItem = (UINT32*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<LONG>()
		{
			if ( 0 != m_ptr )
			{
				LONG * pItem = (LONG*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<DWORD>()
		{
			if ( 0 != m_ptr )
			{
				DWORD * pItem = (DWORD*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<double>()
		{
			if ( 0 != m_ptr )
			{
				double * pItem = (double*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<float>()
		{
			if ( 0 != m_ptr )
			{
				float * pItem = (float*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<char>()
		{
			if ( 0 != m_ptr )
			{
				char * pItem = (char*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<WCHAR>()
		{
			if ( 0 != m_ptr )
			{
				WCHAR * pItem = (WCHAR*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		template<> void Release<UCHAR>()
		{
			if ( 0 != m_ptr )
			{
				UCHAR * pItem = (UCHAR*)((DWORD)m_ptr + GetBaseAddress());
				clt::Free( pItem );
				m_ptr = 0;
			}
		}
		

		template<class Other>
		void reset( const Other* ptr = NULL ) throw()
		{	// destroy designated object and store new pointer
			if ( NULL != ptr )
			{
				m_ptr = (DWORD)(ptr) - GetBaseAddress();
			}
			else
			{
				m_ptr = 0;
			}
		}

		void reset( const Ty* ptr = NULL ) throw()
		{	// destroy designated object and store new pointer
			if ( NULL != ptr )
			{
				m_ptr = (DWORD)(ptr) - GetBaseAddress();
			}
			else
			{
				m_ptr = 0;
			}
		}

		DWORD release() throw()
		{	// return wrapped pointer and give up ownership
			DWORD Tmp = m_ptr;
			m_ptr = 0;
			return ( Tmp );
		}

	private:
		DWORD m_ptr;	// the wrapped object pointer
	};
	
    struct AccessInfoView
    {
		AccessInfoView()
			:Ptr(NULL)
		{
		}
        StablePtr<DWORD> Ptr;
    };
	
    struct ServiceInfoView
    {
        ServiceInfoView():
        bIsFilled(FALSE),
        uType(0),
        uKey(0),
        Ptr(NULL)
        {

        }

        /**
        * if the value of bIsFilled is FALSE, the memory of the service information is empty
        */
        BOOL bIsFilled;
        /**
        * The type of the service is 0, means the memory is empty.
        */
        UINT32 uType;

        /**
        * The key of this info.Mainly used group id.
        */
        UINT32 uKey;

        /**
        * the pointer to the buffer of the service information
        */
        StablePtr<DWORD> Ptr;
    };

    /**
    * @brief	<b>Summary:</b>	创建ClientInfo内存映射与视图
    * @detail   在命令行解析后调用，在一进程内只需调用一次
    * @caller   Client进程调用
    * @param 
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT CreateViewMappingOfClientInfo();

    /**
    * @brief	<b>Summary:</b>	打开ClientInfo内存映射与视图
    * @detail   
    * @caller   Client进程调用
    * @param [in] szMappingName: 内存映射块的名称
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT OpenViewMappingOfClientInfo(LPCTSTR szMappingName);

    /**
    * @brief	<b>Summary:</b>	开启Daemon进程
    * @detail   
    * @caller   Client进程调用
    * @param
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT StartDaemon();

    /**
    * @brief	<b>Summary:</b>	结束Daemon进程
    * @detail   
    * @caller   Client进程调用
    * @param
    */
    RECORDERSTARTINFOPORT HRESULT StopDaemon();

    /**
    * @brief	<b>Summary:</b>	将Client进程的窗口句柄，写入内存块
    * @detail   将Client进程的窗口句柄，通过写入共享内存，传递给Daemon进程，
                Daemon进程拿到Client进程的窗口句柄
    * @caller   Client进程调用
    * @param [in] hClientWindow:Client进程的窗口句柄
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT SetClientWindowHandle(HWND hClientWindow);
    RECORDERSTARTINFOPORT TCHAR* GetClientMappingName();

    RECORDERSTARTINFOPORT DWORD GetDaemonThreadID();

    /**
    * @brief	<b>Summary:</b>	将Session进程的句柄，写入内存块
    * @detail   将Session进程的句柄，通过写入共享内存，传递给Daemon进程，
                Daemon进程拿到Session进程的句柄
    * @caller   Client进程调用
    * @param [in] hSessionProcess : Session进程的句柄
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT AddClientSessionInfo(DWORD dwSessionProcessID, DWORD dwSessionThreadID);

    /**
    * @brief	<b>Summary:</b>	将Session进程的句柄，移出内存块
    * @caller   Client进程调用
    * @param [in] hSessionProcess : Session进程的句柄
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT RemoveClientSessionInfo(DWORD dwSessionProcessID, DWORD dwSessionThreadID);

    /**
    * @brief	<b>Summary:</b>	更新Client进程
    * @detail   
    * @caller   Client进程调用
    * @param [in] szBatPath:批处理文件路径
    * @param [in] szClientCommandLine:调起Client命令行参数
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT UpdateClient(
        __in LPCTSTR szBatPath,
        __in LPCTSTR szClientCommandLine);
    /**
    * @brief	<b>Summary:</b>	通知Daemon进程，Client进程发生崩溃
    * @detail   
    * @caller   Client进程调用
    * @param
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT TellDaemonClientIsCrashed();

    RECORDERSTARTINFOPORT HRESULT TellDaemonSessionCrashed(UINT32 nSessionID, UINT32 nSessionIDType);

    /**
    * @brief	<b>Summary:</b>	获取接入信息内存块
    * @detail   获得内存块后，可以直接对内存块进行读写操作
    * @caller   Client进程调用
    * @param
    * @return   指向接入信息内存映射块的视图
    */
	RECORDERSTARTINFOPORT AccessInfoView* GetAccessInfoView();

    /**
    * @brief	<b>Summary:</b>	删除接入信息
    * @detail   将接入信息内存块清零
    * @caller   Client进程调用
    * @param
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT DeleteAccessInfoView();

    /**
    * @brief	<b>Summary:</b>	获取服务信息内存块
    * @detail   根据服务信息的type和key，查找内存块，
                如果没有找到，则返回一块闲置的内存块，如果没有闲置的内存块则返回NULL
    * @caller   Client进程调用
    * @param [in] uServiceType:服务类型
    * @param [in] uKey:键值
    * @return   指向服务信息内存映射块的视图，如果返回值的uServiceType为0，表示该视图是闲置的
    */
    RECORDERSTARTINFOPORT ServiceInfoView* GetServiceInfoView(
        __in UINT32 uServiceType, 
        __in UINT32 uKey);

    /**
    * @brief	<b>Summary:</b>	删除服务信息
    * @detail   根据服务信息的type和key，查找内存块，并将内存块清零
    * @caller   Client进程调用
    * @param [in] uServiceType:服务类型
    * @param [in] uKey:键值
    * @return   S_OK:成功；E_FAIL:失败
    */
    RECORDERSTARTINFOPORT HRESULT DeleteServiceInfoView(
        __in UINT32 uServiceType, 
        __in UINT32 uKey);

    /**
    * @brief	<b>Summary:</b>	动态在映射内存区申请空间
    * @detail   
    * @caller   Client进程调用
    * @param [in] tSize:空间大小
    * @return   
    */
    RECORDERSTARTINFOPORT void * Alloc(unsigned int tSize);
    
    /**
    * @brief	<b>Summary:</b>	释放在映射内存区申请的空间
    * @detail   
    * @caller   Client进程调用
    * @param [in] p:指向需要释放空间的首地址
    * @return   
    */
    RECORDERSTARTINFOPORT void Free( void * p);

	RECORDERSTARTINFOPORT DWORD GetBaseAddress();
}

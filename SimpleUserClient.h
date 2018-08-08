/*
	File:			SimpleUserClient.h
	
	Description:	This file shows how to implement a simple I/O Kit user client that is Rosetta-aware.

	Copyright:		Copyright © 2001-2008 Apple Inc. All rights reserved.
	
	Disclaimer:		IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
					("Apple") in consideration of your agreement to the following terms, and your
					use, installation, modification or redistribution of this Apple software
					constitutes acceptance of these terms.  If you do not agree with these terms,
					please do not use, install, modify or redistribute this Apple software.
					
					In consideration of your agreement to abide by the following terms, and subject
					to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
					copyrights in this original Apple software (the "Apple Software"), to use,
					reproduce, modify and redistribute the Apple Software, with or without
					modifications, in source and/or binary forms; provided that if you redistribute
					the Apple Software in its entirety and without modifications, you must retain
					this notice and the following text and disclaimers in all such redistributions of
					the Apple Software.  Neither the name, trademarks, service marks or logos of
					Apple Computer, Inc. may be used to endorse or promote products derived from the
					Apple Software without specific prior written permission from Apple.  Except as
					expressly stated in this notice, no other rights or licenses, express or implied,
					are granted by Apple herein, including but not limited to any patent rights that
					may be infringed by your derivative works or by other works in which the Apple
					Software may be incorporated.
					
					The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
					WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
					WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
					PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
					COMBINATION WITH YOUR PRODUCTS.
					
					IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
					CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
					GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
					ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
					OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
					(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
					ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
				
	Change History (most recent first):
                    
            2.0			08/13/2008			Add Leopard user client API for supporting 64-bit user processes.
											Now requires Xcode 3.0 or later to build.
			
            1.1			05/22/2007			User client performs endian swapping when called from a user process 
											running using Rosetta. Updated to produce a universal binary.
											Now requires Xcode 2.2.1 or later to build.
			
			1.0d3	 	01/14/2003			New sample.

*/


#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>
#include "SimpleDriver.h"

#define SimpleUserClientClassName com_apple_dts_driver_SimpleUserClient

class SimpleUserClientClassName : public IOUserClient
{
    OSDeclareDefaultStructors(com_apple_dts_driver_SimpleUserClient)

protected:
    SimpleDriverClassName*					fProvider;
    task_t									fTask;
	bool									fCrossEndian;
    static const IOExternalMethodDispatch	sMethods[kNumberOfMethods];

public:
    // IOUserClient methods
    virtual void stop(IOService* provider);
    virtual bool start(IOService* provider);
    
	virtual bool initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties);

    virtual IOReturn clientClose(void);
    virtual IOReturn clientDied(void);

	virtual bool willTerminate(IOService* provider, IOOptionBits options);
	virtual bool didTerminate(IOService* provider, IOOptionBits options, bool* defer);
	
    virtual bool terminate(IOOptionBits options = 0);
    virtual bool finalize(IOOptionBits options);
	
protected:	

	// KPI for supporting access from both 32-bit and 64-bit user processes beginning with Mac OS X 10.5.
	virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
									IOExternalMethodDispatch* dispatch, OSObject* target, void* reference);

    // SimpleUserClient methods
	static IOReturn sOpenUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
	virtual IOReturn openUserClient(void);

	static IOReturn sCloseUserClient(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
    virtual IOReturn closeUserClient(void);
    
	static IOReturn sScalarIStructI(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
	virtual IOReturn ScalarIStructI(uint32_t inNumber, MySampleStruct* inStruct, uint32_t inStructSize);

	static IOReturn sScalarIStructO(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
	virtual IOReturn ScalarIStructO(uint32_t inNumber1, uint32_t inNumber2, MySampleStruct* outStruct, uint32_t* outStructSize);

	static IOReturn sScalarIScalarO(SimpleDriverClassName* target, void* reference, IOExternalMethodArguments* arguments);

	static IOReturn sStructIStructO(SimpleUserClientClassName* target, void* reference, IOExternalMethodArguments* arguments);
	virtual IOReturn StructIStructO(MySampleStruct* inStruct, MySampleStruct* outStruct, uint32_t inStructSize, uint32_t* outStructSize);
};

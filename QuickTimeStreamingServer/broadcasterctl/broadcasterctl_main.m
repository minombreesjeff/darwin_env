#import <Foundation/Foundation.h>
#import "BroadcasterAdminController.h"

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    BroadcasterAdminController *mainController;

    mainController = [BroadcasterAdminController broadcasterAdminController];

    if (!mainController)
	[BroadcasterAdminController printUsage];
    
    [pool release];
    return 0;
}

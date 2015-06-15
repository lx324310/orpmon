#include "common.h"
#include "support.h"
#include "spr-defs.h"

/* Camera and CRT test.
   Draws gray cross across the screen, few color boxes at top left and moves around camera captured screen left/right
   in the middle. */ 
    
#define CAMERA_BASE     0x88000000
#define CRT_BASE        0xc0000000
#define VIDEO_RAM_START 0xa8000000      /* till including a83ffffc */
    
#define SCREEN_X        640
#define SCREEN_Y        480
    
#define CAMERA_X        352
#define CAMERA_Y        288
    
#define CAMERA_BUF(idx) (VIDEO_RAM_START + (idx) * CAMERA_X * CAMERA_Y)
#define FRAME_BUF       (CAMERA_BUF(2))
    
#define CAMERA_POS      (camera_pos_x + ((SCREEN_Y - CAMERA_Y) / 2) * SCREEN_X)
    
#define MIN(x,y)        ((x) < (y) ? (x) : (y))
    
#define set_mem32(addr,val)  (*((unsigned long *) (addr)) = (val))
#define get_mem32(addr)  (*((unsigned long *) (addr)))
#define set_palette(idx,r,g,b) set_mem32 (CRT_BASE + 0x400 + (idx) * 4, (((r) >> 3) << 11) | (((g) >> 2) << 5) | (((b) >> 3) << 0))
#define put_pixel(xx,yy,idx)   (*(unsigned char *)(FRAME_BUF + (xx) + (yy) * SCREEN_X) = (idx))




{
	
	    /* Change base addresse of camera */ 
	    set_mem32(CAMERA_BASE, CAMERA_BUF(current_buf));	/* Set address to store to */
	
	    /* Change base addresse of crt */ 
	    set_mem32(CRT_BASE + 8, CAMERA_BUF(1 - current_buf));	/* Tell CRT when camera buffer is */
	
	
	
	    /* move the camera screen around */ 
	    camera_pos_x += camera_move_speed;
	
		
	



{
	
	
		return -1;
	
	    /* Init CRT */ 
	    set_mem32(CRT_BASE + 4, FRAME_BUF);	/* Frame buffer start */
	
	
	    /* Init palette */ 
	    for (i = 0; i < 32; i++) {
		
		
		
		
		
		
		
		
	
	
		
			
	



{
	
	
		return -1;
	
		
			
	
	    /* Draw gray X */ 
	    for (i = 0; i < SCREEN_Y; i++) {
		
		
	
	
	    /* Draw color boxes */ 
	    for (y = 0; y < 50; y++)
		
			
				
	



{
	
		return -1;
	
	



{
	
		return -1;
	
	    /* Init Camera */ 
	    set_mem32(CAMERA_BASE, CAMERA_BUF(current_buf = 0));	/* Set address to store to */
	
	
	    /* Init CRT to display camera */ 
	    set_mem32(CRT_BASE + 8, CAMERA_BUF(1 - current_buf));	/* Tell CRT when camera buffer is */
	
	
	
	
	    /* Enable interrupts */ 
	    mtspr(SPR_SR, mfspr(SPR_SR) | SPR_SR_IEE);
	
	



{
	
		return -1;
	
	    /* Disable interrupts */ 
	    mtspr(SPR_SR, mfspr(SPR_SR) & ~SPR_SR_IEE);
	
	
	    /* Disable Camera */ 
	    set_mem32(CAMERA_BASE + 4, 1);	/* Enable it */
	
	



{
	
	
	
			  "enables CRT and displays some test patterns",
			  crt_test_cmd);
	
			  camera_enable_cmd);
	
			  camera_disable_cmd);

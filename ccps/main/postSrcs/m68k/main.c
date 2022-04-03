// https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Variable-Attributes.html
// https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html
// https://mcuoneclipse.com/2016/11/01/getting-the-memory-range-of-sections-with-gnu-linker-files/
#define GFXRAM   __attribute__ ((section (".gfx_data")))  
#define CPSA_REG __attribute__ ((section (".cpsa_reg")))  
#define CPSB_REG __attribute__ ((section (".cpsb_reg")))  
#define BYTE		unsigned char
#define WORD		unsigned short
#define DWORD		unsigned int
#define BOOL		unsigned int

#define	MAXSPRITES	256

typedef struct
{
	WORD	x;			// Sprite x position
	WORD	y;			// Sprite y position
	WORD	tile;		// Sprite tile

	// 0..4 CB[0..4] Palette ID used to render the tile
   // 5 X Flip Mirrored horizontally
   // 6 Y Flip Mirrored vertically
   // 7 LOOKUP Looks up the CB value into the RAM for each tile (see later section)
   // 8..11 XB[0..3] Horizontal size in tiles
   // 12..15 YB[0..3] Vertical size in tiles
	WORD	attributes;   	// Sprite attribute
} Sprite;

GFXRAM Sprite sprites [MAXSPRITES]  =  {};

#define Color WORD
typedef struct {
  Color colors[16];
} Palette;
GFXRAM Palette palettes[32];

CPSA_REG WORD cpsa_reg[0x20] = {};
CPSB_REG WORD cpsb_reg[0x20] = {};

int vsyncCounter = 0;
int soundCounter = 0;
int someCounter = 0x6666;
int uinVARRRRRR;
const int mYcOnStT = 1;


static const Palette ryu = {0xF111 ,0xFFD9,0xFFB8,0xFE97,0xFC86,0xF965,0xF643,0xFb00,
   	                   0xFfff,0xFeec,0xFdca,0xFba8,0xFa87,0xF765,0xFf00,0x0000};

void setPalette(int page, int paletteID, const Palette* palette) {

   for (int j = 0 ; j < 16 ; j++) {
  	  palettes[paletteID].colors[j] = (*palette).colors[j];
   }

   // Set all palettes to red
   for(int i = 0 ; i < 32 ; i++) {
      for (int j = 0 ; j < 16 ; j++) {
      	  palettes[i].colors[j] = (*palette).colors[j];
      }
    }

	// Palette control
   // bit 0: copy page 0 (sprites)
   // bit 1: copy page 1 (scroll1)
   // bit 2: copy page 2 (scroll2)
   // bit 3: copy page 3 (scroll3)
   // bit 4: copy page 4 (stars1)
   // bit 5: copy page 5 (stars2)
   cpsb_reg[0x30 / 2] =  1;

   // Set palette base
	cpsa_reg[0xa / 2] = (WORD)(((DWORD)palettes) >> 8);
}


void onVSync() {

   // final fight uses CPS_B_04
   // https://github.com/mamedev/mame/blob/master/src/mame/video/cps1.cpp#L478

   // sf2 uses CPS_B_11 
   // https://github.com/mamedev/mame/blob/master/src/mame/video/cps1.cpp#L480


   setPalette(0, 2, &ryu);

   


   // Enable layers
   cpsb_reg[0x26 / 2] = 0x3 << 6 ;//| 0x3 << 0xc; 
   // sprite = 6, scroll1=8, scroll2=a, scroll3=c
   // cpsb_reg[0x13] = 0x12CE;
   // *((WORD*)0x00800166) = 0x12CE;



   
   int i=0;
   Sprite* s;

    for (i = 0 ; i < 4 ; i++) {
    s = &sprites[i];
    s->x = 220;
    s->y = 100;
    s->tile = 	4;

    s->attributes = 2 |  0x5 << 12 | 0x3 << 8; // Use palette 2 dim 5+1x3+1 = 6x4 tiles
    }

   sprites[i].attributes	= 0xFF00; // Last sprite marker

   cpsa_reg[0] = (WORD)(((DWORD)sprites) >> 8);

   vsyncCounter++;
   if (vsyncCounter >= 60) {
   	uinVARRRRRR++;
   	vsyncCounter = 0;
   	soundCounter += 1;
   	*((char*)0x800180) = (0x22 + soundCounter);
   	
   } else {
   	*((char*)0x800180) = 0xFF;
   }

  
  
}


int run() {

   
   

	return 0;
}
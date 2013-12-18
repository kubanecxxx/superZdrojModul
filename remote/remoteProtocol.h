/**
 * @file remoteProtocol.h
 * @author kubanec
 * @date 12.9.2013
 *
 */

#ifndef REMOTEPROTOCOL_H_
#define REMOTEPROTOCOL_H_

/**
 * @defgroup remote
 * @brief remote ovládání přes SPI
 *
 * @details
 * spi protokol z pohledu mástra
 *
 * protokol je:
 * 	- 8bitů příkaz
 * 	- 16bitů data (tam/zpátky závisi na přikazu)
 * 	  -# low byte
 * 	  -# high byte
 * 	- 8bitů kontrolní součet = (lowbyte + highbyte) ^ 0b10101010 při čtení z pohledu mastera
 *							(cmd + lowbyte + highbyte) ^ 0b10101010 při zápisu z pohledu mastera
 *
 *	pokud zapisuje sem tak slave odpovidá nějaky hovna (0x00 00 00 00)
 *	pokud vyčitá tak master posilá nějaky hovna a slave posilá zpátky data
 *
 * @{
 */

/**
 * @brief příkazy na čtení dat od slavů (první bajt v přenosu)
 */
typedef enum
{
	rVOLTAGE = 0, /// výstupní napětí na svorkách zdroje
	rCURRENT, ///< výstupní proud
	rCONVERTERVOLTAGE, ///< napětí co leze z předměniče
	rOUTPUTENABLED, ///< výstup aktivován - 1 ; vypnut - 0
	rTHERMALFAILURE, ///< přehřátí OPA - 1; nepřehřáto - 0
	rCURRENTLIMITED, ///< záběr proudové pojistky - 1 ; není nadproud - 0
	rCOUNT
///< poslední abych věděl přesně kolik jich tam je
} remRead_t;

/**
 * @brief příkazy na zápis do slavů (zase první bajt)
 */
typedef enum
{
	wCURRENTLIMIT = 0, ///< zápis hodnoty proudové pojistky
	wOUTPUTVOLTAGE, ///< zápis výstupního napětí
	wENABLE, ///< aktivace výstupu aktivní - 1; vypnuto - 0
	wCOUNT
///< poslední abych přesně věděl kolik jich tam je
} remWrite_t;

/**
 * @brief flagy pro rozeznání zápisu nebo čtení
 */
typedef enum
{
	READ = 0, WRITE = 0x80
} remReadWrite_t;

/**
 * @}
 */

#endif /* REMOTEPROTOCOL_H_ */

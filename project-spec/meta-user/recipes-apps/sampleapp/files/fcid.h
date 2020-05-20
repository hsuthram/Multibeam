#include "datadict.h"

#define MBIW MAKE_FOURCHAR_ID('M','B','I','W') /* mbs in water - in/or out for 3 seconds (with YBIW) */
#define MBSW MAKE_FOURCHAR_ID('M','B','S','W') /* mbs in water - actual state */
#define SEDU MAKE_FOURCHAR_ID('S','E','D','U') /* burn-in date code */
#define SEIU MAKE_FOURCHAR_ID('S','E','I','U') /* post-burn-in line number */
#define SENU MAKE_FOURCHAR_ID('S','E','N','U') /* sequential unit id */
#define SMDL MAKE_FOURCHAR_ID('S','M','D','L') /* model ID */
#define SM2L MAKE_FOURCHAR_ID('S','M','2','L') /* mbs chirp frequency start (with YM2L) */
#define SM2U MAKE_FOURCHAR_ID('S','M','2','U') /* mbs chirp frequency end (with YM2U) */
#define SMBN MAKE_FOURCHAR_ID('S','M','B','N') /* Sonar enabled (0==DISABLED completely - with YMBN) */
#define SMRD MAKE_FOURCHAR_ID('S','M','R','D') /* down range - save always in meters (with YMRD) */
#define SMRF MAKE_FOURCHAR_ID('S','M','R','F') /* forward range - save always in meters (with YMRF) */
#define SVER MAKE_FOURCHAR_ID('S','V','E','R') /* version number */
#define YBIW MAKE_FOURCHAR_ID('Y','B','I','W') /* mbs in water - in/or out for 3 seconds (with MBIW) */
#define YM2L MAKE_FOURCHAR_ID('Y','M','2','L') /* mbs chirp frequency start(with SM2L) */
#define YM2U MAKE_FOURCHAR_ID('Y','M','2','U') /* mbs chirp frequency end (with SM2U) */
#define YMBN MAKE_FOURCHAR_ID('Y','M','B','N') /* Sonar enabled (0==DISABLED completely - with SMBN) */
#define YMRD MAKE_FOURCHAR_ID('Y','M','R','D') /* down range - save always in meters (with SMRD) */
#define YMRF MAKE_FOURCHAR_ID('Y','M','R','F') /* forward range - save always in meters (with SMRF) */

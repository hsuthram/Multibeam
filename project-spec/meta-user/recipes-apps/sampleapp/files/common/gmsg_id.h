/*******************************************************************************
*           Copyright (c) 2008  Techsonic Industries, Inc.                     *
*         - Contains CONFIDENTIAL and TRADE SECRET information -               *
********************************************************************************

           Description: message ids organized by service class (service.h)

      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      + This software is the proprietary property of:                   +
      +                                                                 +
      +  Techsonic Industries, Inc.                                     +
      +                                                                 +
      +  1220 Old Alpharetta Rd           1 Humminbird Lane             +
      +  Suite 340                        Eufaula, AL  36027            +
      +  Alpharetta, GA  30005                                          +
      +                                                                 +
      + Use, duplication, or disclosure of this material, in any form,  +
      + is forbidden without permission from Techsonic Industries, Inc. +
      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*******************************************************************************/


/********************           COMPILE FLAGS                ******************/
#ifndef GMSG_ID_H
   #define GMSG_ID_H
#ifdef __cplusplus
   extern "C" {
#endif
//#include "compile_flags.h"
/********************           INCLUDE FILES                ******************/
#include "global.h"
#include "types.h"
#include "service.h"

/********************               ENUMS                    ******************/

/********************              DEFINES                   ******************/

/********************               MACROS                   ******************/
#define GMSG_SERVICE_CLASS(gmsg_id) ((gmsg_id) >> 16)
#define GMSG_ID_OFFSET(gmsg_id)     ((gmsg_id) & 0x0000FFFF)
#define GMSG_ID_BASE(class_id)     ((class_id) << 16)
#define GMSG_STATUS_BASE(class_id)  ((class_id) << 16)

/********************         TYPE DEFINITIONS               ******************/
typedef enum
{
/************************** SERVICE_CLASS_GENERIC *********************************/
   GMSG_ID_DEFAULT_UNKNOWN     = GMSG_ID_BASE(SERVICE_CLASS_DEFAULT),
   GMSG_ID_STATUS,
   GMSG_ID_TEST,
   GMSG_ID_TIMER,
   GMSG_ID_TIMER2,
   GMSG_ID_NOTIFY,

   GMSG_ID_DEFAULT_LAST,
   GMSG_ID_DEFAULT_COUNT       = (GMSG_ID_DEFAULT_LAST-GMSG_ID_DEFAULT_UNKNOWN),

/*************************** SERVICE_CLASS_SYSTEM *********************************/
   GMSG_ID_SYSTEM_UNKNOWN      = GMSG_ID_BASE(SERVICE_CLASS_SYSTEM),
   GMSG_ID_SYSTEM_STARTUP,
   GMSG_ID_SYSTEM_HYBERNATE,
   GMSG_ID_SYSTEM_RESUME,
   GMSG_ID_SYSTEM_SHUTDOWN,
   GMSG_ID_SYSTEM_SNAPSHOT,
   GMSG_ID_SYSTEM_EXTERNAL_STORAGE_TYPE,
   GMSG_ID_REQUEST_SYSTEM_INFO,
   GMSG_ID_RESPONSE_SYSTEM_INFO,
   GMSG_ID_SYSTEM_CURSOR_XEVENT,
   GMSG_ID_SYSTEM_CURSOR_RUBBERBAND,
   GMSG_ID_SYSTEM_CURSOR_RUBBERBAND_COLOR,
   GMSG_ID_SYSTEM_UPDATE_SOFTWARE,
   GMSG_ID_SYSTEM_SOFTWARE_PERCENTAGE,

   GMSG_ID_SYSTEM_LAST,
   GMSG_ID_SYSTEM_COUNT        = (GMSG_ID_SYSTEM_LAST-GMSG_ID_SYSTEM_UNKNOWN),

/************************** SERVICE_CLASS_SERVICE *********************************/
   GMSG_ID_SERVICE_UNKNOWN      = GMSG_ID_BASE(SERVICE_CLASS_SERVICE),
   GMSG_ID_SERVICE_REGISTER,
   GMSG_ID_SERVICE_UNREGISTER,
   GMSG_ID_SERVICE_UP,
   GMSG_ID_SERVICE_DOWN,

   GMSG_ID_SERVICE_LAST,
   GMSG_ID_SERVICE_COUNT        = (GMSG_ID_SERVICE_LAST-GMSG_ID_SERVICE_UNKNOWN),

/************************** SERVICE_CLASS_DATABASE ********************************/
   GMSG_ID_DATABASE_UNKNOWN    = GMSG_ID_BASE(SERVICE_CLASS_DATABASE),
   GMSG_ID_DATABASE_FETCH,
   GMSG_ID_DATABASE_REGISTER,
   GMSG_ID_DATABASE_UNREGISTER,
   GMSG_ID_DATABASE_UPDATE,
   GMSG_ID_DATABASE_NEW_DATA,

   GMSG_ID_DATABASE_LAST,
   GMSG_ID_DATABASE_COUNT      = (GMSG_ID_DATABASE_LAST-GMSG_ID_DATABASE_UNKNOWN),

/*************************** SERVICE_CLASS_SERIAL *********************************/
   GMSG_ID_SERIAL_UNKNOWN      = GMSG_ID_BASE(SERVICE_CLASS_SERIAL),
   GMSG_ID_SERIAL_REGISTER,
   GMSG_ID_SERIAL_UNREGISTER,
   GMSG_ID_SERIAL_CONNECT,
   GMSG_ID_SERIAL_DISCONNECT,
   GMSG_ID_SERIAL_RECEIVE,
   GMSG_ID_SERIAL_TRANSMIT,
   GMSG_ID_SERIAL_CONFIG,
   GMSG_ID_SERIAL_ACQUIRE,
   GMSG_ID_SERIAL_RELEASE,

   GMSG_ID_SERIAL_LAST,
   GMSG_ID_SERIAL_COUNT        = (GMSG_ID_SERIAL_LAST-GMSG_ID_SERIAL_UNKNOWN),

/*************************** SERVICE_CLASS_SONAR **********************************/
   GMSG_ID_SONAR_UNKNOWN       = GMSG_ID_BASE(SERVICE_CLASS_SONAR),

   GMSG_ID_SONAR_LAST,
   GMSG_ID_SONAR_COUNT         = (GMSG_ID_SONAR_LAST-GMSG_ID_SONAR_UNKNOWN),

/*************************** SERVICE_CLASS_XSONAR *********************************/
   GMSG_ID_XSONAR_UNKNOWN      = GMSG_ID_BASE(SERVICE_CLASS_XSONAR),
   GMSG_ID_XSONAR_SET_VIEW,
   GMSG_ID_XSONAR_INIT_DONE,
   GMSG_ID_XSONAR_SET_PALETTE,
   GMSG_ID_XSONAR_SET_ASCOPE_FEATURE,
   GMSG_ID_XSONAR_SET_ASCOPE_TYPE,
   GMSG_ID_XSONAR_SET_BG_COLOR,
   GMSG_ID_XSONAR_SET_LOWER_RANGE,
   GMSG_ID_XSONAR_SET_UPPER_RANGE,
   GMSG_ID_XSONAR_SET_BEAM,
   GMSG_ID_XSONAR_SET_SENSITIVITY_LF,
   GMSG_ID_XSONAR_SET_SENSITIVITY_HF,
   GMSG_ID_XSONAR_SET_BOTTOM_VIEW,
   GMSG_ID_XSONAR_SET_BOTTOM_LOCK,
   GMSG_ID_XSONAR_SET_BOTTOM_LOCK_RANGE,
   GMSG_ID_XSONAR_SET_WATER_TYPE,
   GMSG_ID_XSONAR_XEVENT,
   GMSG_ID_XSONAR_CHART_SPEED,
   GMSG_ID_XSONAR_TWR_OVERLAY_ENABLE,

   GMSG_ID_XSONAR_LAST,
   GMSG_ID_XSONAR_COUNT        = (GMSG_ID_XSONAR_LAST-GMSG_ID_XSONAR_UNKNOWN),

/**************************** SERVICE_CLASS_NMEA **********************************/
   GMSG_ID_NMEA_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_NMEA),
   GMSG_ID_NMEA_SOURCE,

   GMSG_ID_NMEA_LAST,
   GMSG_ID_NMEA_COUNT          = (GMSG_ID_NMEA_LAST-GMSG_ID_NMEA_UNKNOWN),

/**************************** SERVICE_CLASS_GPS ***********************************/
   GMSG_ID_GPS_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_GPS),

   GMSG_ID_GPS_LAST,
   GMSG_ID_GPS_COUNT           = (GMSG_ID_GPS_LAST-GMSG_ID_GPS_UNKNOWN),

/**************************** SERVICE_CLASS_AIS ***********************************/
   GMSG_ID_AIS_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_AIS),
   GMSG_ID_AIS_CLIENT_ADD,
   GMSG_ID_AIS_CLIENT_REMOVE,
   GMSG_ID_AIS_CLIENT_SET_RECEIVE_INDIVIDUAL_SENTENCES,
   GMSG_ID_AIS_CLIENT_SET_RECEIVE_INDIVIDUAL_MESSAGES,
   GMSG_ID_AIS_DATA_UPDATE,
   GMSG_ID_AIS_SENTENCE_VDM,
   GMSG_ID_AIS_SENTENCE_VDO,
   GMSG_ID_AIS_SENTENCE_ABM,
   GMSG_ID_AIS_SENTENCE_BBM,
   GMSG_ID_AIS_MESSAGE_TYPE_1,
   GMSG_ID_AIS_MESSAGE_TYPE_5,
   GMSG_ID_AIS_MESSAGE_TYPE_18,
   GMSG_ID_AIS_LAST,
   GMSG_ID_AIS_COUNT           = (GMSG_ID_AIS_LAST-GMSG_ID_AIS_UNKNOWN),

/************************** SERVICE_CLASS_COMPASS *********************************/
   GMSG_ID_COMPASS_UNKNOWN     = GMSG_ID_BASE(SERVICE_CLASS_COMPASS),

   GMSG_ID_COMPASS_LAST,
   GMSG_ID_COMPASS_COUNT       = (GMSG_ID_COMPASS_LAST-GMSG_ID_COMPASS_UNKNOWN),

/************************* SERVICE_CLASS_AUTOPILOT ********************************/
   GMSG_ID_AUTOPILOT_UNKNOWN   = GMSG_ID_BASE(SERVICE_CLASS_AUTOPILOT),

   GMSG_ID_AUTOPILOT_LAST,
   GMSG_ID_AUTOPILOT_COUNT     = (GMSG_ID_AUTOPILOT_LAST-GMSG_ID_AUTOPILOT_UNKNOWN),

/*************************** SERVICE_CLASS_CHART **********************************/
   GMSG_ID_CHART_UNKNOWN       = GMSG_ID_BASE(SERVICE_CLASS_CHART),
   GMSG_ID_CHART_SET_VIEW,
   GMSG_ID_CHART_XEVENT,
   GMSG_ID_CHART_REGISTER_TIMER,
   GMSG_ID_CHART_REDRAW_TIMER,
   GMSG_ID_CHART_CHANGE_VISMODE,             /**<change Vismode 2D/3D @see CHART_VISMODE*/
   GMSG_ID_CHART_CHANGE_ORIENTATION,         /**<change Orientation @see CHART_ORIENTATION*/
   GMSG_ID_CHART_CHANGE_NAVIGATIONMODE,      /**<change NavigationMode @see CHART_NAVIGATIONMODE*/
   GMSG_ID_CHART_CHANGE_3DPRESETTING,        /**<change NavigationMode @see CHART_3DNAV_PRESET*/
   GMSG_ID_CHART_ZOOM_TIMER,
   GMSG_ID_CHART_CHANGE_3DTXTURE,           /**<change 3D satellite photo texture*/
   GMSG_ID_CHART_RADAR_ENABLE,              /**<enable radar */
   GMSG_ID_CHART_LAYER_FEATURE_ENABLE,      /**<enable chart layer feature */
   GMSG_ID_CHART_LAYER_FEATURE_DOUBLE,      /**<set layer feature double value */
   GMSG_ID_CHART_DRAW_FEATURE_ENABLE,       /**<enable chart draw features */
   GMSG_ID_CHART_TWR_OVERLAY_ENABLE,     /** enable twr display */
   GMSG_ID_CHART_CHANGE_CURSOR,
   GMSG_ID_CHART_CHANGE_BOAT_ICON,
   GMSG_ID_CHART_VESSEL_OFFSET,
   GMSG_ID_CHART_VESSEL_AUTOSHIFT_MAXSPEED,
   GMSG_ID_CHART_3D_ADJUSTMENT,
   GMSG_ID_CHART_AUTO_ZOOM,
   GMSG_ID_CHART_MAP_OFFSET_ENABLE,
   GMSG_ID_CHART_MAP_OFFSET,
   GMSG_ID_CHART_SCALE,
   GMSG_ID_CHART_DECLUTTER_TIMER,
   GMSG_ID_CHART_EXTERNAL_STORAGE_PATH,       /** external storage has been mounted */
   GMSG_ID_CHART_AUTO_ZOOM_TO,
   GMSG_ID_CHART_CURSOR_TO,
   GMSG_ID_CHART_QUICK_INFO,
   GMSG_ID_CHART_QUICK_INFO_REPLY,
   GMSG_ID_CHART_COG_VECTOR_TIME,                  /**COG length setting, in minutes */
   GMSG_ID_CHART_CURSOR_PAN_STOP,          /**<internal message for chart itself*/
   GMSG_ID_CHART_EJECT_SD_CARD,
   GMSG_ID_CHART_APP_STATE,
   GMSG_ID_CHART_RANGE_SYNC,               /** chart range sync with radar */
   GMSG_ID_CHART_MOUSE_DROP_REPLY,
   GMSG_ID_CHART_VESSEL_ROT_ENABLE,
   GMSG_ID_CHART_EBL_VRM_EDIT,
   GMSG_ID_CHART_EBL_VRM_DATA,
   GMSG_ID_CHART_AIS_SET_DISPLAY_TYPE,
   GMSG_ID_CHART_AIS_RANGE_FILTER,
   GMSG_ID_CHART_AIS_RANGE,
   GMSG_ID_CHART_TIDE_TABLE,
   GMSG_ID_CHART_TIDE_TABLE_REPLY,
   GMSG_ID_CHART_CURRENT_TABLE,
   GMSG_ID_CHART_CURRENT_TABLE_REPLY,
   GMSG_ID_CHART_PANORAMIC_PHOTO,
   GMSG_ID_CHART_PANORAMIC_PHOTO_REPLY,
   GMSG_ID_CHART_LAST,
   GMSG_ID_CHART_COUNT         = (GMSG_ID_CHART_LAST-GMSG_ID_CHART_UNKNOWN),

/**************************** SERVICE_CLASS_NAV ***********************************/
   GMSG_ID_NAV_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_NAV),

   GMSG_ID_NAV_BEGIN_NAVIGATION,
   GMSG_ID_NAV_CANCEL_NAVIGATION,
   GMSG_ID_NAV_SET_CONTINUOUS_NAVIGATION,
   GMSG_ID_NAV_SET_ROUTE_TRAVEL_DIRECTION_TO_FORWARD,
   GMSG_ID_NAV_RESET_CROSSTRACKERROR,
   GMSG_ID_NAV_SKIP_NEXT_ROUTE_POINT,
   GMSG_ID_NAV_GO_TO_ROUTE_POINT,

   GMSG_ID_NAV_LAST,
   GMSG_ID_NAV_COUNT           = (GMSG_ID_NAV_LAST-GMSG_ID_NAV_UNKNOWN),

/**************************** SERVICE_CLASS_TWR ***********************************/
   GMSG_ID_TWR_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_TWR),

   //requests                          //description                 //data type                         //response msg
   GMSG_ID_TWR_DELETE,                 //delete a gr/tr/wp/rt        D_TWR_ID                            GMSG_ID_TWR_STATUS
   GMSG_ID_TWR_SELECT_GROUP,           //fetch/reg groups            D_GMSG_TWR_SELECT_GROUP             GMSG_ID_TWR_GROUP_DATA (fetch/reg)
                                       //dereg groups                                                    GMSG_ID_TWR_STATUS (dereg)
   GMSG_ID_TWR_SELECT_TRACK,           //fetch/reg tracks            D_GMSG_TWR_SELECT_TRACK             GMSG_ID_TWR_TRACK_DATA (fetch/reg)
                                       //dereg tracks                                                    GMSG_ID_TWR_STATUS (dereg)

   GMSG_ID_TWR_SELECT_CURRENT_TRACK,   //fetch/reg current track     D_GMSG_TWR_SELECT_CURRENT_TRACK     GMSG_ID_TWR_CURRENT_TRACK_DATA (fetch/reg)
                                       //dereg current track                                             GMSG_ID_TWR_STATUS (dereg)
   GMSG_ID_TWR_SELECT_WPT,             //fetch/reg wpts              D_GMSG_TWR_SELECT_WPT               GMSG_ID_TWR_WPT_DATA   (fetch/reg)
                                       //dereg wpts                                                      GMSG_ID_TWR_STATUS (dereg)
   GMSG_ID_TWR_SELECT_ROUTE,           //fetch/reg routes            D_GMSG_TWR_SELECT_ROUTE             GMSG_ID_TWR_ROUTE_DATA (fetch/reg)
                                       //dereg routes                                                    GMSG_ID_TWR_STATUS (dereg)

   GMSG_ID_TWR_EDIT_GROUP,             ///create/edit twr group      D_GMSG_TWR_EDIT_GROUP               GMSG_ID_TWR_GROUP_STATUS
   GMSG_ID_TWR_ADD_TO_GROUP,           //add twr to group            D_GMSG_TWR_ADD_TO_GROUP             GMSG_ID_TWR_STATUS
   GMSG_ID_TWR_REMOVE_FROM_GROUP,      //remove twr from group       D_GMSG_TWR_REMOVE_FROM_GROUP        GMSG_ID_TWR_STATUS

   GMSG_ID_TWR_EDIT_TRACK,             //create/edit a track         D_GMSG_TWR_EDIT_TRACK               GMSG_ID_TWR_TRACK_STATUS

   GMSG_ID_TWR_EDIT_WPT,               //create/edit waypoint        D_GMSG_TWR_EDIT_WPT                 GMSG_ID_TWR_WPT_STATUS

   GMSG_ID_TWR_EDIT_ROUTE,             //create/edit route           D_GMSG_TWR_EDIT_ROUTE               GMSG_ID_TWR_ROUTE_STATUS
   GMSG_ID_TWR_ADD_POINT_TO_ROUTE,     //add point to route          D_GMSG_TWR_ADD_POINT_TO_ROUTE       GMSG_ID_TWR_ROUTE_POINT_STATUS
   GMSG_ID_TWR_EDIT_ROUTE_POINT,       //edit a route point          D_GMSG_TWR_EDIT_ROUTE_POINT         GMSG_ID_TWR_ROUTE_POINT_STATUS
   GMSG_ID_TWR_REMOVE_POINT_FROM_ROUTE,//remove point from route     D_GMSG_TWR_REMOVE_POINT_FROM_ROUTE  GMSG_ID_TWR_STATUS

   GMSG_ID_TWR_IMPORT_EXPORT_DATA,     //import/export gpx data

   //responses                         //description                 //data type                         //request msg
   GMSG_ID_TWR_STATUS,                 //status of twr commands      D_GMSG_TWR_STATUS                   VARIOUS

   GMSG_ID_TWR_GROUP_STATUS,           //status of group edit        D_GMSG_TWR_GROUP_STATUS             GMSG_ID_TWR_EDIT_GROUP
   GMSG_ID_TWR_TRACK_STATUS,           //status of track edit        D_GMSG_TWR_TRACK_STATUS             GMSG_ID_TWR_EDIT_TRACK
   GMSG_ID_TWR_WPT_STATUS,             //status of wpt edit          D_GMSG_TWR_WPT_STATUS               GMSG_ID_TWR_EDIT_WPT
   GMSG_ID_TWR_ROUTE_STATUS,           //status of route edit        D_GMSG_TWR_ROUTE_STATUS             GMSG_ID_TWR_EDIT_ROUTE
   GMSG_ID_TWR_ROUTE_POINT_STATUS,     //stauts rtpt add,edit        D_GMSG_TWR_ROUTE_POINT_STATUS       GMSG_ID_TWR_ADD_POINT_TO_ROUTE
                                       //                                                                GMSG_ID_TWR_EDIT_ROUTE_POINT

   GMSG_ID_TWR_GROUP_DATA,             //returned group data         D_GMSG_TWR_GROUP_DATA               GMSG_ID_TWR_SELECT_GROUP
   GMSG_ID_TWR_TRACK_DATA,             //returned track data         D_GMSG_TWR_TRACK_DATA               GMSG_ID_TWR_SELECT_TRACK
   GMSG_ID_TWR_CURRENT_TRACK_DATA,     //returned current track data D_GMSG_TWR_CURRENT_TRACK_DATA       GMSG_ID_TWR_SELECT_CURRENT_TRACK
   GMSG_ID_TWR_WPT_DATA,               //returned wpt data           D_GMSG_TWR_WPT_DATA                 GMSG_ID_TWR_SELECT_WPT
   GMSG_ID_TWR_ROUTE_DATA,             //returned route data         D_GMSG_TWR_ROUTE_DATA               GMSG_ID_TWR_SELECT_ROUTE

   //registered status messages        //description                 //data type                         //original registration request
   GMSG_ID_TWR_GROUP_CHANGE,           //group change                D_GMSG_TWR_GROUP_CHANGE             GMSG_ID_TWR_SELECT_GROUP
   GMSG_ID_TWR_TRACK_CHANGE,           //track change                D_GMSG_TWR_TRACK_CHANGE             GMSG_ID_TWR_SELECT_TRACK
   GMSG_ID_TWR_CURRENT_TRACK_CHANGE,   //current track change        D_GMSG_TWR_CURRENT_TRACK_CHANGE     GMSG_ID_TWR_SELECT_CURRENT_TRACK
   GMSG_ID_TWR_WPT_CHANGE,             //wpt change                  D_GMSG_TWR_WPT_CHANGE               GMSG_ID_TWR_SELECT_WPT
   GMSG_ID_TWR_ROUTE_CHANGE,           //route change                D_GMSG_TWR_ROUTE_CHANGE             GMSG_ID_TWR_SELECT_ROUTE

   GMSG_ID_TWR_LAST,
   GMSG_ID_TWR_COUNT           = (GMSG_ID_TWR_LAST-GMSG_ID_TWR_UNKNOWN),

/*************************** SERVICE_CLASS_RADAR **********************************/
   GMSG_ID_RADAR_UNKNOWN       = GMSG_ID_BASE(SERVICE_CLASS_RADAR),
   GMSG_ID_RADAR_RECEIVE_DATA,
   GMSG_ID_UDP_MSG,
   GMSG_ID_RADAR_REGISTER,
   GMSG_ID_RADAR_REGISTER_ACK,
   GMSG_ID_RADAR_UNREGISTER,
   GMSG_ID_RADAR_IP_REQUEST,
   GMSG_ID_RADAR_SET_IP_ADDR,
   GMSG_ID_RADAR_SET_SUBNET_MASK,
   GMSG_ID_RADAR_STATUS_REQUEST,
   GMSG_ID_RADAR_PRESET_VALUES_REQUEST,
   GMSG_ID_RADAR_DEFAULT_VALUES,
   GMSG_ID_RADAR_SETUP_START_TRANSMISSION,
   GMSG_ID_RADAR_START_TRANSMISSION,
   GMSG_ID_RADAR_STOP_TRANSMISSION,
   GMSG_ID_RADAR_OP_RANGE,
   //GMSG_ID_RADAR_SWEEP_DATA,
   //GMSG_ID_RADAR_SWEEPS_PER_ROTATION,
   GMSG_ID_RADAR_GAIN_ADJUST,
   GMSG_ID_RADAR_STC_ADJUST,
   GMSG_ID_RADAR_FTC_ADJUST,
   GMSG_ID_RADAR_IR_ADJUST,
   GMSG_ID_RADAR_AUTO_TUNE_START,
   GMSG_ID_RADAR_MANUAL_COARSE_TUNE,
   GMSG_ID_RADAR_MANUAL_FINE_TUNE,
   GMSG_ID_RADAR_MANUAL_TUNE_START,
   GMSG_ID_RADAR_MANUAL_TUNE_END,
   GMSG_ID_RADAR_SECTOR_TRANSMISSION_OFF,
   GMSG_ID_RADAR_TIMED_TRANSMISSION_ON,
   GMSG_ID_RADAR_TIMED_TRANSMISSION_STANDBY,
   GMSG_ID_RADAR_START_TIMED_TRANSMISSION,
   GMSG_ID_RADAR_CLIENT_ADD,
   GMSG_ID_RADAR_CLIENT_REMOVE,
   GMSG_ID_RADAR_DISPLAY_UPDATE,
   GMSG_ID_RADAR_DATA,
   GMSG_ID_RADAR_RECORD,
   GMSG_ID_RADAR_SET_VIEW,
   GMSG_ID_RADAR_DELETE_DISPLAY,
   GMSG_ID_RADAR_WARMUP_TIMER,
   GMSG_ID_RADAR_TIMED_TRANSMISSION_TIMER,
   GMSG_ID_RADAR_TRIGGER_DELAY,
   GMSG_ID_RADAR_MAIN_BANG_SUPPRESSION,
   GMSG_ID_RADAR_HEADING_LINE_ADJUSTMENT,
   GMSG_ID_RADAR_HEADING_LINE_ADJUSTMENT_START,
   GMSG_ID_RADAR_HEADING_LINE_ADJUSTMENT_END,
   GMSG_ID_RADAR_PARK_POSITION,
   GMSG_ID_RADAR_OVERLAY_DRAW,
   GMSG_ID_RADAR_MARPA_ACQUIRE_TARGET,
   GMSG_ID_RADAR_MARPA_CLEAR_TARGET,
   GMSG_ID_RADAR_GUARD_ZONE_MIN_RETURN,
   GMSG_ID_RADAR_VIEW_TIMER,
   GMSG_ID_RADAR_COLOR_PALETTE,
   GMSG_ID_RADAR_BG_COLOR,
   GMSG_ID_RADAR_OVERLAY_ENABLE,
   GMSG_ID_RADAR_DB_MODIFY,
   GMSG_ID_RADAR_XEVENT,
   GMSG_ID_RADAR_ORIENTATION,   // HU, NU, CU
   //GMSG_ID_RADAR_NAVIGATIONMODE,
   GMSG_ID_RADAR_VESSEL_OFFSET,
   GMSG_ID_RADAR_VESSEL_AUTOSHIFT_MAXSPEED,
   GMSG_ID_RADAR_EBL_VRM,
   GMSG_ID_RADAR_REDRAW_TIMER,
   GMSG_ID_RADAR_REGISTER_TIMER,
   GMSG_ID_RADAR_RESET_DEFAULTS, // Reset Defults via UI selection
   GMSG_ID_RADAR_QUICK_INFO,     // Info key
   GMSG_ID_RADAR_QUICK_INFO_REPLY,
   GMSG_ID_RADAR_MOUSE_DROP_REPLY,
   GMSG_ID_RADAR_CHANGE_CURSOR,
   GMSG_ID_RADAR_CHANGE_BOAT_ICON,
   GMSG_ID_RADAR_SETTING_CHANGE,              // HB radar Networked settings changes
   GMSG_ID_RADAR_REQUEST_DATA,
   GMSG_ID_RADAR_LAST,
   GMSG_ID_RADAR_COUNT         = (GMSG_ID_RADAR_LAST-GMSG_ID_RADAR_UNKNOWN),

/************************** SERVICE_CLASS_WEATHER *********************************/
   GMSG_ID_WEATHER_UNKNOWN     = GMSG_ID_BASE(SERVICE_CLASS_WEATHER),

   GMSG_ID_WEATHER_LAST,
   GMSG_ID_WEATHER_COUNT       = (GMSG_ID_WEATHER_LAST-GMSG_ID_WEATHER_UNKNOWN),

/**************************** SERVICE_CLASS_LOG ***********************************/
   GMSG_ID_LOG_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_LOG),
   GMSG_ID_LOG_DATA,
   GMSG_ID_LOG_ECHO,
   GMSG_ID_LOG_DISPLAY,
   GMSG_ID_LOG_CLEAR,

   GMSG_ID_LOG_LAST,
   GMSG_ID_LOG_COUNT           = (GMSG_ID_LOG_LAST-GMSG_ID_LOG_UNKNOWN),

/**************************** SERVICE_CLASS_SOCKET_SERVER *************************/
   GMSG_ID_SOCKET_SERVER_UNKNOWN = GMSG_ID_BASE(SERVICE_CLASS_SOCKET_SERVER),

   GMSG_ID_SOCKET_SERVER_LAST,
   GMSG_ID_SOCKET_SERVER_COUNT   = (GMSG_ID_SOCKET_SERVER_LAST-GMSG_ID_SOCKET_SERVER_UNKNOWN),

/**************************** SERVICE_CLASS_VIDEO *************************/
   GMSG_ID_VIDEO_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_VIDEO),
   GMSG_ID_VIDEO_SET_VIEW,
   GMSG_ID_VIDEO_INIT_DONE,

   GMSG_ID_VIDEO_LAST,
   GMSG_ID_VIDEO_COUNT           = (GMSG_ID_VIDEO_LAST-GMSG_ID_VIDEO_UNKNOWN),

/**************************** SERVICE_CLASS_PANE ***********************************/
   GMSG_ID_PANE_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_PANE),
   GMSG_ID_PANE_QUERY_POINT_INFO,
   GMSG_ID_PANE_QUERY_POINT_INFO_REPLY,

   GMSG_ID_PANE_PROXIMITY_SEARCH,
   GMSG_ID_PANE_PROXIMITY_WPT_REPLY,
   GMSG_ID_PANE_PROXIMITY_ROUTE_REPLY,
   GMSG_ID_PANE_PROXIMITY_PORT_REPLY,
   GMSG_ID_PANE_PROXIMITY_WRECKS_REPLY,
   GMSG_ID_PANE_PROXIMITY_POI_REPLY,

   GMSG_ID_PANE_LAST,
   GMSG_ID_PANE_COUNT           = (GMSG_ID_PANE_LAST-GMSG_ID_PANE_UNKNOWN),

/**************************** SERVICE_CLASS_TEST ***********************************/
   GMSG_ID_TEST_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_TEST),
   GMSG_ID_TEST_01,
   GMSG_ID_TEST_02,
   GMSG_ID_TEST_03,
   GMSG_ID_TEST_04,
   GMSG_ID_TEST_05,
   GMSG_ID_TEST_06,
   GMSG_ID_TEST_07,
   GMSG_ID_TEST_08,
   GMSG_ID_TEST_09,
   GMSG_ID_TEST_10,
   GMSG_ID_TEST_11,
   GMSG_ID_TEST_PING,
   GMSG_ID_TEST_PONG,

   GMSG_ID_TEST_LAST,
   GMSG_ID_TEST_COUNT           = (GMSG_ID_TEST_LAST-GMSG_ID_TEST_UNKNOWN),


/**************************** SERVICE_CLASS_HB_NMEA ***********************************/
   GMSG_ID_HB_NMEA_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_HB_NMEA),
   GMSG_ID_NMEA_REQUEST_DATA,
   GMSG_ID_NMEA_SEND_DIAG_GPS_DATA,
   GMSG_ID_NMEA_SEND_CHART_GPS_DATA,
   GMSG_ID_NMEA_RECEIVE_DATA,

   GMSG_ID_HB_NMEA_LAST,
   GMSG_ID_HB_NMEA_COUNT           = (GMSG_ID_HB_NMEA_LAST-GMSG_ID_HB_NMEA_UNKNOWN),

/**************************** SERVICE_CLASS_HB_TEMP ***********************************/
   GMSG_ID_HB_TEMP_UNKNOWN         = GMSG_ID_BASE(SERVICE_CLASS_HB_TEMP),
   GMSG_ID_TEMP_REQUEST_DATA,
   GMSG_ID_TEMP_RECEIVE_DATA,

   GMSG_ID_HB_TEMP_LAST,
   GMSG_ID_HB_TEMP_COUNT           = (GMSG_ID_HB_TEMP_LAST-GMSG_ID_HB_TEMP_UNKNOWN),

/**************************** SERVICE_CLASS_HB_SONAR ***********************************/
   GMSG_ID_HB_SONAR_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_HB_SONAR),
   GMSG_ID_SONAR_REQUEST_DATA,
   GMSG_ID_SONAR_RECEIVE_DATA,
   GMSG_ID_SONAR_CONTROL_CHANGE,
   GMSG_ID_SONAR_PING_DEPTH_DATA,
   GMSG_ID_SONAR_TRANSDUCER_CHANGE,

   GMSG_ID_HB_SONAR_LAST,
   GMSG_ID_HB_SONAR_COUNT           = (GMSG_ID_HB_SONAR_LAST-GMSG_ID_HB_SONAR_UNKNOWN),

/**************************** SERVICE_CLASS_HB_ALARM ***********************************/
   GMSG_ID_HB_ALARM_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_HB_ALARM),
   GMSG_ID_ALARM_RECEIVE_DATA,

   GMSG_ID_HB_ALARM_LAST,
   GMSG_ID_HB_ALARM_COUNT           = (GMSG_ID_HB_ALARM_LAST-GMSG_ID_HB_ALARM_UNKNOWN),

/**************************** SERVICE_CLASS_HB_NAV ***********************************/
   GMSG_ID_HB_NAV_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_HB_NAV),
   GMSG_ID_NAV_REQUEST_DATA,
   GMSG_ID_NAV_RECEIVE_DATA,

   GMSG_ID_HB_NAV_LAST,
   GMSG_ID_HB_NAV_COUNT           = (GMSG_ID_HB_NAV_LAST-GMSG_ID_HB_NAV_UNKNOWN),

/**************************** SERVICE_CLASS_IPILOT ***********************************/
   GMSG_ID_IPILOT_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_IPILOT),

   GMSG_ID_IPILOT_REGISTER,            // 0x01 DT_IPILOT_REGISTER
   GMSG_ID_IPILOT_REGISTER_ACK,        // 0x02 DT_UNSIGNED32( status )
   depr_ID_IPILOT_STORE_ITRACK,        // 0x03 DT_IPILOT_ITRACK
   depr_ID_IPILOT_STORE_ITRACK_ACK,    // 0x04 DT_IPILOT_ITRACK
   GMSG_ID_IPILOT_QUERY_ITRACKS,       // 0x05 DT_IPILOT_QUERY
   GMSG_ID_IPILOT_ITRACK_INFO,         // 0x06 DT_UNSIGNED32( count )
   GMSG_ID_IPILOT_GET_ITRACKS,         //   -- unused
   depr_ID_IPILOT_ITRACK_LIST,         // 0x08 DT_IPILOT_QUERY_LIST
   GMSG_ID_IPILOT_GET_ITRACK_DATA,     // 0x09 DT_UNSIGNED32( index )
   GMSG_ID_IPILOT_ITRACK_DATA,         // 0x0A DT_IPIILOT_ITRACK
   GMSG_ID_IPILOT_DATA_ACK,            //  -- unused
   depr_ID_IPILOT_FOLLOW_ITRACK,       // 0x0C DT_IPILOT_ITRACK
   depr_ID_IPILOT_FOLLOW_ITRACK_ACK,   // 0x0D DT_IPILOT_ITRACK
   GMSG_ID_IPILOT_DISPLAY_ITRACK,      //   -- unused
   depr_ID_IPILOT_STORE_WAYPOINT,      // 0x0F DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_STORE_WP_ACK,        // 0x10 DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_START_SPOTLOCK,      // 0x11 DT_IPILOT_WAYPOINT
   GMSG_ID_IPILOT_START_SL_ACK,        // 0x12 DT_UNSIGNED32
   depr_ID_IPILOT_GOTO_SPOTLOCK,       // 0x13 DT_IPILOT_WAYPOINT
   GMSG_ID_IPILOT_GOTO_SL_ACK,         //   -- sent by M-FF, unused by ctlr
   GMSG_ID_IPILOT_QUERY_WAYPOINTS,     // 0x15 DT_IPILOT_QUERY
   GMSG_ID_IPILOT_WAYPOINT_INFO,       // 0x16 DT_UNSIGNED32( count )
   GMSG_ID_IPILOT_GET_WAYPOINTS,       // 0x17 DT_UNSIGNED32( start, max )
   depr_ID_IPILOT_WAYPOINT_LIST,       // 0x18 DT_IPILOT_QUERY_LIST
   depr_ID_IPILOT_GOTO_WAYPOINT,       // 0x19 DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_GOTO_WP_ACK,         // 0x1A DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_FOLLOW_ROUTE,        // 0x1B DT_IPILOT_ROUTE
   GMSG_ID_IPILOT_FOLLOW_ROUTE_ACK,    // 0x1C DT_UNSIGNED32( status )
   depr_ID_IPILOT_UPDATE_ROUTE,        // 0x1D DT_IPILOT_ROUTE -- unused
   GMSG_ID_IPILOT_UPDATE_ROUTE_ACK,    //   -- unused
   GMSG_ID_IPILOT_DISPLAY_GOTO,        //   -- unused
   GMSG_ID_IPILOT_PLAY_SOUND,          // 0x20 unused
   depr_ID_IPILOT_START_RECORDING,     // 0x21 DT_IPILOT_ITRACK_HDR start with arguments
   GMSG_ID_IPILOT_STATUS_REPORT,       // 0x22 DT_IPILOT_STATUS
   GMSG_ID_IPILOT_STATUS_ACK,          // 0x23 DT_UNSIGNED32
   GMSG_ID_IPILOT_CLEAR_DISPLAY,       //   -- unused clear a previous DISPLAY_GOTO/_ITRACK
   GMSG_ID_IPILOT_ACK,                 // 0x25 DT_UNSIGNED32
   GMSG_ID_IPILOT_STATUS_CHANGE,       // 0x26 DT_UNSIGNED32( status, info )
   GMSG_ID_IPILOT_COMMAND,             // 0x27 DT_IPILOT_COMMAND
   depr_ID_IPILOT_CONFIG_ITEM,         // 0x28 deprecated
   GMSG_ID_IPILOT_SWUPDATE,            // 0x29 DT_IPILOT_SWUPD
   GMSG_ID_IPILOT_OPERATION_STATUS,    // 0x2A DT_IPILOT_OPSTATUS
   depr_ID_IPILOT_CONFIG_LIST,         // 0x2B deprecated

   depr_ID_IPILOT_UPLOAD_WAYPT,        // 0x2C DT_IPILOT_WAYPOINT
   GMSG_ID_IPILOT_UPLOAD_WP_ACK,       // 0x2D DT_UNSIGNED32
   GMSG_ID_IPILOT_UPLOAD_ITRACK,       // 0x2E DT_IPILOT_ITRACK/DT_IPLT_U_ITRACK
   GMSG_ID_IPILOT_UPLOAD_IT_ACK,       // 0x2F DT_UNSIGNED32
   GMSG_ID_IPILOT_START_UPLOAD_ACK,    // 0x30 DT_UNSIGNED32
   GMSG_ID_IPILOT_GET_WAYPOINT_DATA,   // 0x31 DT_UNSIGNED32( index )

   depr_ID_IPILOT_ROUTE_ADD,           // 0x32 DT_IPILOT_WAYPOINT
   GMSG_ID_IPILOT_ROUTE_ADD_ACK,       // 0x33 DT_UNSIGNED32
   depr_ID_IPILOT_DELETE_ITRACK,       // 0x34 DT_IPILOT_ITRACK
   GMSG_ID_IPILOT_DELETE_ITRACK_ACK,   // 0x35 DT_UNSIGNED32
   depr_ID_IPILOT_DELETE_WAYPOINT,     // 0x36 DT_IPILOT_WAYPOINT
   GMSG_ID_IPILOT_DELETE_WAYPT_ACK,    // 0x37 DT_UNSIGNED32

   depr_ID_IPILOT_FOLLOW_CONTOUR,      // 0x38 DT_IPILOT_ITRACK
   GMSG_ID_IPILOT_FOLLOW_CONTOUR_ACK,  // 0x39 DT_UNSIGNED32
   depr_ID_IPILOT_UPDATE_CONTOUR,      // 0x3A DT_IPILOT_ITRACK
   GMSG_ID_IPILOT_UPDATE_CONTOUR_ACK,  // 0x3B DT_UNSIGNED32

   GMSG_ID_IPILOT_CONFIG_ITEM,         // 0x3C DT_IPILOT_CFGITEM
   GMSG_ID_IPILOT_CONFIG_LIST,         // 0x3D DT_IPILOT_CFGLIST

   GMSG_ID_IPILOT_AUTH,                // 0x3E DT_IPILOT_AUTH

   depr_ID_IPILOT_UPDATE_SPOTLOCK,     // 0x3F DT_IPILOT_WAYPOINT not used ctlr
   GMSG_ID_IPILOT_UPDATE_SL_ACK,       // 0x40
   depr_ID_IPILOT_MARKFOR_WAYPOINT,    // 0x41 DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_MARKFOR_ITRACK,      // 0x42 DT_IPILOT_ITRACK
   depr_ID_IPILOT_MARK_WAYPOINT_ACK,   // 0x43 DT_IPILOT_WAYPOINT
   depr_ID_IPILOT_MARK_ITRACK_ACK,     // 0x44 DT_IPILOT_ITRACK

   GMSG_ID_IPILOT_STARTUP_CONFIG,      // 0x45 DT_IPILOT_CFGLIST
   GMSG_ID_IPILOT_STARTUPCONFIG_ACK,   // 0x46 DT_UNSIGNED32

   depr_ID_IPILOT_NAME_CHANGE,         // 0x47 DT_IPILOT_NAME_CHANGE


   GMSG_ID_IPILOT_MARKFOR_UID_ACK,     // 0x48 DT_UNSIGNED64
   GMSG_ID_IPILOT_MARKFOR_WP_UID,      // 0x49 DT_UNSIGNED64
   GMSG_ID_IPILOT_MARKFOR_IT_UID,      // 0x4A DT_UNSIGNED64

   GMSG_ID_IPLT_U_STORE_WAYPOINT,      // 0x4B DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_STORE_WP_ACK,        // 0x4C DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_START_SPOTLOCK,      // 0x4D DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_GOTO_SPOTLOCK,       // 0x4E DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_GOTO_WAYPOINT,       // 0x4F DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_UPLOAD_WAYPT,        // 0x50 DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_ROUTE_ADD,           // 0x51 DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_DELETE_WAYPOINT,     // 0x52 DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_WAYPOINT_LIST,       // 0x53 DT_IPLT_U_QUERY_LIST

   GMSG_ID_IPLT_U_STORE_ITRACK,        // 0x54 DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_STORE_ITRACK_ACK,    // 0x55 DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_FOLLOW_ITRACK,       // 0x56 DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_FOLLOW_ITRACK_ACK,   // 0x57 DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_START_RECORDING,     // 0x58 DT_IPLT_U_ITRACK_HDR start with arguments
   GMSG_ID_IPLT_U_DELETE_ITRACK,       // 0x59 DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_FOLLOW_CONTOUR,      // 0x5A DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_UPDATE_CONTOUR,      // 0x5B DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_MARKFOR_ITRACK,      // 0x5C DT_IPLT_U_ITRACK
   GMSG_ID_IPLT_U_ITRACK_LIST,         // 0x5D DT_IPLT_U_QUERY_LIST

   GMSG_ID_IPLT_U_FOLLOW_ROUTE,        // 0x5E DT_IPLT_U_ROUTE
   GMSG_ID_IPLT_U_UPDATE_ROUTE,        // 0x5F DT_IPLT_U_ROUTE

   GMSG_ID_IPLT_U_NAME_CHANGE,         // 0x60 DT_IPLT_U_NAME_CHANGE

   GMSG_ID_IPLT_U_UPDATE_WAYPOINT,     // 0x61 DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_U_MARKFOR_WAYPOINT,    // 0x62 DT_IPLT_U_WAYPOINT
   GMSG_ID_IPLT_UPDATE_WAYPOINT_ACK,   // 0x63 DT_IPLT_U_WAYPOINT

   GMSG_ID_IPLT_MOTOR_INIT,            // 0x64 DT_UNSIGNED32
   GMSG_ID_IPLT_MOTORINIT_ACK,         // 0x65 DT_IPILOT_CFGLIST

   GMSG_ID_IPILOT_EXT_STATUS,          // 0x66 DT_IPILOT_CFGLIST
   GMSG_ID_IPILOT_EXT_STATUS_ACK,      // 0x67 DT_IPILOT_CFGLIST

   GMSG_ID_IPLT_PATTERN_MODE,          // 0x68 DT_IPLT_PATTERN_MODE

   GMSG_ID_IPLT_GPS_DATA,              // 0x69 DT_IPLT_PATTERN_MODE
   GMSG_ID_IPLT_HDG_DATA,              // 0x6A DT_IPLT_PATTERN_MODE

   GMSG_ID_IPILOT_LAST,
   GMSG_ID_IPILOT_COUNT          = (GMSG_ID_IPILOT_LAST - GMSG_ID_IPILOT_UNKNOWN),

   GMSG_ID_IPILOT_TEMPORARY = GMSG_ID_IPILOT_UNKNOWN + 0x4000,
   GMSG_ID_IPILOT_SCREENSHOT,

/**************************** SERVICE_CLASS_BLUETOOTH ***********************************/
   GMSG_ID_BLUETOOTH_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_BLUETOOTH),
   GMSG_ID_TALON_NETWORKING_SETTING_CHANGE,
   GMSG_ID_TALON_NETWORKING_DEVICE_INFO,
   GMSG_ID_TALON_REQUEST_DATA,
   GMSG_ID_TALON_NETWORKING_FORGET_DEVICE,
   GMSG_ID_BLUETOOTH_LAST,
   GMSG_ID_BLUETOOTH_COUNT           = (GMSG_ID_BLUETOOTH_LAST-GMSG_ID_BLUETOOTH_UNKNOWN),

/**************************** SERVICE_CLASS_HB_FCID ***********************************/
   GMSG_ID_HB_FCID_UNKNOWN        = GMSG_ID_BASE(SERVICE_CLASS_HB_FCID),
   GMSG_ID_FCID_REQUEST_DATA,
   GMSG_ID_FCID_RECEIVE_DATA,
   GMSG_ID_HB_FCID_LAST,
   GMSG_ID_FCID_NAV_COUNT           = (GMSG_ID_HB_FCID_LAST-GMSG_ID_HB_FCID_UNKNOWN),

} GMSG_ID_TYPE;

/********************        FUNCTION PROTOTYPES             ******************/

/********************          LOCAL VARIABLES               ******************/

/********************          GLOBAL VARIABLES              ******************/

/********************              FUNCTIONS                 ******************/
#ifdef __cplusplus
   }
#endif
#endif //GMSG_ID_H

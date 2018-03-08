/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 3.0                                 */
/* (c) Copyright International Business Machines Corporation, 1988, 1992  */
/*                                                                        */
/**************************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  ACCESS.H                                   *
 *                                                                  *
 *  This file contains information about the NetUser, NetGroup, and *
 *  NetAccess APIs.  There is a section for each set of APIs.  Each *
 *  section contains:                                               *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H.             *
 *                                                                  *
 *         This file is always included by LAN.H                    *
 *                                                                  *
 ********************************************************************/


#ifdef  INCL_32
#pragma pack(1)
#endif





/****************************************************************
 *                                                              *
 *              User Class                                      *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETUSER ) || !defined( LAN_INCLUDED )) \
    && !defined( NETUSER_INCLUDED )

#define NETUSER_INCLUDED


/****************************************************************
 *                                                              *
 *              Function prototypes - USER                      *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetUserAdd ( const char LSFAR * pszServer,
               short              sLevel,
               char LSFAR *       pbBuffer,
               unsigned short     cbBuffer );

extern API_FUNCTION
  NetUserDel ( const char LSFAR * pszServer,
               char LSFAR *       pszUserName );

extern API_FUNCTION
  NetUserEnum ( const char LSFAR *     pszServer,
                short                  sLevel,
                char LSFAR *           pbBuffer,
                unsigned short         cbBuffer,
                unsigned short LSFAR * pcEntriesRead,
                unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetUserGetInfo ( const char LSFAR *     pszServer,
                   char LSFAR *           pszUserName,
                   short                  sLevel,
                   char LSFAR *           pbBuffer,
                   unsigned short         cbBuffer,
                   unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetUserSetInfo ( const char LSFAR * pszServer,
                   char LSFAR *       pszUserName,
                   short              sLevel,
                   char LSFAR *       pbBuffer,
                   unsigned short     cbBuffer,
                   short              sParmNum );

extern API_FUNCTION
  NetUserPasswordSet ( const char LSFAR * pszServer,
                       char LSFAR *       pszUserName,
                       char LSFAR *       pszOldPassword,
                       char LSFAR *       pszNewPassword );

extern API_FUNCTION
  NetUserGetGroups ( const char LSFAR *     pszServer,
                     const char LSFAR *     pszUserName,
                     short                  sLevel,
                     char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short LSFAR * pcEntriesRead,
                     unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetUserSetGroups ( const char LSFAR * pszServer,
                     const char LSFAR * pszUserName,
                     short              sLevel,
                     char LSFAR *       pbBuffer,
                     unsigned short     cbBuffer,
                     unsigned short     cEntries );

extern API_FUNCTION
  NetUserModalsGet ( const char LSFAR *     pszServer,
                     short                  sLevel,
                     char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetUserModalsSet ( const char LSFAR * pszServer,
                     short              sLevel,
                     char LSFAR *       pbBuffer,
                     unsigned short     cbBuffer,
                     short              sParmNum );

extern API_FUNCTION
  NetUserValidate ( char LSFAR *           pszReserved,
                    char LSFAR *           pszUserName,
                    char LSFAR *           pszPassword,
                    unsigned short LSFAR * pusPrivilege );

extern API_FUNCTION
  NetUserValidate2 ( char LSFAR *           pszReserved1,
                     short                  sLevel,
                     char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short         usReserved2,
                     unsigned short LSFAR * pcbTotalAvail );



/****************************************************************
 *                                                              *
 *              Data structure templates - USER                 *
 *                                                              *
 ****************************************************************/

struct user_info_0 {
        char usri0_name[UNLEN+1];
};      /* user_info_0 */

struct user_info_1 {
        char               usri1_name[UNLEN+1];
        char               usri1_pad_1;
        char               usri1_password[ENCRYPTED_PWLEN];/* See note below */
        long               usri1_password_age;
        unsigned short     usri1_priv;                     /* See values below */
        char LSFAR * LSPTR usri1_home_dir;
        char LSFAR * LSPTR usri1_comment;
        unsigned short     usri1_flags;                    /* See values below */
        char LSFAR * LSPTR usri1_script_path;
};      /* user_info_1 */

/*
 *      NOTE:  The maximum length of a user password is PWLEN.  The
 *      field usri1_password contains extra room for transporting
 *      the encrypted form of the password over the network.  When
 *      setting the user's password, check length vs. PWLEN, not
 *      the size of this field.  PWLEN is defined in NETCONS.H.
 */



struct user_info_2 {
        char                        usri2_name[UNLEN+1];
        char                        usri2_pad_1;
        char                        usri2_password[ENCRYPTED_PWLEN];
        long                        usri2_password_age;
        unsigned short              usri2_priv;
        char LSFAR * LSPTR          usri2_home_dir;
        char LSFAR * LSPTR          usri2_comment;
        unsigned short              usri2_flags;
        char LSFAR * LSPTR          usri2_script_path;
        unsigned long               usri2_auth_flags;
        char LSFAR * LSPTR          usri2_full_name;
        char LSFAR * LSPTR          usri2_usr_comment;
        char LSFAR * LSPTR          usri2_parms;
        char LSFAR * LSPTR          usri2_workstations;
        long                        usri2_last_logon;
        long                        usri2_last_logoff;
        long                        usri2_acct_expires;
        unsigned long               usri2_max_storage;
        unsigned short              usri2_units_per_week;
        unsigned char LSFAR * LSPTR usri2_logon_hours;
        unsigned short              usri2_bad_pw_count;
        unsigned short              usri2_num_logons;
        char LSFAR * LSPTR          usri2_logon_server;
        unsigned short              usri2_country_code;
        unsigned short              usri2_code_page;
};      /* user_info_2 */


struct user_info_10 {
        char               usri10_name[UNLEN+1];
        char               usri10_pad_1;
        char LSFAR * LSPTR usri10_comment;
        char LSFAR * LSPTR usri10_usr_comment;
        char LSFAR * LSPTR usri10_full_name;
};      /* user_info_10 */

struct user_info_11 {
        char               usri11_name[UNLEN+1];
        char               usri11_pad_1;
        char LSFAR * LSPTR usri11_comment;
        char LSFAR * LSPTR usri11_usr_comment;
        char LSFAR * LSPTR usri11_full_name;
        unsigned short     usri11_priv;
        unsigned long      usri11_auth_flags;
        long               usri11_password_age;
        char LSFAR * LSPTR usri11_home_dir;
        char LSFAR * LSPTR usri11_parms;
        long               usri11_last_logon;
        long               usri11_last_logoff;
        unsigned short     usri11_bad_pw_count;
        unsigned short     usri11_num_logons;
        char LSFAR * LSPTR usri11_logon_server;
        unsigned short     usri11_country_code;
        char LSFAR * LSPTR usri11_workstations;
        unsigned long      usri11_max_storage;
        unsigned short     usri11_units_per_week;
        char LSFAR * LSPTR usri11_logon_hours;
        unsigned short     usri11_code_page;
};      /* user_info_11 */

/*
 *      For User Modals
*/

struct  user_modals_info_0 {
        unsigned short  usrmod0_min_passwd_len;
        unsigned long   usrmod0_max_passwd_age;
        unsigned long   usrmod0_min_passwd_age;
        unsigned long   usrmod0_force_logoff;
        unsigned short  usrmod0_password_hist_len;
        unsigned short  usrmod0_reserved1;
};      /* user_modals_info_0 */


struct  user_modals_info_1 {
        unsigned short     usrmod1_role;
        char LSFAR * LSPTR usrmod1_primary;
};      /* user_modals_info_1 */



/*
 *      For User Logon Validation
*/

struct user_logon_req_1 {
        char               usrreq1_name[UNLEN+1];
        char               usrreq1_pad_1;
        char               usrreq1_password[SESSION_PWLEN];
        char LSFAR * LSPTR usrreq1_workstation;
};      /* user_logon_req_1 */


struct user_logon_info_0 {
        char            usrlog0_eff_name[UNLEN+1];
        char            usrlog0_pad_1;
};      /* user_logon_info_0 */

struct user_logon_info_1 {
        unsigned short     usrlog1_code;
        char               usrlog1_eff_name[UNLEN+1];
        char               usrlog1_pad_1;
        unsigned short     usrlog1_priv;
        unsigned long      usrlog1_auth_flags;
        unsigned short     usrlog1_num_logons;
        unsigned short     usrlog1_bad_pw_count;
        unsigned long      usrlog1_last_logon;
        unsigned long      usrlog1_last_logoff;
        unsigned long      usrlog1_logoff_time;
        unsigned long      usrlog1_kickoff_time;
        long               usrlog1_password_age;
        unsigned long      usrlog1_pw_can_change;
        unsigned long      usrlog1_pw_must_change;
        char LSFAR * LSPTR usrlog1_computer;
        char LSFAR * LSPTR usrlog1_domain;
        char LSFAR * LSPTR usrlog1_script_path;
        unsigned long      usrlog1_reserved1;
};      /* user_logon_info_1 */


struct user_logon_info_2 {
        char               usrlog2_eff_name[UNLEN+1];
        char               usrlog2_pad_1;
        char LSFAR * LSPTR usrlog2_computer;
        char LSFAR * LSPTR usrlog2_full_name;
        char LSFAR * LSPTR usrlog2_usrcomment;
        unsigned long      usrlog2_logon_time;
};      /* user_logon_info_2 */



struct user_logoff_req_1 {
        char            usrlfreq1_name[UNLEN+1];
        char            usrlfreq1_pad_1;
        char            usrlfreq1_workstation[CNLEN+1];
};      /* user_logoff_req_1 */

struct user_logoff_info_1 {
        unsigned short  usrlogf1_code;
        unsigned long   usrlogf1_duration;
        unsigned short  usrlogf1_num_logons;
};      /* user_logoff_info_1 */

/****************************************************************
 *                                                              *
 *              Special values and constants - USER             *
 *                                                              *
 ****************************************************************/


/*
 *      Bit masks for field usriX_flags of user_info_X (X = 0/1).
 */

#define         UF_SCRIPT               0x1
#define         UF_ACCOUNTDISABLE       0x2
#define         UF_DELETE_PROHIBITED    0x4
#define         UF_HOMEDIR_REQUIRED     0x8
#define         UF_PASSWD_NOTREQD       0x20
#define         UF_PASSWD_CANT_CHANGE   0x40

/*
 *      Bit masks for field usri2_auth_flags of user_info_2.
*/

#define         AF_OP_PRINT             0x1
#define         AF_OP_COMM              0x2
#define         AF_OP_SERVER            0x4
#define         AF_OP_ACCOUNTS          0x8


/*
 *      UAS role manifests under NETLOGON
 */
#define         UAS_ROLE_STANDALONE     0
#define         UAS_ROLE_MEMBER         1
#define         UAS_ROLE_BACKUP         2
#define         UAS_ROLE_PRIMARY        3

/*
 *      Values for parmnum for NetUserSetInfo.
 */

/* LM1.0 style */
#define         U1_ALL                  0
#define         U1_NAME                 1
#define         U1_PAD                  2
#define         U1_PASSWD               3
#define         U1_PASSWDAGE            4
#define         U1_PRIV                 5
#define         U1_DIR                  6
#define         U1_COMMENT              7
#define         U1_USER_FLAGS           8
#define         U1_SCRIPT_PATH          9


/* LM2.0 style */
#ifndef  PARMNUM_ALL
#define         PARMNUM_ALL             0
#endif

#define         PARMNUM_NAME            1
#define         PARMNUM_PAD             2
#define         PARMNUM_PASSWD          3
#define         PARMNUM_PASSWDAGE       4
#define         PARMNUM_PRIV            5
#define         PARMNUM_DIR             6
#define         PARMNUM_COMMENT         7
#define         PARMNUM_USER_FLAGS      8
#define         PARMNUM_SCRIPT_PATH     9
#define         PARMNUM_AUTH_FLAGS      10
#define         PARMNUM_FULL_NAME       11
#define         PARMNUM_USR_COMMENT     12
#define         PARMNUM_PARMS           13
#define         PARMNUM_WORKSTATIONS    14
#define         PARMNUM_LAST_LOGON      15
#define         PARMNUM_LAST_LOGOFF     16
#define         PARMNUM_ACCT_EXPIRES    17
#define         PARMNUM_MAX_STORAGE     18
#define         PARMNUM_UNITS_PER_WEEK  19
#define         PARMNUM_LOGON_HOURS     20
#define         PARMNUM_BADPW_COUNT     21
#define         PARMNUM_NUM_LOGONS      22
#define         PARMNUM_LOGON_SERVER    23
#define         PARMNUM_COUNTRY_CODE    24
#define         PARMNUM_CODE_PAGE       25
#define         PARMNUM_PASSWD_EXPIRED 100

/*
 *      For SetInfo call (parmnum 0) when password change not required
 */
#define         NULL_USERSETINFO_PASSWD         "              "



#define         TIMEQ_FOREVER                   ((unsigned long) -1L)
#define         USER_MAXSTORAGE_UNLIMITED       ((unsigned long) -1L)
#define         USER_NO_LOGOFF                  ((unsigned long) -1L)
#define         UNITS_PER_DAY                   24
#define         UNITS_PER_WEEK                  UNITS_PER_DAY * 7


/*
 *      Privilege levels (user_info_X field usriX_priv (X = 0/1)).
 */
#define         USER_PRIV_MASK          0x3
#define         USER_PRIV_GUEST         0
#define         USER_PRIV_USER          1
#define         USER_PRIV_ADMIN         2

/*
 *      user modals related defaults
 */
#define         MAX_PASSWD_LEN          PWLEN
#define         DEF_MIN_PWLEN           6
#define         DEF_PWUNIQUENESS        5
#define         DEF_MAX_PWHIST          8
#define         DEF_MAX_PWAGE           (unsigned long) 90*24*3600 /* 90 days */
#define         DEF_MIN_PWAGE           (unsigned long) 0L         /* 0 days */
#define         DEF_FORCE_LOGOFF        (unsigned long) 0xffffffff /* never */
#define         DEF_MAX_BADPW           0                          /* no limit*/
#define         ONE_DAY                 (unsigned long) 01*24*3600 /* 01 day  */
/*
 *      User Logon Validation (codes returned)
*/
#define         VALIDATED_LOGON         0
#define         PASSWORD_EXPIRED        2
#define         NON_VALIDATED_LOGON     3

#define         VALID_LOGOFF            1


/*
 *      parmnum manifests for user modals
*/

#define         MODAL0_PARMNUM_ALL              0
#define         MODAL0_PARMNUM_MIN_LEN          1
#define         MODAL0_PARMNUM_MAX_AGE          2
#define         MODAL0_PARMNUM_MIN_AGE          3
#define         MODAL0_PARMNUM_FORCEOFF         4
#define         MODAL0_PARMNUM_HISTLEN          5
#define         MODAL0_PARMNUM_RESERVED1        6

#define         MODAL1_PARMNUM_ALL              0
#define         MODAL1_PARMNUM_ROLE             1
#define         MODAL1_PARMNUM_PRIMARY          2


#endif /* NETUSER_INCLUDED */


/****************************************************************
 *                                                              *
 *              Group Class                                     *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETGROUP ) || !defined( LAN_INCLUDED )) \
    && !defined( NETGROUP_INCLUDED )

#define NETGROUP_INCLUDED


/****************************************************************
 *                                                              *
 *              Function prototypes - GROUP                     *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetGroupAdd ( const char LSFAR * pszServer,
                short              sLevel,
                char LSFAR *       pbBuffer,
                unsigned short     cbBuffer );

extern API_FUNCTION
  NetGroupDel ( const char LSFAR * pszServer,
                char LSFAR *       pszGroupName );

extern API_FUNCTION
  NetGroupEnum ( const char LSFAR *     pszServer,
                 short                  sLevel,
                 char LSFAR *           pbBuffer,
                 unsigned short         cbBuffer,
                 unsigned short LSFAR * pcEntriesRead,
                 unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetGroupAddUser ( const char LSFAR * pszServer,
                    char LSFAR *       pszGroupName,
                    char LSFAR *       pszUserName );

extern API_FUNCTION
  NetGroupDelUser ( const char LSFAR * pszServer,
                    char LSFAR *       pszGroupName,
                    char LSFAR *       pszUserName );

extern API_FUNCTION
  NetGroupGetUsers ( const char LSFAR *     pszServer,
                     const char LSFAR *     pszGroupName,
                     short                  sLevel,
                     char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short LSFAR * pcEntriesRead,
                     unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetGroupSetUsers ( const char LSFAR * pszServer,
                     const char LSFAR * pszGroupName,
                     short              sLevel,
                     char LSFAR *       pbBuffer,
                     unsigned short     cbBuffer,
                     unsigned short     cEntries );

extern API_FUNCTION
  NetGroupGetInfo ( const char LSFAR *     pszServer,
                    char LSFAR *           pszGroupName,
                    short                  sLevel,
                    char LSFAR *           pbBuffer,
                    unsigned short         cbBuffer,
                    unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetGroupSetInfo ( const char LSFAR * pszServer,
                    char LSFAR *       pszGroupName,
                    short              sLevel,
                    char LSFAR *       pbBuffer,
                    unsigned short     cbBuffer,
                    short              sParmNum );


/****************************************************************
 *                                                              *
 *             Data structure templates - GROUP                 *
 *                                                              *
 ****************************************************************/


struct group_info_0 {
        char            grpi0_name[GNLEN+1];
};      /* group_info_0 */

struct group_info_1 {
        char               grpi1_name[GNLEN+1];
        char               grpi1_pad;
        char LSFAR * LSPTR grpi1_comment;
};      /* group_info_1 */

struct group_users_info_0 {
        char            grui0_name[UNLEN+1];
};      /* group_users_info_0 */



/****************************************************************
 *                                                              *
 *              Special values and constants - GROUP            *
 *                                                              *
 ****************************************************************/

#define         GROUPIDMASK     0x8000  /* MSB set if uid refers to a group */

/*
 *      Predefined group for all normal users, administrators and guests
 *      LOCAL is a special group for pinball local security.
*/

#define         GROUP_SPECIALGRP_USERS          "USERS"
#define         GROUP_SPECIALGRP_ADMINS         "ADMINS"
#define         GROUP_SPECIALGRP_GUESTS         "GUESTS"
#define         GROUP_SPECIALGRP_LOCAL          "LOCAL"


/*
 *      parmnum manifests for SetInfo calls (only comment is settable)
*/

#define         GRP1_PARMNUM_ALL        0
#define         GRP1_PARMNUM_NAME       1
#define         GRP1_PARMNUM_COMMENT    2

#endif /* NETGROUP_INCLUDED */

/****************************************************************
 *                                                              *
 *                 Access Class                                 *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETACCESS ) || !defined( LAN_INCLUDED )) \
    && !defined( NETACCESS_INCLUDED )

#define NETACCESS_INCLUDED


/****************************************************************
 *                                                              *
 *                  Function prototypes - ACCESS                *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetAccessAdd ( const char LSFAR * pszServer,
                 short              sLevel,
                 char LSFAR *       pbBuffer,
                 unsigned short     cbBuffer );

extern API_FUNCTION
  NetAccessCheck ( char LSFAR *           pszReserved,
                   char LSFAR *           pszUserName,
                   char LSFAR *           pszResource,
                   unsigned short         usOperation,
                   unsigned short LSFAR * pusResult );

extern API_FUNCTION
  NetAccessDel ( const char LSFAR * pszServer,
                 char LSFAR *       pszResource );

extern API_FUNCTION
  NetAccessEnum ( const char LSFAR *     pszServer,
                  char LSFAR *           pszBasePath,
                  short                  fsRecursive,
                  short                  sLevel,
                  char LSFAR *           pbBuffer,
                  unsigned short         cbBuffer,
                  unsigned short LSFAR * pcEntriesRead,
                  unsigned short LSFAR * pcTotalAvail );

extern API_FUNCTION
  NetAccessGetInfo ( const char LSFAR *     pszServer,
                     char LSFAR *           pszResource,
                     short                  sLevel,
                     char LSFAR *           pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short LSFAR * pcbTotalAvail );

extern API_FUNCTION
  NetAccessSetInfo ( const char LSFAR * pszServer,
                     char LSFAR *       pszResource,
                     short              sLevel,
                     char LSFAR *       pbBuffer,
                     unsigned short     cbBuffer,
                     short              sParmNum );

extern API_FUNCTION
  NetAccessGetUserPerms ( char LSFAR *           pszServer,
                          char LSFAR *           pszUgName,
                          char LSFAR *           pszResource,
                          unsigned short LSFAR * pusPerms );


/****************************************************************
 *                                                              *
 *              Data structure templates - ACCESS               *
 *                                                              *
 ****************************************************************/

struct access_list {
        char            acl_ugname[UNLEN+1];
        char            acl_ugname_pad_1;
        short           acl_access;
};      /* access_list */

struct access_info_0 {
        char LSFAR * LSPTR acc0_resource_name;
};      /* access_info_0 */

struct access_info_1 {
        char  LSFAR * LSPTR acc1_resource_name;
        short               acc1_attr;  /* See values below */
        short               acc1_count;
};      /* access_info_1 */

/****************************************************************
 *                                                              *
 *              Special values and constants - ACCESS           *
 *                                                              *
 ****************************************************************/

/*
 *      Maximum number of permission entries for each resource.
 */

#define MAXPERMENTRIES  64


/*
 *      Bit values for the access permissions.  ACCESS_ALL is a handy
 *      way to specify maximum permissions.  These are used in
 *      acl_access field of access_list structures.
 */
#define         ACCESS_NONE     0
#define         ACCESS_ALL     (ACCESS_READ|ACCESS_WRITE|ACCESS_CREATE|ACCESS_EXEC|ACCESS_DELETE|ACCESS_ATRIB|ACCESS_PERM)
#define         ACCESS_READ     0x1
#define         ACCESS_WRITE    0x2
#define         ACCESS_CREATE   0x4
#define         ACCESS_EXEC     0x8
#define         ACCESS_DELETE   0x10
#define         ACCESS_ATRIB    0x20
#define         ACCESS_PERM     0x40

#define         ACCESS_GROUP    0x8000

/*
 *      Bit values for the acc1_attr field of the access_info_1 structure.
 *      Only one bit is currently defined.
 */

#define         ACCESS_AUDIT            0x1

/*
 *      Parmnum value for NetAccessSetInfo.
 */

#define         ACCESS_ATTR_PARMNUM     2


/*
 *      ACCESS_LETTERS defines a letter for each bit position in
 *      the acl_access field of struct access_list.  Note that some
 *      bits have a corresponding letter of ' ' (space).
 */

#define         ACCESS_LETTERS          "RWCXDAP         "


#endif /* NETACCESS_INCLUDED */

/****************************************************************
 *                                                              *
 *              Domain Class                                    *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETDOMAIN ) || !defined( LAN_INCLUDED )) \
    && !defined( NETDOMAIN_INCLUDED )

#define NETDOMAIN_INCLUDED


/****************************************************************
 *                                                              *
 *                  Function prototypes - DOMAIN                *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetGetDCName ( const char LSFAR * pszServer,
                 const char LSFAR * pszDomain,
                 char LSFAR *       pbBuffer,
                 unsigned short     cbBuffer );

extern API_FUNCTION
  NetLogonEnum ( const char LSFAR *     pszServer,
                 short                  sLevel,
                 char LSFAR *           pbBuffer,
                 unsigned short         cbBuffer,
                 unsigned short LSFAR * pcEntriesRead,
                 unsigned short LSFAR * pcTotalAvail );


/****************************************************************
 *                                                              *
 *              Special values and constants - DOMAIN           *
 *                                                              *
 ****************************************************************/

#define         LOGON_INFO_UNKNOWN      -1


#endif /* NETDOMAIN_INCLUDED */

#ifdef  INCL_32
#pragma linkage (NetUserAdd, far16 pascal)
#pragma linkage (NetUserDel, far16 pascal)
#pragma linkage (NetUserEnum, far16 pascal)
#pragma linkage (NetUserGetInfo, far16 pascal)
#pragma linkage (NetUserSetInfo, far16 pascal)
#pragma linkage (NetUserPasswordSet, far16 pascal)
#pragma linkage (NetUserGetGroups, far16 pascal)
#pragma linkage (NetUserSetGroups, far16 pascal)
#pragma linkage (NetUserModalsGet, far16 pascal)
#pragma linkage (NetUserModalsSet, far16 pascal)
#pragma linkage (NetUserValidate, far16 pascal)
#pragma linkage (NetUserValidate2, far16 pascal)
#pragma linkage (NetGroupAdd, far16 pascal)
#pragma linkage (NetGroupDel, far16 pascal)
#pragma linkage (NetGroupEnum, far16 pascal)
#pragma linkage (NetGroupAddUser, far16 pascal)
#pragma linkage (NetGroupDelUser, far16 pascal)
#pragma linkage (NetGroupGetUsers, far16 pascal)
#pragma linkage (NetGroupSetUsers, far16 pascal)
#pragma linkage (NetGroupGetInfo, far16 pascal)
#pragma linkage (NetGroupSetInfo, far16 pascal)
#pragma linkage (NetAccessAdd, far16 pascal)
#pragma linkage (NetAccessCheck, far16 pascal)
#pragma linkage (NetAccessDel, far16 pascal)
#pragma linkage (NetAccessEnum, far16 pascal)
#pragma linkage (NetAccessGetInfo, far16 pascal)
#pragma linkage (NetAccessSetInfo, far16 pascal)
#pragma linkage (NetAccessGetUserPerms, far16 pascal)
#pragma linkage (NetGetDCName, far16 pascal)
#pragma linkage (NetLogonEnum, far16 pascal)
#pragma pack()
#endif

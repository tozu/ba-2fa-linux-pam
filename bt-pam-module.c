#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syslog.h>

/* Include PAM headers */
#include <security/pam_appl.h>
#include <security/pam_modules.h>

/* Define which PAM interface the PAM provides */
#define PAM_SM_AUTH

#include "bluescan.h"

/* AUTH */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	if(argc < 4) {
		syslog(LOG_ERR, "to less arguments\n");
		return PAM_AUTH_ERR;
	}

	const char *user = NULL;
	int pgu_ret;

	pgu_ret = pam_get_user(pamh, &user, NULL);
	if(pgu_ret != PAM_SUCCESS || user == NULL || *user == '\0') {
		syslog(LOG_ERR, "can't determine or invaild user");
		return PAM_AUTH_ERR;
	}

	char *certlocation = argv[1];
	char *host = argv[2];
	char *level = argv[3];
	char *hmac = argv[4];

	// invoke remote BluetoothScan and check if was successful
	if(bluescan(host, level, hmac, certlocation)) {
		size_t res = getFoundBT();	// check on restult of remote BluetoothScan
		if(res == 0) {					// NO BT device found
			return PAM_AUTH_ERR;
		} else if(res == 1) {		// BT device found
			return PAM_SUCCESS;
		} else if(res == 3) {		// Error with remote scan
			return PAM_AUTH_ERR;
		} else {								// catch all the other results
			return PAM_AUTH_ERR;
		}
	}
	// remote BluetoothScan was UNsuccessful, deny access
	return PAM_AUTH_ERR;
}

// defined but not implemented
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return PAM_IGNORE;
}

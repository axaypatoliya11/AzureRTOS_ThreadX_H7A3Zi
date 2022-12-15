/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Component                                                        */
/**                                                                       */
/**   Address Resolution Protocol (ARP)                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define NX_SOURCE_CODE


/* Include necessary system files.  */

#include "nx_api.h"
#include "nx_ip.h"
#include "nx_arp.h"

/* Bring in externs for caller checking code.  */

NX_CALLER_CHECKING_EXTERNS


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nxe_arp_static_entry_delete                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Yuxin Zhou, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the ARP static entry delete      */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ip_ptr                                IP instance pointer           */
/*    ip_address                            IP Address binding to delete  */
/*    physical_msw                          Physical address MSW          */
/*    physical_lsw                          Physical address LSW          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nx_arp_static_entry_delete           Actual static entry delete    */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Yuxin Zhou               Initial Version 6.0           */
/*  09-30-2020     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _nxe_arp_static_entry_delete(NX_IP *ip_ptr, ULONG ip_address,
                                   ULONG physical_msw, ULONG physical_lsw)
{

UINT status;


    /* Check for invalid input pointers.  */
    if ((ip_ptr == NX_NULL) || (ip_ptr -> nx_ip_id != NX_IP_ID))
    {
        return(NX_PTR_ERROR);
    }

    /* Check for invalid IP address.  */
    if (!ip_address)
    {
        return(NX_IP_ADDRESS_ERROR);
    }

    /* Check to see if ARP is enabled.  */
    if (!ip_ptr -> nx_ip_arp_allocate)
    {
        return(NX_NOT_ENABLED);
    }

    /* Check for appropriate caller.  */
    NX_THREADS_ONLY_CALLER_CHECKING

    /* Call actual ARP static entry delete function.  */
    status =  _nx_arp_static_entry_delete(ip_ptr, ip_address, physical_msw, physical_lsw);

    /* Return completion status.  */
    return(status);
}


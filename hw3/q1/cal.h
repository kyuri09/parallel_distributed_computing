/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CAL_H_RPCGEN
#define _CAL_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define RPC_PROG 0x31111111
#define RPC_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define CALCULATE 1
extern  int * calculate_1(char **, CLIENT *);
extern  int * calculate_1_svc(char **, struct svc_req *);
extern int rpc_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define CALCULATE 1
extern  int * calculate_1();
extern  int * calculate_1_svc();
extern int rpc_prog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CAL_H_RPCGEN */

///////////////////////////////////////////////////////////////////////////////
// Description:
//   Defines the one small helper function that determines the number of
//   available columns of the terminal.
// Dependencies (others than those explicitly visible in the #include clause):
//    None.
// Project: PTS
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: jon $
// $Date:$
// $Revision:$
// $Id:$
///////////////////////////////////////////////////////////////////////////////

#include "tty_cols.h"
///////////////////////////////////////////////////////////////////////////////
int tty_cols(void)
{
   char *tmp=getenv("COLUMNS");
   if(tmp==NULL) return (-1);
   return(atoi(tmp));
}

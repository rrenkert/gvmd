/* Test 1 of OMP HELP.
 * $Id$
 * Description:
 * Test OMP HELP before authenticating.  This is an important test of
 * determining the protocol because <help/> is shorter than "< OTP/1.0 >\n".
 *
 * Authors:
 * Matthew Mundell <matt@mundell.ukfsn.org>
 *
 * Copyright:
 * Copyright (C) 2009 Greenbone Networks GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * or, at your option, any later version as published by the Free
 * Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "../tracef.h"

int
main ()
{
  int socket;
  gnutls_session_t session;

  setup_test ();

  socket = connect_to_manager (&session);
  if (socket == -1) return EXIT_FAILURE;

  /* Request the help text. */

  if (send_to_manager (&session, "<help/>") == -1)
     {
      close_manager_connection (socket, session);
      return EXIT_FAILURE;
    }

  /* Read the response. */

  entity_t entity = NULL;
  read_entity (&session, &entity);

  /* Compare to expected response. */

  // FIX actually an auth error
  entity_t expected = add_entity (NULL, "omp_response", NULL);
  add_attribute (expected, "status", "400");

  if (compare_entities (entity, expected))
    {
      free_entity (entity);
      free_entity (expected);
      close_manager_connection (socket, session);
      return EXIT_FAILURE;
    }

  free_entity (expected);
  free_entity (entity);
  close_manager_connection (socket, session);
  return EXIT_SUCCESS;
}

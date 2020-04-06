/*
 * lxc: linux Container library
 *
 * (C) 2020  Pantacor Ltd
 *
 * Authors:
 * Alexander Sack <asac@pantacor.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef __LXC_REALPATH_X_H
#define __LXC_REALPATH_X_H

char *realpath_x(const char* root, const char * __restrict, char * __restrict);

#endif /* __LXC_REALPATH_X_H */


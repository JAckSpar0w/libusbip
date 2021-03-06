/**
 * libusbip - libusbip.c
 * Copyright (C) 2011 Manuel Gebele
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "libusbip.h"
#include "client.h"
#include "server.h"
#include "error.h"

#include <libusb-1.0/libusb.h>

#define IS_VALID_OBJ(__struct) (__struct != NULL)
#define IS_VALID_LENGTH(__length) (__length >= 0 && __length < LIBUSBIP_MAX_DATA)
#define IS_VALID_CONTEXT(__ctx) (__ctx == LIBUSBIP_CTX_CLIENT || __ctx == LIBUSBIP_CTX_SERVER) // Not used yet

static struct libusb_context *libusbip_ctx = NULL;

libusbip_rpc_t
libusbip_get_rpc(int sock) {
    return server_read_rpc(sock);
}

libusbip_error_t
libusbip_init(struct libusbip_connection_info *ci) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_init(ci);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_init(ci, &libusbip_ctx);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

void
libusbip_exit(struct libusbip_connection_info *ci) {    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_exit(ci);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_exit(libusbip_ctx);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

void
libusbip_get_device_list(struct libusbip_connection_info *ci,
                         struct libusbip_device_list *dl) {
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    if (!IS_VALID_OBJ(dl)) {
        error_illegal_libusbip_device_list(__func__);
        return;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_get_device_list(ci, dl);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_device_list(ci, libusbip_ctx);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

libusbip_error_t
libusbip_get_device_descriptor(struct libusbip_connection_info *ci,
                               struct libusbip_device *dev,
                               struct libusbip_device_descriptor *dd) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dev)) {
        error_illegal_libusbip_device(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dd)) {
        error_illegal_libusbip_device_descriptor(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_get_device_descriptor(ci, dev, dd);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_device_descriptor(ci, libusbip_ctx);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_open(struct libusbip_connection_info *ci,
              struct libusbip_device *dev, struct libusbip_device_handle *dh) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;

    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dev)) {
        error_illegal_libusbip_device(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_open(ci, dev, dh);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_open(ci, libusbip_ctx);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

void
libusbip_open_device_with_vid_pid(struct libusbip_connection_info *ci,
                                  struct libusbip_device_handle *dh,
                                  uint16_t vid, uint16_t pid) {
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        return;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_open_device_with_vid_pid(ci, dh, vid, pid);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_open_device_with_vid_pid(ci, libusbip_ctx);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

void
libusbip_close(struct libusbip_connection_info *ci,
               struct libusbip_device_handle *dh) {
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        return;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_close(ci, dh);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_close(ci);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

libusbip_error_t
libusbip_claim_interface(struct libusbip_connection_info *ci,
                         struct libusbip_device_handle *dh, int intf) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_claim_interface(ci, dh, intf);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_claim_interface(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_release_interface(struct libusbip_connection_info *ci,
                           struct libusbip_device_handle *dh, int intf) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_release_interface(ci, dh, intf);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_release_interface(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_get_configuration(struct libusbip_connection_info *ci,
                           struct libusbip_device_handle *dh, int *conf) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_get_configuration(ci, dh, conf);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_configuration(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_set_configuration(struct libusbip_connection_info *ci,
                           struct libusbip_device_handle *dh, int conf) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_set_configuration(ci, dh, conf);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_set_configuration(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_set_interface_alt_setting(struct libusbip_connection_info *ci,
                                   struct libusbip_device_handle *dh,
                                   int intf, int alt_setting) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_set_interface_alt_setting(ci, dh, intf, alt_setting);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_set_interface_alt_setting(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_reset_device(struct libusbip_connection_info *ci,
                      struct libusbip_device_handle *dh) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_reset_device(ci, dh);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_reset_device(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }

    return error;
}

libusbip_error_t
libusbip_clear_halt(struct libusbip_connection_info *ci,
                    struct libusbip_device_handle *dh, uint16_t endpoint) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_clear_halt(ci, dh, endpoint);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_clear_halt(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

int
libusbip_get_string_descriptor_ascii(struct libusbip_connection_info *ci,
                                     struct libusbip_device_handle *dh,
                                     uint16_t idx, unsigned char *data, int length) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_OBJ(data)) {
        error_illegal_buffer(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (ci->ctx == LIBUSBIP_CTX_CLIENT && !IS_VALID_LENGTH(length)) {
        error_illegal_length(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_get_string_descriptor_ascii(ci, dh, idx, data, length);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_string_descriptor_ascii(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

int
libusbip_control_transfer(struct libusbip_connection_info *ci,
                          struct libusbip_device_handle *dh, uint16_t req_type,
                          uint16_t req, uint16_t val, uint16_t idx,
                          unsigned char *data, uint16_t len, uint32_t timeout) {    
    int bytes_transferred = 0;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    if (ci->ctx == LIBUSBIP_CTX_CLIENT && !IS_VALID_LENGTH(len)) { // FIXME: compiler warning
        error_illegal_length(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        bytes_transferred = client_usb_control_transfer(ci, dh, req_type, req, val,
                                                       idx, data, len, timeout);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_control_transfer(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    
    return bytes_transferred;
}

int
libusbip_bulk_transfer(struct libusbip_connection_info *ci,
                       struct libusbip_device_handle *dh, uint16_t endpoint,
                       unsigned char *data, int length, int *transferred, uint32_t timeout) {
    int bytes_transferred = 0;
    
    if (!IS_VALID_OBJ(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    if (!IS_VALID_OBJ(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    if (ci->ctx == LIBUSBIP_CTX_CLIENT && !IS_VALID_LENGTH(length)) {
        error_illegal_length(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    
    if (ci->ctx == LIBUSBIP_CTX_CLIENT)
        bytes_transferred = client_usb_bulk_transfer(ci, dh, endpoint, data,
                                                     length, transferred, timeout);
    else if (ci->ctx == LIBUSBIP_CTX_SERVER)
        server_usb_bulk_transfer(ci);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        return LIBUSBIP_E_FAILURE;
    }
    
    return bytes_transferred;
}

libusbip_error_t
libusbip_rpc_call(libusbip_rpc_t rpc, struct libusbip_rpc_info *ri) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;

    if (!IS_VALID_OBJ(ri)) {
        error_illegal_libusbip_rpc_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        goto done;
    }
    
    switch (rpc) {
    case LIBUSBIP_RPC_USB_INIT:
        error = libusbip_init(&ri->ci);
        break;
            
    case LIBUSBIP_RPC_USB_EXIT:
        libusbip_exit(&ri->ci);
        break;
    case LIBUSBIP_RPC_USB_GET_DEVICE_LIST:
        libusbip_get_device_list(&ri->ci, &ri->dl);
        break;
            
    case LIBUSBIP_RPC_USB_GET_DEVICE_DESCRIPTOR:
        error= libusbip_get_device_descriptor(&ri->ci, &ri->dev, &ri->dd);
        break;
            
    case LIBUSBIP_RPC_USB_OPEN:
        error = libusbip_open(&ri->ci, &ri->dev, &ri->dh);
        break;
            
    case LIBUSBIP_RPC_USB_OPEN_DEVICE_WITH_VID_PID:
        libusbip_open_device_with_vid_pid(&ri->ci, &ri->dh, ri->vid, ri->pid);
        break;
            
    case LIBUSBIP_RPC_USB_CLOSE:
        libusbip_close(&ri->ci, &ri->dh);
        break;
            
    case LIBUSBIP_RPC_USB_CLAIM_INTERFACE:
        error = libusbip_claim_interface(&ri->ci, &ri->dh, ri->intf);
        break;
            
    case LIBUSBIP_RPC_USB_RELEASE_INTERFACE:
        error = libusbip_release_interface(&ri->ci, &ri->dh, ri->intf);
        break;
            
    case LIBUSBIP_RPC_USB_GET_CONFIGURATION:
        error = libusbip_get_configuration(&ri->ci, &ri->dh, &ri->conf);
        break;
    
    case LIBUSBIP_RPC_USB_SET_CONFIGURATION:
        error = libusbip_set_configuration(&ri->ci, &ri->dh, ri->conf);
        break;            
    
    case LIBUSBIP_RPC_USB_SET_INTERFACE_ALT_SETTING:
        error = libusbip_set_interface_alt_setting(&ri->ci, &ri->dh, ri->intf,
                                                   ri->alt_setting);
        break;        
    
    case LIBUSBIP_RPC_USB_RESET_DEVICE:
        error = libusbip_reset_device(&ri->ci, &ri->dh);
        break;
    
    case LIBUSBIP_RPC_USB_CLEAR_HALT:
        error = libusbip_clear_halt(&ri->ci, &ri->dh, ri->endpoint);
        break;
            
    case LIBUSBIP_RPC_USB_GET_STRING_DESCRIPTOR_ASCII:
        error = libusbip_get_string_descriptor_ascii(&ri->ci, &ri->dh,
                                                     ri->idx, (unsigned char *)ri->data,
                                                     ri->length);
        break;
            
    case LIBUSBIP_RPC_USB_CONTROL_TRANSFER:
        /* NOTE: Not really an error. */
        return libusbip_control_transfer(&ri->ci, &ri->dh, ri->req_type,
                                         ri->req, ri->val, ri->idx, (unsigned char *)ri->data,
                                         ri->len, ri->timeout);
        break; /* NEVER_REACHED */
            
    case LIBUSBIP_RPC_USB_BULK_TRANSFER:
        /* NOTE: Not really an error. */
        return libusbip_bulk_transfer(&ri->ci, &ri->dh, ri->endpoint,
                                      (unsigned char *)ri->data, ri->length,
                                      &ri->transferred, ri->timeout);
        break; /* NEVER_REACHED */
            
    default:
        error_illegal_libusbip_rpc_t(__func__);
        error = LIBUSBIP_E_FAILURE;
        break;
    }
    
done:
    return error;
}

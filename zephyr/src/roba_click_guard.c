/*
 * roBa mouse-click trackball guard
 *
 * While the configured keymap layer is active, discard PMW3610 REL_X/REL_Y
 * events. The keymap uses a macro to hold this layer for a short period after
 * a mouse-button press, preventing accidental movement from turning a click
 * into a drag.
 */

#define DT_DRV_COMPAT zmk_input_processor_roba_click_guard

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#include <drivers/input_processor.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

static bool roba_click_guard_active;

static int roba_click_guard_layer_listener(const zmk_event_t *eh) {
    struct zmk_layer_state_changed *ev;

    ev = as_zmk_layer_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    /*
     * Layer 8 is the mouse-click guard layer.
     */
    if (ev->layer == 8) {
        roba_click_guard_active = ev->state;
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(roba_click_guard_layer, roba_click_guard_layer_listener);
ZMK_SUBSCRIPTION(roba_click_guard_layer, zmk_layer_state_changed);

static int roba_click_guard_handle_event(
    const struct device *dev,
    struct input_event *event,
    uint32_t param1,
    uint32_t param2,
    struct zmk_input_processor_state *state) {

    ARG_UNUSED(dev);
    ARG_UNUSED(param1);
    ARG_UNUSED(param2);
    ARG_UNUSED(state);

    if (!roba_click_guard_active) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    if (event->type == INPUT_EV_REL &&
        (event->code == INPUT_REL_X || event->code == INPUT_REL_Y)) {
        event->value = 0;
    }

    return ZMK_INPUT_PROC_CONTINUE;
}

static const struct zmk_input_processor_driver_api roba_click_guard_api = {
    .handle_event = roba_click_guard_handle_event,
};

#define ROBA_CLICK_GUARD_INIT(n)                                      \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,    \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,        \
                          &roba_click_guard_api);

DT_INST_FOREACH_STATUS_OKAY(ROBA_CLICK_GUARD_INIT)

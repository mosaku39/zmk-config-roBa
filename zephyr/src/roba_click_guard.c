/*
 * roBa mouse-click trackball guard
 *
 * Suppress PMW3610 X/Y movement while the guard layer is active.
 */

#define DT_DRV_COMPAT zmk_input_processor_roba_click_guard

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#include <drivers/input_processor.h>

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

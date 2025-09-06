# Configuration

> [!NOTE]
> This page is a draft

## Device configuration

Define the GPIOs used for scanning keypresses and diode orientation if scanning
in matrix mode.


```c
// For matrix wiring
static const keyscan_gpios_t matrix = {
    diode_direction = {/*...*/},

    rows = {/*...*/},
    rows_len = /*...*/,

    cols = {/*...*/},
    cols_len = /*...*/,
};

// For direct wiring
static const keyscan_gpios_t matrix_direct = {
    direct = {/*...*/},
    direct_len = /*...*/,
};
```

During initialization of the device assign the gpio configuration to a matrix
ID. The matrix ID will be sent along with events from the device to the dongle,
and the dongle will then use these matrix IDs to be able to correctly map
keypresses.

```c
void device_init() {
    keyscan_register_gpios(0, &matrix);

    // If the device has multiple configurations, register them all.
    keyscan_register_gpios(1, &matrix_direct);
}
```


## Dongle configuration

Define the mapping between the device and matrix configuration and the logical
layout.

```c

#define NOOP /*...*/
#define L(device, matrix, row, col) /*...*/

#define LD0(row, col) L(0, 0, row, col)
#define LD0M1(row, col) L(0, 1, row, col)
#define LD1(row, col) L(1, 0, row, col)

// In this example, half of the layout is handled by device 0, with two
// different matrixes, and the other half is handled by device 1, with one
// matrix.
//
// This could be the case of a split keyboard where one side has some of the
// switches wired directly (hence the two matrixes on a single device).
static const keymap_layout_t layout[N_ROWS][N_COLS] = {
    {LD0(0, 0), LD0M1(0, 1), LD1(0, 0), LD1(0, 1)},
    {LD0(1, 0), LD0M1(0, 2), LD1(1, 0), LD1(1, 1)},
    {LD0(2, 0), LD0M1(0, 3), LD1(2, 0), LD1(2, 1)},
    {NOOP     , NOOP       , LD1(3, 0), LD1(3, 1)},
};
```

Define the mapping between the logical layout and the actions to be performed
for the key on the different logical layers.

```c

static const keymap_keymap_t keymaps[N_LAYERS][N_ROWS][N_COLS] = {
    {
        {KC_TAB    , KC_Q      , KC_W      , KC_E      },
        {KC_ENTR   , KC_A      , KC_S      , KC_D      },
        {MOD_SHFT  , KC_Z      , KC_X      , KC_C      },
        {____      , ____      , LT(SPC, 1), KC_QUOT   },
    },
    {
        {____      , ____      , KC_UP     , ____      },
        {____      , KC_LEFT   , KC_DOWN   , KC_RGHT   },
        {____      , ____      , ____      , ____      },
        {____      , ____      , ____      , ____      },
    },
};
```

During dongle initialization, register the desired combination of layout and
keymaps. Multiple configurations can be registered, if for example the same
keymap is used across devices that are wired differently, or if multiple devices
with different keymaps need to be used with the same dongle.

```c
void dongle_init() {
    keymap_register_map(layout, keymaps);

    // `layout` and `layout2` are different physical devices with different
    // wiring that are mapped to the same `keymap`.
    keymap_register_map(layout2, keymaps);

    // `layout3` is a different physical device with different wiring and also
    // uses a different mapping than `layout` and `layout2`.
    keymap_register_map(layout3, keymaps3);
}
```
// DO NOT EDIT - autogenerated by generate.py

typedef enum {
    MYPAINT_BRUSH_INPUT_PRESSURE,
    MYPAINT_BRUSH_INPUT_SPEED1,
    MYPAINT_BRUSH_INPUT_SPEED2,
    MYPAINT_BRUSH_INPUT_RANDOM,
    MYPAINT_BRUSH_INPUT_STROKE,
    MYPAINT_BRUSH_INPUT_DIRECTION,
    MYPAINT_BRUSH_INPUT_DIRECTION_ANGLE,
    MYPAINT_BRUSH_INPUT_ATTACK_ANGLE,
    MYPAINT_BRUSH_INPUT_TILT_DECLINATION,
    MYPAINT_BRUSH_INPUT_TILT_ASCENSION,
    MYPAINT_BRUSH_INPUT_GRIDMAP_X,
    MYPAINT_BRUSH_INPUT_GRIDMAP_Y,
    MYPAINT_BRUSH_INPUT_BRUSH_RADIUS,
    MYPAINT_BRUSH_INPUT_CUSTOM,
    MYPAINT_BRUSH_INPUTS_COUNT
} MyPaintBrushInput;

typedef enum {
    MYPAINT_BRUSH_SETTING_OPAQUE,
    MYPAINT_BRUSH_SETTING_OPAQUE_MULTIPLY,
    MYPAINT_BRUSH_SETTING_OPAQUE_LINEARIZE,
    MYPAINT_BRUSH_SETTING_RADIUS_LOGARITHMIC,
    MYPAINT_BRUSH_SETTING_HARDNESS,
    MYPAINT_BRUSH_SETTING_SOFTNESS,
    MYPAINT_BRUSH_SETTING_ANTI_ALIASING,
    MYPAINT_BRUSH_SETTING_DABS_PER_BASIC_RADIUS,
    MYPAINT_BRUSH_SETTING_DABS_PER_ACTUAL_RADIUS,
    MYPAINT_BRUSH_SETTING_DABS_PER_SECOND,
    MYPAINT_BRUSH_SETTING_GRIDMAP_SCALE,
    MYPAINT_BRUSH_SETTING_GRIDMAP_SCALE_X,
    MYPAINT_BRUSH_SETTING_GRIDMAP_SCALE_Y,
    MYPAINT_BRUSH_SETTING_RADIUS_BY_RANDOM,
    MYPAINT_BRUSH_SETTING_SPEED1_SLOWNESS,
    MYPAINT_BRUSH_SETTING_SPEED2_SLOWNESS,
    MYPAINT_BRUSH_SETTING_SPEED1_GAMMA,
    MYPAINT_BRUSH_SETTING_SPEED2_GAMMA,
    MYPAINT_BRUSH_SETTING_OFFSET_BY_RANDOM,
    MYPAINT_BRUSH_SETTING_OFFSET_Y,
    MYPAINT_BRUSH_SETTING_OFFSET_X,
    MYPAINT_BRUSH_SETTING_OFFSET_ANGLE,
    MYPAINT_BRUSH_SETTING_OFFSET_ANGLE_ASC,
    MYPAINT_BRUSH_SETTING_OFFSET_ANGLE_2,
    MYPAINT_BRUSH_SETTING_OFFSET_ANGLE_2_ASC,
    MYPAINT_BRUSH_SETTING_OFFSET_ANGLE_ADJ,
    MYPAINT_BRUSH_SETTING_OFFSET_MULTIPLIER,
    MYPAINT_BRUSH_SETTING_OFFSET_BY_SPEED,
    MYPAINT_BRUSH_SETTING_OFFSET_BY_SPEED_SLOWNESS,
    MYPAINT_BRUSH_SETTING_SLOW_TRACKING,
    MYPAINT_BRUSH_SETTING_SLOW_TRACKING_PER_DAB,
    MYPAINT_BRUSH_SETTING_TRACKING_NOISE,
    MYPAINT_BRUSH_SETTING_COLOR_H,
    MYPAINT_BRUSH_SETTING_COLOR_S,
    MYPAINT_BRUSH_SETTING_COLOR_V,
    MYPAINT_BRUSH_SETTING_RESTORE_COLOR,
    MYPAINT_BRUSH_SETTING_CHANGE_COLOR_H,
    MYPAINT_BRUSH_SETTING_CHANGE_COLOR_L,
    MYPAINT_BRUSH_SETTING_CHANGE_COLOR_HSL_S,
    MYPAINT_BRUSH_SETTING_CHANGE_COLOR_V,
    MYPAINT_BRUSH_SETTING_CHANGE_COLOR_HSV_S,
    MYPAINT_BRUSH_SETTING_SMUDGE,
    MYPAINT_BRUSH_SETTING_SMUDGE_LENGTH,
    MYPAINT_BRUSH_SETTING_SMUDGE_RADIUS_LOG,
    MYPAINT_BRUSH_SETTING_ERASER,
    MYPAINT_BRUSH_SETTING_STROKE_THRESHOLD,
    MYPAINT_BRUSH_SETTING_STROKE_DURATION_LOGARITHMIC,
    MYPAINT_BRUSH_SETTING_STROKE_HOLDTIME,
    MYPAINT_BRUSH_SETTING_CUSTOM_INPUT,
    MYPAINT_BRUSH_SETTING_CUSTOM_INPUT_SLOWNESS,
    MYPAINT_BRUSH_SETTING_ELLIPTICAL_DAB_RATIO,
    MYPAINT_BRUSH_SETTING_ELLIPTICAL_DAB_ANGLE,
    MYPAINT_BRUSH_SETTING_DIRECTION_FILTER,
    MYPAINT_BRUSH_SETTING_LOCK_ALPHA,
    MYPAINT_BRUSH_SETTING_COLORIZE,
    MYPAINT_BRUSH_SETTING_SNAP_TO_PIXEL,
    MYPAINT_BRUSH_SETTING_PRESSURE_GAIN_LOG,
    MYPAINT_BRUSH_SETTINGS_COUNT
} MyPaintBrushSetting;

typedef enum {
    MYPAINT_BRUSH_STATE_X,
    MYPAINT_BRUSH_STATE_Y,
    MYPAINT_BRUSH_STATE_PRESSURE,
    MYPAINT_BRUSH_STATE_PARTIAL_DABS,
    MYPAINT_BRUSH_STATE_ACTUAL_RADIUS,
    MYPAINT_BRUSH_STATE_SMUDGE_RA,
    MYPAINT_BRUSH_STATE_SMUDGE_GA,
    MYPAINT_BRUSH_STATE_SMUDGE_BA,
    MYPAINT_BRUSH_STATE_SMUDGE_A,
    MYPAINT_BRUSH_STATE_LAST_GETCOLOR_R,
    MYPAINT_BRUSH_STATE_LAST_GETCOLOR_G,
    MYPAINT_BRUSH_STATE_LAST_GETCOLOR_B,
    MYPAINT_BRUSH_STATE_LAST_GETCOLOR_A,
    MYPAINT_BRUSH_STATE_LAST_GETCOLOR_RECENTNESS,
    MYPAINT_BRUSH_STATE_ACTUAL_X,
    MYPAINT_BRUSH_STATE_ACTUAL_Y,
    MYPAINT_BRUSH_STATE_NORM_DX_SLOW,
    MYPAINT_BRUSH_STATE_NORM_DY_SLOW,
    MYPAINT_BRUSH_STATE_NORM_SPEED1_SLOW,
    MYPAINT_BRUSH_STATE_NORM_SPEED2_SLOW,
    MYPAINT_BRUSH_STATE_STROKE,
    MYPAINT_BRUSH_STATE_STROKE_STARTED,
    MYPAINT_BRUSH_STATE_CUSTOM_INPUT,
    MYPAINT_BRUSH_STATE_RNG_SEED,
    MYPAINT_BRUSH_STATE_ACTUAL_ELLIPTICAL_DAB_RATIO,
    MYPAINT_BRUSH_STATE_ACTUAL_ELLIPTICAL_DAB_ANGLE,
    MYPAINT_BRUSH_STATE_DIRECTION_DX,
    MYPAINT_BRUSH_STATE_DIRECTION_DY,
    MYPAINT_BRUSH_STATE_DECLINATION,
    MYPAINT_BRUSH_STATE_ASCENSION,
    MYPAINT_BRUSH_STATE_DIRECTION_ANGLE_DX,
    MYPAINT_BRUSH_STATE_DIRECTION_ANGLE_DY,
    MYPAINT_BRUSH_STATE_ATTACK_ANGLE,
    MYPAINT_BRUSH_STATE_FLIP,
    MYPAINT_BRUSH_STATE_GRIDMAP_X,
    MYPAINT_BRUSH_STATE_GRIDMAP_Y,
    MYPAINT_BRUSH_STATES_COUNT
} MyPaintBrushState;


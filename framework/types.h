#ifndef TYPES_H
#define TYPES_H

/* SECOND 0-59 (111011) - 6 bits - << 0 */
/* MINUTE 0-59 (111011) - 6 bits - << 6 */
/* HOUR   0-23  (10111) - 5 bits - << 12 */
/* DAY    0-31  (11111) - 5 bits - << 17 */
/* MONTH  0-12   (1100) - 4 bits - << 22 */
/* YEAR   0-63 (111111) - 6 bits - << 26 */

/* If year starts on 1970, limit is 12/31/2033 23:59:59 */

#define YEAR_BASE 1970ul

#define SIX_BIT_MASK  0x3f
#define FIVE_BIT_MASK 0x1f
#define FOUR_BIT_MASK 0xf

#define SECOND_SHIFT 0
#define MINUTE_SHIFT 6
#define HOUR_SHIFT   12
#define DAY_SHIFT    17
#define MONTH_SHIFT  22
#define YEAR_SHIFT   26

#define SECOND_MASK (SIX_BIT_MASK  << SECOND_SHIFT)
#define MINUTE_MASK (SIX_BIT_MASK  << MINUTE_SHIFT)
#define HOUR_MASK   (FIVE_BIT_MASK << HOUR_SHIFT)
#define DAY_MASK    (FIVE_BIT_MASK << DAY_SHIFT)
#define MONTH_MASK  (FOUR_BIT_MASK << MONTH_SHIFT)
#define YEAR_MASK   (SIX_BIT_MASK  << YEAR_SHIFT)

/* Our basic types */

typedef unsigned long time_info_t;
typedef unsigned long granularity_t;

#define GRANULARITY_SECOND 0xffffffff /* FIXME : Use macros */
#define GRANULARITY_MINUTE 0xffffffc0
#define GRANULARITY_HOUR   0xfffff000
#define GRANULARITY_DAY    0xfffe0000
#define GRANULARITY_MONTH  0xffc00000
#define GRANULARITY_YEAR   0xfc000000

#define TIME_SET_SECOND(t, s) \
  t = ((t & ~SECOND_MASK) | ((SIX_BIT_MASK & s) << SECOND_SHIFT)
#define TIME_SET_MINUTE(t, m) \
  t = ((t & ~MINUTE_MASK) | ((SIX_BIT_MASK & m) << MINUTE_SHIFT))
#define TIME_SET_HOUR(t, h) \
  t = ((t & ~HOUR_MASK) | ((FIVE_BIT_MASK & h) << HOUR_SHIFT))
#define TIME_SET_DAY(t, d) \
  t = ((t & ~DAY_MASK) | ((FIVE_BIT_MASK & d) << DAY_SHIFT))
#define TIME_SET_MONTH(t, m) \
  t = ((t & ~MONTH_MASK) | ((FOUR_BIT_MASK & m) << MONTH_SHIFT))
#define TIME_SET_YEAR(t, y) \
  t = ((t & ~YEAR_MASK) | ((SIX_BIT_MASK & (y - YEAR_BASE)) << YEAR_SHIFT))

#define TIME_GET_SECOND(t)((t & SECOND_MASK) >> SECOND_SHIFT)
#define TIME_GET_MINUTE(t) ((t & MINUTE_MASK) >> MINUTE_SHIFT)
#define TIME_GET_HOUR(t) ((t & HOUR_MASK) >> HOUR_SHIFT)
#define TIME_GET_DAY(t) ((t & DAY_MASK) >> DAY_SHIFT)
#define TIME_GET_MONTH(t) ((t & MONTH_MASK) >> MONTH_SHIFT)
#define TIME_GET_YEAR(t) (((t & YEAR_MASK) >> YEAR_SHIFT) + YEAR_BASE)

#define TIMECMP(t1, t2, g) (int)((t1 & g) - (t2 & g))

#endif

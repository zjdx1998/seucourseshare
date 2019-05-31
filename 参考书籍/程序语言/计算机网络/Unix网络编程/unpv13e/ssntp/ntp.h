#define	JAN_1970	2208988800UL	/* 1970 - 1900 in seconds */

struct l_fixedpt {		/* 64-bit fixed-point */
  uint32_t	int_part;
  uint32_t	fraction;
};

struct s_fixedpt {		/* 32-bit fixed-point */
  uint16_t	int_part;
  uint16_t	fraction;
};

struct ntpdata {		/* NTP header */
  u_char			status;
  u_char			stratum;
  u_char			ppoll;
  int				precision:8;
  struct s_fixedpt	distance;
  struct s_fixedpt	dispersion;
  uint32_t			refid;
  struct l_fixedpt	reftime;
  struct l_fixedpt	org;
  struct l_fixedpt	rec;
  struct l_fixedpt	xmt;
};

#define	VERSION_MASK	0x38
#define	MODE_MASK		0x07

#define	MODE_CLIENT		3
#define	MODE_SERVER		4
#define	MODE_BROADCAST	5

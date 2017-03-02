#pragma once
//#define FLASH_ENABLE        0xef2c
//#define FLASH_DATA          0xef30

#define SW_TRIGER                0x8108
#define SW_RESET                 0xef24
#define SW_CLEAR                 0xef28

#define BUFFER_ORG               0x800c
#define BUFFER_FREE              0x8010
#define CUSTOM_SIZE              0x8020
#define ACQ_CONTRL               0x8100
#define ACQ_STATUS               0x8104
#define VME_STATUS               0xef04
#define BOARD_INFO               0x8140
#define EVT_STORED               0x812c
#define EVT_SIZE                 0x814c
#define BOARD_ID                 0xef08
#define CONFIG_BOARD_VERS        0xf030
#define CONFIG_BOARD_VERS2       0xf034
#define FIRMWARE_REV             0x8124

#define CHANNEL_CONF             0x8000
#define CHANNEL_CONF_SET         0x8004
#define CHANNEL_CONF_CLEAR       0x8008
#define TRG_SRC_ENABLE_MASK      0x810c
#define CHANNEL_ENABLE_MASK      0x8120
#define VME_CONTRL               0xef00
#define CHANNEL_DAC              0x1098
#define CHANNEL_DAC_ALL          0x8098
#define CHANNEL_THRE             0x1080
#define CHANNEL_THRE_ALL         0x8080
#define CHANNEL_PRETRG           0x1038
#define CHANNEL_PRETRG_ALL       0x8038
#define CHANNEL_NEV_AGG          0x1034
#define CHANNEL_NEV_AGG_ALL      0x8034
#define CHANNEL_OVUD_THRE        0x1084
#define CHANNEL_OVUD_THRE_ALL    0x8084
#define CHANNEL_n_STATUS(n)      0x1088+(n)*0x0100
#define CHANNEL_n_DAC(n)         0x1098+(n)*0x0100
#define CHANNEL_n_THRE(n)        0x1080+(n)*0x0100
#define CHANNEL_n_PRETRG(n)      0x1038+(n)*0x0100
#define CHANNEL_n_NEV_AGG(n)     0x1034+(n)*0x0100

#define MCST_BASE_CONTRL         0xef0c
#define RELOC_ADDR               0xef10
#define IRQ_STATUS_ID            0xef14
#define IRQ_EVT_NUM              0xef18
#define BLT_EVT_NUM              0xef1c

#define DPP_MISC                 0x1080
#define DPP_MISC_ALL             0x8080
#define DPP_TRG_FILT_SMOOTH      0x1054
#define DPP_TRG_FILT_SMOOTH_ALL  0x8054
#define DPP_INPUT_RISETIME       0x1058
#define DPP_INPUT_RISETIME_ALL   0x8058
#define DPP_TRPZ_RISETIME        0x105c
#define DPP_TRPZ_RISETIME_ALL    0x805c
#define DPP_TRPZ_FLATTOP         0x1060
#define DPP_TRPZ_FLATTOP_ALL     0x8060
#define DPP_PEAKINGTIME          0x1064
#define DPP_PEAKINGTIME_ALL      0x8064
#define DPP_DECAYTIME            0x1068
#define DPP_DECAYTIME_ALL        0x8068
#define DPP_TRG_THRE             0x106c
#define DPP_TRG_THRE_ALL         0x806c
#define DPP_RTD_WIN_WID          0x1070
#define DPP_RTD_WIN_WID_ALL      0x8070
#define DPP_TRG_HOLDOFF          0x1074
#define DPP_TRG_HOLDOFF_ALL      0x8074
#define DPP_PEAK_HOLDOFF         0x1078
#define DPP_PEAK_HOLDOFF_ALL     0x8078
#define DPP_BASELINE_HOLDOFF     0x107c
#define DPP_BASELINE_HOLDOFF_ALL 0x807c
#define DPP_n_MISC(n)            0x1080+0x100*n

#define WIN_GATE                 0x00000000
#define SINGLE_GATE              0x00000001
#define TRG_OVERLAP              0x00000002
#define TEST_PATTERN             0x00000008
#define RONDAM_ACCESS            0x00000000
#define SEQENTIAL_ACCESS         0x00000010
#define OVER_THRE                0x00000000
#define UNDER_THRE               0x00000040
#define ANALOG_MONITOR           0x000000f0
#define FULL_ZEROSUPP            0x00010000
#define ZERO_LEN_ENC             0x00020000
#define FULL_ZS_AMP              0x00030000
#define STARNDARD_MASK           0x000300db
#define DPP_PHA_MASK             0x01f7fb14
#define DPP_PHA_RESERVE          0x00000010
#define DPP_PHA_BITSET(a)        (((a)&DPP_PHA_MASK)|DPP_PHA_RESERVE)
#define DPP_GPIO_TRG             0x00000004
#define DPP_INV_INPUT            0x00000200
#define DPP_DUAL_TRACE           0x00000800
#define DPP_TRACE1_INPUT         0x00000000
#define DPP_TRACE1_DELTA         0x00001000
#define DPP_TRACE1_DELTA2        0x00002000
#define DPP_TRACE1_TRPZ          0x00003000
#define DPP_TRACE2_INPUT         0x00000000
#define DPP_TRACE2_THRTRG        0x00004000
#define DPP_TRACE2_TRPZ          0x00008000
#define DPP_TRACE2_BASELINE      0x0000c000
#define DPP_OSCILOSCOPE_MODE     0x00010000
#define DPP_ENERGY_MODE          0x00020000
#define DPP_TIMETAG_MODE         0x00040000
#define DPP_ENG_TIME_MODE        (DPP_ENERGY_MODE|DPP_TIMETAG_MODE)
#define DPP_ALL_MODE             (DPP_ENG_TIME_MODE|DPP_OSCILOSCOPE_MODE)
#define DPP_VIRPROBE_RTD         0x00000000
#define DPP_VIRPROBE_ARMED       0x00100000
#define DPP_VIRPROBE_PEAKSEARCH  0x00200000
#define DPP_VIRPROBE_PUR         0x00300000
#define DPP_VIRPROBE_PEAKPOS     0x00400000
#define DPP_VIRPROBE_TRIGER      0x00500000
#define DPP_VIRPROBE_BASEOFF     0x00600000
#define DPP_VIRPROBE_TRG_HOLD    0x00700000
#define DPP_VIRPROBE_TRG_VALID   0x00800000
#define DPP_VIRPROBE_ACQ_VETO    0x00900000
#define DPP_ENABLE_EVT_DATAWORD  0x01000000

/**
 * Furnace Tracker - multi-system chiptune tracker
 * Copyright (C) 2021-2022 tildearrow and contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _INSTRUMENT_H
#define _INSTRUMENT_H
#include "safeWriter.h"
#include "dataErrors.h"
#include "../ta-utils.h"

// NOTICE!
// before adding new instrument types to this struct, please ask me first.
// absolutely zero support granted to conflicting formats.
enum DivInstrumentType {
  DIV_INS_STD=0,
  DIV_INS_FM=1,
  DIV_INS_GB=2,
  DIV_INS_C64=3,
  DIV_INS_AMIGA=4,
  DIV_INS_PCE=5,
  DIV_INS_AY=6,
  DIV_INS_AY8930=7,
  DIV_INS_TIA=8,
  DIV_INS_SAA1099=9,
  DIV_INS_VIC=10,
  DIV_INS_PET=11,
  DIV_INS_VRC6=12,
  DIV_INS_OPLL=13,
  DIV_INS_OPL=14,
  DIV_INS_FDS=15,
  DIV_INS_VBOY=16,
  DIV_INS_N163=17,
  DIV_INS_SCC=18,
  DIV_INS_OPZ=19,
  DIV_INS_POKEY=20,
  DIV_INS_BEEPER=21,
  DIV_INS_SWAN=22,
  DIV_INS_MIKEY=23,
  DIV_INS_VERA=24,
  DIV_INS_X1_010=25,
  DIV_INS_VRC6_SAW=26,
  DIV_INS_MAX,
};

// FM operator structure:
// - OPN:
//   - AM, AR, DR, MULT, RR, SL, TL, RS, DT, D2R, SSG-EG
// - OPM:
//   - AM, AR, DR, MULT, RR, SL, TL, DT2, RS, DT, D2R
// - OPLL:
//   - AM, AR, DR, MULT, RR, SL, TL, SSG-EG&8 = EG-S
//   - KSL, VIB, KSR
// - OPL:
//   - AM, AR, DR, MULT, RR, SL, TL, SSG-EG&8 = EG-S
//   - KSL, VIB, WS (OPL2/3), KSR
// - OPZ:
//   - AM, AR, DR, MULT (CRS), RR, SL, TL, DT2, RS, DT, D2R
//   - WS, DVB = MULT (FINE), DAM = REV, KSL = EGShift, EGT = Fixed

struct DivInstrumentFM {
  unsigned char alg, fb, fms, ams, ops, opllPreset;
  bool fixedDrums;
  unsigned short kickFreq, snareHatFreq, tomTopFreq;
  struct Operator {
    unsigned char am, ar, dr, mult, rr, sl, tl, dt2, rs, dt, d2r, ssgEnv;
    unsigned char dam, dvb, egt, ksl, sus, vib, ws, ksr; // YMU759/OPL/OPZ
    Operator():
      am(0),
      ar(0),
      dr(0),
      mult(0),
      rr(0),
      sl(0),
      tl(0),
      dt2(0),
      rs(0),
      dt(0),
      d2r(0),
      ssgEnv(0),
      dam(0),
      dvb(0),
      egt(0),
      ksl(0),
      sus(0),
      vib(0),
      ws(0),
      ksr(0) {}
  } op[4];
  DivInstrumentFM():
    alg(0),
    fb(0),
    fms(0),
    ams(0),
    ops(2),
    opllPreset(0),
    fixedDrums(false),
    kickFreq(0x520),
    snareHatFreq(0x550),
    tomTopFreq(0x1c0) {
    // default instrument
    fb=4;
    op[0].tl=42;
    op[0].ar=31;
    op[0].dr=8;
    op[0].sl=15;
    op[0].rr=3;
    op[0].mult=5;
    op[0].dt=5;

    op[2].tl=18;
    op[2].ar=31;
    op[2].dr=10;
    op[2].sl=15;
    op[2].rr=4;
    op[2].mult=1;
    op[2].dt=0;

    op[1].tl=48;
    op[1].ar=31;
    op[1].dr=4;
    op[1].sl=11;
    op[1].rr=1;
    op[1].mult=1;
    op[1].dt=5;

    op[3].tl=2;
    op[3].ar=31;
    op[3].dr=9;
    op[3].sl=15;
    op[3].rr=9;
    op[3].mult=1;
    op[3].dt=0;
  }
};

// this is getting out of hand
struct DivInstrumentSTD {
  int volMacro[256];
  int arpMacro[256];
  int dutyMacro[256];
  int waveMacro[256];
  int pitchMacro[256];
  int ex1Macro[256];
  int ex2Macro[256];
  int ex3Macro[256];
  int algMacro[256];
  int fbMacro[256];
  int fmsMacro[256];
  int amsMacro[256];
  int panLMacro[256];
  int panRMacro[256];
  int phaseResetMacro[256];
  int ex4Macro[256];
  int ex5Macro[256];
  int ex6Macro[256];
  int ex7Macro[256];
  int ex8Macro[256];

  bool arpMacroMode;
  unsigned char volMacroHeight, dutyMacroHeight, waveMacroHeight;

  bool volMacroOpen, arpMacroOpen, dutyMacroOpen, waveMacroOpen;
  bool pitchMacroOpen, ex1MacroOpen, ex2MacroOpen, ex3MacroOpen;
  bool algMacroOpen, fbMacroOpen, fmsMacroOpen, amsMacroOpen;
  bool panLMacroOpen, panRMacroOpen, phaseResetMacroOpen, ex4MacroOpen;
  bool ex5MacroOpen, ex6MacroOpen, ex7MacroOpen, ex8MacroOpen;

  unsigned char volMacroLen, arpMacroLen, dutyMacroLen, waveMacroLen;
  unsigned char pitchMacroLen, ex1MacroLen, ex2MacroLen, ex3MacroLen;
  unsigned char algMacroLen, fbMacroLen, fmsMacroLen, amsMacroLen;
  unsigned char panLMacroLen, panRMacroLen, phaseResetMacroLen, ex4MacroLen;
  unsigned char ex5MacroLen, ex6MacroLen, ex7MacroLen, ex8MacroLen;

  signed char volMacroLoop, arpMacroLoop, dutyMacroLoop, waveMacroLoop;
  signed char pitchMacroLoop, ex1MacroLoop, ex2MacroLoop, ex3MacroLoop;
  signed char algMacroLoop, fbMacroLoop, fmsMacroLoop, amsMacroLoop;
  signed char panLMacroLoop, panRMacroLoop, phaseResetMacroLoop, ex4MacroLoop;
  signed char ex5MacroLoop, ex6MacroLoop, ex7MacroLoop, ex8MacroLoop;

  signed char volMacroRel, arpMacroRel, dutyMacroRel, waveMacroRel;
  signed char pitchMacroRel, ex1MacroRel, ex2MacroRel, ex3MacroRel;
  signed char algMacroRel, fbMacroRel, fmsMacroRel, amsMacroRel;
  signed char panLMacroRel, panRMacroRel, phaseResetMacroRel, ex4MacroRel;
  signed char ex5MacroRel, ex6MacroRel, ex7MacroRel, ex8MacroRel;
  struct OpMacro {
    // ar, dr, mult, rr, sl, tl, dt2, rs, dt, d2r, ssgEnv;
    unsigned char amMacro[256];
    unsigned char arMacro[256];
    unsigned char drMacro[256];
    unsigned char multMacro[256];
    unsigned char rrMacro[256];
    unsigned char slMacro[256];
    unsigned char tlMacro[256];
    unsigned char dt2Macro[256];
    unsigned char rsMacro[256];
    unsigned char dtMacro[256];
    unsigned char d2rMacro[256];
    unsigned char ssgMacro[256];
    unsigned char damMacro[256];
    unsigned char dvbMacro[256];
    unsigned char egtMacro[256];
    unsigned char kslMacro[256];
    unsigned char susMacro[256];
    unsigned char vibMacro[256];
    unsigned char wsMacro[256];
    unsigned char ksrMacro[256];
    bool amMacroOpen, arMacroOpen, drMacroOpen, multMacroOpen;
    bool rrMacroOpen, slMacroOpen, tlMacroOpen, dt2MacroOpen;
    bool rsMacroOpen, dtMacroOpen, d2rMacroOpen, ssgMacroOpen;
    bool damMacroOpen, dvbMacroOpen, egtMacroOpen, kslMacroOpen;
    bool susMacroOpen, vibMacroOpen, wsMacroOpen, ksrMacroOpen;
    unsigned char amMacroLen, arMacroLen, drMacroLen, multMacroLen;
    unsigned char rrMacroLen, slMacroLen, tlMacroLen, dt2MacroLen;
    unsigned char rsMacroLen, dtMacroLen, d2rMacroLen, ssgMacroLen;
    unsigned char damMacroLen, dvbMacroLen, egtMacroLen, kslMacroLen;
    unsigned char susMacroLen, vibMacroLen, wsMacroLen, ksrMacroLen;
    signed char amMacroLoop, arMacroLoop, drMacroLoop, multMacroLoop;
    signed char rrMacroLoop, slMacroLoop, tlMacroLoop, dt2MacroLoop;
    signed char rsMacroLoop, dtMacroLoop, d2rMacroLoop, ssgMacroLoop;
    signed char damMacroLoop, dvbMacroLoop, egtMacroLoop, kslMacroLoop;
    signed char susMacroLoop, vibMacroLoop, wsMacroLoop, ksrMacroLoop;
    signed char amMacroRel, arMacroRel, drMacroRel, multMacroRel;
    signed char rrMacroRel, slMacroRel, tlMacroRel, dt2MacroRel;
    signed char rsMacroRel, dtMacroRel, d2rMacroRel, ssgMacroRel;
    signed char damMacroRel, dvbMacroRel, egtMacroRel, kslMacroRel;
    signed char susMacroRel, vibMacroRel, wsMacroRel, ksrMacroRel;
    OpMacro():
      amMacroOpen(false), arMacroOpen(false), drMacroOpen(false), multMacroOpen(false),
      rrMacroOpen(false), slMacroOpen(false), tlMacroOpen(true), dt2MacroOpen(false),
      rsMacroOpen(false), dtMacroOpen(false), d2rMacroOpen(false), ssgMacroOpen(false),
      damMacroOpen(false), dvbMacroOpen(false), egtMacroOpen(false), kslMacroOpen(false),
      susMacroOpen(false), vibMacroOpen(false), wsMacroOpen(false), ksrMacroOpen(false),
      amMacroLen(0), arMacroLen(0), drMacroLen(0), multMacroLen(0),
      rrMacroLen(0), slMacroLen(0), tlMacroLen(0), dt2MacroLen(0),
      rsMacroLen(0), dtMacroLen(0), d2rMacroLen(0), ssgMacroLen(0),
      damMacroLen(0), dvbMacroLen(0), egtMacroLen(0), kslMacroLen(0),
      susMacroLen(0), vibMacroLen(0), wsMacroLen(0), ksrMacroLen(0),
      amMacroLoop(-1), arMacroLoop(-1), drMacroLoop(-1), multMacroLoop(-1),
      rrMacroLoop(-1), slMacroLoop(-1), tlMacroLoop(-1), dt2MacroLoop(-1),
      rsMacroLoop(-1), dtMacroLoop(-1), d2rMacroLoop(-1), ssgMacroLoop(-1),
      damMacroLoop(-1), dvbMacroLoop(-1), egtMacroLoop(-1), kslMacroLoop(-1),
      susMacroLoop(-1), vibMacroLoop(-1), wsMacroLoop(-1), ksrMacroLoop(-1),
      amMacroRel(-1), arMacroRel(-1), drMacroRel(-1), multMacroRel(-1),
      rrMacroRel(-1), slMacroRel(-1), tlMacroRel(-1), dt2MacroRel(-1),
      rsMacroRel(-1), dtMacroRel(-1), d2rMacroRel(-1), ssgMacroRel(-1),
      damMacroRel(-1), dvbMacroRel(-1), egtMacroRel(-1), kslMacroRel(-1),
      susMacroRel(-1), vibMacroRel(-1), wsMacroRel(-1), ksrMacroRel(-1) {
        memset(amMacro,0,256);
        memset(arMacro,0,256);
        memset(drMacro,0,256);
        memset(multMacro,0,256);
        memset(rrMacro,0,256);
        memset(slMacro,0,256);
        memset(tlMacro,0,256);
        memset(dt2Macro,0,256);
        memset(rsMacro,0,256);
        memset(dtMacro,0,256);
        memset(d2rMacro,0,256);
        memset(ssgMacro,0,256);
        memset(damMacro,0,256);
        memset(dvbMacro,0,256);
        memset(egtMacro,0,256);
        memset(kslMacro,0,256);
        memset(susMacro,0,256);
        memset(vibMacro,0,256);
        memset(wsMacro,0,256);
        memset(ksrMacro,0,256);
      }
  } opMacros[4];
  DivInstrumentSTD():
    arpMacroMode(false),
    volMacroHeight(15),
    dutyMacroHeight(3),
    waveMacroHeight(63),
    volMacroOpen(true),
    arpMacroOpen(false),
    dutyMacroOpen(false),
    waveMacroOpen(false),
    pitchMacroOpen(false),
    ex1MacroOpen(false),
    ex2MacroOpen(false),
    ex3MacroOpen(false),
    algMacroOpen(false),
    fbMacroOpen(false),
    fmsMacroOpen(false),
    amsMacroOpen(false),
    panLMacroOpen(false),
    panRMacroOpen(false),
    phaseResetMacroOpen(false),
    ex4MacroOpen(false),
    ex5MacroOpen(false),
    ex6MacroOpen(false), 
    ex7MacroOpen(false),
    ex8MacroOpen(false),

    volMacroLen(0),
    arpMacroLen(0),
    dutyMacroLen(0),
    waveMacroLen(0),
    pitchMacroLen(0),
    ex1MacroLen(0),
    ex2MacroLen(0),
    ex3MacroLen(0),
    algMacroLen(0),
    fbMacroLen(0),
    fmsMacroLen(0),
    amsMacroLen(0),
    panLMacroLen(0),
    panRMacroLen(0),
    phaseResetMacroLen(0),
    ex4MacroLen(0),
    ex5MacroLen(0),
    ex6MacroLen(0), 
    ex7MacroLen(0),
    ex8MacroLen(0),

    volMacroLoop(-1),
    arpMacroLoop(-1),
    dutyMacroLoop(-1),
    waveMacroLoop(-1),
    pitchMacroLoop(-1),
    ex1MacroLoop(-1),
    ex2MacroLoop(-1),
    ex3MacroLoop(-1),
    algMacroLoop(-1),
    fbMacroLoop(-1),
    fmsMacroLoop(-1),
    amsMacroLoop(-1),
    panLMacroLoop(-1),
    panRMacroLoop(-1),
    phaseResetMacroLoop(-1),
    ex4MacroLoop(-1),
    ex5MacroLoop(-1),
    ex6MacroLoop(-1), 
    ex7MacroLoop(-1),
    ex8MacroLoop(-1),

    volMacroRel(-1),
    arpMacroRel(-1),
    dutyMacroRel(-1),
    waveMacroRel(-1),
    pitchMacroRel(-1),
    ex1MacroRel(-1),
    ex2MacroRel(-1),
    ex3MacroRel(-1),
    algMacroRel(-1),
    fbMacroRel(-1),
    fmsMacroRel(-1),
    amsMacroRel(-1),
    panLMacroRel(-1),
    panRMacroRel(-1),
    phaseResetMacroRel(-1),
    ex4MacroRel(-1),
    ex5MacroRel(-1),
    ex6MacroRel(-1), 
    ex7MacroRel(-1),
    ex8MacroRel(-1) {
      memset(volMacro,0,256*sizeof(int));
      memset(arpMacro,0,256*sizeof(int));
      memset(dutyMacro,0,256*sizeof(int));
      memset(waveMacro,0,256*sizeof(int));
      memset(pitchMacro,0,256*sizeof(int));
      memset(ex1Macro,0,256*sizeof(int));
      memset(ex2Macro,0,256*sizeof(int));
      memset(ex3Macro,0,256*sizeof(int));
      memset(algMacro,0,256*sizeof(int));
      memset(fbMacro,0,256*sizeof(int));
      memset(fmsMacro,0,256*sizeof(int));
      memset(amsMacro,0,256*sizeof(int));
      memset(panLMacro,0,256*sizeof(int));
      memset(panRMacro,0,256*sizeof(int));
      memset(phaseResetMacro,0,256*sizeof(int));
      memset(ex4Macro,0,256*sizeof(int));
      memset(ex5Macro,0,256*sizeof(int));
      memset(ex6Macro,0,256*sizeof(int)); 
      memset(ex7Macro,0,256*sizeof(int));
      memset(ex8Macro,0,256*sizeof(int));
    }
};

struct DivInstrumentGB {
  unsigned char envVol, envDir, envLen, soundLen;
  DivInstrumentGB():
    envVol(15),
    envDir(0),
    envLen(2),
    soundLen(64) {}
};

struct DivInstrumentC64 {
  bool triOn, sawOn, pulseOn, noiseOn;
  unsigned char a, d, s, r;
  unsigned short duty;
  unsigned char ringMod, oscSync;
  bool toFilter, volIsCutoff, initFilter, dutyIsAbs, filterIsAbs;
  unsigned char res;
  unsigned short cut;
  bool hp, lp, bp, ch3off;

  DivInstrumentC64():
    triOn(false),
    sawOn(true),
    pulseOn(false),
    noiseOn(false),
    a(0),
    d(8),
    s(0),
    r(0),
    duty(2048),
    ringMod(0),
    oscSync(0),
    toFilter(false),
    volIsCutoff(false),
    initFilter(false),
    dutyIsAbs(false),
    filterIsAbs(false),
    res(0),
    cut(0),
    hp(false),
    lp(false),
    bp(false),
    ch3off(false) {}
};

struct DivInstrumentAmiga {
  short initSample;
  bool useNoteMap;
  int noteFreq[120];
  short noteMap[120];

  DivInstrumentAmiga():
    initSample(0),
    useNoteMap(false) {
    memset(noteMap,-1,120*sizeof(short));
    memset(noteFreq,0,120*sizeof(int));
  }
};

struct DivInstrumentN163 {
  int wave, wavePos, waveLen;
  unsigned char waveMode;

  DivInstrumentN163():
    wave(-1),
    wavePos(0),
    waveLen(32),
    waveMode(3) {}
};

struct DivInstrumentFDS {
  signed char modTable[32];
  int modSpeed, modDepth;
  // this is here for compatibility.
  bool initModTableWithFirstWave;
  DivInstrumentFDS():
    modSpeed(0),
    modDepth(0),
    initModTableWithFirstWave(false) {
    memset(modTable,0,32);
  }
};

struct DivInstrument {
  String name;
  bool mode;
  DivInstrumentType type;
  DivInstrumentFM fm;
  DivInstrumentSTD std;
  DivInstrumentGB gb;
  DivInstrumentC64 c64;
  DivInstrumentAmiga amiga;
  DivInstrumentN163 n163;
  DivInstrumentFDS fds;
  
  /**
   * save the instrument to a SafeWriter.
   * @param w the SafeWriter in question.
   */
  void putInsData(SafeWriter* w);

  /**
   * read instrument data in .fui format.
   * @param reader the reader.
   * @param version the format version.
   * @return a DivDataErrors.
   */
  DivDataErrors readInsData(SafeReader& reader, short version);

  /**
   * save this instrument to a file.
   * @param path file path.
   * @return whether it was successful.
   */
  bool save(const char* path);
  DivInstrument():
    name(""),
    mode(false),
    type(DIV_INS_FM) {
  }
};
#endif

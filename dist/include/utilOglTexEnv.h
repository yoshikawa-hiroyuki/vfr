/*
 * CES Utilities
 *
 * Copyright(c) FUJITSU NAGANO SYSTEMS ENGINEERING LIMITED
 *      CES Project, 2002-2004, All Right Reserved.
 */
#ifndef _CES_UTIL_OGL_TEXENV_H_
#define _CES_UTIL_OGL_TEXENV_H_

#if defined(_WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#else // WIN32
#define GL_GLEXT_PROTOTYPES 1
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif // WIN32

#ifndef GL_ARB_imaging
#define GL_CONSTANT_COLOR                   0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR         0x8002
#define GL_CONSTANT_ALPHA                   0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA         0x8004
#define GL_BLEND_COLOR                      0x8005
#define GL_FUNC_ADD                         0x8006
#define GL_MIN                              0x8007
#define GL_MAX                              0x8008
#define GL_BLEND_EQUATION                   0x8009
#define GL_FUNC_SUBTRACT                    0x800A
#define GL_FUNC_REVERSE_SUBTRACT            0x800B
#endif // !GL_ARB_imaging

#ifndef GL_EXT_texture_env_combine 
// Texture Rnv Combine tokens
#define GL_COMBINE_EXT                      0x8570
#define GL_COMBINE_RGB_EXT                  0x8571
#define GL_COMBINE_ALPHA_EXT                0x8572
#define GL_SOURCE0_RGB_EXT                  0x8580
#define GL_SOURCE1_RGB_EXT                  0x8581
#define GL_SOURCE2_RGB_EXT                  0x8582
#define GL_SOURCE0_ALPHA_EXT                0x8588
#define GL_SOURCE1_ALPHA_EXT                0x8589
#define GL_SOURCE2_ALPHA_EXT                0x858A
#define GL_OPERAND0_RGB_EXT                 0x8590
#define GL_OPERAND1_RGB_EXT                 0x8591
#define GL_OPERAND2_RGB_EXT                 0x8592
#define GL_OPERAND0_ALPHA_EXT               0x8598
#define GL_OPERAND1_ALPHA_EXT               0x8599
#define GL_OPERAND2_ALPHA_EXT               0x859A
#define GL_RGB_SCALE_EXT                    0x8573
#define GL_ADD_SIGNED_EXT                   0x8574
#define GL_INTERPOLATE_EXT                  0x8575
#define GL_CONSTANT_EXT                     0x8576
#define GL_PRIMARY_COLOR_EXT                0x8577
#define GL_PREVIOUS_EXT                     0x8578
#endif // !GL_EXT_texture_env_combine


namespace CES {
  enum TexEnvPartType {TexEnvRGB =GL_RGB, TexEnvALPHA =GL_ALPHA};

  struct OglTexState {
    GLenum combine;
    GLenum op[4];
    GLenum src[4];

    OglTexState(const TexEnvPartType part =TexEnvRGB) {
      combine = GL_MODULATE;
      if ( part == TexEnvRGB ) {
        op[0] = op[1] = op[2] = op[3] = GL_SRC_COLOR;
      } else {
        op[0] = op[1] = op[2] = op[3] = GL_SRC_ALPHA;
      }
      src[0] = GL_TEXTURE;
#ifdef GL_EXT_texture_env_combine
      src[1] = GL_PREVIOUS_EXT; src[2] = GL_CONSTANT_EXT;
#else
      src[1] = GL_TEXTURE; src[2] = GL_TEXTURE;
#endif
      src[3] = GL_ZERO;
    } 
  };

  /*--------------------------------------------------------------------
    struct TexEnvCombiner : for GL_EXT_texture_env_combine

        COMBINE_RGB_EXT or
        COMBINE_ALPHA_EXT       Texture Function
        ------------------      ----------------
        REPLACE                 Arg0
        MODULATE                Arg0 * Arg1
        ADD                     Arg0 + Arg1
        ADD_SIGNED_EXT          Arg0 + Arg1 - 0.5
        INTERPOLATE_EXT         Arg0 * (Arg2) + Arg1 * (1-Arg2)

        SOURCE<n>_RGB_EXT       OPERAND<n>_RGB_EXT      Argument
        -----------------       --------------          --------
        TEXTURE                 SRC_COLOR               Ct
                                ONE_MINUS_SRC_COLOR     (1-Ct)
                                SRC_ALPHA               At
                                ONE_MINUS_SRC_ALPHA     (1-At)
        CONSTANT_EXT            SRC_COLOR               Cc
                                ONE_MINUS_SRC_COLOR     (1-Cc)
                                SRC_ALPHA               Ac
                                ONE_MINUS_SRC_ALPHA     (1-Ac)
        PRIMARY_COLOR_EXT       SRC_COLOR               Cf
                                ONE_MINUS_SRC_COLOR     (1-Cf)
                                SRC_ALPHA               Af
                                ONE_MINUS_SRC_ALPHA     (1-Af)
        PREVIOUS_EXT            SRC_COLOR               Cp
                                ONE_MINUS_SRC_COLOR     (1-Cp)
                                SRC_ALPHA               Ap
                                ONE_MINUS_SRC_ALPHA     (1-Ap)

        SOURCE<n>_ALPHA_EXT     OPERAND<n>_ALPHA_EXT    Argument
        -----------------       --------------          --------
        TEXTURE                 SRC_ALPHA               At
                                ONE_MINUS_SRC_ALPHA     (1-At)
        CONSTANT_EXT            SRC_ALPHA               Ac
                                ONE_MINUS_SRC_ALPHA     (1-Ac)
        PRIMARY_COLOR_EXT       SRC_ALPHA               Af
                                ONE_MINUS_SRC_ALPHA     (1-Af)
        PREVIOUS_EXT            SRC_ALPHA               Ap
                                ONE_MINUS_SRC_ALPHA     (1-Ap)

    --------------------------------------------------------------------*/
  struct TexEnvCombiner {
    OglTexState rgb, alp;

    TexEnvCombiner() : rgb(TexEnvRGB), alp(TexEnvALPHA) {}
    void Apply(const TexEnvPartType part) const {
#ifdef GL_EXT_texture_env_combine
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
      if ( part == TexEnvRGB ) {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, rgb.combine);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, rgb.src[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, rgb.src[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, rgb.src[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, rgb.op[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, rgb.op[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, rgb.op[2]);
      }
      else if ( part == TexEnvALPHA ) {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, alp.combine);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, alp.src[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, alp.src[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_EXT, alp.src[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, alp.op[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_EXT, alp.op[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_EXT, alp.op[2]);
      }
#endif // GL_EXT_texture_env_combine
    }
  };


  /*--------------------------------------------------------------------
    struct TexEnvCombiner4 : for GL_NV_texture_env_combine4

        COMBINE_RGB_EXT or
        COMBINE_ALPHA_EXT       Texture Function
        ------------------      ----------------
        ADD                     Arg0 * Arg1 + Arg2 * Arg3
        ADD_SIGNED_EXT          Arg0 * Arg1 + Arg2 * Arg3 - 0.5


        SOURCE<n>_RGB_EXT       OPERAND<n>_RGB_EXT      Argument
        -----------------       --------------          --------
        ZERO                    SRC_COLOR               0
                                ONE_MINUS_SRC_COLOR     1
                                SRC_ALPHA               0
                                ONE_MINUS_SRC_ALPHA     1
        TEXTURE                 SRC_COLOR               Ct
                                ONE_MINUS_SRC_COLOR     (1-Ct)
                                SRC_ALPHA               At
                                ONE_MINUS_SRC_ALPHA     (1-At)
        CONSTANT_EXT            SRC_COLOR               Cc
                                ONE_MINUS_SRC_COLOR     (1-Cc)
                                SRC_ALPHA               Ac
                                ONE_MINUS_SRC_ALPHA     (1-Ac)
        PRIMARY_COLOR_EXT       SRC_COLOR               Cf
                                ONE_MINUS_SRC_COLOR     (1-Cf)
                                SRC_ALPHA               Af
                                ONE_MINUS_SRC_ALPHA     (1-Af)
        PREVIOUS_EXT            SRC_COLOR               Cp
                                ONE_MINUS_SRC_COLOR     (1-Cp)
                                SRC_ALPHA               Ap
                                ONE_MINUS_SRC_ALPHA     (1-Ap)
        TEXTURE<n>_ARB          SRC_COLOR               Ct<n>
                                ONE_MINUS_SRC_COLOR     (1-Ct<n>)
                                SRC_ALPHA               At<n>
                                ONE_MINUS_SRC_ALPHA     (1-At<n>)


        SOURCE<n>_ALPHA_EXT     OPERAND<n>_ALPHA_EXT    Argument
        -----------------       --------------          --------
        ZERO                    SRC_ALPHA               0
                                ONE_MINUS_SRC_ALPHA     1
        TEXTURE                 SRC_ALPHA               At
                                ONE_MINUS_SRC_ALPHA     (1-At)
        CONSTANT_EXT            SRC_ALPHA               Ac
                                ONE_MINUS_SRC_ALPHA     (1-Ac)
        PRIMARY_COLOR_EXT       SRC_ALPHA               Af
                                ONE_MINUS_SRC_ALPHA     (1-Af)
        PREVIOUS_EXT            SRC_ALPHA               Ap
                                ONE_MINUS_SRC_ALPHA     (1-Ap)
        TEXTURE<n>_ARB          SRC_ALPHA               At<n>
                                ONE_MINUS_SRC_ALPHA     (1-At<n>)

   --------------------------------------------------------------------*/
  struct TexEnvCombiner4 {
    OglTexState rgb, alp;
    TexEnvCombiner4() : rgb(TexEnvRGB), alp(TexEnvALPHA) {
      rgb.combine = alp.combine = GL_ADD;
      rgb.op[3] = GL_ONE_MINUS_SRC_COLOR;
      alp.op[3] = GL_ONE_MINUS_SRC_ALPHA;
    }
    void Apply(const TexEnvPartType part) const {
#ifdef GL_NV_texture_env_combine4
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE4_NV);
      if ( part == TexEnvRGB ) {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, rgb.combine);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, rgb.src[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, rgb.src[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, rgb.src[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE3_RGB_NV, rgb.src[3]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, rgb.op[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, rgb.op[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, rgb.op[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND3_RGB_NV, rgb.op[3]);
      }
      else if ( part == TexEnvALPHA ) {
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, alp.combine);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, alp.src[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, alp.src[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_EXT, alp.src[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE3_ALPHA_NV, alp.src[3]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, alp.op[0]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_EXT, alp.op[1]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_EXT, alp.op[2]);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND3_ALPHA_NV, alp.op[3]);
      }
#endif // GL_NV_texture_env_combine4
    }
  };


  /*--------------------------------------------------------------------
    util structures for GL_NV_register_combiners
    --------------------------------------------------------------------*/
#ifdef GL_NV_register_combiners
  struct RegVariable {
    GLenum input, mapping, component;
    RegVariable(const GLenum i = GL_PRIMARY_COLOR_NV,
                const GLenum c = GL_RGB,
                const GLenum m = GL_UNSIGNED_IDENTITY_NV)
     : input(i), component(c), mapping(m) {}
  };

  struct RegState {
    RegVariable a, b, c, d;
    GLenum ab_output, cd_output, sum_output;
    GLenum scale, bias;
    bool ab_dot_product;
    bool cd_dot_product;
    bool mux_sum;
    void Zero(const RegVariable& v) {
      a = b = c = d = v;
      ab_dot_product = cd_dot_product = mux_sum = GL_FALSE;
      scale = bias = GL_NONE;
      ab_output = cd_output = sum_output = GL_DISCARD_NV;
    }
  };

  struct RegCombinerNV {
    RegState rgb, alp;

#ifdef WIN32
	PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
	PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
#endif // WIN32

    //----------------- initialize (for WIN32) -----------------
    bool Init() {
      int err = 0;
#if defined(WIN32)
#define XADDR(functype, funcname) \
  ((funcname = (functype)wglGetProcAddress( #funcname )) == 0)
      err |= XADDR(PFNGLCOMBINERINPUTNVPROC, glCombinerInputNV);
      err |= XADDR(PFNGLCOMBINEROUTPUTNVPROC, glCombinerOutputNV);
#undef XADDR
#endif // WIN32
      return (err == 0);
    }

    RegCombinerNV() {
      rgb.a = RegVariable(GL_PRIMARY_COLOR_NV);
      rgb.b = RegVariable(GL_TEXTURE0_ARB);
      rgb.c = RegVariable(GL_ZERO); 
      rgb.d = RegVariable(GL_ZERO); 
      alp.a = RegVariable(GL_PRIMARY_COLOR_NV, GL_ALPHA);
      alp.b = RegVariable(GL_TEXTURE0_ARB, GL_ALPHA);
      alp.c = RegVariable(GL_ZERO, GL_ALPHA);
      alp.d = RegVariable(GL_ZERO, GL_ALPHA);
      rgb.ab_output = rgb.cd_output = GL_DISCARD_NV;
      rgb.sum_output = GL_SPARE0_NV;
      rgb.ab_dot_product = rgb.cd_dot_product = rgb.mux_sum = GL_FALSE;
      rgb.scale = rgb.bias = GL_NONE;
      alp.ab_output = alp.cd_output = GL_DISCARD_NV;
      alp.sum_output = GL_SPARE0_NV;
      alp.ab_dot_product = alp.cd_dot_product = alp.mux_sum = GL_FALSE;
      alp.scale = alp.bias = GL_NONE;
    }

    void Apply(const GLenum stage, const TexEnvPartType portion) const {
      if ( portion == TexEnvRGB ) {
        glCombinerInputNV(stage, GL_RGB, GL_VARIABLE_A_NV,
                          rgb.a.input, rgb.a.mapping, rgb.a.component);
        glCombinerInputNV(stage, GL_RGB, GL_VARIABLE_B_NV,
                          rgb.b.input, rgb.b.mapping, rgb.b.component);
        glCombinerInputNV(stage, GL_RGB, GL_VARIABLE_C_NV,
                          rgb.c.input, rgb.c.mapping, rgb.c.component);
        glCombinerInputNV(stage, GL_RGB, GL_VARIABLE_D_NV,
                          rgb.d.input, rgb.d.mapping, rgb.d.component);
        glCombinerOutputNV(stage, GL_RGB, rgb.ab_output, rgb.cd_output,
                           rgb.sum_output, rgb.scale, rgb.bias,
                           rgb.ab_dot_product,rgb.cd_dot_product,rgb.mux_sum);
      }
      else if ( portion == TexEnvALPHA ) {
        glCombinerInputNV(stage, GL_ALPHA, GL_VARIABLE_A_NV,
                          alp.a.input, alp.a.mapping, alp.a.component);
        glCombinerInputNV(stage, GL_ALPHA, GL_VARIABLE_B_NV,
                          alp.b.input, alp.b.mapping, alp.b.component);
        glCombinerInputNV(stage, GL_ALPHA, GL_VARIABLE_C_NV,
                          alp.c.input, alp.c.mapping, alp.c.component);
        glCombinerInputNV(stage, GL_ALPHA, GL_VARIABLE_D_NV,
                          alp.d.input, alp.d.mapping, alp.d.component);
        glCombinerOutputNV(stage, GL_ALPHA, alp.ab_output, alp.cd_output,
                           alp.sum_output, alp.scale, alp.bias,
                           GL_FALSE, GL_FALSE, alp.mux_sum);
      }
    }

    void Zero() {
      rgb.Zero(RegVariable(GL_ZERO, GL_RGB));
      alp.Zero(RegVariable(GL_ZERO, GL_ALPHA));
    }
  };

  struct RegFinalCombinerNV {
    RegVariable a, b, c, d, e, f, g;

#ifdef WIN32
	PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
#endif // WIN32

    //----------------- initialize (for WIN32) -----------------
    bool Init() {
      int err = 0;
#if defined(WIN32)
#define XADDR(functype, funcname) \
  ((funcname = (functype)wglGetProcAddress( #funcname )) == 0)
      err |= XADDR(PFNGLFINALCOMBINERINPUTNVPROC, glFinalCombinerInputNV);
#undef XADDR
#endif // WIN32
      return (err == 0);
    }

    RegFinalCombinerNV() {
      a = RegVariable(GL_FOG, GL_ALPHA);
      b = RegVariable(GL_SPARE0_PLUS_SECONDARY_COLOR_NV);
      c = RegVariable(GL_FOG);
      d = RegVariable();
      e = RegVariable();
      f = RegVariable();
      g = RegVariable(GL_SPARE0_NV, GL_ALPHA);
    }

    void Apply() const {
      glFinalCombinerInputNV(GL_VARIABLE_A_NV, a.input,a.mapping,a.component);
      glFinalCombinerInputNV(GL_VARIABLE_B_NV, b.input,b.mapping,b.component);
      glFinalCombinerInputNV(GL_VARIABLE_C_NV, c.input,c.mapping,c.component);
      glFinalCombinerInputNV(GL_VARIABLE_D_NV, d.input,d.mapping,d.component);
      glFinalCombinerInputNV(GL_VARIABLE_E_NV, e.input,e.mapping,e.component);
      glFinalCombinerInputNV(GL_VARIABLE_F_NV, f.input,f.mapping,f.component);
      glFinalCombinerInputNV(GL_VARIABLE_G_NV, g.input,g.mapping,g.component);
    }

    void Zero() {
      a = b = c = d = e = RegVariable();
      g = RegVariable(GL_ZERO, GL_ALPHA);
    }
  };
#endif // GL_NV_register_combiners

}; // end of namespace CES

#endif // _CES_UTIL_OGL_TEXENV_H_

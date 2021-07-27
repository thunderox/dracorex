#!/usr/bin/env python
from waflib.extras import autowaf as autowaf
import waflib.Options as Options
import re
import sys 
import os

# Variables for 'waf dist'
APPNAME = 'dracorex.lv2'
VERSION = '0.0.1'

# Mandatory variables
top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_c')
    opt.load('compiler_cxx')
    autowaf.set_options(opt)

def configure(conf):	
    conf.load('compiler_c')
    conf.load('compiler_cxx')
    autowaf.configure(conf)

    autowaf.configure(conf)
    print('stegosaurus Configuration')

    autowaf.check_pkg(conf, 'lv2', atleast_version='1.4.1',
                      uselib_store='LV2_1_4_1')

    if conf.env['MSVC_COMPILER']:
        conf.env.append_unique('CXXFLAGS', ['-TP', '-MD','-g'])
    else:
        conf.env.append_unique('CXXFLAGS', ['-O2','-funroll-loops','-std=c++0x','-g'])
        conf.env.append_unique('CXXFLAGS', ['-fPIC','-fpermissive','-finline-functions','-g'])

    conf.env.LV2DIR = os.getenv('LV2_PATH').split(':')[2]

    autowaf.display_msg(conf, "LV2 bundle directory", conf.env['LV2DIR'])
    print('')

def build(bld):
    bundle = 'dracorex.lv2'

    # Make a pattern for shared objects without the 'lib' prefix
    module_pat = re.sub('^lib', '', bld.env.cshlib_PATTERN)
    module_ext = module_pat[module_pat.rfind('.'):]

    # Determine platform options
    ui_framework = []
    ui_libs      = []
    ui_lang      = 'c'
    if bld.env.DEST_OS == 'win32':
        pugl_impl = 'pugl/pugl_win.cpp'
        ui_type   = 'http://lv2plug.in/ns/extensions/ui#WindowsUI'
        ui_libs   = ['gdi32', 'user32']
        ui_lang   = 'cxx'
    elif bld.env.DEST_OS == 'darwin':
        pugl_impl    = 'pugl/pugl_osx.m'
        ui_type      = 'http://lv2plug.in/ns/extensions/ui#CocoaUI'
        ui_framework = ['Cocoa']
    else:
        pugl_impl = 'pugl/pugl_x11.c'
        ui_type   = 'http://lv2plug.in/ns/extensions/ui#X11UI'
        ui_libs   = ['X11']

    # Build Turtle files by substitution
    for i in ['manifest.ttl', 'dracorex.ttl']:
        bld(features     = 'subst',
            source       = i + '.in',
            target       = '%s/%s' % (bundle, i),
            install_path = '${LV2DIR}/%s' % bundle,
            LIB_EXT      = module_ext,
            UI_TYPE      = ui_type)

    # Build plugin library
    obj = bld(features     = 'c cshlib',
              source       = 'dracorex.c dracorex_synth.c wavetable.c fastmaths.c chorus.c',
              name         = 'dracorex',
              target       = '%s/dracorex' % bundle,
              install_path = '${LV2DIR}/%s' % bundle,
              use          = 'LV2_1_4_1',
              lib          = ['m'])
    obj.env.cshlib_PATTERN = module_pat

    # Build UI library
    obj = bld(features     = '%s %sshlib' % (ui_lang, ui_lang),
              source       = ['dracorex_ui.c', "deliriumUI/deliriumUI.c", "deliriumUI/button.c", "deliriumUI/knob.c", "deliriumUI/microknob.c", "deliriumUI/fader.c", "deliriumUI/switch.c", "deliriumUI/adsr.c","deliriumUI/wave_selector.c", "deliriumUI/lfo.c", pugl_impl],
              name         = 'dracorex_ui',
              target       = '%s/dracorex_ui' % bundle,
              install_path = '${LV2DIR}/%s' % bundle,
              lib          = ui_libs + ['m', 'pthread', 'cairo' ],
              framework    = ui_framework,
              use          = 'LV2_1_4_1')
    obj.env['%sshlib_PATTERN' % ui_lang] = module_pat

    bld.install_files('${LV2DIR}/%s/waves'% bundle, bld.path.ant_glob('waves/*.*'))
    bld.install_files('${LV2DIR}/%s/dracorex-presets.lv2' % bundle, bld.path.ant_glob('presets.lv2/*.*'))



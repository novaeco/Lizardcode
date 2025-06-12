#ifndef LGFX_CONFIG_HPP
#define LGFX_CONFIG_HPP

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// Configuration pour l'écran 7" Waveshare basé sur le contrôleur ST7262.
// LovyanGFX ne propose pas encore de classe dédiée à ce contrôleur.
// On utilise donc un panneau LCD générique (Panel_ST7789 ici) uniquement pour compiler
// l'exemple. Les paramètres RGB devront être ajustés manuellement pour le ST7262.

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789 _panel_instance; // ST7262 est un contrôleur RGB, pas ST7789. Ceci est un placeholder.
                                     // LovyanGFX n'a pas de driver ST7262 direct. Il faut configurer en RGB.
  lgfx::Bus_Parallel16 _bus_instance; // Utilisation du bus parallèle 16 bits pour RGB

public:

  LGFX(void)
  {
    { // Bus parallèle RGB
      auto cfg = _bus_instance.config();

      cfg.panel = &_panel_instance; // Associe le bus au panneau
      cfg.pin_wr = 18;  // WR (Write) pin
      cfg.pin_rd = 19;  // RD (Read) pin
      cfg.pin_rs = 20;  // RS (Register Select) pin

      cfg.pin_d0 = 39;  // Data pins (D0-D15). Ces broches sont des exemples et DOIVENT être vérifiées.
      cfg.pin_d1 = 40;
      cfg.pin_d2 = 41;
      cfg.pin_d3 = 42;
      cfg.pin_d4 = 45;
      cfg.pin_d5 = 46;
      cfg.pin_d6 = 47;
      cfg.pin_d7 = 48;
      cfg.pin_d8 = 38;
      cfg.pin_d9 = 0;
      cfg.pin_d10 = 1;
      cfg.pin_d11 = 2;
      cfg.pin_d12 = 3;
      cfg.pin_d13 = 4;
      cfg.pin_d14 = 5;
      cfg.pin_d15 = 6;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // Panneau d'affichage (Configuration RGB)
      auto cfg = _panel_instance.config();

      cfg.pin_cs           = -1;  // CS non utilisé en mode RGB parallèle
      cfg.pin_rst          = 14;  // Broche RST (Reset) (peut varier)
      cfg.pin_backlight    = 15;  // Broche du rétroéclairage (peut varier)
      cfg.panel_width      = 800; // Largeur de l'écran (pour 7 pouces)
      cfg.panel_height     = 480; // Hauteur de l'écran (pour 7 pouces)
      cfg.offset_x         = 0;   // Décalage X
      cfg.offset_y         = 0;   // Décalage Y
      cfg.invert           = false; // Inversion des couleurs
      cfg.rgb_order        = false; // Ordre des couleurs RGB/BGR
      cfg.with_depth       = lgfx::color_depth_16; // Profondeur de couleur 16 bits
      cfg.display_dir      = lgfx::direction_Right; // Orientation de l'affichage
      cfg.backlight_pwm    = true; // Utilisation du PWM pour le rétroéclairage

      // Paramètres spécifiques au contrôleur RGB (ST7262)
      // Ces valeurs sont génériques et peuvent nécessiter des ajustements précis pour le ST7262
      cfg.pll_d2           = 0; // PLL_D2
      cfg.pll_m            = 0; // PLL_M
      cfg.pll_n            = 0; // PLL_N
      cfg.hsync_pulse_width = 1; // Largeur d'impulsion HSYNC
      cfg.hsync_back_porch = 40; // Porche arrière HSYNC
      cfg.hsync_front_porch = 8; // Porche avant HSYNC
      cfg.vsync_pulse_width = 1; // Largeur d'impulsion VSYNC
      cfg.vsync_back_porch = 8;  // Porche arrière VSYNC
      cfg.vsync_front_porch = 8; // Porche avant VSYNC
      cfg.pclk_active_neg  = false; // Polarité du PCLK
      cfg.de_active_high   = true; // Polarité du DE
      cfg.hsync_active_high = false; // Polarité du HSYNC
      cfg.vsync_active_high = false; // Polarité du VSYNC

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};

#endif



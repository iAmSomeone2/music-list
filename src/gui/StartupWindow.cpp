/*
  BSD 3-Clause License
  
  Copyright (c) 2020, Brenden Davidson
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  
  3. Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
*/

#include "StartupWindow.hpp"

using namespace MusicList::GUI;

StartupWindow::StartupWindow()
{
    // Set up icons
    this->musicIcon = Gtk::Image("audio-x-generic", Gtk::ICON_SIZE_BUTTON);
    this->openIcon = Gtk::Image("document-open", Gtk::ICON_SIZE_BUTTON);
    this->dirIcon = Gtk::Image("folder", Gtk::ICON_SIZE_BUTTON);

    // Set up window.
    this->window = Gtk::Window();
    this->window.set_default_size(300, 400);
    this->window.set_resizable(false);
    this->window.set_title("MusicList - Welcome!");

    // Set up layout box
    this->box = Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
    this->box.set_homogeneous(true);
    this->box.set_vexpand(true);
    this->box.set_hexpand(true);

    // Set up each button
    this->btnMusicScan = Gtk::Button("Scan Music folder");
    this->btnMusicScan.set_focus_on_click(true);
    this->btnMusicScan.set_halign(Gtk::Align::ALIGN_CENTER);
    this->btnMusicScan.set_valign(Gtk::Align::ALIGN_CENTER);
    this->btnMusicScan.set_size_request(250, 100);
    this->btnMusicScan.set_image(this->musicIcon);
    this->box.pack_start(this->btnMusicScan);

    this->btnOtherScan = Gtk::Button("Scan a different folder");
    this->btnOtherScan.set_focus_on_click(true);
    this->btnOtherScan.set_halign(Gtk::Align::ALIGN_CENTER);
    this->btnOtherScan.set_valign(Gtk::Align::ALIGN_CENTER);
    this->btnOtherScan.set_size_request(250, 100);
    this->btnOtherScan.set_image(this->dirIcon);
    this->box.pack_start(this->btnOtherScan);

    this->btnOpen = Gtk::Button("Open previous scan results");
    this->btnOpen.set_focus_on_click(true);
    this->btnOpen.set_halign(Gtk::Align::ALIGN_CENTER);
    this->btnOpen.set_valign(Gtk::Align::ALIGN_CENTER);
    this->btnOpen.set_size_request(250, 100);
    this->btnOpen.set_image(this->openIcon);
    this->box.pack_start(this->btnOpen);

    // Add layout box to window.
    this->window.add(this->box);
}

Gtk::Window& StartupWindow::getWindow()
{
    return this->window;
}

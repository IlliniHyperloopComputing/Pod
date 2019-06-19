import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BatteryComponent } from './components/battery/battery.component';
import { PositionComponent } from './components/position/position.component';
import { DiagnosticsComponent } from './components/diagnostics/diagnostics.component';
import { WarningComponent } from './components/warning/warning.component';

@NgModule({
  declarations: [
    AppComponent,
    BatteryComponent,
    PositionComponent,
    DiagnosticsComponent,
    WarningComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

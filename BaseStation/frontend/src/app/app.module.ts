import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BatteryComponent } from './components/battery/battery.component';
import { PositionComponent } from './components/position/position.component';
import { DiagnosticsComponent } from './components/diagnostics/diagnostics.component';
import { WarningComponent } from './components/warning/warning.component';
import { StatsComponent } from './components/stats/stats.component';
import { ButtonsComponent } from './components/buttons/buttons.component';
import { HttpClientModule } from '@angular/common/http';
import { StateComponent } from './components/state/state.component';

@NgModule({
  declarations: [
    AppComponent,
    BatteryComponent,
    PositionComponent,
    DiagnosticsComponent,
    WarningComponent,
    StatsComponent,
    ButtonsComponent,
    StateComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

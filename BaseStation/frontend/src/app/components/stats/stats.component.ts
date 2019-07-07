import { Component, OnInit } from '@angular/core';
import { interval } from 'rxjs';

import { StatsService } from '../../services/stats.service';
import { Stat, Row } from '../../models/stat';

@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: ['./stats.component.css']
})
export class StatsComponent implements OnInit {
  stats:Row[];

  constructor(private statsService: StatsService) { 
    interval(500).subscribe(x => {
      this.getValues();
    })
  }

  ngOnInit() {
    this.getValues();
  }

  getValues() {
    this.statsService.getStatStatus().subscribe((data: Row[]) => this.stats = data)
  }

  applyStyle(color:string) {
    return {"background-color": color};
  }
}

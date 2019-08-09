import { Component, OnInit } from '@angular/core';
import { interval } from 'rxjs';

import { PositionService } from '../../services/position.service';
import { Position } from '../../models/position';
@Component({
  selector: 'app-position',
  templateUrl: './position.component.html',
  styleUrls: ['./position.component.css']
})
export class PositionComponent implements OnInit {
  position: Position = { currentDistance:0, totalDistance:100};

  constructor(private positionService: PositionService) {
    interval(500).subscribe(x => {
      this.getPosition();
    })
   }

  ngOnInit() {
    this.getPosition()
  }

  getPosition() {
    this.positionService.getPositionStatus().subscribe((data: Position) => this.position = data)
  }
}

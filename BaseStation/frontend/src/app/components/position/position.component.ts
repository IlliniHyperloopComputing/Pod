import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-position',
  templateUrl: './position.component.html',
  styleUrls: ['./position.component.css']
})
export class PositionComponent implements OnInit {
  position:number = 500;
  totalDistance:number = 1000;

  constructor() { }

  ngOnInit() {
  }

}

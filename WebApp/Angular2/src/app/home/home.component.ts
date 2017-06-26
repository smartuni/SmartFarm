import { Component, OnInit } from '@angular/core';
import { Http } from '@angular/http';
import { Router } from '@angular/router';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {

  logotypePath: string;

  constructor() { 
    this.logotypePath = '../images/smartfarm_animated.gif'
  }

  ngOnInit() {
  }
  
}

import { Injectable } from '@angular/core';

import { Http } from '@angular/http';

@Injectable()
export class UsersService {

  constructor(private http: Http) { }

  getData() {
    this.http.get('');
  }

}

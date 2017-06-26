import { RiotAppPage } from './app.po';

describe('riot-app App', () => {
  let page: RiotAppPage;

  beforeEach(() => {
    page = new RiotAppPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});

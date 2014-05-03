#include "SFApp.h"

SFApp::SFApp() : keyCounter(0), coinCounter(0), score(0), is_running(true) {

  surface = SDL_GetVideoSurface();
  app_box = make_shared<SFBoundingBox>(Vector2(surface->w/2, surface->h/2), surface->w/2, surface->h/2);
  player  = make_shared<SFAsset>(SFASSET_PLAYER);
  auto player_pos = Point2(surface->w/2, 88.0f);
  player->SetPosition(player_pos);

  const int number_of_walls = 15;
  for(int o=0; o<number_of_walls; o++) {
    // place an alien at width/number_of_aliens * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL);
    auto wall2 = make_shared<SFAsset>(SFASSET_WALL);
    auto wall3 = make_shared<SFAsset>(SFASSET_WALL);

    auto pos = Point2(40.0f *  (o+0.63), 160.0f);
    auto pos2 = Point2(40.0f * (o+1.38f), 300.0f);
    auto pos3 = Point2(40.0f * (o+0.63), 432.0f);
    
    wall->SetPosition(pos);
    wall2->SetPosition(pos2);
    wall3->SetPosition(pos3);
    
    walls.push_back(wall);
    walls.push_back(wall2);
    walls.push_back(wall3);
    
    //(surface->w/number_of_aliens) * i
  }
const int number_of_coins = 15;

for(int c=0; c < number_of_coins; c++){
  auto coin = make_shared<SFAsset>(SFASSET_COIN);
  auto coin2 = make_shared<SFAsset>(SFASSET_COIN);
  auto coin3 = make_shared<SFAsset>(SFASSET_COIN);
  auto coin4 = make_shared<SFAsset>(SFASSET_COIN);
  auto pos  = Point2(40.0f * (c+1), 190.0f);
  auto pos2 = Point2(40.0f * (c+1), 330.0f);
  auto pos3 = Point2(40.0f * (c+1), 210.0f);
  auto pos4 = Point2(40.0f * (c+1), 350.0f);
  coin->SetPosition(pos);
  coin2->SetPosition(pos2);
  coin3->SetPosition(pos3);
  coin4->SetPosition(pos4);
  coins.push_back(coin);
  coins.push_back(coin2);
  coins.push_back(coin3);
  coins.push_back(coin4);
}

 auto skull = make_shared<SFAsset>(SFASSET_SKULL);
 auto skull2 = make_shared<SFAsset>(SFASSET_SKULL);

 auto pos = Point2(40.0f,190.0f);
 auto pos2 = Point2(40.0f,330.0f);

 skull->SetPosition(pos);
 skull2->SetPosition(pos2);

 skulls.push_back(skull);
 skulls.push_back(skull2);

 auto key = make_shared<SFAsset>(SFASSET_KEY);
 auto key2 = make_shared<SFAsset>(SFASSET_KEY);
 auto pos3 = Point2(surface->w/2, 190.0f);
 auto pos4 = Point2(surface->w/2, 350.0f);
 key->SetPosition(pos3);
 key2->SetPosition(pos4);
 keys.push_back(key);
 keys.push_back(key2);

 auto door = make_shared<SFAsset>(SFASSET_DOOR);
 auto door2 = make_shared<SFAsset>(SFASSET_DOOR);
 auto pos5 = Point2(15.0f, 291.0f);
 auto pos6 = Point2(625.0f, 422.5f);
 door->SetPosition(pos5);
 door2->SetPosition(pos6);
 doors.push_back(door);
 doors.push_back(door2);

 auto end = make_shared<SFAsset>(SFASSET_END);
 auto pos7 = Point2(40.0f,surface->h);
 end->SetPosition(pos7);
 ends.push_back(end);
}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {

  SFEVENT the_event = event.GetCode();
      switch (the_event) {
  case SFEVENT_QUIT:
    cout << "score: " << score << endl;
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_PLAYER_UP:
    player->GoNorth();
    break;
  case SFEVENT_PLAYER_DOWN:
    player->GoSouth();
    break;
  } 
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {
    std::stringstream sstm;
    sstm << "score: " << score << " coins: " << coinCounter << " keys: " << keyCounter;

    SDL_WM_SetCaption(sstm.str().c_str(),  sstm.str().c_str());
  // Update projectile positions


  for(auto c: coins){
    if (c->CollidesWith(player)){
      c->HandleCollision();
        score = score + 100;
        coinCounter++;
    }
  }

  for (auto s: skulls) {

  }

  for(auto k: keys){
	if(k->CollidesWith(player)){
		k->HandleCollision();
		keyCounter++;
   }
}
  for(auto w: walls){ 
   if(player->CollidesWith(w) && w->DetectBottomCollision(player))
     player->GoSouth();
   if(player->CollidesWith(w) && w->DetectTopCollision(player))
     player->GoNorth();
   if(player->CollidesWith(w) && w->DetectLeftCollision(player))
     player->GoWest();
   if(player->CollidesWith(w) && w->DetectRightCollision(player))
      player->GoEast();

   }
 
  for(auto d: doors){
   if(player->CollidesWith(d) && d->DetectBottomCollision(player) && keyCounter == 0)
     player->GoSouth();

   if(player->CollidesWith(d) && d->DetectTopCollision(player) && keyCounter == 0)
     player->GoNorth();

   if(player->CollidesWith(d) && d->DetectLeftCollision(player) && keyCounter == 0)
     player->GoWest();

   if(player->CollidesWith(d) && d->DetectRightCollision(player) && keyCounter == 0)
     player->GoEast();

   if(player->CollidesWith(d) && keyCounter > 0){
     d->HandleCollision();
	   keyCounter--;
}
}

for(auto e: ends){
  if(player->CollidesWith(e))
    is_running = false;
}


  list<shared_ptr<SFAsset>> tmp2;
  for(auto c : coins){
    if(c->IsAlive()) {
      tmp2.push_back(c);
    }
  }
  coins.clear();
  coins = list<shared_ptr<SFAsset>>(tmp2);


list<shared_ptr<SFAsset>> tmp4;
  for(auto k: keys){
    if(k->IsAlive()){
	tmp4.push_back(k);
	
    }
}
  keys.clear();
  keys = list<shared_ptr<SFAsset>>(tmp4);

list<shared_ptr<SFAsset>> tmp5;
  for(auto d: doors){
    if(d->IsAlive()){
	tmp5.push_back(d);
    }
}
  doors.clear();
  doors = list<shared_ptr<SFAsset>>(tmp5);


  //this method ends here
}

void SFApp::OnRender() {
  // clear the surface
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 8, 54, 129) );

  // draw the player
  player->OnRender(surface);

  for(auto c: coins) {
    if(c->IsAlive()) {c->OnRender(surface);}
  }

   for(auto s: skulls) {
    if(s->IsAlive()) {s->OnRender(surface);}
  }

  for(auto w: walls){
   w->OnRender(surface);
  }

  for(auto k: keys){
    if(k->IsAlive()) {k->OnRender(surface);}
  }

  for(auto e: ends){
    e->OnRender(surface);
  }

  for(auto d: doors){
    if(d->IsAlive()) {d->OnRender(surface);}
  }

  // Switch the off-screen buffer to be on-screen
  SDL_Flip(surface);
}



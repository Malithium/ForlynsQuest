#include "SFAsset.h"

int SFAsset::SFASSETID=0;

SFAsset::SFAsset(SFASSETTYPE type){
  this->type = type;
  this->id   = ++SFASSETID;
  SDL_Surface * tmp_surf;

  switch (type) {
  case SFASSET_PLAYER:
    tmp_surf = IMG_Load("assets/Adventurer.png");
    break;
  case SFASSET_SKULL:
    tmp_surf = IMG_Load("assets/skull.png");
    break;
  case SFASSET_COIN:
    tmp_surf = IMG_Load("assets/goldCoin.png");
    break;
  case SFASSET_WALL:
    tmp_surf = IMG_Load("assets/wall.png");
    break;
  case SFASSET_KEY:
    tmp_surf = IMG_Load("assets/Key.png");
    break;
  case SFASSET_DOOR:
    tmp_surf = IMG_Load("assets/Door.png");
    break;
  case SFASSET_END:
    tmp_surf = IMG_Load("assets/End.png");
    break;
  }

  if(!tmp_surf) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  sprite = SDL_DisplayFormatAlpha(tmp_surf);
  SDL_FreeSurface(tmp_surf);

  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), sprite->w, sprite->h));
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  bbox   = a.bbox;
  type   = a.type;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_FreeSurface(sprite);
    sprite = NULL;
  }
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */
Vector2 GameSpaceToScreenSpace(Vector2 &r) {
  return Vector2 (
		  r.getX(),
		  (SDL_GetVideoSurface()->h - r.getY())
		  );
}

void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFAssetId SFAsset::GetId() {
  return id;
}

void SFAsset::OnRender(SDL_Surface * level) {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * -1);
  Vector2 ss = GameSpaceToScreenSpace(gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_BlitSurface(sprite, NULL, level, &rect);
}

void SFAsset::GoWest() {
  Vector2 c = *(bbox->centre) + Vector2(-8.0f, 0.0f);
  if(!(c.getX() < 0) ) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoEast() {
  Vector2 c = *(bbox->centre) + Vector2(8.0f, 0.0f);
  if(!(c.getX() > SDL_GetVideoSurface()->w)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoNorth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, 8.0f);
    if(!(c.getY() > SDL_GetVideoSurface()->h)){
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoSouth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -8.0f);
  if(!(c.getY() < 0)){
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

float SFAsset::GetObjX(){
  bbox->centre->getX();
}

bool SFAsset::DetectLeftCollision(shared_ptr<SFAsset> obj){
     float pX = bbox->centre->getX();
     float wX = obj->bbox->centre->getX();

     if(pX >= wX)
	return true;
     else
    	return false;
}

bool SFAsset::DetectRightCollision(shared_ptr<SFAsset> obj){
     float pX = bbox->centre->getX();
     float wX = obj->bbox->centre->getX();
     

     if(pX <= wX)
	return true;
     else
    	return false;
}

bool SFAsset::DetectTopCollision(shared_ptr<SFAsset> obj){
     float pY = bbox->centre->getY();
     float wY = obj->bbox->centre->getY();
     float pX = bbox->centre->getX();
     float wX = obj->bbox->centre->getX();

     if(pY < wY)
	return true;
     else if(pY < wY && pX <= wX)
    	return false;
     else if(pY < wY && pX >= wX)
	return false;
     else if(pY > wY)
	return false;
}

bool SFAsset::DetectBottomCollision(shared_ptr<SFAsset> obj){
     float pY = bbox->centre->getY();
     float wY = obj->bbox->centre->getY();
     float pX = bbox->centre->getX();
     float wX = obj->bbox->centre->getX();

     if(pY > wY)
	return true;
     else if(pY > wY && pX <= wX)
    	return false;
     else if(pY > wY && pX >= wX)
	return false;
     else if(pY < wY)
	return false;

}


bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

void SFAsset::SetNotAlive() {
  type = SFASSET_DEAD;
}

bool SFAsset::IsAlive() {
  return (SFASSET_DEAD != type);
}

void SFAsset::HandleCollision() {
  if(SFASSET_SKULL == type || SFASSET_COIN == type || SFASSET_PLAYER == type || SFASSET_KEY == type || SFASSET_DOOR == type) {
    SetNotAlive();
  }
}


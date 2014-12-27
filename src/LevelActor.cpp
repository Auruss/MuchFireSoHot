#include "LevelActor.h"

LevelActor::LevelActor(LevelModel* model) {
	
}

LevelActor::~LevelActor() {

}

//////////////////////////////////////////////////////////////////////////

void LevelActor::doUpdate(const oxygine::UpdateState &us) {
	Polygon::doUpdate(us);
}

void LevelActor::doRender(const oxygine::RenderState &rs) {
	Polygon::doRender(rs);
}

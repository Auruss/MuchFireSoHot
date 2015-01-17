#include <Common/BaseModel.h>

Common::BaseModel::BaseModel() {
	_hasChanges = false;
}

void Common::BaseModel::updateChanges() {
	_hasChanges = true;
}

void Common::BaseModel::skipChanges() {
	_hasChanges = false;
}

bool Common::BaseModel::hasChanges() {
	if(_hasChanges) {
		_hasChanges = false;
		return true;
	}
	return _hasChanges;
}
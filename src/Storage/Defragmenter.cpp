#include <Storage/Defragmenter.h>

using namespace Storage;

Defragmenter::Defragmenter() {
	this->reset();
}

void Defragmenter::addFragment(int index, int length) {
	for(auto iter = _fragments.begin(); iter != _fragments.end(); iter++) {
		// is fragment before?
		if(iter->index == (index+length)) {
			iter->index -= length;
			_defrag_count++;
			return;
		}
		// is fragment after?
		if((iter->index + iter->length) == index) {
			iter->length += length;
			_defrag_count++;
			return;
		}
	}

	// need to create new fragment :(
	fragment frag;
	frag.index = index;
	frag.length = length;
	_fragments.push_back(frag);
}

std::vector<Storage::Defragmenter::fragment>* Defragmenter::defrag(unsigned int* count) {
	combine();
	*count = _defrag_count;
	return &_fragments;
}

bool Defragmenter::combine() {
	return false;
}

void Defragmenter::reset() {
	_defrag_count = 0;
	_fragments.clear();
}
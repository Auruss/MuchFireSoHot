#pragma once

#include <vector>

namespace Storage {

	class Defragmenter {
		public:
			struct fragment {
				int index;
				int length;
			};

		public:
			Defragmenter();

			void addFragment(int index, int length);

			std::vector<fragment>* defrag(unsigned int* count);
			void reset();
			
		private:
			bool combine();

			unsigned int _defrag_count;
			std::vector<fragment> _fragments;
	};

}
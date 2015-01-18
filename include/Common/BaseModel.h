#pragma once

namespace Common {
	class BaseModel {
	public:
		BaseModel();
		
		void updateChanges();
		void skipChanges();

		bool hasChanges();

	private:
		bool _hasChanges;

	};
}
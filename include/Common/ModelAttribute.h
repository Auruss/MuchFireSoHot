#pragma once

namespace Common {
	template <class T>	
	class ModelAttribute {
	
	public:
		ModelAttribute() {
			_hasChanged = false;
		}

		T& operator = (T& rigth) {
			_value = right;
			_hasChanged = true;
			return _value;
		}

		operator T() {
			return _value;
		}

		bool hasChanged() {
			_hasChanged = !_hasChanged;
			return !_hasChanged;
		}
	
	private:
		T _value;
		bool _hasChanged;

	};
}
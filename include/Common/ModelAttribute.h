#pragma once

namespace Common {
	template <class T>	
	class ModelAttribute {
	
	public:
		ModelAttribute() {
			_hasChanged = false;
		}

		T& operator = (T& right) {
			_value = right;
			_hasChanged = true;
			return _value;
		}

		T& operator = (T right) {
			_value = right;
			_hasChanged = true;
			return _value;
		}

		operator T() {
			return _value;
		}

		bool hasChanged() {
			if(_hasChanged) {
				_hasChanged = false;
				return true;
			} else {
				return false;
			}
		}
	
	private:
		T _value;
		bool _hasChanged;

	};
}
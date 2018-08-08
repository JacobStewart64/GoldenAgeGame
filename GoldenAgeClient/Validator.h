#pragma once

class Validator {
public:
	template<unsigned int buffer_max = LOGINBUFFERMAX>
	void validateInput(irr::gui::IGUIEditBox* edit_box)
	{
		debug("checking if login input string is too long");
		unsigned int len = lstrlenW(edit_box->getText());
		if (len >= buffer_max)
		{
			debug("login input string too long");
			wchar_t fixedtext[buffer_max];
			std::memcpy(fixedtext, edit_box->getText(), sizeof(fixedtext) - sizeof(wchar_t));
			fixedtext[buffer_max - 1] = L'\0';
			edit_box->setText(fixedtext);
			debug("login input string set back a letter");
		}
		debug("login input string good");
	}
};

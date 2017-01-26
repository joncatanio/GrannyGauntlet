#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class InputHandler {
public:
	InputHandler() {}

	virtual ~InputHandler() = 0;

	virtual void handleInput() = 0;

private:

};

#endif

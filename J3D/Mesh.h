
#include "Bindable.h"
#include "IndexBuffer.h"

#include <vector>
#include <memory>

class Mesh {

public:

	void draw(class Graphics& gfx);
	void addBindable(std::shared_ptr<Bindable> pBindable);

private:
	std::vector<std::shared_ptr<Bindable>> bindables;
	std::shared_ptr<IndexBuffer> pIndexBuffer;
	
};

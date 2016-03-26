#include "DreamTest.h"
#include "SceneTest.h"
#include "ProjectTest.h"


int main(void) {
	int result = 0;

	DreamTest::DreamTest dreamTest;
	dreamTest.run();
	result += dreamTest.getResult();

	DreamTest::ProjectTest projectTest;
	projectTest.run();
	result += projectTest.getResult();

	DreamTest::SceneTest sceneTest;
	sceneTest.run();
	result += sceneTest.getResult();

	return result;
}
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(40);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();

	this->ico_sphere = ofIcoSpherePrimitive(65, 3);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	int radius_start = 65;
	int radius_len = 115;
	int radius_max = radius_start + radius_len;
	for (auto& triangle : this->ico_sphere.getMesh().getUniqueFaces()) {

		auto avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		auto power = ofGetFrameNum() % 120 < 10 ? 0 : ofGetFrameNum() % 120 < 60 ? ofMap(ofGetFrameNum() % 120, 10, 60, 0, 1) : ofGetFrameNum() % 120 < 70 ? 1 : ofMap(ofGetFrameNum() % 120, 70, 120, 1, 0);

		auto noise_value = ofNoise(glm::vec4(avg * 0.03, ofGetFrameNum() * 0.01));
		int radius_end =  radius_start + ofMap(noise_value, 0, 1, 0, radius_len) * power;


		
		this->frame.addVertex(glm::vec3());
		for (int radius = radius_start; radius <= radius_end; radius += 1) {

			auto mesh_index = this->mesh.getNumVertices();
			auto frame_index = this->frame.getNumVertices();

			auto param = ofMap(radius, radius_start, radius_max, 0, PI * 0.1);

			auto angle_x = ofMap(ofNoise(avg.x * 0.8, radius * 0.03 + ofGetFrameNum() * 0.02), 0, 1, -param, param);
			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto angle_y = ofMap(ofNoise(avg.y * 0.8, radius * 0.03 + ofGetFrameNum() * 0.02), 0, 1, -param, param);
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto angle_z = ofMap(ofNoise(avg.z * 0.8, radius * 0.03 + ofGetFrameNum() * 0.02), 0, 1, -param, param);
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			auto loc = glm::normalize(avg) * radius;

			if (radius == radius_end) {

				this->mesh.addVertex(glm::vec4(loc + triangle.getVertex(0) - avg, 0) * rotation_z * rotation_y * rotation_x);
				this->mesh.addVertex(glm::vec4(loc + triangle.getVertex(1) - avg, 0) * rotation_z * rotation_y * rotation_x);
				this->mesh.addVertex(glm::vec4(loc + triangle.getVertex(2) - avg, 0) * rotation_z * rotation_y * rotation_x);

				this->mesh.addIndex(mesh_index + 0); this->mesh.addIndex(mesh_index + 1); this->mesh.addIndex(mesh_index + 2);
			}

			this->frame.addVertex(glm::vec4(loc, 0) * rotation_z * rotation_y * rotation_x);
			this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index - 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateY(ofGetFrameNum() * 0.666666666666666);

	ofSetColor(255, 0, 0, 200);
	this->frame.drawWireframe();
	this->mesh.drawWireframe();

	ofSetColor(0);
	this->mesh.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
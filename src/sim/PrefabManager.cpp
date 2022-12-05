#ifdef WINDOWS
#include "PrefabManager.h"
#include "Shape.h"
#include "Wire.h"
#include "Controller_Button.h"
#include "Controller_Bulb.h"
#include "Controller_SimulatorLink.h"
#include "Junction.h"

class CShape *PrefabManager::generateVDD() {
	CShape *o = new CShape();
	o->setName("VDD");
	o->addJunction(0, 0, "VDD");
	o->addLine(0, -20, 0, 0);
	o->addCircle(0, -30, 10);
	return o;
}
class CShape *PrefabManager::generateGND() {
	CShape *o = new CShape();
	o->setName("GND");
	o->addJunction(0, -20, "GND");
	o->addLine(0, -20, 0, 0);
	o->addLine(-10, 0, 10, 0);
	o->addLine(-8, 2, 8, 2);
	o->addLine(-6, 4, 6, 4);
	o->addLine(-4, 6, 4, 6);
	return o;
}
class CShape *PrefabManager::generateButton() {
	CShape *o = new CShape();
	o->setName("Button");
	CJunction *a = o->addJunction(-40, -10, "pad_a");
	CJunction *b = o->addJunction(40, -10, "pad_b");
	a->setName("pad_a");
	b->setName("pad_b");
	o->addLine(40, -10, 20, -10);
	o->addLine(-40, -10, -20, -10);
#if 0
	CShape *mover = new CShape();
	mover->addLine(20, 10, -20, 10);
	mover->addLine(0, 20, 0, 10);
	o->addShape(mover);
#else
	CShape *mover = o->addLine(20, 10, -20, 10);
	//	mover->addLine(-20, 0, -20, 10);

#endif
	mover->setName("button_mover");
	CControllerButton *btn = new CControllerButton(a, b);
	btn->setMover(mover);
	o->setController(btn);
	o->translateEachChild(0, 10);
	return o;
}
class CShape *PrefabManager::generateTest() {

	CShape *o = new CShape();
	o->setName("Test");
	o->addLine(50, 10, -50, 10);
	o->addLine(50, -10, -50, -10);
	o->addLine(50, -10, 50, 10);
	o->addLine(-50, 10, -50, -10);
	return o;
}
class CShape *PrefabManager::generateWB3S() {
	CShape *o = new CShape();
	o->setName("WB3S");
	CControllerSimulatorLink *link = new CControllerSimulatorLink();
	o->setController(link);
	o->addText(-40, -25, "WB3S");
	o->addRect(-50, -20, 100, 180);
	struct PinDef_s {
		const char *name;
		int gpio;
	};
	PinDef_s wb3sPinsRight[] = {
		{ "TXD1", 11 },
		{ "RXD1", 10 },
		{ "PWM2", 8 },
		{ "PWM3", 9 },
		{ "RXD2", 1 },
		{ "TXD2", 0 },
		{ "PWM1", 7 },
		{ "GND", -1 }
	};
	PinDef_s wb3sPinsLeft[] = {
		{ "CEN", -1 },
		{ "ADC3", 23 },
		{ "EN", -1 },
		{ "P14", 14 },
		{ "PWM5", 26 },
		{ "PWM4", 24 },
		{ "PWM0", 6 },
		{ "VCC", -1 }
	};
	for (int i = 0; i < 8; i++) {
		int y = i * 20;
		o->addLine(50, y, 80, y);
		o->addLine(-50, y, -80, y);
		CJunction *a = o->addJunction(-80, y, "", wb3sPinsLeft[i].gpio);
		a->setName(wb3sPinsLeft[i].name);
		a->addText(-5, -5, wb3sPinsLeft[i].name);
		CJunction *b = o->addJunction(80, y, "", wb3sPinsRight[i].gpio);
		b->setName(wb3sPinsRight[i].name);
		b->addText(-25, -5, wb3sPinsRight[i].name);
		link->addRelatedJunction(a);
		link->addRelatedJunction(b);
	}
	return o;
}
class CShape *PrefabManager::generateLED_CW() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->setName("LED_CW");
	o->addText(-40, -25, "CW");
	CShape *filler = o->addCircle(0, 0, bulb_radius);
	CShape *body = o->addCircle(0, 0, bulb_radius);
	filler->setFill(true);
	CJunction *gnd = o->addJunction(-bulb_radius, 0);
	gnd->setName("GND");
	gnd->addText(-5, -5, "");
	CJunction *cool = o->addJunction(bulb_radius, 20);
	cool->setName("C");
	cool->addText(-5, -5, "C");
	CJunction *warm = o->addJunction(bulb_radius, -20);
	warm->setName("W");
	warm->addText(-5, -5, "W");

	CControllerBulb *bulb = new CControllerBulb(cool, warm, gnd);
	filler->setName("internal_bulb_filler");
	bulb->addShape(filler);
	float mul = sqrt(2) * 0.5f;
	o->addLine(-bulb_radius * mul, -bulb_radius * mul, bulb_radius * mul, bulb_radius * mul);
	o->addLine(-bulb_radius * mul, bulb_radius * mul, bulb_radius * mul, -bulb_radius * mul);
	return o;
}
class CShape *PrefabManager::generateLED_RGB() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->addText(-40, -25, "RGB");
	o->setName("LED_RGB");
	//CShape *filler = o->addCircle(0, 0, bulb_radius);
	CShape *body = o->addCircle(0, 0, bulb_radius);
	//filler->setFill(true);
	CJunction *a = o->addJunction(-bulb_radius, 0);
	a->setName("GND");
	a->addText(-5, -5, "");
	CJunction *b1 = o->addJunction(bulb_radius, 20);
	b1->setName("R");
	b1->addText(-5, -5, "R");
	CJunction *b2 = o->addJunction(bulb_radius, 0);
	b2->setName("G");
	b2->addText(-5, -5, "G");
	CJunction *b3 = o->addJunction(bulb_radius, -20);
	b3->setName("B");
	b3->addText(-5, -5, "B");

	//filler->setName("internal_bulb_filler");
	//bulb->setShape(filler);
	float mul = sqrt(2) * 0.5f;
	o->addLine(-bulb_radius * mul, -bulb_radius * mul, bulb_radius * mul, bulb_radius * mul);
	o->addLine(-bulb_radius * mul, bulb_radius * mul, bulb_radius * mul, -bulb_radius * mul);
	return o;
}

class CShape *PrefabManager::generateBulb() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->addText(-40, -25, "Bulb");
	o->setName("Bulb");
	CShape *filler = o->addCircle(0, 0, bulb_radius);
	CShape *body = o->addCircle(0, 0, bulb_radius);
	filler->setFill(true);
	CJunction *a = o->addJunction(-bulb_radius, 0);
	a->setName("A");
	a->addText(-5, -5, "");
	CJunction *b = o->addJunction(bulb_radius, 0);
	b->setName("B");
	b->addText(-25, -5, "");
	CControllerBulb *bulb = new CControllerBulb(a, b);
	o->setController(bulb);
	filler->setName("internal_bulb_filler");
	bulb->setShape(filler);
	float mul = sqrt(2) * 0.5f;
	o->addLine(-bulb_radius * mul, -bulb_radius * mul, bulb_radius * mul, bulb_radius * mul);
	o->addLine(-bulb_radius * mul, bulb_radius * mul, bulb_radius * mul, -bulb_radius * mul);
	return o;
}

class CShape *PrefabManager::generateStrip_SingleColor() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->addText(-40, -25, "Strip Single Color");
	o->setName("StripSingleColor");
	int w = 200;
	o->addRect(-w, -20, w * 2, 40);
	int leds = 8;
	int ofs = 10;
	float start = ofs - w;
	float len = 2*w - ofs * 2;
	float ledlen = len / (leds + 1);
	CJunction *a = o->addJunction(-w, 0);
	a->setName("A");
	a->addText(-5, -5, "");
	CJunction *b = o->addJunction(w, 0);
	b->setName("B");
	b->addText(-25, -5, "");
	CControllerBulb *bulb = new CControllerBulb(a, b);
	o->setController(bulb);
	for (int i = 0; i < leds; i++) {
		float x = start + (len / leds) * i;
		float y = -15;
		int filler_ofs = 1;
		CShape *shape_outer = o->addRect(x, y, ledlen, 30);
		CShape *shape_filler = o->addRect(x+filler_ofs, y + filler_ofs, ledlen - 2 * filler_ofs, 30 - 2 * filler_ofs);
		shape_filler->setFill(true);
		bulb->addShape(shape_filler);
		char str[16];
		sprintf(str, "LED_%i", i);
		shape_filler->setName(str);
	}
	return o;
}
class CShape *PrefabManager::generateStrip_CW() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->addText(-40, -25, "Strip CW");
	o->setName("StripCW");
	int w = 200;
	o->addRect(-w, -20, w * 2, 40);
	int leds = 8;
	int ofs = 10;
	float start = ofs - w;
	float len = 2 * w - ofs * 2;
	float ledlen = len / (leds + 1);
	CJunction *warm = o->addJunction(-w, 20);
	warm->setName("W");
	warm->addText(-5, -5, "W");
	CJunction *cool = o->addJunction(-w, -20);
	cool->setName("C");
	cool->addText(-5, -5, "C");
	CJunction *gnd = o->addJunction(w, 0);
	gnd->setName("GND");
	gnd->addText(-25, -5, "GND");
	CControllerBulb *bulb = new CControllerBulb(cool, warm, gnd);
	o->setController(bulb);
	for (int i = 0; i < leds; i++) {
		float x = start + (len / leds) * i;
		float y = -15;
		int filler_ofs = 1;
		CShape *shape_outer = o->addRect(x, y, ledlen, 30);
		CShape *shape_filler = o->addRect(x + filler_ofs, y + filler_ofs, ledlen - 2 * filler_ofs, 30 - 2 * filler_ofs);
		shape_filler->setFill(true);
		bulb->addShape(shape_filler);
		char str[16];
		sprintf(str, "LED_%i", i);
		shape_filler->setName(str);
	}
	return o;
}

class CShape *PrefabManager::generateStrip_RGB() {
	float bulb_radius = 20.0f;

	CShape *o = new CShape();
	o->addText(-40, -25, "Strip RGB");
	o->setName("StripRGB");
	int w = 200;
	o->addRect(-w, -20, w * 2, 40);
	int leds = 8;
	int ofs = 10;
	float start = ofs - w;
	float len = 2 * w - ofs * 2;
	float ledlen = len / (leds + 1);
	CJunction *red = o->addJunction(-w, 20);
	red->setName("R");
	red->addText(-5, -5, "R");
	CJunction *green = o->addJunction(-w, 0);
	green->setName("G");
	green->addText(-5, -5, "G");
	CJunction *blue = o->addJunction(-w, -20);
	blue->setName("B");
	blue->addText(-5, -5, "B");
	CJunction *gnd = o->addJunction(w, 0);
	gnd->setName("GND");
	gnd->addText(-25, -5, "GND");
	CControllerBulb *bulb = new CControllerBulb(red, green, blue, gnd);
	o->setController(bulb);
	for (int i = 0; i < leds; i++) {
		float x = start + (len / leds) * i;
		float y = -15;
		int filler_ofs = 1;
		CShape *shape_outer = o->addRect(x, y, ledlen, 30);
		CShape *shape_filler = o->addRect(x + filler_ofs, y + filler_ofs, ledlen - 2 * filler_ofs, 30 - 2 * filler_ofs);
		shape_filler->setFill(true);
		bulb->addShape(shape_filler);
		char str[16];
		sprintf(str, "LED_%i", i);
		shape_filler->setName(str);
	}
	return o;
}
void PrefabManager::addPrefab(CShape *o) {
	prefabs.add_unique(o);
}
CShape *PrefabManager::findPrefab(const char *name) {
	for (int i = 0; i < prefabs.size(); i++) {
		if (prefabs[i]->hasName(name))
			return prefabs[i];
	}
	return 0;
}
CShape *PrefabManager::instantiatePrefab(const char *name) {
	CShape *ret = findPrefab(name);
	if (ret == 0)
		return 0;
	return ret->cloneShape();
}
void PrefabManager::createDefaultPrefabs() {

	
	addPrefab(generateStrip_SingleColor());
	addPrefab(generateStrip_CW());
	addPrefab(generateStrip_RGB());
	addPrefab(generateLED_CW());
	addPrefab(generateLED_RGB());
	addPrefab(generateBulb());
	addPrefab(generateWB3S());
	addPrefab(generateButton());
	addPrefab(generateTest());
	addPrefab(generateGND());
	addPrefab(generateVDD());
}


#endif

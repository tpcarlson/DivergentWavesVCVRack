#include "plugin.hpp"

enum LogicalOperator {
	AND,
	NAND,
	OR,
	XOR,
	NOR,
	XNOR
};

struct Logic : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		_1_A_INPUT,
		_1_B_INPUT,
		_2_A_INPUT,
		_2_B_INPUT,
		_3_A_INPUT,
		_3_B_INPUT,
		_4_A_INPUT,
		_4_B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		_1_OUT_OUTPUT,
		_2_OUT_OUTPUT,
		_3_OUT_OUTPUT,
		_4_OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		NAND_LIGHT,
		AND_LIGHT,
		XOR_LIGHT,
		XNOR_LIGHT,
		OR_LIGHT,
		NOR_LIGHT,
		_1_A_LED_LIGHT,
		_1_B_LED_LIGHT,
		_1_OUT_LED_LIGHT,
		_2_A_LED_LIGHT,
		_2_B_LED_LIGHT,
		_2_OUT_LED_LIGHT,
		_3_A_LED_LIGHT,
		_3_B_LED_LIGHT,
		_3_OUT_LED_LIGHT,
		_4_A_LED_LIGHT,
		_4_B_LED_LIGHT,
		_4_OUT_LED_LIGHT,
		LIGHTS_LEN
	};



	LogicalOperator logicalOperator = AND;

	LogicalOperator getLogicalOperator() {
		return logicalOperator;
	}

	void setLogicalOperator(LogicalOperator newLogicalOperator) {
		logicalOperator = newLogicalOperator;
	}

	bool performLogicalOperation(bool valA, bool valB) {
		switch(logicalOperator) {
			case AND:
				return valA && valB;
			case NAND:
				return !(valA && valB);
			case OR:
				return valA || valB;
			case XOR:
				return !valA != !valB;
			case NOR:
				return !(valA || valB);
			case XNOR:
				return valA == valB;
			default:
				return valA;
		}
	}

	Logic() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(_1_A_INPUT, "1A");
		configInput(_1_B_INPUT, "1B");
		configInput(_2_A_INPUT, "2A");
		configInput(_2_B_INPUT, "2B");
		configInput(_3_A_INPUT, "3A");
		configInput(_3_B_INPUT, "3B");
		configInput(_4_A_INPUT, "4A");
		configInput(_4_B_INPUT, "4B");
		configOutput(_1_OUT_OUTPUT, "1");
		configOutput(_2_OUT_OUTPUT, "2");
		configOutput(_3_OUT_OUTPUT, "3");
		configOutput(_4_OUT_OUTPUT, "4");
	}

	void process(const ProcessArgs& args) override {
		lights[NAND_LIGHT].setBrightness(logicalOperator == NAND ? 1.0f : 0.0f);
		lights[AND_LIGHT].setBrightness(logicalOperator == AND ? 1.0f : 0.0f);
		lights[XOR_LIGHT].setBrightness(logicalOperator == XOR ? 1.0f : 0.0f);
		lights[XNOR_LIGHT].setBrightness(logicalOperator == XNOR ? 1.0f : 0.0f);
		lights[OR_LIGHT].setBrightness(logicalOperator == OR ? 1.0f : 0.0f);
		lights[NOR_LIGHT].setBrightness(logicalOperator == NOR ? 1.0f : 0.0f);
		
		// Row 1
		bool val1A = inputs[_1_A_INPUT].getVoltage() > 1.2f;
		bool val1B = inputs[_1_B_INPUT].getVoltage() > 1.2f;
		bool out1 = performLogicalOperation(val1A, val1B); //!val1A != !val1B; // xor
		outputs[_1_OUT_OUTPUT].setVoltage(out1 ? 10.0f : 0.0f);
		lights[_1_OUT_LED_LIGHT].setBrightness(out1 ? 1.0f : 0.0f);
		lights[_1_A_LED_LIGHT].setBrightness(val1A ? 1.0f : 0.0f);
		lights[_1_B_LED_LIGHT].setBrightness(val1B ? 1.0f : 0.0f);

		// Row 2
		bool val2A = inputs[_2_A_INPUT].getNormalVoltage(inputs[_1_A_INPUT].getVoltage()) > 1.2f;
		bool val2B = inputs[_2_B_INPUT].getNormalVoltage(inputs[_1_B_INPUT].getVoltage()) > 1.2f;
		bool out2 = performLogicalOperation(val2A, val2B);
		outputs[_2_OUT_OUTPUT].setVoltage(out2 ? 10.0f : 0.0f);
		lights[_2_OUT_LED_LIGHT].setBrightness(out2 ? 1.0f : 0.0f);
		lights[_2_A_LED_LIGHT].setBrightness(val2A ? 1.0f : 0.0f);
		lights[_2_B_LED_LIGHT].setBrightness(val2B ? 1.0f : 0.0f);

		// Row 3
		bool val3A = inputs[_3_A_INPUT].getNormalVoltage(
			     inputs[_2_A_INPUT].getNormalVoltage(
		             inputs[_1_A_INPUT].getVoltage())) > 1.2f;
		bool val3B = inputs[_3_B_INPUT].getNormalVoltage(
			     inputs[_2_B_INPUT].getNormalVoltage(
		             inputs[_1_B_INPUT].getVoltage())) > 1.2f;
		bool out3 = performLogicalOperation(val3A, val3B);
		outputs[_3_OUT_OUTPUT].setVoltage(out3 ? 10.0f : 0.0f);
		lights[_3_OUT_LED_LIGHT].setBrightness(out3 ? 1.0f : 0.0f);
		lights[_3_A_LED_LIGHT].setBrightness(val3A ? 1.0f : 0.0f);
		lights[_3_B_LED_LIGHT].setBrightness(val3B ? 1.0f : 0.0f);

		// Row 4
		bool val4A = inputs[_4_A_INPUT].getNormalVoltage(
			     inputs[_3_A_INPUT].getNormalVoltage(
			     inputs[_2_A_INPUT].getNormalVoltage(
		             inputs[_1_A_INPUT].getVoltage()))) > 1.2f;
		bool val4B = inputs[_4_B_INPUT].getNormalVoltage(
			     inputs[_3_B_INPUT].getNormalVoltage(
			     inputs[_2_B_INPUT].getNormalVoltage(
		             inputs[_1_B_INPUT].getVoltage()))) > 1.2f;
		bool out4 = performLogicalOperation(val4A, val4B);
		outputs[_4_OUT_OUTPUT].setVoltage(out4 ? 10.0f : 0.0f);
		lights[_4_OUT_LED_LIGHT].setBrightness(out4 ? 1.0f : 0.0f);
		lights[_4_A_LED_LIGHT].setBrightness(val4A ? 1.0f : 0.0f);
		lights[_4_B_LED_LIGHT].setBrightness(val4B ? 1.0f : 0.0f);
	}

	void dataFromJson(json_t* rootJ) override {
		json_t* targetJ = json_object_get(rootJ, "operator");
		// During the first load, the data blob won't be present
		if(targetJ) {
			setLogicalOperator(static_cast<LogicalOperator>(json_integer_value(targetJ)));
		}
	}
	
	json_t* dataToJson() override {
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "operator", json_integer(getLogicalOperator()));
		return rootJ;
	}
};


struct LogicWidget : ModuleWidget {
	LogicWidget(Logic* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Logic.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4.849, 24.967)), module, Logic::_1_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.188, 25.046)), module, Logic::_1_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.452, 44.89)), module, Logic::_2_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.034, 45.016)), module, Logic::_2_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.339, 87.381)), module, Logic::_3_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.678, 87.46)), module, Logic::_3_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4.697, 107.523)), module, Logic::_4_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.037, 107.602)), module, Logic::_4_B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.425, 25.122)), module, Logic::_1_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.921, 44.913)), module, Logic::_2_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.915, 87.381)), module, Logic::_3_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.273, 107.523)), module, Logic::_4_OUT_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(2.719, 13.365)), module, Logic::AND_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(8.569, 13.38)), module, Logic::NAND_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(14.595, 13.363)), module, Logic::OR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(20.181, 13.402)), module, Logic::XOR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(26.118, 13.36)), module, Logic::NOR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(32.046, 13.357)), module, Logic::XNOR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(8.067, 19.707)), module, Logic::_1_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(16.877, 19.697)), module, Logic::_1_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(32.409, 19.7)), module, Logic::_1_OUT_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(8.006, 39.542)), module, Logic::_2_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(17.345, 39.544)), module, Logic::_2_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(32.955, 39.57)), module, Logic::_2_OUT_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(8.157, 81.872)), module, Logic::_3_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(17.496, 81.859)), module, Logic::_3_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(33.209, 81.891)), module, Logic::_3_OUT_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(7.088, 101.711)), module, Logic::_4_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(16.427, 101.708)), module, Logic::_4_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(32.76, 101.716)), module, Logic::_4_OUT_LED_LIGHT));
	}

	void appendContextMenu(Menu* menu) override {
		Logic* module = dynamic_cast<Logic*>(this->module);

		menu->addChild(new MenuSeparator);
		menu->addChild(createMenuLabel("Logic Operation"));
		menu->addChild(createBoolMenuItem("AND", "",
    		[=]() {
        		return module->getLogicalOperator() == AND;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(AND);
    		}));

		menu->addChild(createBoolMenuItem("NAND", "",
    		[=]() {
        		return module->getLogicalOperator() == NAND;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(NAND);
    		}));

		menu->addChild(createBoolMenuItem("OR", "",
    		[=]() {
        		return module->getLogicalOperator() == OR;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(OR);
    		}));

		menu->addChild(createBoolMenuItem("XOR", "",
    		[=]() {
        		return module->getLogicalOperator() == XOR;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(XOR);
    		}));

		menu->addChild(createBoolMenuItem("NOR", "",
    		[=]() {
        		return module->getLogicalOperator() == NOR;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(NOR);
    		}));

		menu->addChild(createBoolMenuItem("XNOR", "",
    		[=]() {
        		return module->getLogicalOperator() == XNOR;
    		},
    		[=](bool ignored) {
        		module->setLogicalOperator(XNOR);
    		}));
	}
};


Model* modelLogic = createModel<Logic, LogicWidget>("Logic");
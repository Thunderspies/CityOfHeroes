#############################################################
## Child_RandomSparks1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	10
	Chance	0.1

	Event
		Type	PositOnly
		At	LArmL
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_04 60 60 .4
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	11
	Chance	0.1

	Event
		Type	PositOnly
		At	LArmr
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_01 60 60 .4
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.1

	Event
		Type	PositOnly
		At	LLegL
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_03 60 60 .4
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	10
	Chance	0.1

	Event
		Type	PositOnly
		At	LLegR
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_02 60 60 .4
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	11
	Chance	0.1

	Event
		Type	PositOnly
		At	Head
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_01 60 60 .4
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.1

	Event
		Type	PositOnly
		At	WepR
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_03 60 60 .4
		Lifespan 1
	End
End


Condition
	On 	Cycle
	Time 	10
	Chance	0.1

	Event
		Type	PositOnly
		At	WepL
		Part	:SmallSparks1.part
		Part	:SmallSparks_Flash1.part
		Part	:SmallSparks_Smoke1.part
		Sound RobotSelfDestructSparks_02 60 60 .4
		Lifespan 1
	End
End

#############################################################
End
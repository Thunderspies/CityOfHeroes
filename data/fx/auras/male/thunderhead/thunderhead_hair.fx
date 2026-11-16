#############################################################
## Thunderhead_Body.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:Cloud_Trail_Small.part
		part	:BodyCloud.part
		Part	:BodyCloud_Rear.part
		POther  Neck
	End
End


Condition
	On	Cycle
	Time	10
	Chance	0.1

	Event
		Type	Start
		At	Head
		Part	:Body_Flash_Small.part
		Part	:Body_Flash_Highlight_Small.part
		POther	Neck
		Lifespan	4
		LifespanJitter	2
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.2

	Event
		Type	Local
		At	Head
		Part	:Body_Flash_Trail_Small.part
		Part	:Body_Flash_Highlight_Trail_Small.part
		POther	Neck
		Lifespan	4
		LifespanJitter	2
	End
End

#############################################################

Condition
	On	Cycle
	Time	36
	Chance	0.1
	Random 	1

	Event
		Type	PositOnly
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Arcs_Small.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan 8
	End

	Event
		Type	PositOnly
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Arcs_Small.part
		Pmagnet UArmR
		POther  UArmR
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	29
	Chance	0.1

	Event
		Type	PositOnly
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		#part	:Body_Glows_Large2.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet Neck
		POther  Neck
		Lifespan 8
	End
End

#############################################################

End
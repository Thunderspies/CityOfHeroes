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
		At	LArmR
		part	:BodyCloud.part
		POther  UArmR
	End

	Event
		Type	Local
		At	HandR
		part	:BodyCloud.part
		Part	:BodyCloud_Rear.part
		part	:Cloud_Trail_Small.part
		POther  LArmR
	End

End

Condition
	On	Cycle
	Time	15
	Chance	0.15

	Event
		Type	Start
		At	UArmR
		Part	:Body_Flash_Small.part
		Part	:Body_Flash_Highlight_Small.part
		POther	HandR
		Lifespan	4
		LifespanJitter	2
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.35

	Event
		Type	Local
		At	UArmR
		Part	:Body_Flash_Trail_Small.part
		Part	:Body_Flash_Highlight_Trail_Small.part
		POther	HandR
		Lifespan	4
		LifespanJitter	2
	End
End

#############################################################

Condition
	On	Cycle
	Time	40
	Chance	0.05

	Event
		Type	PositOnly
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet HandR
		POther  HandR
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	29
	Chance	0.05

	Event
		Type	PositOnly
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet UArmR
		POther  UArmR
		Lifespan 8
	End
End

#############################################################

End
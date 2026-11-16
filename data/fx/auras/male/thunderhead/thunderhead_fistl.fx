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
		At	LArmL
		part	:BodyCloud.part
		POther  UArmL
	End

	Event
		Type	Local
		At	HandL
		part	:BodyCloud.part
		Part	:BodyCloud_Rear.part
		part	:Cloud_Trail_Small.part
		POther  LArmL
	End

End

Condition
	On	Cycle
	Time	15
	Chance	0.15

	Event
		Type	Start
		At	UArmL
		Part	:Body_Flash_Small.part
		Part	:Body_Flash_Highlight_Small.part
		POther	HandL
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
		At	UArmL
		Part	:Body_Flash_Trail_Small.part
		Part	:Body_Flash_Highlight_Trail_Small.part
		POther	HandL
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
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet HandL
		POther  HandL
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	29
	Chance	0.05

	Event
		Type	PositOnly
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan 8
	End
End

#############################################################

End
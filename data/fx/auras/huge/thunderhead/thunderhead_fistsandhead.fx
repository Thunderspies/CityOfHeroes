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
		At	Root
		ChildFX	:Thunderhead_Eyes.fx
	End

	Event
		Type	Local
		At	Head
		part	:Cloud_Trail_Small.part
		part	:BodyCloud.part
		Part	:BodyCloud_Rear.part
		POther  Neck
	End

	Event
		Type	Local
		At	LArmL
		part	:BodyCloud.part
		POther  UArmL
	End

	Event
		Type	Local
		At	LArmR
		part	:BodyCloud.part
		POther  UArmR
	End

	Event
		Type	Local
		At	HandL
		part	:BodyCloud.part
		Part	:BodyCloud_Rear.part
		part	:Cloud_Trail_Small.part
		POther  LArmL
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
	Time	20
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
	Chance	0.11

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
	Time	29
	Chance	0.06

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

Condition
	On	Cycle
	Time	31
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
	Time	36
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

Condition
	On	Cycle
	Time	29
	Chance	0.05

	Event
		Type	PositOnly
		At	HandL
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
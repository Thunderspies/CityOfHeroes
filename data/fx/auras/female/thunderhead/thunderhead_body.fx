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
		At	Hair
		part	:BodyCloud.part
		part	:BodyCloud_Rear.part
		POther  Chest
	End

	Event
		Type	Local
		At	Chest
		part	:Cloud_Trail.part
	End

	Event
		Type	Local
		At	UArmL
		part	:BodyCloud.part
		part	:BodyCloud_Rear.part
		POther 	Neck
	End

	Event
		Type	Local
		At	UArmR
		part	:BodyCloud.part
		part	:BodyCloud_Rear.part
		POther 	Neck
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
		part	:Cloud_Trail.part
		part	:BodyCloud.part
		POther  UArmR
	End

	Event
		Type	Local
		At	HandL
		part	:BodyCloud.part
		POther  LArmL
	End

	Event
		Type	Local
		At	Hips
		part	:BodyCloud.part
		part	:BodyCloud_Rear.part
		part	:Cloud_Trail.part
		POther 	Chest
	End

	Event
		Type	Local
		At	HandR
		part	:BodyCloud.part
		POther  LArmR
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	ULegL
		part	:BodyCloud.part
		POther  LLegL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	ULegR
		part	:BodyCloud.part
		POther  LLegR
	End

	Event
		Type	Local
		At	LLegL
		part	:BodyCloud.part
		Part	:Cloud_Trail_Small.part
		POther  FootL
	End

	Event
		Type	Local
		At	LLegR
		part	:BodyCloud.part
		Part	:Cloud_Trail_Small.part
		POther  FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodyCloud.part
		POther	ULegL
	End

	Event
		Type	Local
		At	FootR
		part	:BodyCloud.part
		POther  ULegR
	End
End

Condition
	On	Cycle
	Time	45
	Chance	.3

	Event
		Type	Local
		At	FootR
		Part	:Body_Flash.part
		Part	:Body_Flash_Highlight.part
		POther	ULegR
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
		At	FootL
		Part	:Body_Flash.part
		Part	:Body_Flash_Highlight.part
		POther	ULegL
		Lifespan	4
		LifespanJitter	2
	End

	Event
		Type	Local
		At	Head
		Part	:Body_Flash_Trail.part
		Part	:Body_Flash_Highlight_Trail.part
		Lifespan 	4
		LifespanJitter	2
		POther	Root
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.3

	Event
		Type	Local
		At	Chest
		Part	:Body_Flash.part
		Part	:Body_Flash_Highlight.part
		POther	Hips
		Lifespan	4
		LifespanJitter	2
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.07

	Event
		Type	Local
		At	UArmL
		Part	:Body_Flash.part
		Part	:Body_Flash_Highlight.part
		POther	LArmL
		Lifespan	4
		LifespanJitter	2
	End
End

Condition
	On	Cycle
	Time	18
	Chance	0.06

	Event
		Type	Local
		At	UArmR
		Part	:Body_Flash.part
		Part	:Body_Flash_Highlight.part
		POther	LArmR
		Lifespan	4
		LifespanJitter	2
	End
End


#############################################################

Condition
	On	Cycle
	Time	25
	Chance	0.05

	Event
		Type	PositOnly
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet UArmR
		POther  UArmR
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	35
	Chance	0.065

	Event
		Type	PositOnly
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Arcs_Small.part
		part	:Body_Arcs_Small2.part
		Pmagnet UArmL
		POther  UArmL
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

Condition
	On	Cycle
	Time	25
	Chance	0.05

	Event
		Type	PositOnly
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Large.part
		part	:Body_Arcs_Large2.part
		Pmagnet ULegL
		POther  ULegL
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	30
	Chance	0.05

	Event
		Type	PositOnly
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Body_Glows_Large.part
		part	:Body_Glows_Large2.part
		part	:Body_Arcs_Large.part
		part	:Body_Arcs_Large2.part
		Pmagnet ULegR
		POther  ULegR
		Lifespan 8
	End
End

#############################################################

End
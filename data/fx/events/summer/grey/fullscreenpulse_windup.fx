#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	UArmL
	#	Part	:Nuke_Body_Flash.part
	#	Part	:Nuke_Body_Shockwave.part
	#	Part	:Nuke_Body_Smoke.part
		Lifespan	12
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	WepR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	FootR
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LArmL
	End


	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LLegR
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow_Big.part
		Lifespan	120
		POther	Hips
	End
End

#############################################################

End

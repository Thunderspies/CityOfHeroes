#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Chest.part
		POther	Neck
		Lifespan 1
	End

#############################################################
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Body_Big.part
		Part	:Glows_Static_Body_Big.part
		POther	Chest
		Lifespan 1
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Body_Big.part
		Part	:Glows_Static_Body_Big.part
		POther	Chest
		Lifespan 1
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Body_Big.part
		Part	:Glows_Static_Body_Big.part
		POther	Neck
		Lifespan 1
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Body_Big.part
		Part	:Glows_Static_Body_Big.part
		POther	Neck
		Lifespan 1
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Body_Big.part
		Part	:Glows_Static_Body_Big.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Static_Head.part
		Lifespan 10
	End
End

#Condition
#	On	Time
#	Time	15
#
#	Event
#		Type	Local
#		At	Neck
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	UArmL
#		Lifespan 60
#	End
#
#	Event
#		Type	Local
#		At	Neck
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	UArmR
#		Lifespan 60
#	End
#End
#
#Condition
#	On	Time
#	TIme	25
#
#	Event
#		Type	Local
#		At	UArmL
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	LarmL
#		Lifespan 60
#	End
#
#	Event
#		Type	Local
#		At	UArmR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	LarmR
#		Lifespan 60
#	End
#End
#
#Condition
#	On	Time
#	Time	35
#
#	Event
#		Type	Local
#		At	LarmL
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	WepL
#		Lifespan 60
#	End
#
#	Event
#		Type	Local
#		At	LarmR
#		Bhvr	behaviors/GenericParticleFade.bhvr
#		Part	:Glows_Static_Body.part
#		POther	WepR
#		Lifespan 60
#	End
#End



#############################################################

End
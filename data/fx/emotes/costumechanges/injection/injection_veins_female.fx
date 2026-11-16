#############################################################
## Injection_Veins.fx
#############################################################

FxInfo

Lifespan 330
#############################################################

Condition
	On Time
	Time 	5
	
	Event
		Type	Local
		At	Root
		sound	Emote_SSInjectionSheild 200 190 1
	End
End

#############################################################

Condition
	On	Time
	Time	12

	Event
		EName	NodeSpin
		Type	Local
		At	WepR

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumPos.bhvr
		Geom	GEO_BoostSerum
		Lifespan 125
	End
End

#############################################################

Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	Root
		ChildFX	:BodyGlow.fx
	End
End

#############################################################

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Veins_Female.part
		POther  LArmL
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Veins_Female.part
		POther  WepL
		Lifespan 60
	End
End

Condition
	On	Time
	Time 	80

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Veins_Female.part
		POther  LArmR
		Lifespan 15
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Veins_Female.part
		POther  HandR
		Lifespan 20
	End
End

Condition
	On	Time
	Time 	90

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Veins_Legs_Female.part
		POther  LLegL
		Lifespan 10
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Veins_Legs_Female.part
		POther  FootL
		Lifespan 10
	End
End

Condition
	On	Time
	Time 	95
	Event
		ENAME	RightUarmGlow
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Veins_Legs_Female.part
		POther  LLegR
		Lifespan 10
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Veins_Legs_Female.part
		POther  FootR
		Lifespan 10
	End
End

#Condition
#	On	Time
#	Time	120
#
#	Event
#		Type	Local
#		At	Root
#		ChildFX	:BodyGlow2_Female.fx
#	End
#End

#############################################################

End
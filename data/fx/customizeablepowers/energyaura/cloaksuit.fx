#########################################################
##	template

FxInfo

LifeSpan 30


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound rumble1 80 80 .8
	End
End


Condition
	On	Time
	Time	10


	Event
		Type	Local
		At 	Origin
		Sound invisible1 80 80 .5
	End
End
#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

End

Condition
	On	Time
	Time	18

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

End

Condition
	On	Time
	Time	5

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow02.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow01.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 23

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	CustomizeablePowers\EnergyAura\CloakSuitGlow01.part
		part	CustomizeablePowers\EnergyAura\CloakSuitGlowBlue.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 23

	End


End


End
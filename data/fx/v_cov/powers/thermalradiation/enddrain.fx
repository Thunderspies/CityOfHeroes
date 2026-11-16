#############################################################
## HEADER
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepR
		Sound steam3_loop 60.0 60.0 0.11
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	local
		At	WepR
		Sound melt3_loop 60.0 60.0 0.22
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	neck
		part	V_COV\Powers\ThermalRadiation\Rainfall.part
	End

	Event
		Type	Local
		At	UARML
		part	V_COV\Powers\ThermalRadiation\RainfallArms.part
	End

	Event
		Type	Local
		At	UARMR
		part	V_COV\Powers\ThermalRadiation\RainfallArms.part
	End

	Event
		Type	Local
		At	WepL
		part	V_COV\Powers\ThermalRadiation\RainfallArms.part
	End

	Event
		Type	Local
		At	WepR
		part	V_COV\Powers\ThermalRadiation\RainfallArms.part
	End

	Event
		Type	Local
		At	LLEGL
		part	V_COV\Powers\ThermalRadiation\RainfallArms2.part
	End

	Event
		Type	Local
		At	LLEGR
		part	V_COV\Powers\ThermalRadiation\RainfallArms2.part
	End

	Event
		Type	Local
		At	Root
		part	V_COV\Powers\ThermalRadiation\Steam.part
	End
End

##########################################################################

Condition
	On	cycle
	Time	10
	Chance	.2

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End


Condition
	On	cycle
	Time	7
	Chance	.2

	Event
		Type	local
		At	UARML
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	8
	Chance	.2

	Event
		Type	local
		At	UARMR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.2

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	9
	Chance	.2

	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	7
	Chance	.2

	Event
		Type	local
		At	LLEGL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	8
	Chance	.2

	Event
		Type	local
		At	LLEGR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	9
	Chance	.2

	Event
		Type	local
		At	ULEGL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	8
	Chance	.2

	Event
		Type	local
		At	ULEGR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.2

	Event
		Type	local
		At	LArmL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	8
	Chance	.2

	Event
		Type	local
		At	LArmR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\DripWellupInvert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup_Invert.part
		part	V_COV\Powers\ThermalRadiation\DripWellup.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

Condition
	On	cycle
	Time	10
	Chance	.1

	Event
		Type	local
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part	V_COV\Powers\ThermalRadiation\Drips.part
		part	V_COV\Powers\ThermalRadiation\Metal.part
		LifeSpan 20
	End
End

#############################################################

End
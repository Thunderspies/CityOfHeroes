#############################################################
## ENDDrain.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepR
		Sound steam3_loop 60.0 60.0 .11
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	local
		At	WepR
		Sound melt3_loop 60.0 60.0 .22
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	neck
		part  :Rainfall.part
	End

	Event
		Type	Local
		At	UARML
		part   :RainfallArms.part
	End

	Event
		Type	Local
		At	UARMR
		part   :RainfallArms.part
	End

	Event
		Type	Local
		At	WepL
		part  :RainfallArms.part
	End

	Event
		Type	Local
		At	WepR
		part  :RainfallArms.part
	End

	Event
		Type	Local
		At	LLEGL
		part   :RainfallArms2.part
	End

	Event
		Type	Local
		At	LLEGR
		part   :RainfallArms2.part
	End

	Event
		Type	Local
		At	Root
		part  	:Steam.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part 	:DripWellupInvert.part
		part	:DripWellup_Invert.part
		part    :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :DripWellupInvert.part
		part   :DripWellup_Invert.part
		part   :DripWellup.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :Drips.part
		part   :Metal.part
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
		part   :Drips.part
		part   :Metal.part
		LifeSpan 20
	End
End

#############################################################

End
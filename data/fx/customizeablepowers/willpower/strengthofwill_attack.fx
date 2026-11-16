#########################################################
##  Willpower - Activation FX
#########################################################

FxInfo

Lifespan 90

#########################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Start
		At	Root
		Sound StrengthOfWill 50 50 .99
	End

End

Condition
	On	Time
	Time	0
	
	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:WillPower_Continuing_Pulse.fx
		Lifespan	60
	End
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ChestOffset
		Type	Local 
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		Part	CustomizeablePowers\Willpower\WillPower_Activation_CoreGlow1.part
		##Part	:WillPower_Activation_CoreRing1.part
		Part	CustomizeablePowers\Willpower\WillPower_Activation_CoreStar1.part
		Sound reconstruction 60 60 .8
		pOther	Offset1
		Lifespan	35
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	ChestOffsetForward
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		0 0 2
		End
	End

	Event
		EName	ChestOffsetBackward
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		0 0 -2
		End
	End

	Event
		EName	ChestOffsetLeft
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		-2 0 0
		End
	End

	Event
		EName	ChestOffsetRight
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		2 0 0
		End
	End

	Event
		EName	ChestOffsetUp
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		0 2 0
		End
	End

	Event
		EName	ChestOffsetDown
		Type	Local 
		At	ChestOffset
		BhvrOverride
			Scale			0.25 0.25 0.25
			PositionOffset		0 -2 0
		End
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetForward
		Lifespan	20
	End

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetBackward
		Lifespan	20
	End

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetLeft
		Lifespan	20
	End

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetRight
		Lifespan	20
	End

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetUp
		Lifespan	20
	End

	Event
		Type	Local 
		At	ChestOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	CustomizeablePowers\Willpower\WillPower_Activation_Ray1.part
		pOther	ChestOffsetDown
		Lifespan	20
	End
End

#########################################################

End			
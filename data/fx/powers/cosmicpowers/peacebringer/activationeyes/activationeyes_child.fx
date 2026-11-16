#############################################################
## Peacebringer - Glowing Eyes Child FX
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 		Triggerbits
	Triggerbits	MALE
	Domany		1

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		Lifespan	120
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	1
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
		Lifespan	90
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
		Lifespan	90
	End
End

Condition
	On 		Triggerbits
	Triggerbits	FEMALE
	Domany		1

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		BhvrOverride
			PositionOffset 0 0.05 -0.08
		End
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		Lifespan	120
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	1
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
		Lifespan	90
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
		Lifespan	90
	End
End

Condition
	On 		Triggerbits
	Triggerbits	HUGE
	Domany		1

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		BhvrOverride
			PositionOffset 0 0 -0.13
		End
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		Lifespan	120
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	1
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
		Lifespan	90
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
		Lifespan	90
	End
End

#############################################################

End
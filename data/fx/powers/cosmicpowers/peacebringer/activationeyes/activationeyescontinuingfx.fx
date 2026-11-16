#############################################################
## Peacebringer - Glowing Eyes Continuing FX
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 		Triggerbits
	Triggerbits	MALE
	Domany		1

	Event
		EName 	Sound
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		bhvr	behaviors/soundFade1.bhvr
		sound	glow9b_loop 50 40 .21
		#Lifespan	150
	End

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		Flags	OneAtATime
		Until	SHAPESHIFT
		AltPiv	1
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
	End
End

Condition
	On 		Triggerbits
	Triggerbits	FEMALE
	Domany		1

	Event
		EName 	Sound
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		bhvr	behaviors/soundFade1.bhvr
		sound	glow9b_loop 50 40 .21
		#Lifespan	150
	End

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		BhvrOverride
			PositionOffset 0 0.03 -0.08
		End
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		Flags	OneAtATime
		Until	SHAPESHIFT
		AltPiv	1
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
	End
End

Condition
	On 		Triggerbits
	Triggerbits	HUGE
	Domany		1

	Event
		EName 	Sound
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		bhvr	behaviors/soundFade1.bhvr
		sound	glow9b_loop 50 40 .21
		#Lifespan	150
	End

	Event
		EName 	EyeOffset
		Type	Local
		At	Eyes
		Flags	OneAtATime
		Until	SHAPESHIFT
		BHVR	Behaviors/OffsetEyes.bhvr
		BhvrOverride
			Scale 1.2 1 1
			PositionOffset 0 -0.03 -0.13
		End
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
	End

	Event
		EName 	LeftEyeGlow
		Type	Local
		At	EyeOffset
		Flags	OneAtATime
		Until	SHAPESHIFT
		AltPiv	1
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistLeftBLUE.part
	End

	Event
		EName 	RightEyeGlow
		Type	Local
		At	EyeOffset
		AltPiv	2
		Flags	OneAtATime
		Until	SHAPESHIFT
		Part	:CircleOfThornsEyes_21Green.part
		Part	:EyesMistRightBLUE.part
	End
End

#############################################################

End
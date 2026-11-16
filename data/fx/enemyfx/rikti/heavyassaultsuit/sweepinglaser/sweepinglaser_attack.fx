#########################################################
## Rikti Heavy Assault Suit - Sweeping Laser Attack	
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	HeadOffset
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 0.75 0.75
		End
	End
End

Condition
	On	time
	time	6

	Event
		Type	Local
		At	HeadOffset
		Sound xray5 80 80 .9
	End
End

## LASER SWEEP 1 #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LookAtTarget
		Type	Start
		At	T_Chest
		LookAt	HeadOffset
	End

	Event
		Ename	LaserFlash
		Type	Local
		At	HeadOffset
		Part	:Flash_Glow1.Part
		Part	:Flash_Rings1.Part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	SweepTargetL
		Type	Local
		At	LookAtTarget
		BhvrOverride
			PositionOffset	10 0 0
			InitialVelocity	-0.333 0.0 0.0
		End
		Lifespan	30
	End

	Event
		Ename	LaserSweepFlareL
		Type	Local
		At	SweepTargetL
		Part	:Beam_Flare1.Part
		Part	:Beam_Flare2.Part
		Part	:Beam_Trail1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepL
		Type	Local
		At	SweepTargetL
		Part	:Beam_Core1.Part
		Part	:Beam_Glow1.Part
		pOther	HeadOffset
		Lifespan	30
	End
End

## LASER SWEEP 2 #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	SweepTargetR
		Type	Local
		At	LookAtTarget
		BhvrOverride
			PositionOffset	-10 0 0
			InitialVelocity	0.333 0.0 0.0
		End
		Lifespan	30
	End

	Event
		Ename	LaserSweepFlareR
		Type	Local
		At	SweepTargetR
		Part	:Beam_Flare1.Part
		Part	:Beam_Flare2.Part
		Part	:Beam_Trail1.Part
		Lifespan	30
	End

	Event
		Ename	LaserSweepR
		Type	Local
		At	SweepTargetR
		Part	:Beam_Core1.Part
		Part	:Beam_Glow1.Part
		pOther	HeadOffset
		Lifespan	30
	End
End

## LASER FAN #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	BeamOffset
		Type	Start
		At	T_Chest
		LookAt	Head
	End

	Event
		Ename	BeamScatter
		Type	Local
		At	BeamOffset
		#Part	:Beams1.Part
		Part	:Beams2.Part
		pother	Head
		Lifespan	30
	End
End

## CLEAN UP #######################################################

#Condition
#	On	Time
#	Time	20
#
#	Event
#		Ename	LaserSweep
#		Type	Destroy
#	End
#End

#########################################################

End
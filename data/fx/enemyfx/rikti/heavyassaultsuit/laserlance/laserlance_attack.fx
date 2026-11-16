#########################################################
## Rikti Heavy Assault Suit - Laser Lance Attack
#########################################################

FxInfo

LifeSpan	200

#########################################################

Condition
	On	time
	time	6

	Event
		Ename	HeadOffset
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 0.75 0.75
		End
		Sound xray5 80 80 .9
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	HeadOffset
		Bhvr	:Beam.bhvr		
		Part	:Beam_Glow1.Part
		Part	:Beam_Rings1.Part
	End

	Event
		Type	Local
		At	HeadOffset
		Bhvr	:Beam.bhvr		
		Part	:Beam_Glow2.Part
		Part	:Beam_Core1.Part
		pOther	Target
		Lifespan	25
	End

#	Event
#		Type	Local
#		At	HeadOffset
#		Bhvr	behaviors/XRayBeam2.bhvr
#		BhvrOverride
#			Scale		0.25 0.25 1.0
#			ScaleRate	1.0 1.0 1.0
#			EndScale	0.25 0.25 1.0
#			Alpha		255
#			StartColor	255 255 255
#		End
#		geom	XrayBeamYellow
#		Magnet	Target
#		LookAt	Target
#		LifeSpan	25
#	End
End

#########################################################

End
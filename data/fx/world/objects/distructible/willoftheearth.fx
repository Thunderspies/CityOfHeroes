#########################################################
##	Fireweapon
########################################################

FxInfo

Flags InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Root
		Sound grexplo4 60 60 .66
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:Splat_WillOfTheEarth.bhvr
		splat	FadeBorder.tga
		Sound miasma_loop 50 50 .25
	End
End

Condition
	On 	Time
	Time 	1
#	Chance	1

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:Splat_WillOfTheEarth.bhvr
		#splat	FadeBorder.tga
		Sound miasma_loop 50 50 .25

#		LifeSpan	20
	End
End

########################################################

End
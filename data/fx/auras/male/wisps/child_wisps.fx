#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	SpinnerAnchor
		Type	Local
		At	Root
		BhvrOverride
			Spin	0 -0.115 0
		End
	End

	Event
		EName	OverheadAnchor
		Type	Local
		At	SpinnerAnchor
		BhvrOverride
			PositionOffset	-5.0 12.85 0.0
		End
	End

	Event
		EName	PRIME
		Type	Start
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	0.0 -1.5 0.0
			StartJitter	3.0 0.0 3.0
			Trackrate	0.2
		End
		Lookat	OverheadAnchor
		Magnet	OverheadAnchor

		Lifespan 60
	End

	Event
		EName	WispAnchor
		Type	Local
		At	PRIME
		BhvrOverride
			PositionOffset	1.0 -0.5 0.0
			StartJitter	0.5 1.0 0.0

		End
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 60
	End

	Event
		Type	Local
		At	WispAnchor
		Part	:Soul_Trail_Bright.part

		Lifespan 45
	End
End


#############################################################

End
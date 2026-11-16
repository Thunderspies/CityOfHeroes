#############################################################
## ElectricityCage.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:ElectricityCageRingBright.part
		Part2	:ElectricityCageGlow.part
		Sound eleccage_loop 100 100 .8
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	PositOnly
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:ElectricityCageArchs2.part
		Part3	:ElectricityCageArchs.part
	End

	Event

		Type	PositOnly
		At	Hips
		Bhvr	behaviors\cagespin2.bhvr
		BhvrOverride
			StartColor	255 48 40
		End
		Geom	Cagearchs2
	End
End

Condition
	On 	Time
	Time 	16

	Event

		Type	PositOnly
		At	Hips
		Bhvr	behaviors\cagespin.bhvr
		BhvrOverride
			StartColor	255 0 0
		End
		Geom	Cagearchs
	End
End

#############################################################

End

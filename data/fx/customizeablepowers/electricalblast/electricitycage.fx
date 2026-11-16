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
		Part2	:ElectricityCageGlow.part
		Part3	:ElectricityCageArchs.part
	End

	Event
		Type	PositOnly
		At	Hips
		bhvr	behaviors\cagespin2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Cagearchs2
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Type	PositOnly
		At	Hips
		bhvr	behaviors\cagespin.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Cagearchs
	End
End

#############################################################

End
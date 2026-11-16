#
FxInfo
Lifespan 120

###########################################################

Condition
	On 	Time
	Time	81
	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidSmash_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	81

	Event
		EName	ElectricityAnchor
		Type	StartPositOnly
		At	WepR
		Bhvr	behaviors/CameraShake_Strong.bhvr
		BhvrOverride
			PositionOffset	0.0 -30 0.0
		End
		Part	:DustPillar_Initial.part
		Part	:DustPillar_Initial_Additive.part
	End

	Event
		EName	ElectricityAnchor
		Type	StartPositOnly
		At	Origin
		CHILDFX	:CHILD_FrigidSmash_Attack.fx
	End

End

###########################################################

End
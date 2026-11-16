#############################################################
## BeamRifle_SingleShot.fx
#############################################################

FxInfo
Lifespan 120

#########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_PenetratingRay_Chargeup_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		ENAME   WeaponOffset
		Type	Local
		At	C_Chest
	End
End

Condition
	On	Time
	Time	3

	Event
		Ename	ChargeUp
		Type	Local
		At	WeaponOffset
		Bhvr	:GrowInPlace.bhvr
	End

	Event
		Type	Local
		At	ChargeUp
		Part	:Muzzle_Glow_Core_Fast.part
		PArt	:BeamCharge_Arcs_Medium.part
		Lifespan 7
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	ChargeUp
		Part	:Muzzle_Glow_LargeCore_Fast.part
		PArt	:BeamCharge_Arcs_Large.part
	End
End

Condition
	On	Time
	Time	20


	Event
		ENAME   MuzzleOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
		Bhvr	:Tracer_Fade.bhvr
		BhvrOverride
			PositionOffset	1 0.0 0.1
		End
	End

	Event
		Type	PositOnly
		At	MuzzleOffset
		Part	:Muzzle_Smoke01.part
	End
End

#############################################################

End

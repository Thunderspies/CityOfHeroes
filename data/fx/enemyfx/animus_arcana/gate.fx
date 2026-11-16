###########################################################

FxInfo

	Flags DontSuppress

###########################################################

Condition
		On Cycle
		Time 50

	Event
		Type	Local
		At	Head
		BhvrOverride
			PyrRotate 	-90 180 0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Demons_Rune.part
		Part	:Demons_Rune_2.part
		Part	:Demons_Rune_3.part
		Lifespan 50
	End
End

Condition

	On 	Cycle
	Time 	35

	Event
		Type	Local
		At	Head
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.1 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.2 1.2 1.2
 			EndScale		1.0 1.0 1.0
			PyrRotate 	-90 180 0
		End
		Part	:HellfireBurst_Shockwave.part
		Part	:HellfireBurst_FireRing_Origin.part
		Part	:HellfireBurst_FireRing_Flat.part
		Part	:HellfireBurst_FireRing_Small.part
		Part	:HellfireBurst_FireRing_Highlight.part
		Part	:HellfireBurst_FireRing_Alpha_Small.part
		Lifespan 35
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

###########################################################

End

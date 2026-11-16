#############################################################
## HellfireAura.fx
#############################################################

FxInfo

#############################################################

Condition

	On 	Cycle
	Time 	45

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.1 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.2 1.2 1.2
 			EndScale		1.0 1.0 1.0
		End
		Part	:HellfireBurst_Shockwave.part
		Part	:HellfireBurst_FireRing_Origin.part
		Part	:HellfireBurst_FireRing_Flat.part
		Part	:HellfireBurst_FireRing_Small.part
		Part	:HellfireBurst_FireRing_Highlight.part
		Part	:HellfireBurst_FireRing_Alpha_Small.part
		Lifespan 60
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End


#############################################################

End



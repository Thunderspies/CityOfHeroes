#############################################################
## HellfireBurstAttack.fx
#############################################################

FxInfo
Lifespan	90

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	PositOnly
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part
		Part	:HellfireBurst_ArmFire_Core.part
		Part	:Sparks_Hellfire.part
		LifeSpan	60
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part
		Part	:HellfireBurst_ArmFire_Core.part
		Part	:Sparks_Hellfire.part
		LifeSpan	60
		LifespanJitter	5
	End
End

Condition

	On 	Time
	Time 	60

	Event
		Type	PositOnly
		At	Root
		Part	:HellfireBurst_Smoke_Big.part
		Part	:AbyssalReconstruction_Sparks.part
		Lifespan 5
	End

	Event
		Type	StartPositOnly
		At	Hips
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset	0 -2.5 0
			Scale			0.01 0.01 0.01
			ScaleRate		15.0 15.0 15.0
 			EndScale		2.0 2.0 2.0
		End
		Part	:HellfireBurst_FireRing.part
		Part	:HellfireBurst_FireRing_Highlight.part
		Part	:HellfireBurst_FireRing_Alpha.part
		Part	:HellfireBurst_FireRing_Origin.part
		Part	:HellfireBurst_FireRing_Flat.part
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



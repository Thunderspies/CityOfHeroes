#############################################################
## FurSpin.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

Condition

	Event
		Type	Local
		At	Root
		Sound AnimalPack_FurSpin 100 100 1
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Spin		0.000 -0.35 0.000000
		End
		Part	:DustDevil_Dust_Ground.part
		Part	:Fur_Expanding.part

		Lifespan 110
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
			Spin		0.000 -0.35 0.000000
		End
		Part	:DustDevil_Dust_Ground_Circle.part
		Lifespan 120
	End

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin3.bhvr
		Part	:DustDevil_Dust_Core.part
		Lifespan 90
	End

	Event
		Type	Local
		At	Root
		bhvr    behaviors\SSSpin3.bhvr
		Part	:DustDevil_Dust_OuterCore.part
		Lifespan 90
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		Part	:DustDevil_Dust_Expanding.part
		Part	:Fur_Expanding_Lingering.part
		Lifespan 105
	End

#############################################################
##################   PHYSICS  ###############################
#############################################################

	Event
		Ename	PHYSICS-ExplosiveForce
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		15
			PhysForcePower		400
			PhysForcePowerJitter	75
		End
		Lifespan	25

	End
End

#############################################################

End


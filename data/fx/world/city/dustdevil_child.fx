#############################################################
## Tornado.fx
#############################################################

FxInfo
Lifespan 550

#############################################################
Condition

	Event
		Type	Local
		At	Root
		Sound WindDustDevil1_Loop 200 200 .35
		bhvr	behaviors/Sound/SoundIn5Out5.bhvr
		Lifespan 450
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName	GalesAnchor
		Type	Start
		At	Root
		BhvrOverride
			InitialVelocityJitter	0.1 0.0 0.1
		End
		Lifespan 550
	End
	
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	GalesAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Spin		0.000 0.35 0.000000
			SpinJitter	0.000 0.02 0.0000
		End
		Part	:DustDevil_Dust_Ground.part
		Lifespan 500
	End

	Event
		Type	Local
		At	GalesAnchor
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			Spin		0.000 0.35 0.000000
			SpinJitter	0.000 0.02 0.0000
		End
		Part	:DustDevil_Dust_Ground_Circle.part
		Lifespan 500
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	TornadoAnchor
		Type	Start
		At	GalesAnchor
		Lifespan 35
	End

	Event
		Type	Local
		At	GalesAnchor
		bhvr    behaviors\SSSpin3.bhvr
		BhvrOverride
			Spin	0.000 -0.8 0.000000
		End
		Part	:DustDevil_Dust_Core.part
		Lifespan 425
	End

	Event
		Type	Local
		At	GalesAnchor
		bhvr    behaviors\SSSpin3.bhvr
		BhvrOverride
			Spin	0.000 -0.65 0.000000
		End
		Part	:DustDevil_Dust_OuterCore.part
		Lifespan 400
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	GalesAnchor
		Part	:DustDevil_Dust_Expanding.part
		Lifespan 425
	End
End
#############################################################
##################   PHYSICS  ###############################
#############################################################

Condition

	On	Time
	Time 	40

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


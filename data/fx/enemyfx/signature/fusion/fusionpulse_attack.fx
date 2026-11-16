#########################################################
## Stone Control - test_EarthQuake
#########################################################

FxInfo
LifeSpan 300

########################################################

Input
	Inpname	Hips
End

########################################################


## POWER FX #######################################################


Condition
	On	Time
	Time	25

	Event
		Type	start
		At	Root
		Sound Earthquake_FusionPulse_01 200 200 .8
	End

	Event
		EName	AOERing
		Type	start
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		Part2	:FusionPulse_Attack_Crack.part
		Lifespan	10
	End

	Event
		EName	AOERing
		Type	start
		At	Root
		Part2	:FusionPulse_Attack_StalagmiteHitRing.part
		Part2	:FusionPulse_Attack_StalagmiteHitRingB.part

		Lifespan	25
	End

	Event
		EName	AOERing
		Type	start
		At	Root
		Part2	:FusionPulse_Attack_Crack2.part
		Part2	:FusionPulse_Attack_Crack3.part
		Part2	:FusionPulse_Attack_Dust.part

		Lifespan	1
	End

End

Condition
	On	Time
	Time	27
	Event
		Type	Start
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			FadeOutLength		25
			PyrRotate 		0.0 0.0 0.0
			PyrRotateJitter 	0.0 360.0 0.0
			PositionOffset		0.0 0.5 0.0
		End


		Geom	FX_EarthQuakeRingA

		Lifespan 30
		LifespanJitter	0
		CThresh 	0.00001
	End
End

### PHYSICS PARTICLES #######################################################


Condition
	On 	Time
	Time 	25
	Repeat	10
	Event
		Ename   RockANchor
		Type	Start
		At	Root
		ChildFX	:FusionPulse_Attack_RockScatter2.fx
	End
End

## PHYSICS FORCES #######################################################

Condition
	On	Time
	Time	25

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan 10
	End
End

#########################################################

End
#############################################################
## DronePlasmaShot_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Root
		Sound	BAF_TurretBlast_Attack_01 300 300 .9
	End

	Event
		Ename 	Prime
		Type	Start
		At	Target
		BhvrOverride
			TrackRate		20.0
		End
		Part	:Projectile_Smoke_Drone_Small.part
		Part	:Projectile_Head_Drone_Small.part
		Part	:Projectile_Glow_Drone.part
		POther 	Chest
		Magnet	Chest
		LookAt	Chest
	End
End

Condition
	On	Time
	Time	5



	Event
		EName 	BeamGeo
		Type	Start
		At	Target
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	255 0 0
			#FadeInLength	5
			FadeOutLength	10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.35 .35 0.2
		End
		Part	:Beam_Glow_Drone.part
		POther	Chest
		Magnet	Chest
		LookAt	Chest
		LifeSpan		10
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	EVent
		Ename	Child
		Type	Start
		At	Chest
		ChildFX	:BAF_TurretBlast_Hit.fx
	End
End

#############################################################

End
#############################################################
## SonicBlast_Attack.fx
#############################################################

FxInfo
LifeSpan	200

#############################################################

#Condition
#	On	Time
#	Time	12
#
#	Event
#		Type	Local
#		At	head
#		Sound sonicsmallblast3 80 80 .97
#	End
#End

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Hips
		BhvrOverride
			StartColor	255 0 0
		End
	#	Geom	GEO_MonitorSphereGlowBand
	End

End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	HandR
		Part	:Sonic_Muzzle_Flash_Circle.part
		Part	:Sonic_Muzzle_Flash.part
		Sound PPD_MonitorSphere_SonicAttack_01 200 200 .8
		LifeSpan	6

	End

End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	HandL
		Part	:Sonic_Muzzle_Flash_Circle.part
		Part	:Sonic_Muzzle_Flash.part
		LifeSpan	6

	End

End

Condition
	On	Time
	Time	6

	Event
		EName	Prime
		Type	Start
		At	HandR
	#	Geom	FrontFacingLocalPivot

		BhvrOverride
			TrackRate		3.0
			PyrRotate		90.0 0.0 0.0
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		20
			PhysForcePowerJitter	10
		End
		Magnet	T_Chest
		LookAt  T_Chest

	End

	Event
		EName	Projectile
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		90.0 0.0 0.0
		End
		Part	:Sonic_Trail_Core.part
		Part	:Sonic_Trail_Pulse.part
		Part	:Sonic_Trail_Rings_Static.part
		Part	:Sonic_Trail_Rings_Static_Beam.part
		Part	:Projectile_Trail_Ember_Small.part
	End

End

Condition
	On 	Time
	Time 	14

	Event
		EName	Prime2
		Type	Start
		At	HandL
	#	Geom	FrontFacingLocalPivot

		BhvrOverride
			TrackRate		3.0
			PyrRotate		90.0 0.0 0.0
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		20
			PhysForcePowerJitter	10
		End
		Magnet	T_Chest
		LookAt  T_Chest

	End

	Event
		EName	Projectile
		Type	Local
		At	Prime2
		BhvrOverride
			PyrRotate		90.0 0.0 0.0
		End
		Part	:Sonic_Trail_Core.part
		Part	:Sonic_Trail_Pulse.part
		Part	:Sonic_Trail_Rings_Static.part
		Part	:Sonic_Trail_Rings_Static_Beam.part
		Part	:Projectile_Trail_Ember_Small.part
	End
End


######################################################################################

Condition
	On	PrimeHit

	Event
		EName 	Prime
		type	Destroy

	End
End

Condition
	On	Prime2Hit

	Event
		EName 	Prime2
		type	Destroy

	End
End

#########################################################################################


End



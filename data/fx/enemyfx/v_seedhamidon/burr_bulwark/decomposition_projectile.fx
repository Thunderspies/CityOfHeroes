#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	behaviors\Braz\Projectile_Fast_01.bhvr
		BhvrOverride
			Spin			0 0.8 0

			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	20
		End
		ChildFX	:Child_Decomp_ToxicBeam.fx
		Magnet	Target
		LookAt	Target
		Lifespan 30
	End
End


Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End
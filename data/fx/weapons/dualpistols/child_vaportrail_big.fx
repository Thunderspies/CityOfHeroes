#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

## OFFSETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	StartOffset
		Type	Start
		At	Origin
		BhvrOverride
			Scale		5 5 5
		End
		Geom	FX_ConeOffsets
		LookAt	T_Chest
	End

	Event
		Ename	TargetOffset
		Type	Start
		At	StartOffset
		AltPiv	1
	End
End

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		4.0
		End
		Geom	Bullet01
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		LookAt	TargetOffset
		Magnet	TargetOffset
	End

	Event
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		90 0 0
		End
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail02.part
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	Prime
	End
End

#############################################################

End
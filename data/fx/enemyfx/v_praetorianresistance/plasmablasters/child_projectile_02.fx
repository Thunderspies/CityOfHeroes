#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

## VAPOR TRAIL ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	TargetOffset
		Type	Local
		At	Target
		BhvrOverride
			StartJitter		1.0 1.0 1.0
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Prime
		Type	Start
		At	Origin
		Bhvr	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Fade.bhvr
		BhvrOverride
			TrackRate	10.0
		End
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		LookAt	TargetOffset
		Magnet	TargetOffset
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End
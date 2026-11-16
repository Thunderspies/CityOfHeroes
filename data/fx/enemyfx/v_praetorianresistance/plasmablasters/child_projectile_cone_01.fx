#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ConeOffsets1
		Type	Local
		At	Origin
		BhvrOverride
			PYRRotate	0 90 0
			PYRRotateJitter	20 10 0
			Scale		120 120 120
		End
		Geom	FX_ConeOffsets
	End

	Event
		Ename	TargetOffset1
		Type	Start
		At	ConeOffsets1
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
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		BhvrOverride
			StartJitter 		0.1 0.1 0.1
			TrackRate		10.0
		End
		LookAt	TargetOffset1
		Magnet	TargetOffset1
		Lifespan 30
	End

	Event
		Type	Local
		At	Prime
		Bhvr	ENEMYFX\V_PraetorianResistance\PowerRifle\Tracer_Fade.bhvr
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Core01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Projectile_Trail01.part
		Lifespan 20
	End
End

#############################################################

End